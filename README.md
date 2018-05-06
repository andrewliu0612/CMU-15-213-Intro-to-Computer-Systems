# CMU-15-213-Intro-to-Computer-Systems
Notes and labs for the course 15-213 Introduction to Computer Systems at CMU

## Integer representation
* Data types
    * char, short, int, long, float, double, pointer
    * Word size equals length of pointer datatype
* Bit-level operations
* Signed / unsigned conversion
* Byte ordering
    * Big Endian: Sun, PPC Mac, Internet
    * Little Endian: x86, ARM  
    <img src="Note_Images/endian.png" width="60%">


## IEEE 754
* Numeric form
    $(-1)^sM2^E$
* Encoding
    * s: sign bit
    * exp: E
    * frac: M  
    <img src="Note_Images/ieee754.png" width="60%">
* Three kinds of values
    * Denormalized: exp = 0
    * Normalized: 0 < exp < 11..11
    * Special: exp = 11.11 (e.g. inf & NaN)  
    <img src="Note_Images/ieee754range.png" width="80%">
* Roundings

## x86-64