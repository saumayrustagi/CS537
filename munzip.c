#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	char buf[4096];
	ssize_t nread;

	if (argc < 2)
		fd = STDIN_FILENO;
	else if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		return -1;
	}

	int count;
	char c;

	while (read(fd, &count, sizeof count) > 0 && read(fd, &c, sizeof c) > 0)
	{
		printf("%c: %d\n", c, count);
	}

	if (argc >= 2)
		close(fd);
	printf("%c", '\n');

	return 0;
}