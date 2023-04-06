/**********************************************************************/
// ENGR-2350 Activity 3
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



// Add global variables here, as needed.



int main(void) /* Main Function */
{
    // Add local variables here, as needed.
    uint8_t a, b, c, d;

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Place initialization code (or run-once) code here
    a = 0x80;
    b = 0x1E;
    c = 0xC3;

    printf("BEGIN CALCULATIONS\r\n\n");

    printf("Problem 1.\r\n");
    d = a & b;
    printf("Result of d = a & b: %x\r\n", d);
    d = b & c;
    printf("Result of d = b & c: %x\r\n", d);
    d = b & 0xF0;
    printf("Result of d = b & 0xF0;: %x\r\n", d);
    d = c & 0x01;
    printf("Result of d = c & 0x01;: %x\r\n", d);
    d = c & ~b;
    printf("Result of d = c & ~b: %x\r\n", d);
    d = c & !b;
    printf("Result of d = c & !b: %x\r\n", d);

    printf("\r\n");
    printf("Problem 2.\r\n");
    d = a | b;
    printf("Result of d = a | b: %x\r\n", d);
    d = b | c;
    printf("Result of d = b | c: %x\r\n", d);
    d = b | 0xF0;
    printf("Result of d = b | 0xF0: %x\r\n", d);
    d = c | 0x01;
    printf("Result of d = c | 0x01: %x\r\n", d);
    d = c | ~b;
    printf("Result of d = c | ~b: %x\r\n", d);
    d = c | !b;
    printf("Result of d = c | !b: %x\r\n", d);

    printf("\r\n");
    printf("Problem 3.\r\n");
    printf("Result of b << 1: %x\r\n", (b << 1));
    printf("Result of b << 4: %x\r\n", (b << 4));
    printf("Result of b << 8: %x\r\n", (b << 8));
    printf("Result of b >> 1: %x\r\n", (b >> 1));
    printf("Result of b >> 4: %x\r\n", (b >> 4));

    printf("\r\n");
    printf("Problem 4.\r\n");
    printf("Result of ~b: %x\r\n", (~b));
    printf("Result of !b: %x\r\n", (!b));
    printf("Result of !(!b): %x\r\n", (!(!b)));
    printf("Result of a && b: %x\r\n", (a && b));
    printf("Result of b && c: %x\r\n", (b && c));
    printf("Result of a || b: %x\r\n", (a || b));
    printf("Result of (a + b) == (a | b): %x\r\n", ((a + b) == (a | b)));
    printf("Result of ~b != b: %x\r\n", (~b != b));

    printf("\r\n\n\n\n");


    //while(1){
        // Place code that runs continuously in here

    //}
}

// Add function declarations here as needed



// Add interrupt functions last so they are easy to find
