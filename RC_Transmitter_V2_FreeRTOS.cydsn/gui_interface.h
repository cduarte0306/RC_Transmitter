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
#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H
    
#include <cytypes.h>
    
    
#define MSG_SIZE  (80u)
    
    
typedef struct __attribute__((__packed__))
{
    uint8 speed_set;
    float kp;
    float ki;
    float kd;
    float theta;
    uint8 cmd;
} gui_rx_data_t;
    

typedef struct __attribute__((__packed__))
{
    uint8 speed;
    float front_distance;
    float right_distance;
    float left_distance;
    uint16 x_axis;
    uint16 y_axis_msb;
    uint16 z_axis_msb;
    uint8 gngga_msg[MSG_SIZE];
    uint8 charge_level;
} gui_tx_data_t;


typedef struct __attribute__((__packed__))
{
    uint8 start_msg;
    gui_tx_data_t gui_data;
    uint8 checksum;
    uint8 carriage_return;
    uint8 line_feed;
}gui_data_out_t;


typedef struct __attribute__((__packed__))
{
    uint8 start_msg;
    gui_rx_data_t gui_data;
    uint8 checksum;
    uint8 carriage_return;
    uint8 line_feed;
}gui_data_in_t;


typedef struct __attribute__((__packed__))
{
    uint8 start_msg;
    uint8 msg_1;
    uint8 msg_2;
    uint8 carriage_return;
    uint8 line_feed;
} handshake_struct_t;


gui_tx_data_t gui_tx_data;
gui_rx_data_t gui_rx_data; 
gui_data_out_t gui_data_out;
gui_data_in_t gui_data_in;
handshake_struct_t handshake_struct;


void gui_setup(void);
void process_gui(void);
    
#endif

/* [] END OF FILE */
