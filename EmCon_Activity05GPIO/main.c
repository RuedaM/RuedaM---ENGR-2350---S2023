// **ACTIVITY**: Simple GPIO
// ENGR-2350
// Name: Rueda, Maximillian
// RIN: 662021896

#include "engr2350_msp432.h"

// Add function prototypes here, as needed.
void GPIOInit();
void GetInputs();
void RegisterLogic();
void DriverLibLogic();

// Add global variables here, as needed.
uint8_t pb1;
     // **ACTIVITY**: Add pb2 and ss1 variables
uint8_t pb2;
uint8_t ss1;

// Main Function
int main(void)
{
    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Place initialization code (or run-once) code here
    GPIOInit();

    while(1){
        // Place code that runs continuously in here
        GetInputs();


        // Functions to calculate outputs
        // RegisterLogic();
        DriverLibLogic();



        //// TEST CODE BELOW ////

        // Test print of inputs
        printf("PB1: %u, PB2: %u, SS1: %u\r\n",pb1,pb2,ss1);

        // Code to test the outputs
        /*
        uint32_t count;
        // &= (AND=) sets bits low, |= (OR=) sets bits high,
        // ^= (Exclusive OR=) toggles the value of a bit
        P2OUT ^= 0x10;  // Replace number to toggle one leg of BiLED1
        P3OUT ^= 0x01;  // Replace number to toggle LED1
        for(count=100000; count>0; count--); // This Creates a crude delay
        P2OUT ^= 0x20;  // Replace number to toggle the other leg of BiLED1
        for(count=100000; count>0; count--); // This Creates a crude delay
        */

        //// END OF TEST CODE ////
    }
}

// Add function declarations here as needed
void GPIOInit(){
    /*
    // Configure inputs and outputs
    P2DIR |= 0x30; // Set P2.4 and P2.5 to output (BiLED)
    P3DIR &= ~0x04; // Set P3.2 to input (PB1)
    // **ACTIVITY**: Add initializations for missing inputs and outputs
    P3DIR |= 0x01; // Set P3.0 to output (LED)
    P3DIR &= ~0x08; // Set P3.3 to input (PB2)
    P2DIR &= ~0x08; // Set P2.3 to input (SS1)
    */
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN2|GPIO_PIN3);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4|GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
}

void GetInputs(){
    /*
    // Read the input values
    pb1 = (P3IN & 0x04) != 0;    // Determine pb1 value.
                       // Notice the repeated use of 0x04 from GPIOInit().
    // **ACTIVITY**: Get input state for pb2 and ss1
    pb2 = (P3IN & 0x08) != 0;
    ss1 = (P2IN & 0x08) != 0;
    */
    pb1 = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN2);
    pb2 = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN3);
    ss1 = GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN3);
}

void RegisterLogic(){
    // **ACTIVITY**: Finish the code in this function (look at comments)
    if( ss1 ){    // Check if slide switch is ON
        if( pb1 && pb2 ){ // **ACTIVITY**: Check if Both pushbuttons are pressed
            // Turn BiLED1 OFF by setting both pins to the same value
            P2OUT |= 0x30;  // This sets both 4&5 to 1 (x11xxxxx)
            // **ACTIVITY**: Turn LED1 ON (add command below)
            P3OUT |= 0x01;

        }else if( pb1 ){ // **ACTIVITY**: Check if pushbutton 1 is pressed
            // Turn BiLED1 to 1 color by setting both pins to the opposite value
           P2OUT |= 0x20;  // Set 5 to 1 (xx1xxxxx)
           P2OUT &= ~0x10; // Then set 4 to 0 (xx10xxxx)
           // **ACTIVITY**: Turn LED1 OFF (add command below)
           P3OUT &= ~0x01;

        }else if( pb2 ){ // Check if pushbutton 2 is pressed
            // **ACTIVITY**: Turn BiLED1 to the other color by... and also turn LED1 ON
            P2OUT |= 0x10;
            P2OUT &= ~0x20;
            P3OUT |= 0x01;



        }else{
            // **ACTIVITY**: Turn everything off
            P3OUT &= ~0x01;
            P2OUT &= ~0x30;

        }
    }else{
        // Turn everything off
        P2OUT &= ~0x30;  // This sets both 4&5 to 0 (xx00xxxx)
        P3OUT &= ~0x01; // set pin 0 to 0 (xxxxxxx0)
    }
}

void DriverLibLogic(){
    // **ACTIVITY**: Finish the code in this function (look at comments)
    if( ss1 ){    // Check if slide switch is ON
        if( pb1 && pb2 ){ // **ACTIVITY**: Check if Both pushbuttons are pressed
            // Turn BiLED1 OFF by setting both pins to the same value
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4|GPIO_PIN5);
            // **ACTIVITY**: Turn LED1 ON (add command below)
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN0);

        }else if( pb1 ){ // **ACTIVITY**: Check if pushbutton 1 is pressed
            // Turn BiLED1 to 1 color by setting both pins to the opposite value
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
           // **ACTIVITY**: Turn LED1 OFF (add command below)
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0);

        }else if( pb2 ){ // Check if pushbutton 2 is pressed
            // **ACTIVITY**: Turn BiLED1 to the other color by... and also turn LED1 ON
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);

            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN0);

        }else{
            // **ACTIVITY**: Turn everything off
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0);
        }
    }else{
        // Turn everything off
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0);
    }
}

// Add interrupt functions last so they are easy to find
