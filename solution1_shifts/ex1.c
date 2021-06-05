// Shlomi Ben-Shushan

#include "ex1.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned char *bytePtr;

// Aux function that iterate bytes and print them to the screen.
void showBytes(bytePtr start, size_t len) {
	for (int i = 0; i < len; i++)
		printf("%.2x ", start[i]);
	printf("\n");
}

// Aux function that gets an unsigned long number and print it's bytes.
void showUnsignedLong(unsigned long x) {
	showBytes((bytePtr) &x, sizeof(unsigned long));
}

/*******************************************************************************
* Solution for question 1.
* Input:        None.
* Output:       1 - the system is a Big endian, 0 - it is a little endian.
* Operation:    Creates a long and read it's first byte as stored in the memory.
                If it's equal to 1, than the system is a little endian.
                Otherwise the system is a big endian.
*******************************************************************************/
int is_big_endian() {
    long num = 1;
    if (*(char *)&num == 1) 
        return 0;
    return 1;
}

/*******************************************************************************
* Solution for question 2.
* Input:        Two unsigned longs.
* Output:       Combined unsigned long as the question request.
* Operation:    This function creates a new number by taking x and shift it
                halfway to the right and then back to its original position
                while insert zeros to the right. Afterwards, it creates another
                number by taking y and shift it halfway to the left and then
                back to the original position. Then return the sum of the two
                new numbers. Because of mathematical features, it doesn’t
                matter what system the program run on.
*******************************************************************************/
unsigned long merge_bytes(unsigned long x, unsigned long y) {

    // Initialize length variable and define 'halfway'.
    int len = sizeof(unsigned long);
    unsigned long halfWay = len * len / 2;

    // Creates xPart by shifting x halfway to the right and halfway left.
    unsigned long xPart = x >> halfWay;
    xPart = xPart << halfWay;

    // Creates yPart by shifting y halfway to the left and halfway right.
    unsigned long yPart = y << halfWay;
    yPart = yPart >> halfWay;

    // Return sun of xPart and yPart as unsigned long.
    return xPart + yPart;

    
}

/*******************************************************************************
* Solution for question 3.
* Input:        Unsigned long, unsigned char and an int.
* Output:       Unsigned long with replaced byte as the question demands.
* Operation:    Like the previous solution, this function uses shifts to create
                new numbers. But this time it creates 3 new numbers: right is
                created by shifting (i+1)*length to the left and then back to
                normal, left is created by shifting (length - i)*length to the
                right and then back to normal and middle is created by taking b
                and shift it to its correct position according to I. Finally,
                the function sums the three and return.
*******************************************************************************/
unsigned long put_byte(unsigned long x, unsigned char b, int i) {

    // Initialize length variable.
    int len = sizeof(unsigned long);

    // Handle edge-case where i is out of range.
    if (i < 0 || i >= len) {
        return x;
    }
    
    // Creates "right".
    unsigned long right = 0;
    if (i != len - 1) {
        right = x << (i + 1) * len;
        right = right >> (i + 1) * len;
    }
    
    // Creates "left".
    unsigned long left = 0;
    if (i != 0) {
        left = x >> (len - i) * len;
        left = left << (len - i) * len;
    }
    
    // Creates "middle".
    unsigned long middle = (unsigned long)b;
    middle = middle << (len - i - 1) * len;
    
    // Return the sum as unsigned long.
    return (left + middle + right);

}


