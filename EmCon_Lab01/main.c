// Lab 1
// ENGR-2350
// Name: Rueda, Maximillian
// RIN: 662021896

#include "engr2350_msp432.h"

void GPIOInit();
void TestIO();
void ControlSystem();

uint8_t LEDL = 0; // Two variables to store the state of
uint8_t LEDR = 0; // the LEDs
uint8_t SS1;
uint8_t SS2;
uint8_t BMP0;
uint8_t BMP5;

int main(void)
{

    SysInit();
    GPIOInit();

    printf("\r\n\n"
           "***********\r\n"
           "Lab 1 Start\r\n"
           "***********\r\n");

    while(1){
        //TestIO();
        ControlSystem();
    }
}

void GPIOInit(){
    // Add initializations of inputs and outputs
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4|GPIO_PIN5);

    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6|GPIO_PIN7);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5);

    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN0|GPIO_PIN5);
}

void TestIO(){
    // Add printf statement(s) for testing inputs

    // Example code for testing outputs
    while(1){
        printf("Key pressed: ");

        uint8_t cmd = getchar();
        printf("%c\r\n", cmd);

        if(cmd == 'a'){
            // Turn LEDL On
            GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN0);
        }else if(cmd == 'z'){
            // Turn LEDL Off
            GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN0);
        }else if(cmd == 's'){
            // Turn LEDR On
            GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN5);
        }else if(cmd == 'x'){
            // Turn LEDR Off
            GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN5);
        }else if(cmd == 'q'){
            // Turn BiLED Red
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
        }else if(cmd == 'w'){
            // Turn BiLED Off
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4|GPIO_PIN5);
        }else if(cmd == 'e'){
            // Turn BiLED Green
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
        }else if(cmd == 'd'){
            // Turn MotorL On
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
        }else if(cmd == 'c'){
            // Turn MotorL Off
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
        }else if(cmd == 'f'){
            // MotorL Forward
            GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN4);
        }else if(cmd == 'v'){
            // MotorL Backward
            GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN4);
        }else if(cmd == 'g'){
            // Turn MotorR On
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);
        }else if(cmd == 'b'){
            // Turn MotorR Off
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);
        }else if(cmd == 'h'){
            //MotorR Forward
            GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN5);
        }else if(cmd == 'n'){
            //MotorR Backward
            GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN5);
        }
    }
}

void ControlSystem(){
    SS1 = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN2);
    SS2 = GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN3);
    BMP0 = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0);
    BMP5 = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7);

    if (SS1){
        if (SS2){
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN4|GPIO_PIN5);
        }
        else{
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN4|GPIO_PIN5);
        }
        if (!BMP0){
            __delay_cycles(240e3); // Debounce
            LEDR = !LEDR;
            if (LEDR){GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN5);}
            else{GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN5);}
            while(!BMP0){BMP0 = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0);}
            __delay_cycles(240e3); // Debounce
        }
        if (!BMP5){
            __delay_cycles(240e3); // Debounce
            LEDL = !LEDL;
            if (LEDL){GPIO_setOutputHighOnPin(GPIO_PORT_P8,GPIO_PIN0);}
            else{GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN0);}
            while(!BMP5){BMP5 = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7);}
            __delay_cycles(240e3); // Debounce
        }
        if (LEDR){GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);}
        else{GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);}
        if (LEDL){GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);}
        else{GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);}
    }
    else{
       GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4|GPIO_PIN5);
       GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6|GPIO_PIN7);
    }
}
