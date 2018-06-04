#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define READ_FILE "LICENSE"
#define NUM_PROC 20
#define BUFF 10
#define USLEEP_MODULO 400000

void error(char *err)
{
	fprintf(stderr, "%s: %d - %s\n", err, errno, strerror(errno));
	exit(1);
}

void stroke(FILE *in)
{
	char buff[BUFF];
	int num_read;

	for (;;){
		if ((num_read = fread(buff, 1, BUFF, in)) <= 0)
			exit(1);

		fwrite(buff, 1, num_read, stdout);
		usleep(rand() % USLEEP_MODULO);
		fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	pid_t procid;
	FILE *in;
	int i;
	
	srand(time(NULL));

	//file descriptor is preserved across all processes
	if ((in = fopen(READ_FILE,"r+")) == NULL)
		error("file open fail");

	//generate all the processes
	for (i = 0; i < NUM_PROC; i++)
		fork();

	//run process-local routine	
	stroke(in);

	return 0;
}
