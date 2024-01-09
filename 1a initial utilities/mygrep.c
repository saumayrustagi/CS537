#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: mygrep PATTERNS [FILE]\n");
		return 1;
	}

	int fd;

	if ((fd = open(argv[2], O_RDONLY)) == -1)
		return 1;

	char buf[4096];
	ssize_t nread;

	while ((nread = read(fd, buf, sizeof buf)) > 0)
	{
		char *line = strtok(buf, "\n");
		while (line != NULL)
		{
			if (strstr(line, argv[1]))
			{
				ssize_t written;
				ssize_t total_written = 0;
				while (total_written < strlen(line))
				{
					written = write(STDOUT_FILENO, line + total_written, strlen(line) - total_written);
					if (written < 1)
					{
						return 1;
					}
					total_written += written;
				}
				if (write(STDOUT_FILENO, "\n", sizeof "\n") < 1)
				{
					return 1;
				}
			}
			line = strtok(NULL, "\n");
		}
	}

	close(fd);

	return 0;
}