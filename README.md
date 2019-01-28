Van der Waals Force Example
===

Example of a leapfrog integrator for a van der waals force.

You can download a .zip file of this project, unzip it onto some directory in the imac, and type "make" to run all the commands in the file "makefile". This will compile the simulation, run the simulation, run the python file, and then run convert to make the output animation. "make clean" will delete all of the output files. 

Some comments:
 1. For a Plummer sphere simulation, you would have to make this into a 3-dimensional code
 2. vdwAcceleration() can be modified to provide a gravitational force instead
 3. Later, we'll go over the Aarseth code. If you're not comfortable with creating your own n-body simulation, you can use the Aarseth code to do n-body calculations without writing your own for partial credit. You still have to code in C, but it's easier.
 4. Molecular dynamics simulations like these are fun! [This simulation](http://physics.weber.edu/schroeder/md/) is my favorite.
[This gravitational collision simulation](https://www.youtube.com/watch?v=P-oc1eYZO2Y) has long-distance attraction, short-range repulsion, and friction.
[More sophisticated simulations to model galaxy formation](https://www.youtube.com/watch?v=h9za1CP9ImA) use techniques like
smoothed particle hydrodynamics to model the interaction of gases. 
