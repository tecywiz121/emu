Introduction
============

Motivation
----------

Tomato was created as a learning experience, both in processor design and in efficient emulation.
Most of the reasoning behind Tomato's design choices is easy to see when keeping that in mind.

Units
-----

Since Tomato has a rather interesting memory model, the following unit definitions will be used
throughout this document. Whenever possible, the use of "byte" shall be avoided, since it is
ambiguous.

    Unit        Size        Prefixes

    octet        8 Bits     o, Kio, Mio
    short       16 Bits         -
    word        32 Bits         -

Resources
=========

Registers
---------

Tomato has seven 32-bit registers that are accessible to programmers. Register 7 is the program counter,
while registers one through 6 are general purpose.  The six general purpose registers are: A, B, C, D, E, F.
The program counter may be accessed as PC.

Each register may also be accessed as two 16-bit registers with XH and XL where X is one of the given names
above.  XH represents the most significant short, and XL represents the least significant short of the register
X.

Finally, each register may also be accessed as a sequence of octets.  X[0], X[1], X[2], X[3] represent the four
octets that comprise the register X. X[0] is the most significant octet, and X[3] is the least significant.

Stack
-----

TODO

Has three hidden registers, current, bottom, and max size.

Pages
-----

The processor supports a number of pages to implement virtual memory. The maximum number of pages is implementation
defined, but must be greater than or equal to 4096.

Instruction Set
===============

Legend
------

    r8, r16, r32                        -   Register with 8, 16, or 32 bits, respectively                                           (5, 4, 3)
    addr                                -   Signed address to a 32 bit value; relative to program counter                           (10)
    inst                                -   Mnemonic for an opcode
    dest                                -   The first argument to an instruction, usually the destination for any result
    op                                  -   The second argument to an instruction, usually only read during execution
    o                                   -   One of lt, lte, gt, gte                                                                 (2)
    e                                   -   One of eq, neq                                                                          (1)

Reserved Behaviour
------------------

If execution reaches a reserved instruction a trap will be raised. Do not rely on this to generate traps, it is likely to change.

Instructions
------------

    Memory Layout                       Inst    Dest    Op

    0 1 0|r r r|x x x x x x x x x x     LDD     <r32>,  <addr>              -   Load the value at op into dest
    0 1 1|r r r|x x x x x x x x x x     STO     <r32>,  <addr>              -   Store dest into op
    1 0 0|r r r h h|x x x x x x x x     SET     <r8>,   <c8>                -   Set dest to op
    1 0 1|0|o o|x x x x x x x x x x     JCS     <o>,    <addr>              -   Jump to dest if the last compare matches the mode specified by dest (signed)
    1 0 1|1|o o|x x x x x x x x x x     JCU     <o>,    <addr>              -   Jump to dest if the last compare matches the mode specified by dest (unsigned)
    1 1 0|0 0 e|x x x x x x x x x x     JEQ     <e>,    <addr>              -   Jump to dest if the last compare matches the mode specified by dest (equality only)
    1 1 0|0 1 x x x x x x x x x x x     *RESERVED*
    1 1 0|1 x x x x x x x x x x x x     *RESERVED*
    1 1 1|r r r|x x x x x x x x x x     SET     <r32>,  <addr>              -   Set dest to op (NB: this is PC relative, so more of a jump than a set)

    0 0 1 0 0 0 0 0|r r r h|r r r h     CMP     <r16>,  <r16>               -   Compare two values
    0 0 1 0 0 0 0 1|r r r h|r r r h     CPY     <r16>,  <r16>               -   Set dest to the same thing as op
    0 0 1 0 0 0 1 0|r r r h|r r r h     ADD     <r16>,  <r16>               -   Add op to dest and put the result in dest
    0 0 1 0 0 0 1 1|r r r h|r r r h     SUB     <r16>,  <r16>               -   Subtract op from dest and put the result in dest
    0 0 1 0 0 1 0 0|r r r h|r r r h     MUL     <r16>,  <r16>               -   Multiply op by dest and put the result in dest
    0 0 1 0 0 1 0 1|r r r h|r r r h     DIV     <r16>,  <r16>               -   Divide dest by op and put the result in dest
    0 0 1 0 0 1 1 0|r r r h|r r r h     MOD     <r16>,  <r16>               -   Divide dest by op and put the remainder in dest
    0 0 1 0 0 1 1 1|r r r h|r r r h     AND     <r16>,  <r16>               -   Bitwise AND dest and op, put the result in dest
    0 0 1 0 1 0 0 0|r r r h|r r r h     ORR     <r16>,  <r16>               -   Bitwise OR dest and op, put the result in dest
    0 0 1 0 1 0 0 1|r r r h|r r r h     XOR     <r16>,  <r16>               -   Bitwise exclusive OR dest and op, put the result in dest
    0 0 1 0 1 0 1 0|r r r h|r r r h     SHL     <r16>,  <r16>               -   Logical shift left of dest by op, put the result in dest
    0 0 1 0 1 0 1 1|r r r h|r r r h     SHR     <r16>,  <r16>               -   Logical shift right of dest by op, put the result in dest
    0 0 1 0 1 1 0 0|r r r h|r r r h     SAR     <r16>,  <r16>               -   Arithmetic shift right of dest by op, put the result in dest
    0 0 1 0 1 1 0 1|r r r h|r r r h     ROR     <r16>,  <r16>               -   Rotate dest right by op bits, put the result in dest
    0 0 1 0 1 1 1 x|x x x x x x x x     *RESERVED*
    0 0 1 1 x x x x|x x x x x x x x     *RESERVED*

    0 0 0 1 x x x x x|x x x x x x x     *RESERVED*

    0 0 0 0 1 0 0 0 0 0|r r r|r r r     CMP     <r32>,  <r32>               -   Compare two values
    0 0 0 0 1 0 0 0 0 1|r r r|r r r     CPY     <r32>,  <r32>               -   Set dest to the same thing as op
    0 0 0 0 1 0 0 0 1 0|r r r|r r r     ADD     <r32>,  <r32>               -   Add op to dest and put the result in dest
    0 0 0 0 1 0 0 0 1 1|r r r|r r r     SUB     <r32>,  <r32>               -   Subtract op from dest and put the result in dest
    0 0 0 0 1 0 0 1 0 0|r r r|r r r     MUL     <r32>,  <r32>               -   Multiply op by dest and put the result in dest
    0 0 0 0 1 0 0 1 0 1|r r r|r r r     DIV     <r32>,  <r32>               -   Divide dest by op and put the result in dest
    0 0 0 0 1 0 0 1 1 0|r r r|r r r     MOD     <r32>,  <r32>               -   Divide dest by op and put the remainder in dest
    0 0 0 0 1 0 0 1 1 1|r r r|r r r     AND     <r32>,  <r32>               -   Bitwise AND dest and op, put the result in dest
    0 0 0 0 1 0 1 0 0 0|r r r|r r r     ORR     <r32>,  <r32>               -   Bitwise OR dest and op, put the result in dest
    0 0 0 0 1 0 1 0 0 1|r r r|r r r     XOR     <r32>,  <r32>               -   Bitwise exclusive OR dest and op, put the result in dest
    0 0 0 0 1 0 1 0 1 0|r r r|r r r     SHL     <r32>,  <r32>               -   Logical shift left of dest by op, put the result in dest
    0 0 0 0 1 0 1 0 1 1|r r r|r r r     SHR     <r32>,  <r32>               -   Logical shift right of dest by op, put the result in dest
    0 0 0 0 1 0 1 1 0 0|r r r|r r r     SAR     <r32>,  <r32>               -   Arithmetic shift right of dest by op, put the result in dest
    0 0 0 0 1 0 1 1 0 1|r r r|r r r     ROR     <r32>,  <r32>               -   Rotate dest right by op bits, put the result in dest
    0 0 0 0 1 0 1 1 1 x|x x x x x x     *RESERVED*
    0 0 0 0 1 1 0 0 0 0|r r r|r r r     LDD     <r32>,  <r32>               -   Load the value at the location specified by op into dest
    0 0 0 0 1 1 0 0 0 1|r r r|r r r     STO     <r32>,  <r32>               -   Store the value of dest into the location specified by op
    0 0 0 0 1 1 0 0 1 0|r r r|r r r     SKI     <r32>,  <r32>               -   (Re)initialize the stack, setting the base at dest, with a maximum size of op
    0 0 0 0 1 1 0 0 1 1|r r r|r r r     SKG     <r32>,  <r32>               -   Get the stack base pointer into dest, and the maximum size into op
    0 0 0 0 1 1 0 1 0 0|r r r|r r r     SKR     <r32>,  <r32>               -   Reserve a number, equal to op, of 32-bit values on the stack and put the address into dest
    0 0 0 0 1 1 0 1 0 1|x x x x x x     *RESERVED*
    0 0 0 0 1 1 0 1 1 x|x x x x x x     *RESERVED*
    0 0 0 0 1 1 1 0 0 0|r r r|r r r     MMP     <r32>,  <r32>               -   Map a virtual page pointer (in dest), to a physical address (in op) (see section on paging)
    0 0 0 0 1 1 1 0 0 1|r r r|r r r     MMG     <r32>,  <r32>               -   Look up the mapping for a virtual address (in dest), put the virtual page pointer into dest, and the physical address into op (see section on paging)


    ? ? ? ? ? ? ? ? ? ? ? ?|r r r h     NOT     <r16>                       -   Bitwise NOT

    ? ? ? ? ? ? ? ? ? ? ? ? ?|r r r     SKC     <r32>                       -   Release a number, equal to dest, of 32-bit values from the stack
    ? ? ? ? ? ? ? ? ? ? ? ? ?|r r r     PSH     <r32>                       -   Push the value of dest onto the stack
    ? ? ? ? ? ? ? ? ? ? ? ? ?|r r r     POP     <r32>                       -   Pop the top of the stack into dest

TODO: floating point
TODO: hardware discovery and interaction
TODO: processor feature discovery
TODO: interrupts
TODO: set page size

Paging
======

Introduction
------------

Paging is a process by which virtual addresses are mapped to physical addresses.

In the Tomato architecture, paging is always enabled.

Virtual Page Pointer
--------------------

The virtual page pointer (VPP) is a word that specifies a page in the virtual address space, and holds flags associated to that page.

The VPP is divided into two segments: the flags, and the page number.

    +---------------+-----------------------------------------------+
    |               |                  Page Number                  |
    |     Flags     +-------+-------+-------+-----------------------+
    |               |   A   |   B   |   C   |           D           |
    +---------------+-------+-------+-------+-----------------------+
    |p u m m a d f f|a a a a|b b b b|c c c c|d d d d d d d d d d d d|
    +---------------+-------+-------+-------+-----------------------+
    |v 0 9 8 7 6 5 4 v 2 1 0 9 8 7 6 v 4 3 2 1 0 9 8 v 6 5 4 3 2 1 v|
    |31              23              15              7             0|
    +---------------------------------------------------------------+

### Flags

* p - Present: If set, the page is marked as present, and ready to be used. Otherwise, the page is cleared.
* u - Privileged: If set, the page belongs to kernel space, otherwise it belongs to user space.
* m - Mode: Chooses the access mode for the page: 00 = None, 01 = Read-Only, 10 = Read-Write, 11 = Read-Execute
* a - Accessed: Bit is set when the page is read, not automatically cleared
* d - Dirty: Bit is set when the page is written to, not automatically cleared
* f - Available for application use

### Page Number

The page number is a pointer to the start of a page in the virtual address space shifted right. The active page size determines which part of the address field are active:
  *   1 kio - Parts A, B, C, D
  *  16 kio - Parts B, C, D
  * 256 kio - Parts C, D
  *   4 Mio - Part D
Parts which are not active must be set to zero.

Boot Process
============

TODO: these are just my notes below.

The processor:
* starts in 1 kio page size, with the bios code executing from the first page;
* identity maps the second and third pages of memory with the privileged bit set, in read-write mode; then
* copies 2 kio of data from the boot device into the mapped pages; then
* remaps the pages into read-execute mode; then
* jumps to the first instruction of the second page

