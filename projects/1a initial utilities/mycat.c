#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int cat(int fd)
{
	char buf[4096];
	ssize_t nread;

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

	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	if (argc < 2)
	{
		fd = STDIN_FILENO;
		cat(fd);
	}
	else
		for (int i = 1; i < argc; ++i)
		{
			if ((fd = open(argv[i], O_RDONLY)) == -1)
			{
				return 1;
			}

			if (cat(fd) == 1)
				return 1;

			close(fd);
		}

	return 0;
}
