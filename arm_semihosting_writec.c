char the_char = 'x';

static inline long
semihost_syscall (long id, long *data_block)
{
  register long a0 asm ("a0") = id;
  register long a1 asm ("a1") = (long) data_block;

  /* RISC-V semihosting trap sequence.  Must be uncompressed and must not
     cross page boundary.  */
  asm volatile (
    ".balign 16             \n"
    ".option push           \n"
    ".option norvc          \n"
    "slli zero, zero, 0x1f  \n"
    "ebreak                 \n"
    "srai zero, zero, 0x7   \n"
    ".option pop            \n"
      : "+r"(a0) : "r"(a1) : "memory");

  return a0;
}

int main() {
    register long a0 asm ("a0") = semihost_syscall(0x03 /*SEMIHOST_writec*/, &the_char);
    asm volatile ("ebreak");
}
