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
#ifndef UART_H
#define UART_H
    
#include <cytypes.h>
#include "data.h"
    
    
#define MESSAGE_LEN     (19u)
#define START_DELIM     (0x7E)
#define CR              '\r'
    
    
typedef struct __attribute__((__packed__))  /* Defining structure for data out */
{
    uint8 start_byte;
    tx_data_t data_out;
    uint8 cheksum;
    uint8 end_char;
} uart_buff_data_out_t;


typedef struct __attribute__((__packed__))  /* Defining structure for data out */
{
    uint8 start_byte;
    rx_data_t data_in;
    uint8 cheksum;
    uint8 end_char;
} uart_buff_data_in_t;


uart_buff_data_out_t uart_buff_data_out;
uart_buff_data_in_t uart_buff_data_in;
    

void uart_process(void);
void uart_init(void);
void uart_send_data(void);
    
#endif

/* [] END OF FILE */
