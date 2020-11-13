# steeringPlots

The develop branch is currently in a working state, being developed to have a single 
executable to run all the files. This is working, but many features are yet to be included. 

These plotting scripts can be used to create the plots used for the SK Steering Meetings. 
In order to run the scripts, you need ROOT6, and on sukap a version can be found in my 
software directory and setup via:

$ source /home/jmcelwee/software/root6/root/bin/thisroot.sh

To compile the scripts, they need to find the ROOT flags and libraries and compiled with 
g++: 

$ g++ \*.cc -o \<executable\> $(root-config --cflags --libs)
  
This can be run easily by just running the executable. For example: 
$ ./steeringPlots

will run all the files. 

Any questions email:
Jordan McElwee,
jmmcelwee1@sheffield.ac.uk
