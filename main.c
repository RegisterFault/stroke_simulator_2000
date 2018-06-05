#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sched.h>

#define READ_FILE "foo.txt"
#define NUM_THREADS 20
#define BUFF 10
#define USLEEP_MODULO 400000


void error(char *err)
{
	fprintf(stderr, "%s: %d - %s\n", err, errno, strerror(errno));
	exit(1);
}

static void * stroke(void *arg)
{
	int infd = *((int *) arg);
	char buff[BUFF];
	int num_read;
	
	for (;;){
		if ((num_read = read(infd, buff, BUFF)) <= 0)
			pthread_exit(NULL);

		usleep(rand() % USLEEP_MODULO);
		sched_yield();
		fwrite(buff, 1, num_read, stdout);
	}
}

int main(int argc, char *argv[])
{
	int infd;
	int i;
	pthread_t tmp;
	pthread_t tlist[NUM_THREADS];
	pthread_t *tcursor = tlist;

	srand(time(NULL));
	
	//file descriptor is preserved across all threads
	if ((infd = open(READ_FILE,O_RDONLY)) < 0)
		error("file open fail");

	//generate all the threads
	for (i=0; i<NUM_THREADS; i++){
		pthread_create(&tmp, NULL,stroke,(void *)&infd);
		*tcursor = tmp;
		tcursor++;
	}

	//wait for all threads to exit
	for(i=0; i<NUM_THREADS; i++)
		pthread_join(tlist[i],NULL);

	return 0;
}
