/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"


////////////////////////////////////////////////////////////////////////////////
// function
// name: configSIM
// returns: void
// description: configures board System Integration Module
////////////////////////////////////////////////////////////////////////////////
/*
void configSIM() {
	// delay function
	SIM_SCGC6 |= (1 << 24); // Enable TPM0
	SIM_SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK

	SIM_SCGC5 |= (1<<13); // Enable Light Sensor I/O Port E
	SIM_SCGC5 |= (1<<12); // Enable Port D
	SIM_SCGC6 |= (1<<27); // Enable ADC Module
}
*/


////////////////////////////////////////////////////////////////////////////////
// function
// name: configLS
// returns: void
// description: configures board light sensor
////////////////////////////////////////////////////////////////////////////////
/*
void configLS() {
	// Setup Analog Input - Default is analog (PTE22), No need to do anything.

	// TODO - LED stuff can be taken out for production
	// Setup LED Pin for GPIO
	PORTD_PCR5 &= ~0x700; // Clear First
	PORTD_PCR5 |= 0x700 & (1 << 8); // Set MUX bits

	// Setup Pin 5 as output
	GPIOD_PDDR |= (1 << 5);

	// Setup ADC Clock ( < 4 MHz)
	ADC0_CFG1 = 0;  // Default everything.

	// Analog Calibrate
	ADC0_SC3 = 0x07; // Enable Maximum Hardware Averaging
	ADC0_SC3 |= 0x80; // Start Calibration

	// Wait for Calibration to Complete (either COCO or CALF)
	while(!(ADC0_SC1A & 0x80)){	}


	// Calibration Complete, write calibration registers.
	cal_v = ADC0_CLP0 + ADC0_CLP1 + ADC0_CLP2 + ADC0_CLP3 + ADC0_CLP4 + ADC0_CLPS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0_PG = cal_v;

	cal_v = 0;
	cal_v = ADC0_CLM0 + ADC0_CLM1 + ADC0_CLM2 + ADC0_CLM3 + ADC0_CLM4 + ADC0_CLMS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0_MG = cal_v;


	ADC0_SC3 = 0; // Turn off Hardware Averaging
}
*/

////////////////////////////////////////////////////////////////////////////////
// function
// name: delay
// returns: void
// description: delay's program for a period of n milliseconds
////////////////////////////////////////////////////////////////////////////////
void delay(unsigned short n) {
	SIM_SCGC6 |= (1 << 24); // Enable TPM0
	SIM_SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK

	TPM0_CONF |= (0x1 << 17);
	TPM0_SC = (1 << 7) | (0x7);

	TPM0_MOD = ((n*62) + (n/2)) - 1;

	TPM0_SC |= (0x1 << 3);

	while(!(TPM0_SC & 0x80)){}
	return;
}

////////////////////////////////////////////////////////////////////////////////
// function
// name: printMoorseChar
// returns: void
// description: prints char base on moorseCode.
////////////////////////////////////////////////////////////////////////////////
void printMoorseChar(unsigned int code) {
	switch(code) {
	case 0xFFFFFF10:
		PRINTF("A");
		break;
	case 0xFFFF0111:
		PRINTF("B");
		break;
	case 0xFFFF0101:
		PRINTF("C");
		break;
	case 0xFFFFF011:
		PRINTF("D");
		break;
	case 0xFFFFFFF1:
		PRINTF("E");
		break;
	case 0xFFFFFFF0:
		PRINTF("T");
		break;
	case 0x10000000:
		PRINTF(" ");
		break;
	default:
		break;
	}
}


int main(void)
{
	// Hardware initializations and configurations
	hardware_init();
	//configSIM();
	//configLS();

	// Variables
	unsigned int timeOn = 0;
	unsigned int timeOff = 0;
	unsigned int moorseCode = 0xFFFFFFFF;

	// Light sensor Variables
	unsigned short cal_v = 0;
	unsigned char light_val = 0;

	SIM_SCGC5 |= (1<<13); // Enable Light Sensor I/O Port E
	SIM_SCGC5 |= (1<<12); // Enable Port D
	SIM_SCGC6 |= (1<<27); // Enable ADC Module

	// Setup Analog Input - Default is analog (PTE22), No need to do anything.

	// TODO - LED stuff can be taken out for production
	// Setup LED Pin for GPIO
	PORTD_PCR5 &= ~0x700; // Clear First
	PORTD_PCR5 |= 0x700 & (1 << 8); // Set MUX bits

	// Setup Pin 5 as output
	GPIOD_PDDR |= (1 << 5);
	GPIOD_PSOR |= (1 << 5);

	// Setup ADC Clock ( < 4 MHz)
	ADC0_CFG1 = 0;  // Default everything.

	// Analog Calibrate
	ADC0_SC3 = 0x07; // Enable Maximum Hardware Averaging
	ADC0_SC3 |= 0x80; // Start Calibration

	// Wait for Calibration to Complete (either COCO or CALF)
	while(!(ADC0_SC1A & 0x80)){	}


	// Calibration Complete, write calibration registers.
	cal_v = ADC0_CLP0 + ADC0_CLP1 + ADC0_CLP2 + ADC0_CLP3 + ADC0_CLP4 + ADC0_CLPS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0_PG = cal_v;

	cal_v = 0;
	cal_v = ADC0_CLM0 + ADC0_CLM1 + ADC0_CLM2 + ADC0_CLM3 + ADC0_CLM4 + ADC0_CLMS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0_MG = cal_v;


	ADC0_SC3 = 0; // Turn off Hardware Averaging

	int firstLight = 0;

	while(1) {
		ADC0_SC1A = 0x03; // Set Channel, starts conversion.
		while(!(ADC0_SC1A & 0x80)){	}
		light_val = ADC0_RA; // Resets COCO

		do {
			ADC0_SC1A = 0x03; // Set Channel, starts conversion.
			while(!(ADC0_SC1A & 0x80)){	}
			light_val = ADC0_RA; // Resets COCO
			if(light_val < 250)
				firstLight = 1;
		} while ( firstLight == 0);



		if(light_val < 250) {

			if(timeOff >= 225 && timeOff <= 275) { // +- 25ms wiggle room?

				// Time between symbols
				timeOff = 0; // re-init

			} else if(timeOff >= 725 && timeOff <= 775) {

				// Time between letters
				printMoorseChar(moorseCode);
				moorseCode = 0xFFFFFFFF; // re-init
				timeOff = 0;

			} else if(timeOff >= 975 && timeOff <= 1025) {

				// New word
				printMoorseChar(0x10000000);
				moorseCode = 0xFFFFFFFF; // re-init
				timeOff = 0;

			}

			// light is on
			delay(1);
			timeOn++;

			// GPIOD_PDOR &= ~(1<<5); // LED off when light on
		}
		else {

			if(timeOn >= 225 && timeOn <= 275 ) {

				// that was a dot
				moorseCode = (moorseCode << 4) + 1; // shift "1" into code
				timeOn = 0; // re-init

			} else if(timeOn >= 725 && timeOn <= 775) {

				// that was a dash
				moorseCode = (moorseCode << 4); // shift "0" into code
				timeOn = 0; // re-init either way

			}

			// light is off
			delay(1);
			timeOff++;
			if(timeOff > 1025) {
				// message stop
				printMoorseChar(moorseCode);
				moorseCode = 0xFFFFFFFF; // re-init
				timeOff = 0;
				while(light_val > 250) {
					ADC0_SC1A = 0x03; // Set Channel, starts conversion.
					while(!(ADC0_SC1A & 0x80)){	}
					light_val = ADC0_RA; // Resets COCO
				}
			}

			// GPIOD_PDOR |= (1<<5); // LED on when light off
		}
	}


    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
