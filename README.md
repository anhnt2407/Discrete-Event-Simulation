# Discrete-Event-Simulation

The working flow of a cargo company is simulated. Whenever a new package comes, first-level units process the package information, and passes the package onto the second-level units, which puts the packet to the correct truck. 

There are two layouts for these units to connect with each other. In both layouts, there is one first level queue for multiple first level units. In layout one, there is also one second level queue for second level units. However, in layout two, there are seperate second level queues for each second level unit. In the end of the program, the performances of these two models are compared.

## Input
Number of first-level units with their process times, number of second-level units with their process times, number of cargo packets and arrival times of packets are given. 

## Output
This program returns average turnaround time for the packages, maximum lengths of first and second level queues, average wait time and longest wait time of all packages, and the total running time of the cargo office. 
