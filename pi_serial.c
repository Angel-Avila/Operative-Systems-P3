
#include <stdio.h>
#include <sys/time.h>

long long num_steps = 1000000000;
double step;

int main(int argc, char* argv[])
{
  long long start_ts;
  long long stop_ts;
  float elapsed_time;
  // long lElapsedTime;
  struct timeval ts;
  double x, pi, sum=0.0;
  int i;

  gettimeofday(&ts, NULL);
  start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial
  step = 1./(double)num_steps;

  for (i=0; i<num_steps; i++)
  {
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }
  pi = sum*step;

  gettimeofday(&ts, NULL);
  stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
  elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
  printf("El valor de PI es %1.12f\n",pi);
  printf("Tiempo = %2.2f segundos\n",elapsed_time);
  return 0;
}
