#include "main.h"
#include <stdio.h>
#define BUFFER_SIZE 1024
/**
 * error_exit - Prints error message and exits
 * @code: Exit code
 * @msg: Error message format
 * @arg: Argument for error message
 */
void error_exit(int code, const char *msg, const char *arg)
{
	dprintf(STDERR_FILENO, msg, arg);
	exit(code);
}
/**
 * main - Copies content of a file to another *file
 * @argc: Number of arguments
 * @argv: Array of arguments
 * Return: 0 on success, or exit code on failure
 */
int main(int argc, char *argv[])
{
	int fd_from, fd_to, rd, wr;
	char *buffer;

	if (argc != 3)
	{
		error_exit(97, "Usage: cp file_from file_to\n", NULL);
	}
	buffer = malloc(sizeof(char) * 1024);
	if (buffer == NULL)
	{
		error_exit(99, "Error: Can't write to %s\n", argv[2]);
	}
	fd_from = open(argv[1], O_RDONLY);
	rd = read(fd_from, buffer, 1024);
	fd_to = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0664);
	do {
		if (fd_from == -1 || rd == -1)
		{
			error_exit(98, "Error: Can't read from file %s\n", argv[1]);
		}
		wr = write(fd_to, buffer, rd);
		if (fd_to == -1 || wr == -1)
		{
			error_exit(99, "Error: Can't write to %s\n", argv[2]);
		}
		rd = read(fd_from, buffer, 1024);
		fd_to = open(argv[2], O_WRONLY | O_APPEND);
	} while (rd > 0);
	free(buffer);
	if (close(fd_from) == -1)
	{
		error_exit(100, "Error: Can't close fd %d\n", argv[1]);
	}
	if (close(fd_to) == -1)
	{
		error_exit(100, "Error: Can't close fd %d\n", argv[2]);
	}
	return (0);
}
