#include "msp.h"


/**
 * main.c
 */
//******************************************************************************
//  MSP432P401 Demo - ADC14, Using an External Reference
//
//  Description: This example shows how to use an external positive reference for
//  the ADC14.The external reference is applied to the VeREF+ pin. VeREF- is used
//  for the negative reference (use an external jumper to GND). A single
//  conversion is performed on channel A0. The conversion results are stored in
//  ADC14->MEM[0]. Test by applying a voltage to channel A0, then setting and
//  running to a break point at the "_NOP()" instruction. To view the conversion
//  results, open an SFR window in debugger and view the contents of
//  ADC14->MEM[0] or from the variable ADCvar.
//  NOTE: VeREF+ range: 1.4V (min) to AVCC (max)
//        VeREF- range: 0V (min) to 1.2V (max)
//        Differential ref voltage range: 1.4V(min) to AVCC(max)
//          (see datasheet for device specific information)
//
//                MSP432P401
//             -------------------
//         /|\|                   |
//          | |                   |
//          --|RST                |
//            |                   |
//     Vin -->|P5.5/A0            |
//            |                   |
//    REF+ -->|P5.6/VREF+/VeREF+  |
//            |                   |
//    REF-|-->|P5.7/VREF-/VeREF-  |
//        |   |                   |
//        |---|AVss               |
//            |                   |
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | June 2014 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "msp.h"
#include <stdint.h>
#include "My_ADC.h"

uint8_t flag = 0;

void setup_ADC14(void);

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             // Stop WDT

    // Configure GPIO
    P5->SEL1 |= BIT5 | BIT6 | BIT7;         // Enable A/D channel A0
    P5->SEL0 |= BIT5 | BIT6 | BIT7;         // and VeREF+ and VeREF-

    //enable global interrupt
    __enable_irq();
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
    setup_ADC14();

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR
    // Ensures SLEEPONEXIT occurs immediately
    __DSB();

    while (1){
        if(flag == 1){
            flag = 0;
        }
        //delay here
        ADC14->CTL0 |= ADC14_CTL0_SC;       // Start conversion-software trigger
        __sleep();
    }
}
