Tomato
======

A forey into CPU instruction set design and dynamic recompilation.

Tomato comes from the name of Ed's computer in the anime Cowboy Bebop.

Architecture
============

The Tomato CPU has (or more accurately will have):
* Privileged and non-privileged modes
* 7 32-bit registers
* 32-bit and 16-bit operations
* Intrinsic support for paging (with page sizes of 1KiB, 16KiB, 256KiB, 4MiB, and up to 1024 pages)
* Restrictions that ease dynamic recompilation
