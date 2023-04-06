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


// Add global variables here, as needed.
uint32_t array1[] = {1,2,3,4,5,6,7,8,9,10};


int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Place initialization code (or run-once) code here
    uint8_t i, j;       // Two loop variables

    printf("====Array from 1-10====\r\n");
    for(i=0;i<10;i++){ // Loop through the indices of the array
        printf("Element %u is:\t%u\r\n",i,array1[i]);
    }

    printf("====Randomized Array from 0-20====\r\n");
    uint32_t array2[10];
    uint8_t topRng = 21; uint8_t botRng = 0;
    for(i=0;i<10;i++){
        array2[i] = botRng + (rand() % (topRng - botRng));
    }
    for(i=0;i<10;i++){ // Loop through the indices of the array
        printf("Element %u is:\t%u\r\n",i,array2[i]);
    }

    printf("====Multiplication Table for Curated Values====\r\n");
    uint32_t multTable[10][10];
    for(i=0;i<10;i++){  // Outer loop
        for(j=0;j<10;j++){  // Inner loop
            multTable[i][j] = array1[i]*array2[j];
        }
    }
    // This portion prints one of the input arrays column header (horizontal)
    printf("\n\n\t");   // Move down a couple lines and skip the first "column"
    for(i=0;i<10;i++){
        printf("\t%u",array1[i]); // Print each element in the array, one each column (tab)
    }
    printf("\r\n"); // move to the next line to print the table

    // This portion prints the other input array as the row headers (vertical)
    // and the multiplication table (2D array)
    for(i=0;i<10;i++){
        printf("\t%u",array2[i]); // Print the i-th element of the input array
        for(j=0;j<10;j++){
            printf("\t%u",multTable[j][i]); // print all the elements of the i-th row for the table
        }
        printf("\r\n"); // move to next line
    }

    while(1){
        // Place code that runs continuously in here

    }
}

// Add function declarations here as needed

// Add interrupt functions last so they are easy to find
