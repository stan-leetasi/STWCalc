# STWCalc
A simple lightweight calculator for Linux Ubuntu distributions.
The app is written in C and uses GTK+ GUI library and GoogleTest testing framework.

![](/screenshots/showcase.gif)

## Development
This calculator was a part of a team project at BUT FIT.

Our team was composed of 4 members:
- Michal Krulich (team leader)
    - GUI design and implementation
    - engine of the calculator
    - tests for the engine interface
    - Make
- František Holáň
    - implementation of math library
    - *program for calculation of standard deviation*
    - *program profiling with gprof*
- Stanislav Letaši
    - header for the math library
    - tests for the math library using the gtest suite
    - *mockup of the next version*
- Lukáš Španír
    - *User manual*
    - debian package (to be added)
    - Doxyfile

Entries in *italic* are not included in this repository.

## Adaptive GUI
GTK adapts its GUI to the current system's theme.

![](/screenshots/screenshot2.png) ![](/screenshots/screenshot3.png)

## Usage

1. Compile the code with the provided makefile:

make stwcalc

2. Run the calculator with:

make run

3. Teardown - deletes all intermediate files and the stwcalc executable

make clean
