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

/* Get 8-bit value at address in ram */
static inline uint8_t ram_get_8(
    void *ram, uint32_t ram_size, uint32_t addr
);
/* Get 16-bit value at address in ram */
static inline uint16_t ram_get_16(
    void *ram, uint32_t ram_size, uint32_t addr
);
/* Get 32-bit value at address in ram */
static inline uint32_t ram_get_32(
    void *ram, uint32_t ram_size, uint32_t addr
);
/* Set 8-bit value at address in ram */
static inline void ram_set_8(
    void *ram, uint32_t ram_size, uint32_t addr, uint8_t val
);
/* Set 16-bit value at address in ram */
static inline void ram_set_16(
    void *ram, uint32_t ram_size, uint32_t addr, uint16_t val
);
/* Set 32-bit value at address in ram */
static inline void ram_set_32(
    void *ram, uint32_t ram_size, uint32_t addr, uint32_t val
);

/* Reset a rv32i cpu */
void rv32i_reset(rv32i_t *cpu, uint32_t pc) {
  cpu->pc = pc;
  for (uint8_t i = 0; i < 32; i++) rv32i_setreg(cpu, i, 0);
}
/* Step a rv32i cpu */
void rv32i_step(rv32i_t *cpu, void *ram, uint32_t ram_size);
