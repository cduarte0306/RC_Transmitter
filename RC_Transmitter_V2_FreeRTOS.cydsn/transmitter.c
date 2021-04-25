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
#include "project.h"
#include "transmitter.h"
#include "data.h"
#include "utils.h"


#define CRUISE_BIT      0b00000001
#define SPEED_MAX       (50)


CY_ISR(cruise_on_handler)
{
    tx_data.drive_mode_bits |= CRUISE_BIT;
    
    cruise_on_ClearInterrupt();
}


CY_ISR(cruise_off_handler)
{
    tx_data.drive_mode_bits &= ~(CRUISE_BIT);
    
    cruise_off_ClearInterrupt();
}


void btn_setup(void)
{
    cruise_on_isr_Start();
    cruise_off_isr_Start();
    
    cruise_on_isr_StartEx(cruise_on_handler);
    cruise_off_isr_StartEx(cruise_off_handler);
}


void pin_process(void)
{
    static int8 speed_setting;
    
    if(!speed_plus_Read())
        speed_setting ++;
    
    else if(!speed_minus_Read())
        speed_setting --;
    
    speed_setting = (speed_setting > SPEED_MAX ) ? SPEED_MAX : speed_setting;
    speed_setting = (speed_setting < 0) ? 0 : speed_setting;
    
    tx_data.speed_set = speed_setting;
}

/* [] END OF FILE */
