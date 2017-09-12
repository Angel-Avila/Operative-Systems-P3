/*
Equipo: Angel Avila, Giovanni Mazzolo
*/
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 4 // Número de hilos

long long num_steps = 1000000000; // ciclos
double step, sum = 0.0; // Inicalización de variables

pthread_mutex_t candado;  // Declaración de candados

void *hilo_pi(void *args){  // Hilo

  int i, tnum, counter_init, counter_end; // Declaración de variables
  double x, lsum = 0.0; // Variables locales

  tnum = *((int *) args);
  counter_init = (num_steps/NTHREADS)*tnum;
  counter_end = (num_steps/NTHREADS)*(tnum+1);

  for (i=counter_init; i<counter_end; i++)
  {
    x = (i + .5)*step;
    lsum = lsum + 4.0/(1.+ x*x);
  }

  pthread_mutex_lock(&candado); // Se bloquea el recurso para evitar colisiones
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
  int i;
  int args[NTHREADS];

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

  step = 1./(double)num_steps;

  pthread_t thread_id[NTHREADS];

  pthread_mutex_init(&candado,NULL);

  for(i=0;i<NTHREADS;i++) // Se crean los hilos
	{
		args[i]=i;
		pthread_create(&thread_id[i],NULL,hilo_pi,(void *) &args[i]);
	}

  for(i=0;i<NTHREADS;i++) // Se esperan a los hilos
  {
		pthread_join(thread_id[i],NULL);
  }

  pi = sum*step;

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
  elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
  printf("El valor de PI es %1.12f\n",pi);
  printf("Tiempo = %2.2f segundos\n",elapsed_time);

  return 0;
}
