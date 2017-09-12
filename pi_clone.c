
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <sched.h>


#define NTHREADS 4  // Procesos
#define FIBER_STACK 1024*64 // 64kB stack (tamaño de stack por proceso)

long long num_steps = 1000000000;
double step, sum = 0.0; // Variables globales

pthread_mutex_t candado;

void *clone_pi(void *args){

  int i, tnum, counter_init, counter_end;
  double x, lsum = 0.0;

  tnum = *((int *) args); // Se recibe el numero del proceso o índice
  counter_init = (num_steps/NTHREADS)*tnum;
  counter_end = (num_steps/NTHREADS)*(tnum+1);

  for (i=counter_init; i<counter_end; i++)
  {
    x = (i + .5)*step;
    lsum = lsum + 4.0/(1.+ x*x);
  }

  pthread_mutex_lock(&candado);
  sum += lsum;
  pthread_mutex_unlock(&candado);

  return(NULL);
}

int main(int argc, char* argv[])
{
  long long start_ts;
  long long stop_ts;
  float elapsed_time;
  // long lElapsedTime;
  struct timeval ts;
  double pi;
  int i, status;l
  int args[NTHREADS];
  void * stack;

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

  step = 1./(double)num_steps;

  stack = malloc( FIBER_STACK * NTHREADS);  // Se asigna el tamaño de memoria total

  pthread_mutex_init(&candado,NULL);

  for(i=0;i<NTHREADS;i++) // Se crean los procesos
	{
		args[i]=i;
    clone( (void*)&clone_pi, (char*) stack + (FIBER_STACK*(i+1)),
                SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*)&args[i]);
	}

  for(i=0;i<NTHREADS;i++) // Se espera a los procesos
  {
		wait(&status);
  }

  pi = sum*step;

  free(stack);

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
  elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
  printf("El valor de PI es %1.12f\n",pi);
  printf("Tiempo = %2.2f segundos\n",elapsed_time);

  return 0;
}
