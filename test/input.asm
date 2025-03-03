.text
main:
    add x1, x2, x3
    beq x1, x2, target
    lw x5, 16(x6)
target:
    addi x4, x5, 10
