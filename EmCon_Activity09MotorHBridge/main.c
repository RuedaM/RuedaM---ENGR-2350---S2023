/**********************************************************************/
// ENGR-2350 Activity 9
// Name: Rueda, Maximillian
// RIN: 662021896
// This is the base project for several activities and labs throughout
// the course.  The outline provided below isn't necessarily *required*
// by a C program; however, this format is required within ENGR-2350
// to ease debugging/grading by the staff.
/**********************************************************************/

// We'll always add this include statement. This basically takes the
// code contained within the "engr_2350_msp432.h" file and adds it here.
#include "engr2350_msp432.h"

// Add function prototypes here, as needed.
void GPIO_Init();
void fs_test();
void HB_test();

// Add global variables here, as needed.


int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();
    GPIO_Init();

    // Place initialization code (or run-once) code here
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN6|GPIO_PIN7);

    while(1){
        // Place code that runs continuously in here
        //fs_test();
        HB_test();
    }
}

// Add function declarations here as needed
void GPIO_Init(){
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2|GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6|GPIO_PIN7);
}

void fs_test(){
    while(1){
        printf("Key pressed: ");
        uint8_t cmd = getchar();
        printf("%c\r\n", cmd);

        if (cmd=='f'){GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);}
        if (cmd=='s'){GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2);}
    }
}

void HB_test(){
    while(1){
        printf("Key pressed: ");
        uint8_t cmd = getchar();
        printf("%c\r\n", cmd);


        if (cmd=='f'){ //BiLED GRN
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
            GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN6|GPIO_PIN7);
            __delay_cycles(240);
            GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN7);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);
        }
        else if (cmd=='r'){ //BiLED RED
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
            GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN6|GPIO_PIN7);
            __delay_cycles(240);
            GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
        }
        else if(cmd=='s'){ //BiLED OFF
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
            GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN6|GPIO_PIN7);
        }
    }
}


// Add interrupt functions last so they are easy to find
