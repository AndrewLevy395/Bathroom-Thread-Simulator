
/*
================================================
============ Project 3 bathroomSim.c ==============
================================================

     Authors:    Matt Hagan & Andrew Levy
     Language:   C
     Date:       2/7/2018
     Version:    2
     Class:      Operating Systems (CS 3013)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <assert.h>
#include "bathroom.h"


struct pargs *args1;
int cond = 0;
void *Individual(void *arg);
void *findStats(void *arg);


double findTime(){
  struct timeval t;
  gettimeofday(&t,NULL);
  return 1000000*t.tv_sec + t.tv_usec;
}//end of findTime


useconds_t randTime (int meanTime){
  int stdDev;
  useconds_t timeRand;
  double a,b;

  stdDev = meanTime/2;
  a = drand48();
  b = drand48();
  float z = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
  timeRand = (fmax(z*stdDev+meanTime,1));

  return timeRand;
}//end of randTime()


int randLoop (int meanLoopCount){
  int stdDev, timeRand;
  double a,b;

  stdDev = meanLoopCount/2;
  a = drand48();
  b = drand48();
  float z = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
  timeRand = (fmax(z*stdDev+meanLoopCount,1));

  return timeRand;
}//end of randLoop()


//prints Gender of Thread
void printGen(enum gender g){
  if(g == male){ 
  printf("Gender: Male\n");
}
  else if(g == female){
  printf("Gender: Female\n");
}
  else{
  printf("INVALID\n");
}
}//end of printGen

//prints Gender State of Bathroom
void printGenState(enum genderState g){
  if(g == men){ 
  printf("Gender State: Male\n");
}
  else if(g == women){
  printf("Gender State: Female\n");
}
  else if(g == vacant){
  printf("VACANT\n");
}
  else{
  printf("INVALID\n");
}
}//end of printGenState


void *Individual(void *arguments){
  pargs *args = (pargs *) arguments;
  int loops = randLoop(args -> looptimes);
  int waitTime = randTime(args -> arrival);
  int stayTime = randTime(args -> stay);
 
  struct timeval sQueue, eQueue;
  double qTime, time, maxTime, minTime;
  qTime = 0;
  maxTime = 0;
  minTime = -1;
  
  for(int i = 0; i < loops; i++){
  
  usleep(waitTime);
  gettimeofday(&sQueue, NULL);
  Enter(args -> gen);
  gettimeofday(&eQueue, NULL);
  usleep(stayTime);
  
  Leave(args -> gen);
    
  time = (eQueue.tv_sec*1000000+eQueue.tv_usec)-(sQueue.tv_sec*1000000+sQueue.tv_usec);
  qTime = qTime + time;
  if(time>maxTime){maxTime=time;}
  if(time<minTime){minTime=time;}
  if(minTime==-1){minTime=time;}
  }

  pthread_mutex_lock(&GBR -> print);
  printf("\nThread ID: %ld\n", (long int)syscall(SYS_gettid));
  printf("Loops: %d\n", loops);
  printGen(args -> gen);
  printf("Average stay time: %d.\n", stayTime);
  printf("Maximum queue time: %f microseconds\n", maxTime);
  printf("Minimum queue time: %f microseconds\n", minTime);
  printf("Average queue time: %f microseconds\n\n", qTime/loops);
  pthread_mutex_unlock(&GBR -> print);
  return 0;
}//end of Individual

//Caculates average queue length and average number of people in bathroom 
void *findStats(void*arg){
  int counter = 1;
  float avg_Q = 0;
  float avg_occ = 0;
  while(!cond){
    avg_Q = avg_Q + (GBR -> total_queue - avg_Q)/counter;
    avg_occ = avg_occ + (GBR -> numOcc - avg_occ)/counter;
    counter++;
  }
  GBR -> avg_queue_length = avg_Q;
  GBR -> avg_num = avg_occ;
  return NULL;
}//end of findStats

//Randomizes gender of thread
enum gender GenderRandomize(){
  enum gender y;
  double t = drand48();
  double z = t * 100;
  int intz = z;
  int x = intz % 2;
  if(x == 1){
    y = male;
  }
  else{
  y = female;
  }
  return y;
}//end of GenderRandomize


int main(int argc, char*argv[]){
  srand48(time(0));
  if(argc !=5){
    printf("Incorrect number of arguments. \nPlease try again with the proper number or arguments:");
    exit(1); 
  }//end of if
  int nUsers = atoi(argv[1]);//the number of users
  int meanLoopCount = atoi(argv[2]); // the average number of times a user repeats the bathroom usage loop
  int meanArrival  = atoi(argv[3]); //the average arrival time
  int meanStay = atoi(argv[4]); // 
  int i;
  Initialize();

  enum gender thread_gen;
  pthread_t pthread0[nUsers];
  pthread_t stats;

  for(i=0; i < nUsers; i++){

  thread_gen = GenderRandomize();
  args1 = (struct pargs*) malloc(sizeof(struct pargs));
  args1 -> gen = thread_gen;
  args1 -> arrival = meanArrival;
  args1 -> stay = meanStay;
  args1 -> looptimes = meanLoopCount;

  pthread_create(&pthread0[i], NULL, Individual, args1);
 }
  pthread_create(&stats, NULL, findStats, NULL);
  for(i=0; i < nUsers; i++){
  pthread_join(pthread0[i], NULL);
}

  cond = 1;
  pthread_join(stats, NULL);
  
  Finalize(nUsers);
  return 0;
}//end of main()
