/* Include guard */
#ifndef RV32I_H
#define RV32I_H

/* Includes */
#include <stdint.h>

/* rv32i */
typedef struct {
  uint32_t pc;
  uint32_t regs[32];
} rv32i_t;

/* Set the value of a rv32i regsiter */
extern void rv32i_setreg(rv32i_t *cpu, uint8_t reg, uint32_t val);
/* Get the value of a rv32i register */
extern uint32_t rv32i_getreg(rv32i_t *cpu, uint8_t reg);

/* Reset a rv32i cpu */
extern void rv32i_reset(rv32i_t *cpu, uint32_t pc);
/* Step a rv32i cpu */
extern void rv32i_step(rv32i_t *cpu, void *ram, uint32_t ram_size);

#endif /* RV32I_H */
