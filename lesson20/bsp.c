#include "stm32f10x.h"
#include "bsp.h"

/******************************************************************************/
__attribute__((naked)) void assert_failed(char const * p_name, int const line)
/******************************************************************************/
{
    /* TBD: damage control. */
    //NVIC_SystemReset(); /* Reset the system. */
}
