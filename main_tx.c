/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_led_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_led_radio_tx_example
*
* @brief Radio Transmitter Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/

#include <stdint.h>
#include "radio_config.h"
#include "nrf.h"
#include "simple_uart.h"
#include "boards.h"

static uint8_t packet[100];  ///< Packet to transmit

void init()
{
  /* Start 16 MHz crystal oscillator */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;

  /* Wait for the external oscillator to start up */
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
  {
  }

  // Set radio configuration parameters
  

  simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);

  // Set payload pointer
	
	radio_configure();
 
}

/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
	char id_str[10];
	init();
	simple_uart_putstring((uint8_t *)"To aqui\n");
  while(true)
  {

		// Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)packet;
		NRF_RADIO->EVENTS_READY = 0U;
    // Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1U;
		
    while(NRF_RADIO->EVENTS_READY == 0U)
    {
    }

    NRF_RADIO->EVENTS_END = 0U;
    // Start listening and wait for address received event
    NRF_RADIO->TASKS_START = 1U;
    // Wait for end of packet
		
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }
		
		if(NRF_RADIO->CRCSTATUS == 1){
			sprintf(id_str,"%02X%02X%02X%02X\\",packet[0],packet[1],packet[2],packet[3]);
			simple_uart_putstring((uint8_t *)id_str);
			
		}
		
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }
  }
}




/**
 *@}
 **/
