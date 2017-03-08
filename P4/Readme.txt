Best combination of N & D found so far is N=7, D= 10k. that has been chosen for further stages.

code is re-writen to handle multithreading.
four parallel threads are used during training phase and 8 threads for testing phase.
This  greatly reduced the time taken to finish the execution of the program to upto 1/8th of initial time without compramising on the efficiency.

in addition to the multithreading concept as part of better encoding technique, all the documents belonging to a given category are combined and put into a single line before feeding it to the main program for training