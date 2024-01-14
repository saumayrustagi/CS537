#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
	pid_t cpid;
	int pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0)
	{
		pid_t c2pid;

		c2pid = fork();
		if (c2pid == -1)
		{
			perror("fork");
			_exit(EXIT_FAILURE);
		}
		if (c2pid == 0)
		{
			char buf;

			close(pipefd[1]);
			while (read(pipefd[0], &buf, 1) > 0)
				write(STDOUT_FILENO, &buf, 1);

			// write(STDOUT_FILENO, '\n', 1);
			write(STDOUT_FILENO, "\n", 1);
			close(pipefd[0]);
			_exit(EXIT_SUCCESS);
		}
		else
		{
			close(pipefd[0]);
			write(pipefd[1], argv[1], strlen(argv[1]));
			close(pipefd[1]);
			wait(NULL);
			_exit(EXIT_SUCCESS);
		}
	}
	else
	{
		close(pipefd[0]);
		close(pipefd[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
	return 0;
}