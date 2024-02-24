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
int tokenize(char *str, char **myargv);
void exec_prog(char *myargv[]);
int wish();

int main()
{
	return wish();
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
			// Initialize and Tokenize
			char **inargv = malloc(sizeof(char *) * 1024);
			int tokln = tokenize(buf, inargv) + 1;

			// Resize
			char **tmp;
			if ((tmp = reallocarray(inargv, tokln, sizeof(char *))) != NULL)
				inargv = tmp;

			// Construct program vector
			char *myargv[tokln];

			for (int i = 0; i < tokln; ++i)
			{
				myargv[i] = inargv[i];
			}
			myargv[tokln - 1] = NULL;
			free(inargv);

			if (tokln > 1)
			{
				exec_prog(myargv);
				for (int i = 0; i < tokln; ++i)
				{
					if (myargv[i] != NULL)
					{
						free(myargv[i]);
					}
				}
			}

			break;
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

void exec_prog(char *myargv[])
{
	if (strstr(myargv[0], "/") == NULL)
	{
		char *fullpath;
		char *path = strdup(getenv("PATH"));
		char *tok = strtok(path, ":");
		while (tok != NULL)
		{
			fullpath = malloc(strlen(tok) + strlen(myargv[0]) + 2);
			strcpy(fullpath, tok);
			strcat(fullpath, "/");
			strcat(fullpath, myargv[0]);

			if (access(fullpath, X_OK) == 0)
			{
				myargv[0] = fullpath;
				execv(myargv[0], myargv);
			}
			free(fullpath);

			tok = strtok(NULL, ":");
		}
		free(path);
	}
	else
	{
		execv(myargv[0], myargv);
	}
	perror("execv failed");
}

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
