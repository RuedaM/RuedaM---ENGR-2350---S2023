/**********************************************************************/
// ENGR-2350 Activity 12
// Names:   Rueda, Maximillian
//          Shriver, Garrett
// section: 3
// side:    A
// seat #:  1-2
// RIN:     662021896
//          661975278
/**********************************************************************/
//include statements
// code contained within the "engr_2350_msp432.h" file added here.
#include "engr2350_msp432.h"



//function prototypes
void GPIO_Init();
void Timer_Init();
void Encoder_ISR();
void Port4_ISR();



//global variables
Timer_A_UpModeConfig T0;//timer 0 on timer A
Timer_A_ContinuousModeConfig T3;//timer 3 on timer A

Timer_A_CaptureModeConfig tccIN;//encoder
Timer_A_CompareModeConfig tccr3;//right wheel pwm
Timer_A_CompareModeConfig tccr4;//left wheel pwm

//right
uint32_t enc_totalRight = 0; //Keep track of total encoder events, useful in measuring distance traveled.
int32_t enc_counts_trackRight = 0;// Keep track the timer counts since the capture event (must be signed!).
int32_t enc_countsRight = 0;// Final value of timer counts between capture events.
uint8_t enc_flagRight = 0;// A flag to denote that a capture event has occurred.

uint32_t speedSumRight = 0;//A variable to store summation of wheel speed (timer counts) measurements <-- For averaging
uint8_t measurementsRight = 0;//A variable to track number of measurements in the summation variable     <-- For averaging

//left
uint32_t enc_totalLeft = 0; //Keep track of total encoder events, useful in measuring distance traveled.
int32_t enc_counts_trackLeft = 0;// Keep track the timer counts since the capture event (must be signed!).
int32_t enc_countsLeft = 0;// Final value of timer counts between capture events.
uint8_t enc_flagLeft = 0;// A flag to denote that a capture event has occurred.

uint32_t speedSumLeft = 0;//A variable to store summation of wheel speed (timer counts) measurements <-- For averaging
uint8_t measurementsLeft = 0;//A variable to track number of measurements in the summation variable     <-- For averaging

//more global variables
//65000 = 25% ; 32500 = 50% (duty cycle)
uint16_t setPoint = 65000;//value compared against to see if things are to fast or slow
uint16_t speed = 200;//200 is 25 % duty
uint8_t drivePattern = 0;
uint8_t brk = 0;//breaks out of drive loops

uint8_t start = 0, i = 0;
uint32_t ISRArray[300];





#ifndef DOING_UNIT_TESTS//when running the test file this main function in main.c isn't included
//otherwise it is the main function used to run the program when we want to run the testFile.c main function
int main(void) /* Main Function */
{
    /* note that anything inside of any infinite loop isn't testable with the testfile(at least not easily... they could be tested w interrupts)
     * because of this write as many things into testable functions as possible and make the looping part small
     * and easy to debug/ work through logically if the functions are assumed to be working(because we tested them seperately)*/

    //local variables

    //initialization code (or run-once) code
    SysInit();
    GPIO_Init();
    Timer_Init();



    while(1){//Place code that runs continuously
        if (start){
            printf("program started\r\n");
            enc_totalRight = 0; enc_totalLeft = 0;
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Enable motors
            printf("motors on\r\n");
            //__delay_cycles(24e6/4);
            while(enc_totalRight < 240){}
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Disable motors
            printf("motors off\r\n");
            __delay_cycles(24e6/2);
            start = 0;
            printf("\r\n");

            if (enc_totalRight >= 240){
            printf("Sample\tValue\r\n");
            uint16_t i;
                for(i = 0 ; i < enc_totalRight ; i++){
                    printf("%u\t%u\r\n", i ,ISRArray[i]);
                }
                printf("finished printing array\r\n");
            }
        }
    }
}
#endif //make sure only the main function is excluded when running the test file





//function declarations
void GPIO_Init(){
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Motor Enable/Disable Toggle
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //Motor Direction

    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7); //Disable motors initially
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5); //Set wheels forward initially

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); //Speed (PWM), R - TA0.3
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); //Speed (PWM), L - TA0.4

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); //Wheel Encoder, R - TA3.0
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P10, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION); //Wheel Encoder, L - TA3.1
    //Note: OUTn signals = TAm.n: m --> Timer_A Module m (TIMER_Am_BASE), n --> CCRn

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN0); //P4.0(BMP0)

    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0, GPIO_LOW_TO_HIGH_TRANSITION); //For selecting pattern/breaking
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_registerInterrupt(GPIO_PORT_P4, Port4_ISR);
}

void Timer_Init(){
    //clocks
    //pwm
    T0.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;//configure Timer A0 to count using SMCLK
    T0.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;//and a divider of 1.
    //set the period such that it will elapse once every 30khz.(1/30khz seconds)(3.33 * 10^-5 s)
    T0.timerPeriod = 800;// 3.33*10^-5 s * 24*10^6 counts/s= 800 counts
    Timer_A_configureUpMode(TIMER_A0_BASE, &T0);//configure Timer A0 to apply the struct field configuration to the timer after specifying values to the Timer_A_UpModeConfig struct.


    //encoder
    T3.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;//configure Timer A0 to count using SMCLK
    T3.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;//and a divider of 1.
    T3.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    T3.timerClear = TIMER_A_DO_CLEAR;
    Timer_A_configureContinuousMode(TIMER_A3_BASE, &T3);//initialize the required Timer_A instance to continuous mode:

    //inputs
    //10.4 is TA3.CCI0A 10.5 is TA3.CCI1A TAm.CCInA is used in the alternate functions table to denote the CCRn input of Timer_A instance m.
    //right
    tccIN.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;//only one may be set at a time
    tccIN.captureMode = TIMER_A_CAPTUREMODE_RISING_EDGE;
    tccIN.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    tccIN.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
    tccIN.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    Timer_A_initCapture(TIMER_A3_BASE, &tccIN);

    //left
    tccIN.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;//only one may be set at a time
    Timer_A_initCapture(TIMER_A3_BASE, &tccIN);

    //outputs
    //2.6: TA0.3: Timer_A Module 0 and CCR3
    //2.7: TA0.4: Timer_A Module 0 and CCR4
    //right
    tccr3.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3;
    tccr3.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    tccr3.compareValue = 100;
    Timer_A_initCompare(TIMER_A0_BASE, &tccr3);

    //80, 160, 240

    //left
    tccr4.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;
    tccr4.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    tccr4.compareValue = 100;
    Timer_A_initCompare(TIMER_A0_BASE, &tccr4);

    //register interrupts
    Timer_A_registerInterrupt(TIMER_A3_BASE, TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, Encoder_ISR);
    Timer_A_registerInterrupt(TIMER_A3_BASE, TIMER_A_CCR0_INTERRUPT, Encoder_ISR);

    //start timers
    Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_UP_MODE);
    Timer_A_startCounter(TIMER_A3_BASE,TIMER_A_CONTINUOUS_MODE);

}





//INTERRUPTS
void Encoder_ISR(){
    if(Timer_A_getEnabledInterruptStatus(TIMER_A3_BASE)){ //Check to see if timer reset interrupt has occurred. If yes:
        Timer_A_clearInterruptFlag(TIMER_A3_BASE); //Clear the reset interrupt flag
        enc_counts_trackRight += 65536; //Add 65536 to enc_counts_track
        enc_counts_trackLeft += 65536;
    }

    //right
    if(Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0) & TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG){//Check to see if a capture event ocurred. If yes:
        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);//Clear the capture event interrupt flag
        //enc_totalRight++;//Increment enc_total (total number of capture events that have occurred)
        enc_countsRight = enc_counts_trackRight + Timer_A_getCaptureCompareCount(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0); //Calculate enc_counts as enc_counts_track + capture value
        enc_counts_trackRight = -1 * Timer_A_getCaptureCompareCount(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);//Set enc_counts_track to -(capture value)
        enc_flagRight = 1;//Set enc_flag to 1
//        speedSumRight += enc_countsRight;    //Sum result with previous results
//        measurementsRight++;    //Add 1 to variable tracking number of results in sum
//        if(measurementsRight == 6){//if sum tracking variable == 6
//            if(speedSumRight/6 > setPoint){//If sum variable / 6 > setpoint
//                tccr3.compareValue++;//Subtract 1 (or Add 1) to PWM value
//            }
//            else if(speedSumRight/6 < setPoint){//If sum variable / 6 < setpoint
//                tccr3.compareValue--;//Add 1 (or Subtract 1) to PWM value
//            }
//            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3,tccr3.compareValue);//Apply PWM value
//            measurementsRight = 0;//reset sum tracking variable
//            speedSumRight = 0;//reset sum variable
//        }

        if (enc_totalRight < 250){
            ISRArray[enc_totalRight] = enc_countsRight;
        }
        enc_totalRight++;
    }

    //left
    if(Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1) & TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG){//Check to see if a capture event ocurred. If yes:
        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1);//Clear the capture event interrupt flag
        enc_totalLeft++;//Increment enc_total (total number of capture events that have occurred)
        enc_countsLeft = enc_counts_trackLeft + Timer_A_getCaptureCompareCount(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1); //Calculate enc_counts as enc_counts_track + capture value
        enc_counts_trackLeft = -1 * Timer_A_getCaptureCompareCount(TIMER_A3_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_1);//Set enc_counts_track to -(capture value)
        enc_flagLeft = 1;//Set enc_flag to 1
//        speedSumLeft += enc_countsLeft;    //Sum result with previous results
//        measurementsLeft++;    //Add 1 to variable tracking number of results in sum
//        if(measurementsLeft == 6){//if sum tracking variable == 6
//            if(speedSumLeft/6 > setPoint){//If sum variable / 6 > setpoint
//                tccr4.compareValue++;//Subtract 1 (or Add 1) to PWM value
//            }
//            else if(speedSumLeft/6 < setPoint){//If sum variable / 6 < setpoint
//                tccr4.compareValue--;//Add 1 (or Subtract 1) to PWM value
//            }
//            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4,tccr4.compareValue);//Apply PWM value
//            measurementsLeft = 0;//reset sum tracking variable
//            speedSumLeft = 0;//reset sum variable
//        }
    }
}

void Port4_ISR(){
    printf("BMP pressed\r\n");
    __delay_cycles(24e6); // 10 ms delay (24 MHz clock)
    GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN0);
    start = 1;
}
