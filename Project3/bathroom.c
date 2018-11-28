
/*
================================================
============ Project 3 bathroom.c ==============
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

#define PI 3.14159265358979323846

//Initializes bathroom struct (GBR)
void Initialize(){
  GBR = (struct bathroom*) malloc(sizeof(struct bathroom));
  GBR->gen = vacant;
  GBR->num_men = 0;
  GBR->num_women = 0;    
  GBR->time_vac = 0;
  GBR->time_occ = 0;
  GBR->usages = 0;
  GBR->numOcc = 0;
  GBR->avg_queue_length = 0;
  GBR->avg_num = 0;
  GBR->total_queue = 0;
  pthread_mutex_init(&GBR -> lock, NULL);
  pthread_mutex_init(&GBR -> loccupied, NULL);
  pthread_mutex_init(&GBR -> locc, NULL);
  pthread_mutex_init(&GBR -> print, NULL);
  pthread_cond_init(&GBR -> occ, NULL);
  timer = findTime();
}//end of Initialize

//Prints statistics
void Finalize (int threads){ 
  printf("Number of thread users executed: %d\n", threads);
  printf("Total bathroom usages: %d\n", GBR -> usages);
  printf("Average queue length: %f\n", GBR -> avg_queue_length);
  printf("Average number of people in the bathroom: %f\n\n", GBR -> avg_num);
  printf("Total time bathroom is occupied: %f microseconds\n", GBR -> time_occ);
  printf("Total time bathroom is vacant: %f microseconds\n", GBR -> time_vac);
  exit(0);
}//end of Finalize

//Thread attempts entry of bathroom
void Enter(enum gender g){
  pthread_mutex_lock(&(GBR -> lock));
  GBR -> total_queue++;
  if(g == male)
    {
    while(GBR -> gen == women){
//    printf("Male waits in queue for female to leave bathroom\n");
    pthread_cond_wait(&(GBR -> occ), &(GBR -> lock));
    }
//  printf("Male Enters\n");
    GBR -> num_men++;
    GBR -> usages++;
    GBR -> numOcc++;
    GBR -> total_queue--;
    GBR -> gen = men;
    
    if(GBR -> numOcc == 1){
      timer = findTime()-timer;
      GBR -> time_vac = GBR -> time_vac + timer;
      timer = findTime();
    }
    }
  else
    {
    while(GBR -> gen == men){
//    printf("Female waits in queue for male to leave bathroom\n");
    pthread_cond_wait(&(GBR -> occ), &(GBR -> lock));
    }
//    printf("Female Enters\n");
    GBR -> num_women ++;
    GBR -> usages++;
    GBR -> numOcc++;
    GBR -> total_queue--;
    GBR -> gen = women;
    
    if(GBR -> numOcc == 1){
      timer = findTime()-timer;
      GBR -> time_vac = GBR -> time_vac + timer;
      timer = findTime();
    }
    }
  pthread_mutex_unlock(&(GBR -> lock));
}//end of Enter
  
//thread leaves bathroom
void Leave(enum gender g){
  pthread_mutex_lock(&GBR -> lock);
  GBR -> numOcc-=1;
  if(g == male)
    {
//    printf("Male Leaves\n");
    GBR -> num_men--;
    }
  else
    {
//    printf("Female Leaves\n");
    GBR -> num_women--;
    }
  if(GBR -> num_men == 0 && GBR -> num_women == 0)
    {
//    printf("Bathroom becomes vacant\n");
    GBR -> gen = vacant;
    timer = findTime()-timer;
    GBR -> time_occ = GBR -> time_occ + timer;
    timer = findTime();
    pthread_cond_signal(&GBR -> occ);
    }
  pthread_mutex_unlock(&GBR -> lock);
}//end of leave
  





                  
                  
                  
                  
                  
                  
