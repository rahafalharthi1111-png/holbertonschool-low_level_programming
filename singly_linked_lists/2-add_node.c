#include "lists.h"

list_t *add_node(list_t **head, const char *str)
{
	list_t *new;
	unsigned int len = 0;

	new = malloc(sizeof(list_t));
	if (new == NULL)
		return (NULL);

	while (str[len])
		len++;

	new->str = strdup(str);
	if (new->str == NULL)
	{
		free(new);
		return (NULL);
	}

	new->len = len;
	new->next = *head;
	*head = new;

	return (new);
}
