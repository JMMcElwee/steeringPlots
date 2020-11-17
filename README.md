# steeringPlots

These plotting scripts can be used to create the plots used for the SK Steering Meetings. 
In order to run the scripts, you need ROOT6, and on sukap a version can be found in my 
software directory and setup via:

$ source /home/jmcelwee/software/root6/root/bin/thisroot.sh

To compile the scripts, they need to find the ROOT flags and libraries and compiled with 
g++: 

$ g++ scripts/\*.cc -o \<executable\> $(root-config --cflags --libs)

All the plots can be created with the executable. Usually the monitor plots aren't worth
showing as the monitor is near saturation. So simply:

$ ./\<executable\>

Any questions email:
Jordan McElwee,
jmmcelwee1@sheffield.ac.uk


--- DEVELOP ---
I'm working on putting in flags to run the collimator, diffuser and top diffuser
separately, along with choosing between running the plots over 3 weeks or all time.
If you have any other suggestions on what you'd like to see here to make it easier
to use please let me know.