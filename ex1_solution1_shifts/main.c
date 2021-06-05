#include <stdio.h>
#include "ex1.h"

int main() {
	
	printf(">> Check 1 - Is Big Endian? ");
	printf("%d\n", is_big_endian());
	printf(">> Check 1 - FINISHED\n");

	printf(">> Check 2 - Merge Bytes\n");
	unsigned long Xs[5] = {0x89ABCDEF12893456,0x0000000000000000,0xABCABDEBFEBABDCE,0x1111111111111111,0x5994A123EF548FE4};
	unsigned long Ys[5] = {0xAB45A2B3AF3F1E67,0x76543210ABCDEF19,0x1111111111111111,0x1561561561561561,0x4821234561234878};
	unsigned long Zs[5] = {0x89ABCDEFAF3F1E67,0x00000000ABCDEF19,0xABCABDEB11111111,0x1111111161561561,0x5994A12361234878};
	for (int i = 0; i < 5; i++) {
		unsigned long check = merge_bytes(Xs[i], Ys[i]);
		if (check != Zs[i])
			printf("   (!) Error in index: %d\n", i);
	}
	printf(">> Check 2 - FINISHED\n");

	printf(">> Check 3 - Put Byte\n");
	unsigned long x = 0x123456789ABCDEAB;
	unsigned char b = 0xFF;
	unsigned long Ans[8] = {0xFF3456789ABCDEAB,0x12FF56789ABCDEAB,0x1234FF789ABCDEAB,0x123456FF9ABCDEAB,0x12345678FFBCDEAB,0x123456789AFFDEAB,0x123456789ABCFFAB,0x123456789ABCDEFF};
	unsigned long out_of_range_ans = 0x123456789ABCDEAB, check = 0;
	int i = -2;
	while (i < 0) {
		check = put_byte(x, b, i);
		if (check != out_of_range_ans)
			printf("   (!) Error in index: %d. Output: 0x%lx Answer: 0x%lx\n", i, check, out_of_range_ans);
		i++;
	}
	while (i < 8) {
		check = put_byte(x, b, i);
		if (check != Ans[i])
			printf("   (!) Error in index: %d. Output: 0x%lx Answer: 0x%lx\n", i, check, Ans[i]);
		i++;
	}
	while (i < 10) {
		check = put_byte(x, b, i);
		if (check != out_of_range_ans)
			printf("   (!) Error in index: %d. Output: 0x%lx Answer: 0x%lx\n", i, check, out_of_range_ans);
		i++;
	}
	printf(">> Check 3 - FINISHED\n");

	// printf("0x%lx\n", merge_bytes(0x89ABCDEF12893456, 0x76543210ABCDEF19));
	// printf("0x%lx\n", merge_bytes(0x89ABCDEF12893456, 0x76543210ABCDEF19));
	// printf("0x%lx\n", put_byte(0x12345678CDEF3456, 0xAB, 7));
	// printf("0x%lx\n", put_byte(0x12345678CDEF3456, 0xAB, 5));
	// printf("0x%lx\n", put_byte(0x12345678CDEF3456, 0xAB, 0));
	// printf("0x%lx\n", merge_bytes(0x56348912EFCDAB89, 0x19EFCDAB10325476));
	// printf("0x%lx\n", put_byte(0x5634EFCD78563412, 0xAB, 5));

}