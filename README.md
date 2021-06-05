# Endiadns

This repo documents my solutions to the first and the second exercises in Computer Structure course I took as part of Computer Science B.Sc at Bar-Ilan university.
The assignments was about data representation in the computer by Little and Big Endians, and differences between Windwos, Unix and Mac files.

## Part 1

### Questions

The soulution ex1.c answer the following questions:

1. Write a function in C language named is_big_endian() which return 1 if the current program compiled and running on a Big Endian Machine, and 0 otherwise (Little Endian). Assume a word is at size of a long data-type.

2. Write a function in C language name merge_byte() that gets two unsigned longs x and y, and return a word which half of its bytes are from y starting from the LSB, the the rest of x.
For example, if x = 0x89ABCDEF12893456 and y = 0x76543210ABCDEF19, than return .0x89ABCDEFABCDEF19.

3. For a word at size w bits, we'll number its bytes from 0 (LSb) to w/8-1 (MSb). Write a function in C language named put_byte() that gets unsigned ling x, unsigned char b and int i, and return the x after the w/8-1 byte of it replaced by b/
For example:
put_byte(0x12345678CDEF3456, 0xAB, 2) --> 0x1234AB78CDEF3456
put_byte(0x12345678CDEF3456, 0xAB, 0) --> 0xAB345678CDEF3456

### Solutions

In this repo you can find two solutions for the question above.
- Solution using shifts - in the directory solution1_shifts.
- Solution using Byte-Pointer - in the directory solution2_bytepointer.



![image](https://user-images.githubusercontent.com/72878018/120885771-41682080-c5f3-11eb-84b9-1cc1dc742cd6.png)


## Part 2

The difference between different operating systems (Mac, Unix, Windows) is also expressed in the encoding method of different characters.
In this part we focused on the difference in saving a file in Little and Big Endians, and the difference in encoding a line-break character.
This solution contains an implementation a program that gets file name and create a new file according to the given flags.



![image](https://user-images.githubusercontent.com/72878018/120886686-ca815680-c5f7-11eb-980e-00e6cb3a2565.png)
