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
  /* General purpose registers */
  uint64_t x[32];
} rv64i_cpu_t;

/* Reset a rv64i CPU */
extern void rv64i_cpu_reset(rv64i_cpu_t *cpu, uint64_t reset_vector);
/* Step a rv64i CPU */
extern void rv64i_cpu_step(rv64i_cpu_t *cpu, void *ram, uint64_t ram_size);

#endif /* RISCV_H */
