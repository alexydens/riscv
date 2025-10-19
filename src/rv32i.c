/* Implements rv32i.h */
#include <rv32i.h>

/* Set the value of a rv32i regsiter */
void nv32i_setreg(rv32i_t *cpu, uint8_t reg, uint32_t val) {
  if (reg == 0) return;
  cpu->regs[reg] = val;
}
/* Get the value of a rv32i register */
uint32_t rv32i_getreg(rv32i_t *cpu, uint8_t reg) {
  if (reg == 0) return 0;
  return cpu->regs[reg];
}

/* Reset a rv32i cpu */
void rv32i_reset(rv32i_t *cpu, uint32_t pc) {
  cpu->pc = pc;
  for (uint8_t i = 0; i < 32; i++) rv32i_setreg(cpu, i, 0);
}
/* Step a rv32i cpu */
void rv32i_step(rv32i_t *cpu, void *ram, uint32_t ram_size);
