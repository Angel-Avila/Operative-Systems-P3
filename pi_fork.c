
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <sys/shm.h>

#define NTHREADS 4

long long num_steps = 1000000000;
double step; // Variable global

pthread_mutex_t candado;

void fork_pi(int args, int shmid){ // Función

  int i, counter_init, counter_end;
  double x, lsum = 0.0;
  double * sum;

  counter_init = (num_steps/NTHREADS)*args;
  counter_end = (num_steps/NTHREADS)*(args+1);

  for (i=counter_init; i<counter_end; i++)
  {
    x = (i + .5)*step;
    lsum = lsum + 4.0/(1.+ x*x);
  }

  sum = (double *)shmat(shmid, (void *)0, 0);
  pthread_mutex_lock(&candado);
  *sum += lsum;
  pthread_mutex_unlock(&candado);

  // return(NULL);
}

int main(int argc, char* argv[])
{
  long long start_ts;
  long long stop_ts;
  float elapsed_time;
  // long lElapsedTime;
  struct timeval ts;
  double pi;
  double * sum;
  int i, status, shmid;
  int args[NTHREADS];
  int key = 1111;

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

  step = 1./(double)num_steps;

  shmid = shmget(key, sizeof(double), IPC_CREAT | 0666);
  sum = (double *)shmat(shmid, (void *)0, 0);
  *sum = 0;

  pthread_mutex_init(&candado,NULL);

  for(i=0;i<NTHREADS;i++) // Se crean hijos con fork()
	{
		args[i]=i;

		if(fork() == 0){
      fork_pi(args[i], shmid);
      exit(0);
    }

	}

  for(i=0;i<NTHREADS;i++) // Se esperan a los hijos
  {
    wait(&status);
  }

  pi = (*sum)*step;

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
  elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
  printf("El valor de PI es %1.12f\n",pi);
  printf("Tiempo = %2.2f segundos\n",elapsed_time);

  return 0;
}
