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
/* Get a halfword from RAM */
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
/* Get a double word from RAM */
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
/* Set a halfword of RAM */
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
/* Set a double word of RAM */
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


/* Reset a rv64i CPU */
void rv64i_cpu_reset(rv64i_cpu_t *cpu, uint64_t reset_vector);
/* Step a rv64i CPU */
void rv64i_cpu_step(rv64i_cpu_t *cpu, void *ram, uint64_t ram_size);
