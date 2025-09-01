#include "main.h"
#include <limits.h>

/**
 * _atoi - Converts a string to an integer
 * @s: The string to convert
 *
 * Return: The converted integer, or 0 if no numbers found
 */
int _atoi(char *s)
{
	int i = 0, sign = 1, num_started = 0;
	int result = 0;

	while (s[i] != '\0')
	{
		if (s[i] == '-')
			sign *= -1;
		else if (s[i] == '+')
			;
		else if (s[i] >= '0' && s[i] <= '9')
		{
			num_started = 1;

			if (sign == -1)
			{
				if (result < (INT_MIN + (s[i] - '0')) / 10)
					return (INT_MIN);
				result = result * 10 - (s[i] - '0');
			}
			else
			{
				if (result > (INT_MAX - (s[i] - '0')) / 10)
					return (INT_MAX);
				result = result * 10 + (s[i] - '0');
			}
		}
		else if (num_started)
			break;

		i++;
	}

	return (result);
}
