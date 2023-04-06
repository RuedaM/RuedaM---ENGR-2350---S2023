/**********************************************************************/
// ENGR-2350 Template Project
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
void Timer_Init(Timer_A_UpModeConfig timerControl);
void GPIO_Init();
void UpdateTime();
void Control_System();

//(Timer_A_UpModeConfig struct already exists in DriverLib)

void Timer_ISR();
void Port4_ISR();


// Add global variables here, as needed.
Timer_A_UpModeConfig timerControl;
uint8_t time[4], timerResets, bigFlag;



int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Place initialization code (or run-once) code here
    Timer_Init(timerControl);
    GPIO_Init();

    while(1){
        // Place code that runs continuously in here
        Timer_A_clearInterruptFlag( TIMER_A1_BASE );

        if (bigFlag){
            UpdateTime();
            bigFlag = 0;
        }
    }
}

// Add function declarations here as needed
void Timer_Init(Timer_A_UpModeConfig timerControl){
    timerControl.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    timerControl.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64; //NOTE: DIVIDER == 64

    timerControl.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;

    float Ttimer = 0.1;  //wanted real-time period
    float Ntimer;  //# of "steps"
    float Ttclk;  //input clock period
    float Ndiv = 64.0;  //Divider #
    float ftclk = 24000000.0;  //frequency of MSP
    Ttclk = Ndiv / ftclk;
    Ntimer = Ttimer / Ttclk;
    timerControl.timerPeriod = Ntimer;

    Timer_A_configureUpMode(TIMER_A1_BASE, &timerControl);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    Timer_A_registerInterrupt(TIMER_A1_BASE, TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, Timer_ISR);
}

void GPIO_Init(){
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN2|GPIO_PIN3); //0==hrs, 2==mins, 3==sec

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);

    GPIO_registerInterrupt(4, Port4_ISR);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN2|GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN2|GPIO_PIN3);
}

void UpdateTime(){
    time[0]++;  // Increment tenths of seconds
    if(time[0] == 10){  // If a whole second has passed...
        time[0] = 0;    // Reset tenths of seconds
        time[1]++;      // And increment seconds
        if(time[1] == 60){ // If a minute has passed...
            time[1] = 0;   // Reset seconds
            time[2]++;     // Increment minutes
            if(time[2] == 60){  // and so on...
                time[2] = 0;
                time[3]++;
                if(time[3] == 24){
                    time[3] = 0;
                }
            }
        }
    }
    printf("%2u:%02u:%02u.%u\r",time[3],time[2],time[1],time[0]);
}


// Add interrupt functions last so they are easy to find
void Timer_ISR(){
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);   // acknowledge the interrupt
    timerResets++;


    if (timerResets % 2 == 0){
        //putchar('A');
        GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);
    }
    else{
        //putchar('B');
        GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2);
    }

    bigFlag = 1;
}

void Port4_ISR(){
    __delay_cycles(240e3); // Debounce

    uint8_t activePins = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    if (activePins & GPIO_PIN0){
        GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN0);
        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0)){ //Verify push (more debouncing)
            time[3]++; if (time[3] == 24){time[3] = 0;}
        }
    }
    if (activePins & GPIO_PIN2){
        GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN2);
        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2)){ //Verify push (more debouncing)
            time[2]++; if (time[2] == 60){time[2] = 0;
                time[3]++; if (time[3] == 24){time[3] = 0;}
            }
        }
    }
    if (activePins & GPIO_PIN3){
        GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN3);
        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3)){ //Verify push (more debouncing)
            time[1]++; if(time[1] == 60){time[1] = 0;
                time[2]++; if(time[2] == 60){time[2] = 0;
                    time[3]++; if(time[3] == 24){time[3] = 0;}
                }
            }
        }
    }
}
