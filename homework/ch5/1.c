#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	printf("My PID = %d\n", getpid());
	fflush(stdout);
	int rc = fork();
	if (rc < 0)
	{
		perror("fork failed");
		return 1;
	}
	else if (rc == 0)
	{
		close(STDOUT_FILENO);
		printf("Child trying to print!!!");
	}
	else
	{
		printf("parent wait returns %d", (int)waitpid(-1, NULL, (long long)NULL));
	}
	return 0;
}