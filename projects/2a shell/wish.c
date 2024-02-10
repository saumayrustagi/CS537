#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

int main()
{
	while (1)
	{
		ssize_t linelength;
		size_t n;
		n = 0;
		char *buf = NULL;

		printf("wish> ");

		if ((linelength = getline(&buf, &n, stdin)) < 1)
		{
			perror(NULL);
			break;
		}

		pid_t rc;
		rc = fork();

		if (rc == -1)
		{
			perror(NULL);
			break;
		}
		if (rc == 0)
		{
			char *token;
			int i = 0;
			while ((token = strsep(&buf, "\n ")) != NULL)
			{
				printf("token = %s || buffer = ", token);
				for(int j=0; j!=NULL; ++j){
					printf("(%d)", buf[j]);
				}
				++i;
			}
			printf("i = %d", i);
			// char* myargv[4096];
			// myargv[0] = "/bin/ls";
			// myargv[1] = NULL;
			// execv(myargv[0], myargv);
		}
		else
		{
			wait(NULL);
		}

		// free(buf);
	}
	exit(0);
}