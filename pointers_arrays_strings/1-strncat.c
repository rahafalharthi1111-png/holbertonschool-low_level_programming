#include "main.h"

/**
 * _strncat - concatenates two strings
 * @dest: destination string
 * @src: source string
 * @n: maximum number of bytes to use from src
 *
 * Return: pointer to dest
 */
char *_strncat(char *dest, char *src, int n)
{
	int i, j;

	/* find the end of dest */
	i = 0;
	while (dest[i] != '\0')
	{
		i++;
	}

	/* append up to n characters from src */
	j = 0;
	while (j < n && src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}

	/* null terminate dest */
	dest[i] = '\0';

	return (dest);
}
