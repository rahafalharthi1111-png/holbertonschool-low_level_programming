#include "main.h"

/**
 * _strcat - Concatenates two strings
 * @dest: Destination string
 * @src: Source string to append
 *
 * Return: Pointer to the resulting string dest
 */
char *_strcat(char *dest, char *src)
{
	int i = 0, j = 0;

	/* Find the end of dest */
	while (dest[i] != '\0')
		i++;

	/* Copy src to the end of dest */
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}

	/* Add terminating null byte */
	dest[i] = '\0';

	return (dest);
}
