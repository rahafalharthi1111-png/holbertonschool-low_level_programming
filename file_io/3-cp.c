#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

#define BUF_SIZE 1024

/**
 * print_err_and_exit - print message to stderr and exit with code
 */
static void print_err_and_exit(int code, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vdprintf(STDERR_FILENO, fmt, ap);
	va_end(ap);
	exit(code);
}

/**
 * main - copy file_from to file_to
 */
int main(int argc, char *argv[])
{
	int fd_from = -1, fd_to = -1;
	ssize_t rlen, wlen;
	char buf[BUF_SIZE];
	ssize_t to_write, written;
	char *buf_ptr;

	if (argc != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}

	fd_from = open(argv[1], O_RDONLY);
	if (fd_from == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", argv[1]);
		exit(98);
	}

	fd_to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd_to == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't write to %s\n", argv[2]);
		if (close(fd_from) == -1)
			dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_from);
		exit(99);
	}

	while (1)
	{
		rlen = read(fd_from, buf, BUF_SIZE);
		if (rlen == 0) /* EOF */
			break;
		if (rlen == -1)
		{
			if (errno == EINTR)
				continue; /* interrupted, retry read */
			/* read error */
			dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", argv[1]);
			if (close(fd_from) == -1)
				dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_from);
			if (close(fd_to) == -1)
				dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_to);
			exit(98);
		}

		/* write loop to handle partial writes */
		to_write = rlen;
		buf_ptr = buf;
		while (to_write > 0)
		{
			wlen = write(fd_to, buf_ptr, (size_t)to_write);
			if (wlen == -1)
			{
				if (errno == EINTR)
					continue; /* retry write on interrupt */
				/* write error */
				dprintf(STDERR_FILENO, "Error: Can't write to %s\n", argv[2]);
				if (close(fd_from) == -1)
					dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_from);
				if (close(fd_to) == -1)
					dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_to);
				exit(99);
			}
			written = wlen;
			to_write -= written;
			buf_ptr += written;
		}
	}

	/* close descriptors, checking errors and printing fd numbers */
	if (close(fd_from) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_from);
		if (close(fd_to) == -1)
			dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_to);
		exit(100);
	}
	if (close(fd_to) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_to);
		exit(100);
	}

	return (0);
}
