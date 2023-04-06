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
// This is one implementation for creating a custom struct variable type named "homework_t"
typedef struct _homework_t {
    float average;      // The struct has a "field" for each value listed
    float stdev;        // in the "Homework Gradebook" table.
    uint8_t min;        // Note that the fields can be all different types
    uint8_t max;
    uint16_t subs;
} homework_t;

// Prototype. Add above main function, below the homework_t definition.
float remove_min_from_avg(homework_t *homework);


// Add global variables here, as needed.
homework_t hw1;
homework_t hwArray[5];


int main(void) /* Main Function */
{
    // Add local variables here, as needed.

    // We always call the "SysInit()" first to set up the microcontroller
    // for how we are going to use it.
    SysInit();

    // Place initialization code (or run-once) code here
    printf("====PROGRAM START====\r\n");

    hw1.average = 94.2;
    hw1.stdev = 10.8;
    hw1.min = 55;
    hw1.max = 100;
    hw1.subs = 109;

    printf("Homework 1 (ONLY) Stats\r\n"
           "    Average: %.2f\r\n"
           "  Std. Dev.: %.2f\r\n"
           "    Minimum: %u\r\n"
           "    Maximum: %u\r\n"
           "Submissions: %u\r\n",
           hw1.average,hw1.stdev,hw1.min,
           hw1.max,hw1.subs);

    printf("====Array of HW Structs====\r\n");
    uint8_t i;
    for(i=0;i<5;i++){
        if (i==0){
            hwArray[i].average = 94.2; hwArray[i].stdev = 10.8;
            hwArray[i].min = 55; hwArray[i].max = 100;
            hwArray[i].subs = 109;
        }
        else if (i==1){
            hwArray[i].average = 76.7; hwArray[i].stdev = 12.6;
            hwArray[i].min = 40; hwArray[i].max = 100;
            hwArray[i].subs = 106;
        }
        else if (i==2){
            hwArray[i].average = 84.5; hwArray[i].stdev = 15.9;
            hwArray[i].min = 25; hwArray[i].max = 100;
            hwArray[i].subs = 101;
        }
        else if (i==3){
            hwArray[i].average = 92.1; hwArray[i].stdev = 12.6;
            hwArray[i].min = 45; hwArray[i].max = 100;
            hwArray[i].subs = 99;
        }
        else if (i==4){
            hwArray[i].average = 66.3; hwArray[i].stdev = 27.3;
            hwArray[i].min = 10; hwArray[i].max = 100;
            hwArray[i].subs = 99;
        }
        printf("Homework %d Stats\r\n"
                   "    Average: %.2f\r\n"
                   "  Std. Dev.: %.2f\r\n"
                   "    Minimum: %u\r\n"
                   "    Maximum: %u\r\n"
                   "Submissions: %u\r\n",
                   i, hwArray[i].average,hwArray[i].stdev,hwArray[i].min,hwArray[i].max,hwArray[i].subs);
    }

    printf("====Pointers w/ Structs====\r\n");
    float result1 = remove_min_from_avg(&hw1);
    printf("Avg. for HW1: %f\r\n", result1);
    float result2 = remove_min_from_avg(&hwArray[1]);
    printf("Avg. for HW1: %f\r\n", result2);

    while(1){
        // Place code that runs continuously in here

    }
}

// Add function declarations here as needed
// Declaration. Add below main function
// This function calculates the average of a homework without the minimum grade.
// The function will return the value of the new average.
float remove_min_from_avg(homework_t *homework){
    float homework_sum = homework->average*homework->subs;
    homework_sum -= homework->min;
    return homework_sum/(homework->subs-1);
}


// Add interrupt functions last so they are easy to find
