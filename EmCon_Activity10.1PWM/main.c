/**********************************************************************/
// ENGR-2350 Activity 10.1
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
void Timer_Init(Timer_A_UpModeConfig timerUp, Timer_A_CompareModeConfig timerComp3, Timer_A_CompareModeConfig timerComp4);

// Add global variables here, as needed.
Timer_A_UpModeConfig timerUp;
Timer_A_CompareModeConfig timerComp3;
Timer_A_CompareModeConfig timerComp4;

int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();
    GPIO_Init();
    Timer_Init(timerUp, timerComp3, timerComp4);

    // Place initialization code (or run-once) code here
    int16_t newCompVal;
    timerUp.timerPeriod = 800;

    while(1){
        // Place code that runs continuously in here
        printf("Key pressed: ");
        uint8_t cmd = getchar();
        printf("%c\r\n", cmd);

        if (cmd=='a'){ //Increase left wheel speed
            timerComp4.compareValue += 50;
            newCompVal = timerComp4.compareValue;
            if (newCompVal > timerUp.timerPeriod * 0.9){newCompVal = timerUp.timerPeriod * 0.9;}
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, newCompVal);
        }else if (cmd=='z'){ //Decrease left wheel speed
            timerComp4.compareValue -= 50;
            newCompVal = timerComp4.compareValue;
            if (newCompVal < 0){newCompVal = 0;}
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, newCompVal);
        }else if (cmd=='s'){ //Increase right wheel speed
            timerComp3.compareValue += 50;
            newCompVal = timerComp3.compareValue;
            if (newCompVal > timerUp.timerPeriod * 0.9){newCompVal = timerUp.timerPeriod * 0.9;}
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, newCompVal);
        }else if (cmd=='x'){ //Decrease right wheel speed
            timerComp3.compareValue -= 50;
            newCompVal = timerComp3.compareValue;
            if (newCompVal < 0){newCompVal = 0;}
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, newCompVal);
        }
    }
}

// Add function declarations here as needed
void GPIO_Init(){
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Enable/Disable
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //Direction

    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Enable motors
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //Set wheels forward

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); //Speed (PWM), R - TA0.3
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); //Speed (PWM), L - TA0.4
    //Note: OUTn signals = TAm.n: m --> Timer_A Module m (TIMER_Am_BASE), n --> CCRn
}

void Timer_Init(Timer_A_UpModeConfig timerUp, Timer_A_CompareModeConfig timerComp3, Timer_A_CompareModeConfig timerComp4){
    timerUp.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    timerUp.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1; //Ndiv = 1
    //Must reset at desired PWM frequency (30 kHz) => 30000
    //MSP's SMCLK = 24MHz, must "slow down the MSP's frequency" to something usable
    //SMCLK / Divider = DivFreq  ->  24000000 / 1 = 24000000
    //Still not usable, so divide it again by timerPeriod
    //DivFreq / timerPeriod = DesiredFreq  ->  24000000 / tP = 30000 => tP = 800
    timerUp.timerPeriod = 800;
    Timer_A_configureUpMode(TIMER_A0_BASE, &timerUp);


    timerComp3.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
    timerComp3.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    timerComp3.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    timerComp3.compareValue = 0;
    Timer_A_initCompare(TIMER_A0_BASE, &timerComp3);

    timerComp4.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
    timerComp4.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    timerComp4.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    timerComp4.compareValue = 0;
    Timer_A_initCompare(TIMER_A0_BASE, &timerComp4);


    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

// Add interrupt functions last so they are easy to find
