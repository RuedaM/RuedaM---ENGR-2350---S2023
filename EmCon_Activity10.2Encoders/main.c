/**********************************************************************/
// ENGR-2350 Activity 10.2
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
void Timer_Init(Timer_A_ContinuousModeConfig timerCont, Timer_A_CaptureModeConfig timerCap);
void Encoder_ISR();

// Add global variables here, as needed.
Timer_A_ContinuousModeConfig timerCont;
Timer_A_CaptureModeConfig timerCap;
uint32_t enc_total; // Total encoder events (for measuring distance traveled)
int32_t enc_counts_track; // Timer counts SINCE the capture event
int32_t enc_counts; // Final value of timer counts between capture events
uint8_t enc_flag; // Denotes that capture event occurred


int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();
    GPIO_Init();
    Timer_Init(timerCont, timerCap);

    // Place initialization code (or run-once) code here


    while(1){
        // Place code that runs continuously in here
        if(enc_flag){ // Check to see if capture occurred
            enc_flag = 0; // reset capture flag
            uint16_t distance = ((3.14159*70)*enc_total)/360 ; // Calculate distance traveled in mm: (circumference * tot.encoder events) * 1/360 ratio
            uint16_t speed_rpm = (24000000*60)/(360*enc_counts) ; // Calculate the instantaneous wheel speed in rpm: 1/360 ratio * (fSMLK / enc_counts) * s-to-min conversion
            uint16_t speed_mm = ((2*3.14159)*24000000*35) / (360*enc_counts) ; // Calculate the instantaneous car speed in mm/s: deg-to-rad conversion * (fSMLK / enc_counts) * radius
            printf("%5u mm\t%5u rpm\t%5u mm/s\r\n",distance,speed_rpm,speed_mm);
        }
    }
}

// Add function declarations here as needed
void GPIO_Init(){
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); // - TA3.0
    //Note: OUTn signals = TAm.n: m --> Timer_A Module m (TIMER_Am_BASE), n --> CCRn
}

void Timer_Init(Timer_A_ContinuousModeConfig timerCont, Timer_A_CaptureModeConfig timerCap){
    timerCont.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; //fSMCLK = 24MHz
    timerCont.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1; //Ndiv = 1
    timerCont.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    Timer_A_configureContinuousMode(TIMER_A3_BASE, &timerCont);


    timerCap.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    timerCap.captureMode = TIMER_A_CAPTUREMODE_RISING_EDGE;
    timerCap.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    timerCap.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
    timerCap.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    Timer_A_initCapture(TIMER_A3_BASE, &timerCap);


    Timer_A_registerInterrupt(TIMER_A3_BASE, TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, Encoder_ISR);
    Timer_A_registerInterrupt(TIMER_A3_BASE, TIMER_A_CCR0_INTERRUPT, Encoder_ISR);


    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_CONTINUOUS_MODE);
}

// Add interrupt functions last so they are easy to find
void Encoder_ISR(){
    if (Timer_A_getEnabledInterruptStatus(TIMER_A3_BASE) == TIMER_A_INTERRUPT_PENDING){ // Timer has reset
        Timer_A_clearInterruptFlag(TIMER_A3_BASE);
        enc_counts_track += 65536;

    }else if (Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0) & TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG){ // Capture event occurred
        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
        enc_total++;
        uint16_t  capVal = Timer_A_getCaptureCompareCount(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
        enc_counts = enc_counts_track + capVal;
        enc_counts_track = -(capVal);
        enc_flag = 1;
    }
}
