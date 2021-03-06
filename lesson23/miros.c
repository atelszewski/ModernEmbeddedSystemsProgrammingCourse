/****************************************************************************
* MInimal Real-time Operating System (MIROS)
* version 0.23 (matching lesson 23)
*
* This software is a teaching aid to illustrate the concepts underlying
* a Real-Time Operating System (RTOS). The main goal of the software is
* simplicity and clear presentation of the concepts, but without dealing
* with various corner cases, portability, or error handling. For these
* reasons, the software is generally NOT intended or recommended for use
* in commercial applications.
*
* Copyright (C) 2018 Miro Samek. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*
* Contact Information:
* https://www.state-machine.com
****************************************************************************/

#include "miros.h"

OSThread * volatile OS_curr; /* Current thread. */
OSThread * volatile OS_next; /* Next thread to run. */

/******************************************************************************/
void OS_init(void)
/******************************************************************************/
{
    /* Set the PendSV interrupt priority to the lowest level. */
    *(uint32_t volatile *) 0xE000ED20U |= (0xFFU << 16);
}

/******************************************************************************/
void OS_sched(void)
/******************************************************************************/
{
    if (OS_next != OS_curr)
    {
        *(uint32_t *) 0xE000ED04U = (1U << 28);
    }
}

/******************************************************************************/
void OSThread_start(OSThread * me,
    OSThreadHandler threadHandler,
    void * stkSto,
    uint32_t stkSize)
/******************************************************************************/
{
    /* Round down the stack top to the 8 byte boundary.
     * NOTE: Arm Cortex-M stack grows down from hi -> low memory. */
    uint32_t * sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8);
    uint32_t * stk_limit;

    /* Fabricate Cortex-M stack frame. */
    *(--sp) = (1U << 24);  /* xPSR. */
    *(--sp) = (uint32_t) threadHandler;  /* PC.   */
    *(--sp) = 0x0000000EU; /* LR.   */
    *(--sp) = 0x0000000CU; /* R12.  */
    *(--sp) = 0x00000003U; /* R3.   */
    *(--sp) = 0x00000002U; /* R2.   */
    *(--sp) = 0x00000001U; /* R1.   */
    *(--sp) = 0x00000000U; /* R0.   */
    /* Additionally, fake registers R4 - R11. */
    *(--sp) = 0x0000000BU; /* R11.  */
    *(--sp) = 0x0000000AU; /* R10.  */
    *(--sp) = 0x00000009U; /* R9.   */
    *(--sp) = 0x00000008U; /* R8.   */
    *(--sp) = 0x00000007U; /* R7.   */
    *(--sp) = 0x00000006U; /* R6.   */
    *(--sp) = 0x00000005U; /* R5.   */
    *(--sp) = 0x00000004U; /* R4.   */

    /* Save the top of the stack in the thread's attribute. */
    me->sp = sp;

    /* Round up the bottom of the stack to the 8 byte boundary. */
    stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);

    /* Pre-fill the unused part of the stack with 0xDEADBEEF. */
    for (sp = sp - 1U; sp >= stk_limit; --sp)
    {
        *sp = 0xDEADBEEF;
    }
}

/******************************************************************************/
__asm
void PendSV_Handler(void)
/******************************************************************************/
{
    IMPORT OS_curr
    IMPORT OS_next

    /* __disable_irq(); */
    CPSID    I

    /* if (OS_curr != (OSThread *)0) */
    /* { */
    LDR      r1,=OS_curr
    LDR      r1,[r1,#0x00]
    CBZ      r1,PendSV_restore

    /*     Push registers r4-r11 on stack. */
    PUSH     {r4-r11}

    /*     OS_curr->sp = sp; */
    LDR      r1,=OS_curr
    LDR      r1,[r1,#0x00]
    STR      sp,[r1,#0x00]
    /* } */

PendSV_restore
    /* sp = OS_next->sp; */
    LDR      r1,=OS_next
    LDR      r1,[r1,#0x00]
    LDR      sp,[r1,#0x00]

    /* OS_curr = OS_next; */
    LDR      r1,=OS_next
    LDR      r1,[r1,#0x00]
    LDR      r2,=OS_curr
    STR      r1,[r2,#0x00]

    /* Pop registers r4-r11. */
    POP      {r4-r11}

    /* __enable_irq(); */
    CPSIE    I

    /* Return to the next thread. */
    BX       lr
}
