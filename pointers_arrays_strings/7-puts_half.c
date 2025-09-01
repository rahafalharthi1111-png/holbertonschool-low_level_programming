#include "main.h"
/**
 * puts_half - prints the second half of a string
 * @str: pointer to the string
 *
 * Return: void
 */
void puts_half(char *str)
{
	int len = 0, i;

	while (str[len] != '\0')
		len++;

	for (i = (len + 1) / 2; i < len; i++)
		_putchar(str[i]);

	_putchar('\n');
}
