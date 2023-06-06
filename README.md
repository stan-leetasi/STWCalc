# STWCalc
A simple lightweight calculator for Linux Ubuntu distributions.
The app is written in C and uses GTK+ GUI library and GoogleTest testing framework.

![](/screenshots/showcase.gif)

## Development
This calculator was a part of a team project at BUT FIT.
<style>
  .container {
    display: flex;
    align-items: center;
  }

  .text {
    margin-right: 20px; /* Adjust the value to control the indentation */
    width: 70%; /* Adjust the width as needed */
  }

  .image {
    display: inline-block;
    vertical-align: top;
    width: 30%; /* Adjust the width as needed */
  }
</style>

<div class="container">
  <div class="text">
    <p>Our team was composed of 4 members:</p>
    <ul>
      <li>Michal Krulich (team leader)
        <ul>
          <li>GUI design and implementation</li>
          <li>Engine of the calculator</li>
          <li>Tests for the engine interface</li>
          <li>Make</li>
        </ul>
      </li>
      <li>František Holáň
        <ul>
          <li>Implementation of math library</li>
          <li>Program for calculation of standard deviation</li>
          <li>Program profiling with gprof</li>
        </ul>
      </li>
      <li>Stanislav Letaši
        <ul>
          <li>Header for the math library</li>
          <li>Tests for the math library using the gtest suite</li>
          <li>Mockup of the next version</li>
        </ul>
      </li>
      <li>Lukáš Španír
        <ul>
          <li>User manual</li>
          <li>Debian package (to be added)</li>
          <li>Doxyfile</li>
        </ul>
      </li>
    </ul>
  </div>
  <div class="image">
    <img src="/screenshots/STWorks_logo.jpg" alt="STWorks Logo" title="STWorks Logo">
  </div>
</div>


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
