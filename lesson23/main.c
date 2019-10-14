#include <stdint.h>
#include "bsp.h"

uint32_t stack_blinky1[40];
uint32_t * sp_blinky1 = &stack_blinky1[40];
uint32_t stack_blinky2[40];
uint32_t * sp_blinky2 = &stack_blinky2[40];

int main_blinky1(void);
int main_blinky2(void);

/******************************************************************************/
int main(void)
/******************************************************************************/
{
    uint32_t volatile run = 0U;
    __disable_irq();

    /* Fabricate Cortex-M stack frame for blinky1. */
    *(--sp_blinky1) = (1U << 24);  /* xPSR. */
    *(--sp_blinky1) =
        (uint32_t) &main_blinky1;  /* PC.   */
    *(--sp_blinky1) = 0x0000000EU; /* LR.   */
    *(--sp_blinky1) = 0x0000000CU; /* R12.  */
    *(--sp_blinky1) = 0x00000003U; /* R3.   */
    *(--sp_blinky1) = 0x00000002U; /* R2.   */
    *(--sp_blinky1) = 0x00000001U; /* R1.   */
    *(--sp_blinky1) = 0x00000000U; /* R0.   */
    /* Additionally, fake registers R4 - R11. */
    *(--sp_blinky1) = 0x0000000BU; /* R11.  */
    *(--sp_blinky1) = 0x0000000AU; /* R10.  */
    *(--sp_blinky1) = 0x00000009U; /* R9.   */
    *(--sp_blinky1) = 0x00000008U; /* R8.   */
    *(--sp_blinky1) = 0x00000007U; /* R7.   */
    *(--sp_blinky1) = 0x00000006U; /* R6.   */
    *(--sp_blinky1) = 0x00000005U; /* R5.   */
    *(--sp_blinky1) = 0x00000004U; /* R4.   */


    /* Fabricate Cortex-M stack frame for blinky1. */
    *(--sp_blinky2) = (1U << 24);  /* xPSR. */
    *(--sp_blinky2) =
        (uint32_t) &main_blinky2;  /* PC.   */
    *(--sp_blinky2) = 0x0000000EU; /* LR.   */
    *(--sp_blinky2) = 0x0000000CU; /* R12.  */
    *(--sp_blinky2) = 0x00000003U; /* R3.   */
    *(--sp_blinky2) = 0x00000002U; /* R2.   */
    *(--sp_blinky2) = 0x00000001U; /* R1.   */
    *(--sp_blinky2) = 0x00000000U; /* R0.   */
    /* Additionally, fake registers R4 - R11. */
    *(--sp_blinky2) = 0x0000000BU; /* R11.  */
    *(--sp_blinky2) = 0x0000000AU; /* R10.  */
    *(--sp_blinky2) = 0x00000009U; /* R9.   */
    *(--sp_blinky2) = 0x00000008U; /* R8.   */
    *(--sp_blinky2) = 0x00000007U; /* R7.   */
    *(--sp_blinky2) = 0x00000006U; /* R6.   */
    *(--sp_blinky2) = 0x00000005U; /* R5.   */
    *(--sp_blinky2) = 0x00000004U; /* R4.   */

    BSP_init();

    while (1)
    {
        /* Loop forever. */
        ++run;
    }

    //return 0;
}

/******************************************************************************/
int main_blinky1(void)
/******************************************************************************/
{
    while (1)
    {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}

/******************************************************************************/
int main_blinky2(void)
/******************************************************************************/
{
    while (1)
    {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}
