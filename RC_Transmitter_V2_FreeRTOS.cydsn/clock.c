/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "clock.h"
#include "project.h"
#include "data.h"
#include "utils.h"


#define MAX_SPEED   (100u)
#define MIN_SPEED   (0u)


static uint16 count;
static uint16 last;
static uint16 diff;


CY_ISR(counter_speed_handler)
{
    if(speed_plus_Read() == FALSE)
        tx_data.speed_set = max(tx_data.speed_set ++, MAX_SPEED);
        
    
    if(speed_minus_Read() == FALSE)
        tx_data.speed_set = min(tx_data.speed_set--, MIN_SPEED);
    
    isr_counter_speed_ClearPending();
}


void clock_setup(void)
{
    isr_counter_speed_Start();
    isr_counter_speed_StartEx(counter_speed_handler);
}

/* [] END OF FILE */
