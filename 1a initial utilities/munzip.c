#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int stdoutput(void *buf, int size)
{
	ssize_t total_written = 0;
	while (total_written < size)
	{
		ssize_t written = write(STDOUT_FILENO, buf + total_written, size - total_written);
		if (written < 1)
			return 1;
		total_written += written;
	}
	return 0;
}

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
		for (int i = 0; i < count; ++i)
		{
			stdoutput(&c, sizeof(c));
		}
	}

	if (argc >= 2)
		close(fd);
	printf("%c", '\n');

	return 0;
}