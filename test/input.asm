.data
word1: .word 5
word2: .word 10, 15, 20
.word 10
half1: .half 3
string1: .asciiz "Hello"
.byte 255

.text
main:
    add x1, x2, x3
    andi x5, x6, 10
    beq x1, x2, target
    jal x0, target
    jalr x0, x3, target  # This should replace `target` with its absolute address
    addi x1, x0, 3
target:
    addi x4, x5, 10
#target is 4 bytes a head of jal and 3 instructions ahead of beq