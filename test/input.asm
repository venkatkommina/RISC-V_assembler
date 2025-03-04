.data
word1: .word 5
word2: .word 10, 15, 20
half1: .half 3
string1: .asciiz "Hello"
byte1: .byte 255

.text
main:
    add x1, x2, x3
    beq x1, x2, target
    lw x5, 16(x6)
    jal x0, target
    addi x1, x0, 3
target:
    addi x4, x5, 10
#target is 4 bytes a head of jal and 3 instructions ahead of beq