# steeringPlots

The develop branch is currently in a working state, being developed to have a single 
executable to run all the files. This is working, but many features are yet to be included. 

These plotting scripts can be used to create the plots used for the SK Steering Meetings. 
In order to run the scripts, you need ROOT6, and on sukap a version can be found in my 
software directory and setup via:

$ source /home/jmcelwee/software/root6/root/bin/thisroot.sh

To compile the scripts, they need to find the ROOT flags and libraries and compiled with 
g++: 

<<<<<<< HEAD
$ g++ scripts/\*.cc -o \<executable\> $(root-config --cflags --libs)

All the plots can be created with the executable. Usually the monitor plots aren't worth
showing as the monitor is near saturation. So simply:

$ ./\<executable\>
=======
$ g++ \*.cc -o \<executable\> $(root-config --cflags --libs)
  
This can be run easily by just running the executable. For example:

$ ./\<executable\>

will run all the plots since pre Gd-loading. If you want plots for x number of weeks, you can run the executable with the -w flag:

$ ./\<executable\> -w \<x\>

Finally, if you want a specific beam type, you can use the following flags:
-c  |  Collimator
-d  |  Diffuser
-t  |  Top Diffuser
-m  |  Monitor PMT


Any questions email:
Jordan McElwee,
jmmcelwee1@sheffield.ac.uk


--- DEVELOP ---
I'm working on putting in flags to change the number of averaging data points, as
well as choosing a certain number of months. I also want the choice to change the
name of the files as you choose (this feature is not working).
There are a few pointless lines of code in this that are left over from when it was
being developed. These will slowly be removed as the code is developed.
If you have any other suggestions on what you'd like to see here to make it easier
to use please let me know.
