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
#include "uart.h"
#include "data.h"
#include "project.h"
#include "utils.h"


static uint8 rx_index = 0;
static uint8 tx_buffer[sizeof(uart_buff_data_out_t)];
static uint8 rx_data_buffer[sizeof(uart_buff_data_in_t)];
static uint8 cheksum_buff[sizeof(rx_data_t)];
static uint8 packet_ok = FALSE;
static uint8 cheksum_in;


void uart_send_data(void);


CY_ISR(rx_handler)
{    
    rx_data_buffer[rx_index] = UART_Radio_GetByte();  // Might change to UART_Radio_GetByte()
    
    if(rx_data_buffer[rx_index] == START_DELIM)  
    {
        rx_index = 0;  // Sets the rx_index to 0 in the case of a start byte detected
    }
        
    if(rx_data_buffer[rx_index] == CR && rx_index == sizeof(uart_buff_data_in_t) - 1u)
    {
        packet_ok = TRUE;
    }   
    
    rx_index++;
    
    if(rx_index == sizeof(uart_buff_data_in_t)) 
    {
        rx_index = 0;
    }
    
    rx_radio_isr_ClearPending();  // Clears the ISR to continue with normal operation
}


void uart_init(void)
{
    uart_buff_data_out.start_byte = START_DELIM;
    uart_buff_data_out.end_char = CR;
    
    rx_radio_isr_Start();
    rx_radio_isr_StartEx(rx_handler);
    
}


static uint8 cheksum_in_calc( rx_data_t* buffer)
{
    uint16 packet_sum_in = 0;  // Resets to 0 on every entry
    memcpy(&cheksum_buff, buffer, sizeof(rx_data_t));
    
    for(uint8 i = 0; i < sizeof(rx_data_t); i++)
    {
        /* Calculate the sum of all bytes received */
        packet_sum_in += cheksum_buff[i];
    }
    
    if((packet_sum_in & 0xFF) == uart_buff_data_in.cheksum)  /* Function checks if the cheksum equals the value at the buffer index */
        return TRUE;
    else return FALSE;
}


static void cheksum_out_calc( tx_data_t* buffer )
{
    uint16 packet_sum_out = 0;
    uint8 checksum_calc_buff[sizeof(tx_data_t)];
    
    memcpy(&checksum_calc_buff, buffer, sizeof(tx_data_t));
    
    for(uint8 i = 0; i < sizeof(tx_data_t); i ++)
    {
        packet_sum_out += checksum_calc_buff[i];
    }
    
    uart_buff_data_out.cheksum = (packet_sum_out & 0xFF);
}


void uart_send_data(void)
{
    UART_Radio_ClearTxBuffer();    
    memcpy(&uart_buff_data_out.data_out, &tx_data, sizeof(tx_data_t));
    cheksum_out_calc(&uart_buff_data_out.data_out);
    
    // Use mcmcpy to copy the contents of one memory location onto another
    memcpy(&tx_buffer, &uart_buff_data_out, sizeof(uart_buff_data_out_t));
    UART_Radio_PutArray(tx_buffer, sizeof(uart_buff_data_out_t));
    UART_Radio_ClearRxBuffer();
}


static void uart_unpack_data()
{
    if(packet_ok == FALSE)
        return;
    
    // The incoming data is placed in the UART buffer for pre-processing before it is placed into the main data  
    memcpy(&uart_buff_data_in, &rx_data_buffer, sizeof(uart_buff_data_in_t));  // Addressign is subject to change, might have to remove the &
    cheksum_in = cheksum_in_calc(&uart_buff_data_in.data_in); 
    
    if(cheksum_in ==  TRUE)
    {
        memcpy(&rx_data, &uart_buff_data_in.data_in, sizeof(rx_data_t));
        //rx_index = 0;  // Resets the RX index once all bytes are received
        UART_Radio_ClearRxBuffer();
        //uart_send_data();
    }
    else
    {
        packet_ok = FALSE;
    }
}


void uart_process(void)
{
    uart_unpack_data();
    //read_flag = ~write_flag;
}



/* [] END OF FILE */
