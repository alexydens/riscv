/* Implements the functions related to rv64i_cpu_t */
#include <riscv.h>

/* Includes */
#include <assert.h>

/* Get a value from a register */
#define GET_REG(reg) (cpu->x[reg])
/* Set a value to a register */
#define SET_REG(reg, val) do {\
  if (reg != 0) cpu->x[reg] = val;\
} while (0)

/* Endianness */
typedef enum {
  LITTLE_ENDIAN,
  BIG_ENDIAN
} endianness_t;

/* Get a byte from RAM */
static inline uint8_t ram_get_b(
    void *ram,
    uint64_t ram_size,
    uint64_t addr
) {
  assert(addr < ram_size);
  return ((uint8_t *)ram)[addr];
}
/* Get a half-word from RAM */
static inline uint16_t ram_get_h(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    endianness_t endianness
) {
  if (endianness == BIG_ENDIAN) {
    return (ram_get_b(ram, ram_size, addr)<<8)
      | ram_get_b(ram, ram_size, addr+1);
  }
  return ram_get_b(ram, ram_size, addr)
    | (ram_get_b(ram, ram_size, addr+1)<<8);
}
/* Get a word from RAM */
static inline uint32_t ram_get_w(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    endianness_t endianness
) {
  if (endianness == BIG_ENDIAN) {
    return (ram_get_b(ram, ram_size, addr)<<24)
      | (ram_get_b(ram, ram_size, addr+1)<<16)
      | (ram_get_b(ram, ram_size, addr+2)<<8)
      | ram_get_b(ram, ram_size, addr+3);
  }
  return ram_get_b(ram, ram_size, addr)
    | (ram_get_b(ram, ram_size, addr+1)<<8)
    | (ram_get_b(ram, ram_size, addr+2)<<16)
    | (ram_get_b(ram, ram_size, addr+3)<<24);
}
/* Get a double-word from RAM */
static inline uint64_t ram_get_d(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    endianness_t endianness
) {
  if (endianness == BIG_ENDIAN) {
    return ((uint64_t)ram_get_b(ram, ram_size, addr)<<56)
      | ((uint64_t)ram_get_b(ram, ram_size, addr+1)<<48)
      | ((uint64_t)ram_get_b(ram, ram_size, addr+2)<<40)
      | ((uint64_t)ram_get_b(ram, ram_size, addr+3)<<32)
      | ((uint64_t)ram_get_b(ram, ram_size, addr+4)<<24)
      | ((uint64_t)ram_get_b(ram, ram_size, addr+5)<<16)
      | ((uint64_t)ram_get_b(ram, ram_size, addr+6)<<8)
      | (uint64_t)ram_get_b(ram, ram_size, addr+7);
  }
  return (uint64_t)ram_get_b(ram, ram_size, addr)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+1)<<8)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+2)<<16)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+3)<<24)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+4)<<32)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+5)<<40)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+6)<<48)
    | ((uint64_t)ram_get_b(ram, ram_size, addr+7)<<56);
}

/* Set a byte of RAM */
static inline void ram_set_b(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    uint8_t val
) {
  assert(addr < ram_size);
  ((uint8_t *)ram)[addr] = val;
}
/* Set a half-word of RAM */
static inline void ram_set_h(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    uint16_t val,
    endianness_t endianness
) {
  if (endianness == BIG_ENDIAN) {
    ram_set_b(ram, ram_size, addr, val >> 8);
    ram_set_b(ram, ram_size, addr+1, val);
  } else {
    ram_set_b(ram, ram_size, addr, val);
    ram_set_b(ram, ram_size, addr+1, val >> 8);
  }
}
/* Set a word of RAM */
static inline void ram_set_w(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    uint32_t val,
    endianness_t endianness
) {
  if (endianness == BIG_ENDIAN) {
    ram_set_b(ram, ram_size, addr, val >> 24);
    ram_set_b(ram, ram_size, addr+1, val >> 16);
    ram_set_b(ram, ram_size, addr+2, val >> 8);
    ram_set_b(ram, ram_size, addr+3, val);
  } else {
    ram_set_b(ram, ram_size, addr, val);
    ram_set_b(ram, ram_size, addr+1, val >> 8);
    ram_set_b(ram, ram_size, addr+2, val >> 16);
    ram_set_b(ram, ram_size, addr+3, val >> 24);
  }
}
/* Set a double-word of RAM */
static inline void ram_set_d(
    void *ram,
    uint64_t ram_size,
    uint64_t addr,
    uint64_t val,
    endianness_t endianness
) {
  if (endianness == BIG_ENDIAN) {
    ram_set_b(ram, ram_size, addr, val >> 56);
    ram_set_b(ram, ram_size, addr+1, val >> 48);
    ram_set_b(ram, ram_size, addr+2, val >> 40);
    ram_set_b(ram, ram_size, addr+3, val >> 32);
    ram_set_b(ram, ram_size, addr+4, val >> 24);
    ram_set_b(ram, ram_size, addr+5, val >> 16);
    ram_set_b(ram, ram_size, addr+6, val >> 8);
    ram_set_b(ram, ram_size, addr+7, val);
  } else {
    ram_set_b(ram, ram_size, addr, val);
    ram_set_b(ram, ram_size, addr+1, val >> 8);
    ram_set_b(ram, ram_size, addr+2, val >> 16);
    ram_set_b(ram, ram_size, addr+3, val >> 24);
    ram_set_b(ram, ram_size, addr+4, val >> 32);
    ram_set_b(ram, ram_size, addr+5, val >> 40);
    ram_set_b(ram, ram_size, addr+6, val >> 48);
    ram_set_b(ram, ram_size, addr+7, val >> 56);
  }
}

/* What to do with an invalid instruction */
static inline void invalid_instr(void) {
  assert(0);
}

/* Reset a rv64i CPU */
void rv64i_cpu_reset(rv64i_cpu_t *cpu, uint64_t reset_vector) {
  /* Set program counter to reset vector */
  cpu->pc = reset_vector;
  /* Zero out the x0 register (required) and the others (not required) */
  for (uint8_t i = 0; i < 32; i++)
    cpu->x[i] = 0;
}
/* Step a rv64i CPU */
void rv64i_cpu_step(rv64i_cpu_t *cpu, void *ram, uint64_t ram_size) {
  /* Fetch instruction */
  /*
   * The program counter must be half-word aligned, but doesn't have to be word
   * aligned, to allow for compressed instructions, which only take up one
   * half-word, to be mixed freely with regular instructions.
   */
  assert(cpu->pc % 2 == 0);
  uint32_t instr = ram_get_w(ram, ram_size, cpu->pc, LITTLE_ENDIAN);

  /* Decode instruction */
  /*
   * Fields of instructions:
   * - opcode: operation type.
   * - rd: destination register.
   * - rs1: source register 1.
   * - rs2: source register 2.
   * - funct3: 3-bit function code.
   * - funct7: 7-bit function code.
   * Instruction types:
   * - R-type: two source registers, one destination register.
   * - I-type: a source register, destination regsiter and 12-bit immediate.
   * - S-type: two source registers and a 12-bit immediate.
   * - B-type: for branching, two source registers and a 12-bit immediate.
   * - U-type: a destination register and the upper 20 bits of an immediate.
   * - J-type: a destination register and a 20 bit immediate.
   * For exact instruction encoding, refer either to the code for the decode
   * stage below or Volume I of the ISA Manual.
   */
  uint8_t opcode = instr & 0x7f;
  uint8_t rd = (instr >> 7) & 0x1f;
  uint8_t rs1 = (instr >> 15) & 0x1f;
  uint8_t rs2 = (instr >> 20) & 0x1f;
  uint8_t funct3 = (instr >> 12) & 0x7;
  uint8_t funct7 = (instr >> 25) & 0x7f;
  uint32_t i_imm = (0xfffff800 * (instr >> 31))                                 
      | ((instr >> 20) & 0x7ff);                                                
  uint32_t s_imm = (0xfffff800 * (instr >> 31))                                 
      | ((instr >> 7) & 0x1f)                                                   
      | ((instr >> 20) & 0x7e0);                                                
  uint32_t b_imm = (0xfffff000 * (instr >> 31))                                 
      | ((instr >> 7) & 0x1e)                                                   
      | ((instr >> 20) & 0x7e0)                                                 
      | ((instr << 4) & 0x800);                                                 
  uint32_t u_imm = instr & 0xfffff000;                                          
  uint32_t j_imm = (0xfff00000 * (instr >> 31))                                 
      | ((instr >> 20) & 0x7fe)                                                 
      | ((instr >> 9) & 0x800)                                                  
      | (instr & 0x000ff000);

  /* Execute instruction */
  bool pc_changed = false;
  switch (0) {
    default: /* Opcode not recognized */
      invalid_instr();
      break;
  }

  /* Increment program counter */
  if (!pc_changed)
    cpu->pc += 4;
}
