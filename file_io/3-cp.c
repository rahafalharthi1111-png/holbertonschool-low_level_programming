#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
/* dprintf is allowed by the requirements */
#include <dprintf.h>

/* Define the buffer size as required (1024 bytes) */
#define BUFF_SIZE 1024

/**
 * error_check - Helper function to close a file descriptor and handle errors.
 * @fd: The file descriptor to be closed.
 *
 * If closing fails, it prints an error message to STDERR_FILENO
 * and exits with code 100.
 */
void error_check(int fd)
{
    if (close(fd) == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
        exit(100);
    }
}

/**
 * main - Copies the content of a file (file_from) to another file (file_to).
 * @argc: The number of arguments passed to the program.
 * @argv: An array of strings (arguments) [program_name, file_from, file_to].
 *
 * Error/Exit handling:
 * - 97: Incorrect number of arguments.
 * - 98: Cannot read from file_from.
 * - 99: Cannot write to file_to.
 * - 100: Cannot close a file descriptor.
 *
 * Return: 0 on success.
 */
int main(int argc, char *argv[])
{
    int fd_from, fd_to;
    ssize_t read_bytes, write_bytes;
    char buffer[BUFF_SIZE];
    char *file_from, *file_to;

    /* 1. Check for correct number of arguments */
    if (argc != 3)
    {
        dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
        exit(97);
    }

    file_from = argv[1];
    file_to = argv[2];

    /* 2. Open the source file (file_from) for read-only access */
    fd_from = open(file_from, O_RDONLY);
    if (fd_from == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", file_from);
        exit(98);
    }

    /* 3. Open/Create the destination file (file_to) */
    /*
     * O_CREAT: Create the file if it does not exist.
     * O_WRONLY: Open for writing only.
     * O_TRUNC: Truncate the file to zero length if it already exists.
     * Permissions: rw-rw-r-- (0664)
     */
    fd_to = open(file_to, O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (fd_to == -1)
    {
        /* If open/create fails, close the source file first */
        error_check(fd_from);
        dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file_to);
        exit(99);
    }

    /* 4. Read/Write Loop: Read 1024 bytes at a time */
    while ((read_bytes = read(fd_from, buffer, BUFF_SIZE)) > 0)
    {
        /* Write the bytes that were just read */
        write_bytes = write(fd_to, buffer, read_bytes);

        /* Check for write success (write_bytes must equal read_bytes) */
        if (write_bytes == -1 || write_bytes != read_bytes)
        {
            /* If write fails, close both file descriptors before exiting */
            error_check(fd_from);
            error_check(fd_to);
            dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file_to);
            exit(99);
        }
    }

    /* 5. Check for read errors (read_bytes == -1) after loop exit */
    if (read_bytes == -1)
    {
        /* If read failed (not EOF), close both FDs and report error 98 */
        error_check(fd_from);
        error_check(fd_to);
        dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", file_from);
        exit(98);
    }

    /* 6. Close file descriptors on success */
    error_check(fd_to);
    error_check(fd_from);

    return (0);
}
