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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "project.h"


#define  STATUS_DELAY        (1000u)  // One second delay
#define  SPEED_SETTING_DELAY (400u)   // 400ms delay

// FreeRTOS libraies
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

// Driver libraries
#include "data.h"
#include "uart.h"
#include "utils.h"
#include "adc.h"
#include "transmitter.h"


void hardware_init(void)
{
    UART_Radio_Start();
    UART_GUI_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
}


extern void FreeRTOS_Start();


/* Task space - All calbacks will be defined in other files and called on their respective tasks. 
   The application switches between tasks based on the internal core timer*/
void uart_task(void *ptr);
void adc_task(void *ptr);
void status_task(void *ptr);
void uart_data_send(void *ptr);
void uart_gui_task(void *ptr);
void uart_gui_send_task(void *ptr);
void pin_task(void *ptr);


int main(void)
{   
    /* All function initializations get called here */

    CyGlobalIntEnable; /* Enable global interrupts. */
    FreeRTOS_Start();
    
    uart_init();

    hardware_init();
    
    btn_setup();
    
    /* All task callbacks go here */
    xTaskCreate(uart_task, 
                "uart task", 
                250, 
                NULL, 
                1, 
                NULL);
    xTaskCreate(uart_data_send, 
                "Uart send data task", 
                250, 
                NULL, 
                2, 
                NULL);
    xTaskCreate(uart_gui_task, 
                "Uart gui task", 
                250, 
                NULL, 
                1, 
                NULL);
    xTaskCreate(uart_gui_send_task, 
                "Uart gui send task", 
                250, 
                NULL, 
                1, 
                NULL);
    xTaskCreate(adc_task, 
                "adc task", 
                250, 
                NULL, 
                1, 
                NULL);
    xTaskCreate(status_task, 
                "status task", 
                50, 
                NULL, 
                1, 
                NULL);
    xTaskCreate(pin_task, 
                "pin_task", 
                250, 
                NULL, 
                1, 
                NULL);
    
    vTaskStartScheduler();
    
    while(1)
    {
        /* Do not write any code here as it will not execute */
    }
}


void uart_task(void *ptr)
{
    while(1)
    {
        uart_process();
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}


void uart_data_send(void *ptr)
{
    while(1)
    {
        /* This function is to run every 20 ms */
        uart_send_data();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void uart_gui_task(void *ptr)
{
    while(1)
    {
        //UART_GUI_PutArray(rx_data.gngga_msg, sizeof(rx_data.gngga_msg));
        UART_GUI_WriteTxData((uint8)rx_data.front_distance);   
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}


void uart_gui_send_task(void* ptr)
{
    while(1)
    {
        /* Call UART send fct */
    }
}


void status_task(void *ptr)
{
    uint8 led_status = FALSE;
    
    while(1)
    {
        led_status = !led_status;
        status_Write(led_status);
        vTaskDelay(STATUS_DELAY / portTICK_PERIOD_MS);
    }
}


void adc_task(void *ptr)
{
    while(1)
    {
        joystick_update();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


void pin_task(void *ptr)
{
    while(1)
    {
        pin_process();
        
        /* Update every 400ms */
        vTaskDelay(SPEED_SETTING_DELAY / portTICK_PERIOD_MS);
    }
}
/* [] END OF FILE */
