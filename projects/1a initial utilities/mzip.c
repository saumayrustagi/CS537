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

	if (argc < 2) fd = STDIN_FILENO;
	else if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		return -1;
	}

	while ((nread = read(fd, buf, sizeof buf)) > 0)
	{
		char wrbuf[1];
		wrbuf[0] = buf[0];
		int count = 1;
		for (int i = 0; i < nread - 1; ++i)
		{
			if (buf[i + 1] == buf[i])
			{
				++count;
			}
			else
			{
				if (stdoutput(&count, sizeof count) == 1)
					return 1;
				if (stdoutput(wrbuf, 1) == 1)
					return 1;

				wrbuf[0] = buf[i + 1];
				count = 1;
			}
		}
	}

	if (argc >= 2) close(fd);
	printf("%c", '\n');

	return 0;
}