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

// Aux function that gets a number and return it's bytes in an array.
int* saveBytes(bytePtr start, size_t len) {
    int* bytesArray = (int*) malloc(len*sizeof(int));
	for (int i = 0; i < len; i++)
		bytesArray[i] = start[i];
    return bytesArray;
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
* Operation:    This function uses the function from the previous question to
                identify whether the local system is a little endian or big
                endian. According to this information, it chooses half of the
                bytes from the one given unsigned long and another half from
                the other an multiply each byte as much as needed to put it in
                the correct position in a new number. Then, it sums all the new
                numbers to one result unsigned long and return it.
*******************************************************************************/
unsigned long merge_bytes(unsigned long x, unsigned long y) {

    // Use byte-pointer to store each byte of the given numbers in an array.
    int *xBytes = saveBytes((bytePtr) &x, sizeof(unsigned long));
    int *yBytes = saveBytes((bytePtr) &y, sizeof(unsigned long));
    int len = sizeof(yBytes);

    // Initialize the return value to 0.
    unsigned long value = 0;

    // Split to cases according to system type.
    if (is_big_endian()) {
        int i = 0;
        while (i < len / 2) {
            value += (unsigned long) xBytes[i] << (len - i - 1) * len;
            i++;
        }
        while (i < len) {
            value += (unsigned long) yBytes[i] << (len - i - 1) * len;
            i++;
        }
    } else {
        int i = 0;
        while (i < len / 2) {
            value += (unsigned long) yBytes[i] << i * len;
            i++;
        }
        while (i < len) {
            value += (unsigned long) xBytes[i] << i * len;
            i++;
        }
    }

    // Free memory that no longer needed and return the calculated value.
    free(xBytes);
    free(yBytes);
    return value;

}

/*******************************************************************************
* Solution for question 3.
* Input:        Unsigned long, unsigned char and an int.
* Output:       Unsigned long with replaced byte as the question demands.
* Operation:    This function also uses is_big_endian() to identify system’s
                type. According to this, it works this way: let’s say we have
                a number 12345, b is 7 and i Is 3. So, the algo takes the 1,
                multiply it by 10000 than the 2 multiply it by 1000, the 3 by
                100 than the index is 3 to it takes 7 (not 4) and multiply it
                by 10, and then takes 5 and multiply it by 1. Then, it sums
                all the numbers and gets 12375. So that was a simple
                explanation of the idea behind the algo. The function does
                similar but for bytes.
*******************************************************************************/
unsigned long put_byte(unsigned long x, unsigned char b, int i) {

    // Use byte-pointer to store each byte of the given numbers in an array.
    int *xBytes = saveBytes((bytePtr) &x, sizeof(unsigned long));
    int len = sizeof(xBytes);

    // Initialize the return value to 0.
    unsigned long value = 0;

    // Split to cases according to system type.
    if (is_big_endian()) {
        int j = 0;
        while (j < i) {
            value += (unsigned long) xBytes[j] << (len - j - 1) * len;
            j++;
        }
        value += (unsigned long) b << (len - j - 1) * len;
        j++;
        while (j < len) {
            value += (unsigned long) xBytes[j] << (len - j - 1) * len;
            j++;
        }
    } else {
        int j = 0;
        while (j < len - i - 1) {
            value += (unsigned long) xBytes[j] << j * len;
            j++;
        }
        value += (unsigned long) b << j * len;
        j++;
        while (j < len) {
            value += (unsigned long) xBytes[j] << j * len;
            j++;
        }
    }
    
    // Free memory that no longer needed and return the calculated value.
    free(xBytes);
    return value;

}