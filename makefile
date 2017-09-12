all:	pi_hilos.c pi_fork.c pi_clone.c
		gcc -o pi_hilos pi_hilos.c -lpthread
		gcc -o pi_clone pi_clone.c
		gcc -o pi_fork pi_fork.c

clean:
		rm pi_hilos *.o
	  rm pi_fork *.o
	  rm pi_clone *.o
