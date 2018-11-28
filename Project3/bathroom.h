#ifndef BATHROOM_H_
#define BATHROOM_H_
#endif

#include <sys/time.h>
#include <time.h>

struct bathroom *GBR;
double timer;
enum gender {male,female};
enum genderState {men, women, vacant};
void Initialize();
void Finalize();
void Enter(enum gender g);
void Leave();
void printGen();
double findTime();

typedef struct bathroom{
  enum genderState gen;
  int num_men;
  int num_women;
  int usages;
  int numOcc;
  int total_queue;
  double time_vac;
  double time_occ;
  float avg_queue_length;
  float avg_num;
  pthread_mutex_t lock;
  pthread_mutex_t locc;
  pthread_mutex_t print;
  pthread_mutex_t loccupied;
  pthread_cond_t occ;
} bathroom;

typedef struct pargs{
  enum gender gen;
  int arrival;
  int stay;
  int looptimes;
} pargs;
