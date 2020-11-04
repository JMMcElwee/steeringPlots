# steeringPlots

These plotting scripts can be used to create the plots used for the SK Steering Meetings. 
In order to run the scripts, you need ROOT6, and on sukap a version can be found in my 
software directory and setup via:

$ source /home/jmcelwee/software/root6/root/bin/thisroot.sh

To compile the scripts, they need to find the ROOT flags and libraries and compiled with 
g++: 

$ g++ <scriptname>.cc -o <filename> $(root-config --cflags --libs)
  
The top diffuser script can be run simply with the executable, whilst the barrel injectors
have to be passed a flag for the diffuser and collimator. I.e

$ ./plot3weeks -c ; ./plot3weeks -d

for the collimator and diffuser separately. 

Any questions email:
Jordan McElwee,
jmmcelwee1@sheffield.ac.uk
