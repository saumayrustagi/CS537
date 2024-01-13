#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int x = 100;
	printf("My PID = %d\n", getpid());


	int rc = fork();

	if (rc < 0)
		return 1;
	else if (rc == 0)
	{
		printf("%d\n", x);
		x += 50;
		printf("%d\n", x);
	}
	else
	{
		printf("%d\n", x);
		x += 25;
		printf("%d\n", x);
	}
	return 0;
}