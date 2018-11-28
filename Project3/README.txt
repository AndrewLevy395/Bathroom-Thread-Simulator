================================================
================= Project 3 Final ==============
================================================

     Authors:    Matt Hagan & Andy Levy
     Language:   C & Bash/Dash
     Date:       2/14/2018
     Version:    2
     Class:      Operating Systems (CS 3013)

Useful info
===========
-make sure to compile the programs with -pthread -lm and -Wall in the GCC
-Mutex locks/condition variables are used for critical sections in data structures

How to run w/ recommended settings
==================================
Type Make to make the program

Type ./bathroomSim nUsers meanLoopCount meanArrival meanStay
	-nUsers is the number of users you want to have (threads)
	-meanLoopCount is the average loop count that you want the threads to have. A random 
	one for this will be chosen
	-meanArrival is the average arrival time that the threads have. This will be random 
	for each thread from the input
	-MeanStay is the average time that the threads stay in the bathroom. This will be 
	random for each thread from the input.

Files
=====
-bathroomSim
-bathroom.c
-bathoomSim.c
-bathroom.h
-makefile
-test1.txt
-test2.txt
-test3.txt

Bathroom.h
==========
-Variable definitions for gender and genderState.
-Function Prototypes for Initialize, Finalize, Enter, and Leave.
-Bathroom struct, that contains the arguments for bathroom.
-Pargs struct, which stores the arguments to be piped to the individual function.

Bathroom.c
==========
-Void Initialize()
	-this function initializes the bathroom struct. It mallocs the appropriate 
	amount of data, sets the state to vacant, and zeros all of the parameters.
	-Initializes mutex lock and condition variables
-Void Finalize()
	-prints the number of thread users executed
	-prints the total bathroom usages
-Void printGen()
	-takes in a gender g and prints out its gender
-Void printGenState()
	-prints the gender state, being man, woman, vacant, undefined
-useconds_t rantTime()
	-takes in a mean time and computes a random time using a normal distribution.
-int randLoop()
	-takes in a mean loop count and computes a random time using a normal distribution.
-void Enter()
	-takes a gender g of the thread, and compares it to the current state of the bathroom
	 and sees if the thread can enter the bathroom. If the bathroom is currently occupied,
	 the thread will wait till it can enter.To do this, we used a mutexes and condition 
	 variables.
-void Leave()
	-leave takes the gender of the thread and has that thread leave the bathroom. If the 
	number of other threads is 0, it will set the state to vacant.
-void Individual()
	-they wait a certain amount of time, then they attempt to enter the bathroom. They 
	will wait in a queue if they have to. Then they enter the bathroom, and they stay in 
	their for a certain amount of time, then they leave. The continue to do this for a 
	certain number of loops, and each thread has a different number of loops. As they do 
	this, all of the threads are accessing data, the number of people in the bathroom, 
	number of usages, number of people in the queue, and in order to prevent them to 
	write to the same data at the same time, their are mutex locks that prevent 2 or 
	more threads from writing to the same data.
-enum gender GenderRandomize()
	-uses drand48 to create a random gender.
-int main()
	-threads are created and  each one runs the individual function

In-depth documentation
======================
when bathroom sim is run, thread users can enter the bathroom if it is vacant or the correct 
user is in the bathroom. If the bathroom is of the correct gender state, then they may enter. 
If they are not of the same gender, their is a conditional wait until all threads of the 
opposite gender exit. This is basically a sliding gender sign on the door that only the people 
inside the bathroom can control. If they are males, it will be set to males. If they are 
females, it will be set to female. If it is vacant, it will be set to vacant. Also, each thread 
has their own gender, they go the bathroom a certain number of times, they stay in the bathroom
 a certain amount of time and wait to enter the bathroom to enter the bathroom for a certain
 amount of time. In the main function , each thread is created and each thread runs the 
individual function. In the individual function, they wait a certain amount of time, then they
 attempt to enter the bathroom. They will wait in a queue if they have to. Then they enter the
 bathroom, and they stay in their for a certain amount of time, then they leave. The continue 
to do this for a certain number of loops, and each thread has a different number of loops. As 
they do this, all of the threads are accessing data, the number of people in the bathroom, 
number of usages, number of people in the queue, and in order to prevent them to write t the
 same data at the same time, their are mutex locks that prevent 2 or more threads from writing 
to the same data. before on of the values on our data struct get written to, their is a mutex 
lock place before it, that way two or more threads rant writing to the same value on the struct.
 after the value is written to, it is unlocked, that way something else can write to it later. 
We also included a lock before the print statements in the individual function. which this, each
 individual thread can print its statistics without other threads printing intervening. This way, 
the statistics are correct and clean. our program manages to safely join the treads together for
 exiting buy using pthread_join. When their is males in the bathroom, and their are no females 
trying top change the state, and more males show up, they can enter. this applies to the counter
 example, I.E.  the vice verse. when the bathroom is vacant, it means that no-one is trying to 
enter the bathroom at the given time, and therefore the bathroom will stay vacant until a thread 
is done waiting and ready to go pee. At the end of the bathroom, the function finalize is run, 
that prints out different statistics about the bathroom. This includes total number of threads, 
total number of usages, average queue length, average number of people in the bathroom, and the 
total time the bathroom is occupied and vacant. Our enter function takes a gender g of the thread, 
and compares it to the current state of the bathroom and sees if the thread can enter the bathroom. 
If the bathroom is currently occupied, the thread will wait till it can enter.To do this, we used 
and condition variables.

