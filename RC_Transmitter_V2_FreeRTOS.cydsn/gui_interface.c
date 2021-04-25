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

#include "gui_interface.h"
#include "project.h"
#include "utils.h"
#include "data.h"


#define CR      '\r'
#define LF      '\n'
#define HS_SIZE (5u)  // Defines the size of the ha handshake array


/* Defining the handshake message */
static uint8 handshake_msg[HS_SIZE] = {0x7E, 0x01, 0x02, CR, LF};
static uint8 handshake_data[HS_SIZE];
static uint8 data_in_array[sizeof(gui_data_in_t)];
static uint8 rx_index = 0;
static uint8 rx_hs_index = 0;
static uint8 handshake_flag = FALSE;  // Initialize the handshake flag as 0
static uint8 packet_ok = FALSE;


CY_ISR(rx_gui_handler)
{
    if(!handshake_flag)
    {
        handshake_data[rx_hs_index] = UART_GUI_RXDATA_REG;      

        if((handshake_data[rx_hs_index] == LF) && (handshake_data[rx_hs_index - 1u] == CR))
        {
            packet_ok = TRUE;
            rx_hs_index = 0;
        }
        else
        {
            rx_hs_index ++;
            rx_hs_index = (rx_hs_index >= sizeof(handshake_struct_t)) ? 0 : rx_hs_index;
        }
    }
    else
    {
        data_in_array[rx_index] = UART_GUI_RXDATA_REG;
        
        /* Wait for CR and LF characters */
        if((data_in_array[rx_index] == LF && data_in_array[rx_index - 1u] == CR) && rx_index == sizeof(gui_data_in_t))
        {
            packet_ok = TRUE;
            rx_index = 0;
        }
        else
        {
            rx_index++;
            rx_index = (rx_index >= sizeof(gui_data_in_t)) ? 0 : rx_index;
        }
    }

    rx_gui_isr_ClearPending();
}


void gui_setup(void)
{
    /* Defining end characters */
    gui_data_out.carriage_return = CR;
    gui_data_out.line_feed = LF;
    
    /* Initializing rx interrupt handler */
    rx_gui_isr_Start();
    rx_gui_isr_StartEx(rx_gui_handler);

    memcpy(&handshake_struct, handshake_msg, sizeof(handshake_msg));
}


static void process_handshake(void)
{
    if(!packet_ok)
        return;

    /* Always return true if handshake has already been done */
    if(handshake_flag)
        return;

    for(uint8 i = 0; i < HS_SIZE; i ++)
    {
        /* Check if both arrays have the same data points at their respective indexes */
        if(handshake_msg[i] != handshake_data[i])
        {
            handshake_flag = FALSE;
            break;
        } 
    }

    /* Send a TRUE for a succesful handhsake */
    UART_GUI_TXDATA_REG = TRUE;
}


static uint8 calc_cheksum(gui_tx_data_t* outgoing_packet)
{
    /* Intiialize packet sum as 0 */
    uint8 packet_sum = 0;
    uint8 cheksum_packet[sizeof(gui_tx_data_t)];
    
    memcpy(&cheksum_packet, &outgoing_packet, sizeof(gui_tx_data_t));

    for(uint8 i = 0; i < sizeof(gui_tx_data_t); i++)
    {
        packet_sum += cheksum_packet[i];
    }

    /* And the sum's result with 255 to obtain the cheksum key */
    return (packet_sum & 0xFF);
}


/* Process incoming data packet */
static uint8 calc_cheksum_in(gui_rx_data_t incoming_packet, uint8 checksum)
{
    uint8 packet_sum_out = 0;
    uint8 cheksum_out_packet[sizeof(gui_rx_data_t)];

    memcpy(&cheksum_out_packet, &incoming_packet, sizeof(gui_rx_data_t));

    for(uint8 i = 0; i < sizeof(gui_rx_data_t); i++)
    {
        packet_sum_out += cheksum_out_packet[i];
    }

    if((packet_sum_out & 0xFF) == checksum)
        return TRUE;
    else return FALSE;
}


static void send_data_gui(void)
{
    uint8 data_out_array[sizeof(gui_data_out_t)];

    /* Copy all data received onto transmission buffer */
    memcpy(&gui_data_out, &rx_data, sizeof(rx_data_t));

    gui_data_out.checksum =  calc_cheksum(&gui_data_out.gui_data);
    memcpy(&data_out_array, &gui_data_out, sizeof(gui_data_out_t));

    /* Transmit data to GUI */
    UART_GUI_PutArray(data_out_array, sizeof(data_out_array));
}


void process_gui(void)
{
    /* Process handshake packets */
    process_handshake();

    /* Return if no handshake has been completed or if packet isn't ready */
    if((!handshake_flag) || (!packet_ok))
        return;

    uint8 cheksum_flag = FALSE;

    /* Process data in */
    memcpy(&gui_data_in, &data_in_array, sizeof(data_in_array));
    cheksum_flag = calc_cheksum_in(gui_data_in.gui_data, gui_data_in.checksum);
    
    /* If checksum passes, place data into corresponding locations */
    if(cheksum_flag)
    {
        memcpy(&gui_rx_data, &gui_data_in.gui_data, sizeof(gui_rx_data_t));

       tx_data.pid_calib.kp = gui_rx_data.kp;
       tx_data.pid_calib.ki = gui_rx_data.ki;
       tx_data.pid_calib.kd = gui_rx_data.kd;

        packet_ok = FALSE;
    }
    else
        packet_ok = FALSE;

    send_data_gui();
}

/* [] END OF FILE */
