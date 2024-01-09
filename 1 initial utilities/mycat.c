#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

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
		ssize_t total_written = 0;
		while (total_written < nread)
		{
			ssize_t n_written = write(STDOUT_FILENO, buf + total_written, nread - total_written);
			if (n_written < 1)
				return 1;
			total_written += n_written;
		}
	}

	if (argc >= 2) close(fd);

	return 0;
}
