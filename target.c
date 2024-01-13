#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void noise() {
	usleep(rand() % 1000) ;
}

void * thread(void *arg) {
		pthread_mutex_lock(&mutex1);
		pthread_mutex_lock(&mutex2);
		pthread_mutex_unlock(&mutex1); 
		pthread_mutex_unlock(&mutex2); 

		return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t tid1, tid2;
	srand(time(0x0)) ;

	pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);

	pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

	return 0;
}