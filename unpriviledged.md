# ISA Notes: Volume 1 (Unprivileged Architecture)
## Chapter 1 (Introduction)
## 1.1 (RISC-V Hardware Platform Terminology)
- a core contains an independent instruction fetch unit.
- a core can have many harts (hardware threads)
- coprocessors are attached to cores and contain some functionality not covered
in the spec
- accelerators operate autonomously from a core and is specialised for certain
tasks
## 1.2 (RISC-V Software Execution Environments and Harts)
- the execution environment interface (EEI) defines the initial state, the
number and type of harts, the layout of memory and I/O, and any interrupt or
exception handling.
## 1.3 (RISC-V ISA Overview)
- XLEN is used to refer to the word size of the instruction set (e.g. for RV32I,
XLEN=32)
- RV32I: 32-bit base integer instruction set
- RV64I: 64-bit base integer instruction set
- RV32E: 32-bit base integer instruction set (embedded)
- RV64E: 64-bit base integer instruction set (embedded)
- the regular instruction sets have 32 general-purpose registers, the embedded
versions have 16
> Common extensions (not all listed in ISA V1 1.3)
- extension M: integer multiplication and division
- extension A: atmoic instructions
- extension F: single-precision floating point
- extension D: double-precision floating point
- extension Ziscr: control and status register instructions
- extension Zifencei: instruction-fetch fence
- extension G: extensions MAFDZiscrZifencei
- extension C: compressed instructions
- extension V: vector operations
- extension Zk: scalar cryptography
- extension H: hypervisor
- extension S: supervisor-level instructions
## 1.4 (Memory)
- each hart has its own address space
- address spaces are byte-addressable and 2^XLEN in size
- halfword: 16 bits
- word: 32 bits
- doubleword: 64 bits
- quadword: 128 bits
- memory operations allow addresses to "wrap around" the address space, going
from address 2^XLEN-1 to address 0
- main memory is memory in a hart's address space where writing to/reading from
will have no visible side effects
- I/O devices are portions of a hart's address space where writing to/reading
from can have visible side effects
- explicit memory operations include those performed by load/store instructions
- implicit memory operations include those performed to fetch instructions
- the execution environment can define what implicit memory operations happen
for each instruction
- if an implicit memory operation doesn't cause any side effects, it can be
carried out by the EEI at any time
- to control the order order of implicit memory operations, use related fence
operations
- the percieved ordering of memory operations, according to each hart (several
harts may percieve different orders of memory operations), conforms to either
RISC-V Weak Memory Ordering (RVWMO, Chapter 17) or Total Store Ordering (Chapter
18)
## 1.5 (Base Instruction-Length Encoding)
- IALIGN is used to refer to the alignment required of instructions
- ILEN is used to refer to the maximum size of an instruction, and is always a
multiple of IALIGN
- for the base instruction sets, IALIGN=32 bits
- for the compressed instruction extension -  C
- all 32-bit instructions have their lowest two bits set to 11
