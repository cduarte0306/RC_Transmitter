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
#ifndef DATA_H
#define DATA_H
    
#include <cytypes.h>
    
    
#define MSG_SIZE  (80u)
        
/* The following values will be used to calibrate the PID controller
   form the GUI. Values will come from the transmitter and stored 
   upon reception of the SAVE command */
typedef struct __attribute__((__packed__))
{
    float kp;
    float ki;
    float kd;
    float theta;
} pid_calib_t;
        

/* Defining incoming data structure   */
typedef struct __attribute__((__packed__))
{
    uint16 motor;
    uint16 servo;
    uint8 drive_mode_bits;
    uint8 speed_set;
    pid_calib_t pid_calib;
    uint8 cmd;
} tx_data_t;

typedef struct __attribute__((__packed__))
{
    uint8 speed;
    float front_distance;
    float right_distance;
    float left_distance;
    uint16 x_axis;
    uint16 y_axis;
    uint16 z_axis;
    uint8 gngga_msg[MSG_SIZE];
    uint8 charge_level;
    float input_current;
    float motor_current;
    uint8 eeprom_flag;
} rx_data_t;


rx_data_t rx_data;
tx_data_t tx_data;
uint32 resets;
    
#endif

/* [] END OF FILE */
