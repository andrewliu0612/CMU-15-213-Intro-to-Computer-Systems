## This is an x86-64 bomb for self-study students. 

### Phase 1:
Input a string, and compare it with the string at 0x402400. Bomb defused if two strings are equal.

### Phase 2:
Input 6 numbers from stdin and compares with 1, 2, 4, 8, 16, 32.

### Phase 3:
Input 2 integers. If first integer is 1, PC jumps to 0x400fb9, to compare the second argument with 0x137.

### Phase 4:
Input 2 integers. The first integer has to be below or equal 0xe, and the second integer should be 0. Phase 4 calls subroutine func4, whose return value should be 0, requiring first integer to be 7. 

### Phase 5:
Input a string of length 6. Each char % 16 represents an offset in string at 0x4024b0("maduiersnfotvbyl"). The offset has to match string at 0x6038c0("flyers"). 

### Phase 6:
Input 6 distinct numbers between 1-6. Form a singly-linked list that is sorted, based on the order of input.
