// Lab 2
// ENGR-2350
// Name: Rueda, Maximillian; Shriver, Garrett; Kim, Donggyu
// RIN: 662021896; 661975278; 661979059

#include "engr2350_msp432.h"
#include "stdlib.h"

void GPIOInit();
void Timer_init();
void give_instr();
void give_pattern();
void give_decreasing_pattern();
void updateCount();
void delay_second(float second);
uint8_t RGBtest();
bool guess_color();

void pb_ISR();


uint8_t pattern[5] = {};
uint16_t count = 0;
uint8_t reminder = 1;
uint8_t round = 0;
uint8_t start = 0, end = 0, restart = 0;
Timer_A_UpModeConfig temp;
uint8_t init_guess = 0;

int main(void)
{
    //Initializations
    SysInit();
    GPIOInit();
    Timer_init();

    printf("\r\n\n"
           "***********\r\n"
           "Lab 2 Start\r\n"
           "***********\r\n");
    //Turn of BiLED
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);

    give_instr();

    while(1){

        if (!start){uint8_t test_color = RGBtest();}  // Function gives colors, need "useless" variable to store return-val in

        else{ //Always jump here if game is "start"ed
            printf("\r\n");

            delay_second(1);

            count = 0;
            round++;

            uint8_t i = 0;
            for(i = 0; i < 5; i++){pattern[i]=rand()%5;}//Make random 5 color pattern
            give_pattern();//display 5 color pattern

            delay_second(2);

            bool result = guess_color();//Main game logic: This function will return true or false according to the result

            if(result == false){
                end = 1;
                printf("INCORRECT!\r\n");
                while(restart == 0){//BiLED turn red on and off for every 0.25s until player press the push button
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN3);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);
                    delay_second(0.25);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
                    delay_second(0.25);
                }
            }

            else if(result == true){
                end =1;
                printf("CORRECT!\r\n");
                while(restart == 0){//BiLED turn green on and off for every 0.25s until player press the push button
                    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
                    delay_second(0.25);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);
                    delay_second(0.25);
                }
            }
            //If push button is pressed, prepare to restart the game
            //Turn off BiLED and Set every value to initial condition
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
            restart = 0;
            count = 0;
            reminder = 1;
        }
    }
}



// FUNCTIONS
void GPIOInit(){// GPIO initialization: set input and output and register interrupt
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);

    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5,GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    //Register interrupt for push button with fuction pb_ISR
    GPIO_registerInterrupt( GPIO_PORT_P5, pb_ISR);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN4);
}

void Timer_init() {
    //Timer configured with 64 bit divider and a period that easily adds to 250ms and 500ms.
    float Ndiv = 64;
    float Fsmclk = 24000000; // 24* 10^6 = 24MHz
    float Ttclk = Ndiv / Fsmclk; // 2.667 * 10^-6 seconds/cycle
    float Ntimer = 0.05 / Ttclk; // ~18750 cycles for 50ms

    temp.timerPeriod = Ntimer; // (18749)+1= 18750 cycles per period (cycles = period+1 (aka 50ms))
    temp.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // 24MHz
    temp.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64; // 24Mhz/64 = 375KHz clock
    Timer_A_configureUpMode(TIMER_A1_BASE, &temp); // configure timer A cycle speed for 50 ms a cycle
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);//start A1 in up mode
}

void updateCount(){//increase count variable
    count++;
}

void delay_second(float second){
    //Delay program for given amount of seconds (s)
    float delay = second*20;// Multiply 20 and parameter to get seconds b/c period is 0.05
    while(count < delay){
          if(Timer_A_getInterruptStatus(TIMER_A1_BASE)){
              updateCount();
              Timer_A_clearInterruptFlag(TIMER_A1_BASE);
          }
    }
    count = 0;
}

void give_instr(){
    printf("Welcome to the Game!\r\n"
            "This is a memory game in which you repeat a\r\n"
            "given, randomized 5-color sequence in order.\r\n\n"
            "INSTRUCTIONS:\r\n"
            "- Before starting, each BUMPER (BMP) can be pressed to\r\n   show what color it is assocated with. Try these out!\r\n"
            "- To start the game, press the PUSH BUTTON (PB).\r\n"
            "- A sequence will be given. Memorize it.\r\n"
            "- The sequence will then play again, only this time with\r\n   one less color. From here, fill in the remainder of\r\n   the sequence.\r\n"
            "- If successful, the the game will progress, this\r\n   time with the last two colors missing. Complete the\r\n   sequence each time to win.\r\n"
            "- You are only given three seconds to give an input, after\r\n   which time the game will end and you will lose.\r\n"
            "- If an incorrect sequence is given at any time, you will\r\n   will lose.\r\n"
            "- You are allowed one reminder of the entre sequence per\r\n   game: the entire sequence will play, then the sequence\r\n   you are to complete will play again.\r\n");
}

void give_pattern(){
    //Display 5 color pattern

    //Turn BiLED Red
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);

    uint8_t i;
    for(i=0; i<5; i++){
        if(pattern[i] == 0){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);}// RED
        else if(pattern[i] == 1){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);}// GREED
        else if(pattern[i] == 2){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);}// BLUE
        else if(pattern[i] == 3){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);}// YELLOW
        else if(pattern[i] == 4){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);}// PURPLE
        else if(pattern[i] == 5){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);}// CYAN

        //DIsplay color for 0.5s, turn off for 0.5s
        delay_second(0.5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        delay_second(0.5);
    }
}

void give_decreasing_patten(){
    //Display 5-round color pattern

    //Turn BiLED Red
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);

    uint8_t i;
    for(i = 0; i < 5-round; i++){
        if(pattern[i] == 0){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);}// RED
        else if(pattern[i] == 1){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);}// GREED
        else if(pattern[i] == 2){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);}// BLUE
        else if(pattern[i] == 3){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);}// YELLOW
        else if(pattern[i] == 4){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);}// PURPLE
        else if(pattern[i] == 5){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);}// CYAN

        //Display color for 0.5s, turn off for 0.5s
        delay_second(0.5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        delay_second(0.5);
    }
}

uint8_t RGBtest(){
    //Display color on RGB, return value corresponding to BMP press

    //Get every input pin value for BMP press
    uint8_t bmp0 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0);
    uint8_t bmp1 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2);
    uint8_t bmp2 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3);
    uint8_t bmp3 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5);
    uint8_t bmp4 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6);
    uint8_t bmp5 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7);

    if(!bmp0){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);// BMP0 pressed
        while(!bmp0){
            __delay_cycles(240e3);// wait for release...
            bmp0 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0);// RED
        }
        return 0;// RED = 0
    }

    else if(!bmp1){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);// BMP1 pressed
        while(!bmp1){
            __delay_cycles(240e3);
            bmp1 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2);// GREEN
        }
        return 1;// GREEN = 1
    }
    else if(!bmp2){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);// BMP2 pressed
        while(!bmp2){
            __delay_cycles(240e3);
            bmp2 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3);// BLUE
        }
        return 2;// BLUE = 2
    }
    else if(!bmp3){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);// BMP3 pressed
        while(!bmp3){
            __delay_cycles(240e3);
            bmp3 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5);// YELLOW
        }
        return 3;// YELLOW = 3
    }
    else if(!bmp4){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);//BMP4 pressed
        while(!bmp4){
            __delay_cycles(240e3);
            bmp4 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6);// PURPLE
        }
        return 4;// PURPLE = 4
    }

    else if(!bmp5){GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2|GPIO_PIN1);// BMP5 pressed
        while(!bmp5){
            __delay_cycles(240e3);
            bmp5 = GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7);// CYAN
        }
        return 5;// CYAN = 5
    }
    else{GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        return 10; // if no BMP pressed, return 10 (do nothing)
    }
}

bool guess_color(){
    //Main game logic: returns true/false according to result
    bool result = true;
    while(round <= 5){

        give_decreasing_patten();//Display 5-round pattern
        GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);

        uint8_t index = 5-round;
        init_guess = 1;// Set init_guess to 1 to use reminder
        while(index < 5){// we will guess the color on pattern from index 5-round to 5
            while(count != 60){//Start counting 3 seconds
                // Turn BiLED green
                GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
                if(index == 5){// If player reach index 5, go next round
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
                    break;
                }
                if(Timer_A_getInterruptStatus(TIMER_A1_BASE)){// Increase count variable every 0.05s
                    updateCount();
                    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
                }

                uint8_t guess = RGBtest();// Get value of which BMP pressed
                if(guess == pattern[index]){// If if correct guess: increase index, continue
                    count = 0;
                    index++;
                }
                else if(guess == 10){// If no BMP pressed, do nothing
                    continue;
                }
                else{count = 60;}// If incorrect guess, set count to 60
            }
            init_guess = 0;
            if(count == 60){// If player takes 3+ seconds OR inputs wrong answer: return false
                // Turn off BiLED and RGB
                GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
                return false;
            }
        }

        round++; // Increase round
        GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
        if(round <= 5){delay_second(5);}
    }
    //If every answer is correct, turn off BiLED and RGB; return true
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    return result;
}



// INTERRUPTS
void pb_ISR(){
    //Interrupt function for push button: 3 conditions
    GPIO_clearInterruptFlag(GPIO_PORT_P5,GPIO_PIN4);

    //condition 1: Reminder
    if((reminder==1) && (round!=0) && (end==0) && (init_guess==1)){
        count = 0;
        give_pattern();//Display 5 patterns
        delay_second(2);
        give_decreasing_patten();//Display 5-round patterns

        GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN2);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
        count = 0; reminder = 0;//Set reminder to 0 because only 1 reminder is allowed for this game
    }

    //condition 2: Restart the game
    else if(end==1){
        //Set LED and RGB off
        GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN3);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN2);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        //Set every variable to initial condition
        count = 0; round = 0;
        reminder = 1;
        restart = 1;
        end = 0;
    }
    //condition 3: Start game
    else if((end==0) && (round==0)){start = 1;}
}

/*
 * SEQUENCES:
 * Y Y Y R G
 * B R P B G
 *
 */
