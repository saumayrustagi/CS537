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
#include <stdint.h>

// typedef int64_t i64;
// typedef int32_t i32;
// typedef uint32_t ui32;
// typedef uint64_t ui64;

int tokenize(char *str, char **myargv)
{
	char *tmp = str;
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
	if (tmp != NULL)
		free(tmp);

	return i;
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
			char **myargv = malloc(sizeof(char *) * 1024);
			int tokln = tokenize(buf, myargv) + 1;

			char **pargv = reallocarray(myargv, tokln, sizeof(char *));
			pargv[tokln - 1] = NULL;

			// for (int i = 0; i < tokln; ++i)
			// {
			// 	if (pargv[i] == NULL)
			// 		printf("(null)\n");
			// 	else
			// 		printf("%s\n", pargv[i]);
			// }

			// for (int i = 0; i < tokln; ++i)
			// {
			// 	if (pargv[i] != NULL)
			// 	{
			// 		free(pargv[i]);
			// 	}
			// }
			// if (pargv != NULL)
			// 	free(pargv);
			// break;
		}
		else
		{
			if (buf != NULL)
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