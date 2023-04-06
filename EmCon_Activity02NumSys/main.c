// Activity: Number Systems
// ENGR-2350
// Name: Rueda, Maximillian
// RIN: 662021896

// We'll always add this include statement. This basically takes the
// code contained within the "engr_2350_msp432.h" file and adds it here.
#include "engr2350_msp432.h"

// The "main" function is the start of the program. When the code is
// run, it will sequentially go through each statement in the main
// from top to bottom.
int main(void)
{
    // We can declare variables inside of functions. These are called
    // known as "local variables" and cannot be accessed by other
    // functions in the program. Here we are creating the variables a, b, and c
    uint8_t a;  // uint8_t is an "unsigned integer" type that is 8 bits long
                // it can store any number from 0-255 [0 to 2^8-1]
                // it is equivalent to "unsigned char"
    int8_t b;   // int8_t is a "signed integer" type that is 8 bits long
                // it can store any number from -128-127 [-2^7 to (2^7)-1]
                // it is equivalent to "signed char"
    uint32_t c; // uint32_t is an "unsigned integer" type that is 32 bits long
                // it can store any number from 0-4,294,967,296 [0 to (2^32)-1]
                // it is [usually] equivalent to "unsigned int"


    // We call the "SysInit()" always first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Introduction print statements. This is just giving a title to the
    // output on the terminal. See the class website for instructions
    // on how to use the "printf()" function.
    printf("\r\n\n\n\n\n");
    printf("****************************\r\n");
    printf("**Activity: Number Systems**\r\n");
    printf("****************************\r\n");
    printf("\n\n");

    // **********************************
    // Do the activity requirements below
    // **********************************

    // Performing math. Note that after every line there is a ';'.  This
    // tells the compiler (thing that turns the code into a program) that
    // we are done with our statement and nothing following the 'j' should be
    // included in the operation.
    a = 0xa;

    // printing out the values. A warning to python programmers:
    //    printf() in C and print() in python is very different. You cannot
    //    use printf() to print a variable like such:
    //          printf(a);
    //    It must be done as shown below.
    printf("a = %u and 0x%x\r\n",a, a);
    a = 0x00;       printf("a = %u and 0x%x\r\n",a, a);
    a = 0b00000000; printf("a = %u and 0x%x\r\n",a, a);
    a = 0x10;       printf("a = %u and 0x%x\r\n",a, a);
    a = 0x0C;       printf("a = %u and 0x%x\r\n",a, a);
    a = 0x70;       printf("a = %u and 0x%x\r\n",a, a);
    a = 0xFF;       printf("a = %u and 0x%x\r\n",a, a);
    a = 0b11111111; printf("a = %u and 0x%x\r\n",a, a);
    a = 0x0100;     printf("a = %u and 0x%x\r\n",a, a);
    a = 0xFFFF;     printf("a = %u and 0x%x\r\n",a, a);

    printf("b = %u and 0x%x\r\n",b, b);
    b = 0x00;       printf("b = %u and 0x%x\r\n",b, b);
    b = 0b00000000; printf("b = %u and 0x%x\r\n",b, b);
    b = 0x10;       printf("b = %u and 0x%x\r\n",b, b);
    b = 0x0C;       printf("b = %u and 0x%x\r\n",b, b);
    b = 0x70;       printf("b = %u and 0x%x\r\n",b, b);
    b = 0xFF;       printf("b = %u and 0x%x\r\n",b, b);
    b = 0b11111111; printf("b = %u and 0x%x\r\n",b, b);
    b = 0x0100;     printf("b = %u and 0x%x\r\n",b, b);
    b = 0xFFFF;     printf("b = %u and 0x%x\r\n",b, b);


    printf("c = %u and 0x%x\r\n",c, c);
    c = 0x00;       printf("c = %u and 0x%x\r\n",c, c);
    c = 0b00000000; printf("c = %u and 0x%x\r\n",c, c);
    c = 0x10;       printf("c = %u and 0x%x\r\n",c, c);
    c = 0x0C;       printf("c = %u and 0x%x\r\n",c, c);
    c = 0x70;       printf("c = %u and 0x%x\r\n",c, c);
    c = 0xFF;       printf("c = %u and 0x%x\r\n",c, c);
    c = 0b11111111; printf("c = %u and 0x%x\r\n",c, c);
    c = 0x0100;     printf("c = %u and 0x%x\r\n",c, c);
    c = 0xFFFF;     printf("c = %u and 0x%x\r\n",c, c);


    c = 662021896;
    printf("Name: Max Rueda // RIN: %u", c);



    printf("\n\n--End of Program--\n\n");
    // A while() is a "loop" that will continuously run until the
    // quantity within the () is False (will run while the value is True).
    // In this case, there is nothing in the while loop as we just want the
    // program to lock up and not do anything. In other cases, lines of code
    // would be contained within.
    while(1){
    }
}
