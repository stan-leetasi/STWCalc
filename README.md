# STWCalc
Lightweight simple calculator for Linux Ubuntu distributions.
The app is written in C and uses GTK+ GUI library and GoogleTest testing framework.

![](/screenshots/screenshot1.png)

## Origin of the development
This calculator was a part of an university team project.
The team was composed of 4 members. Here are listed parts of the project that each member has done:
- Michal Krulich (team leader)
    - GUI design and implementation
    - engine of the calculator
    - tests for the engine interface
    - Make
- František Holáň
    - implementation of math library
    - *program for calculation of mstandard deviation*
    - *program profiling with gprof*
- Stanislav Letaši
    - header for the math library
    - tests for math library
    - *mockup of the future version*
- Lukáš Španír
    - *User manual*
    - debian package (to be added)
    - Doxyfile

Entries in *italic* are not included in this repository.

## Different look in different systems and themes
GTK adapts its GUI to current system's theme.

![](/screenshots/screenshot2.png) ![](/screenshots/screenshot3.png)
