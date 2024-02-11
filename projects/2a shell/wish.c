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

void tokenize(char *str, char *myargv[])
{
	char *token;
	int i = 0;
	while ((token = strsep(&str, " \n")) != NULL)
	{
		if (token[0] != '\0')
		{
			myargv[i] = strdup(token);
			++i;
		}
	}
}

int wish()
{
	while (1)
	{
		ssize_t linelength;
		size_t n;
		n = 0;
		char *buf = NULL;

		printf("(%d)> ", getpid());

		if ((linelength = getline(&buf, &n, stdin)) < 1)
		{
			if (buf != NULL)
				free(buf);
			if (feof(stdin))
			{
				fprintf(stderr, "EOF encountered\n");
				exit(EXIT_SUCCESS);
			}
			{
				perror(NULL);
				exit(EXIT_FAILURE);
			}
		}

		pid_t rc;
		rc = fork();

		if (rc == -1)
		{
			perror("fork failure");
			exit(EXIT_FAILURE);
		}
		if (rc == 0)
		{
			char *myargv[2];

			tokenize(buf, myargv);
			free(buf);

			myargv[1] = NULL;
			for (int i = 0; i < 2; ++i)
			{
				printf("%s_\n", myargv[i]);
			}
			break;
		}
		else
		{
			free(buf);
			wait(NULL);
		}
	}
	return EXIT_SUCCESS;
}

int main()
{
	return wish();
}