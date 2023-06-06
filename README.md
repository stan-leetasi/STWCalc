# STWCalc
A simple lightweight calculator for Linux Ubuntu distributions.
The app is written in C and uses GTK+ GUI library and GoogleTest testing framework.

![](/screenshots/showcase.gif)

## Development
This calculator was a part of a team project at BUT FIT.
<div>
  <p>Our team was composed of 4 members:<br>- Michal Krulich (team leader)<br>    - GUI design and implementation<br>    - engine of the calculator<br>    - tests for the engine interface<br>    - Make<br>- František Holáň<br>    - implementation of math library<br>    - *program for calculation of standard deviation*<br>    - *program profiling with gprof*<br>- Stanislav Letaši<br>    - header for the math library<br>    - tests for the math library using the gtest suite<br>    - *mockup of the next version*<br>- Lukáš Španír<br>    - *User manual*<br>    - debian package (to be added)<br>    - Doxyfile<br></p>
  <img src="/screenshots/STworks_logo.jpg" title="STWorks logo">
</div>

Entries in *italic* are not included in this repository.

## Adaptive GUI
GTK adapts its GUI to the current system's theme.

![](/screenshots/screenshot2.png) ![](/screenshots/screenshot3.png)

## Usage

1. Compile the code with the provided makefile:

`make stwcalc`

2. Run the calculator with:

`make run`

3. Teardown - deletes all intermediate files and the stwcalc executable

`make clean`
