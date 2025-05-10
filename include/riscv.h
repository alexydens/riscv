/*
 * The version of the RISC-V specification used for this project is dated to
 * May 2024. Volume I of the manual contains the base functionality, and can be
 * found here:
 * https://drive.google.com/file/d/1uviu1nH-tScFfgrovvFCrj7Omv8tFtkp/view?usp=drive_link
 * Volume II of the manual contains the priviledged functionality, and can be
 * found here:
 * https://drive.google.com/file/d/17GeetSnT5wW3xNuAHI95-SI1gPGd5sJ_/view?usp=drive_link
 * As far as I know, these documents can be generated with the contents of the
 * following git repository:
 * https://github.com/riscv/riscv-isa-manual
 */
/* Include guard */
#if !defined(RISCV_H)
#define RISCV_H

/* Includes */
#include <stdint.h>
#include <stdbool.h>

/* A RISC-V CPU using the rv64i base instruction set */
typedef struct {
  /* Program counter */
  uint64_t pc;
  /*
   * General purpose registers:
   * NOTE: the register x0 is a hardwired zero, so all writes to it are
   * discarded and all reads from it return zero.
   */
  uint64_t x[32];
} rv64i_cpu_t;

/* Reset a rv64i CPU */
extern void rv64i_cpu_reset(rv64i_cpu_t *cpu, uint64_t reset_vector);
/* Step a rv64i CPU */
extern void rv64i_cpu_step(rv64i_cpu_t *cpu, void *ram, uint64_t ram_size);

#endif /* RISCV_H */
