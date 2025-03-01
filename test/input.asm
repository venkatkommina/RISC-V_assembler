.text
main:
    add x1, x2, x3
    lw x5, 16(x6)
    sw x7, 32(x8)
    beq x9, x10, target
target:
    addi x11, x12, 10
