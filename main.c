
#include "header.h"

void	lst_clear(t_alpha **head)
{
	t_alpha	*current;
	t_alpha	*tmp;

	current = *head;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
}

void	fatal_error_handle(t_alpha **map)
{
	for (int i = 0; i < 256; i++)
	{
		if (map[i])
			free(map[i]);
	}
	write(2, "Malloc failure\n", 16);
	exit(1);
}

t_alpha	*lstadd(char c, t_alpha **map, FILE *fp)
{
	t_alpha	*new;

	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		fclose(fp);
		fatal_error_handle(map);
	}
	new->c = c;
	new->freq = 1;
	new->next = NULL;
	return (new);
}

void	update_frequencies(t_alpha **map, char c, FILE *fp)
{
	if (!map[(unsigned char) c])
		map[(unsigned char) c] = lstadd(c, map, fp);
	else
		map[(unsigned char) c]->freq += 1;

}

t_alpha	*read_file(FILE *fp)
{
	char	buffer[BUFFER_SIZE] = {0};
	t_alpha	*head = NULL;
	t_alpha *map[256] = {0};

	while (!feof(fp))
	{
		char c = fgetc(fp);
		update_frequencies(map, c, fp);
	}
	for (int i = 0; i < 256; i++)
	{
		if (map[i])
		{
			map[i]->next = head;
			head = map[i];
		}
	}
	return (head);
}

void	sort_list(t_alpha **head)
{
	t_alpha	*current;
	t_alpha	*tmp;

	current = *head;
	while (current)
	{
		tmp = current->next;
		while (tmp)
		{
			if (tmp->freq < current->freq)
			{
				size_t	tmp_freq = current->freq;
				char	c = current->c;
				current->freq = tmp->freq;
				current->c = tmp->c;
				tmp->freq = tmp_freq;
				tmp->c = c;
			}
			tmp = tmp->next;
		}
		current = current->next;
	}
	current = *head;
	while (current)
	{
		printf("%zu\n", current->freq);
		current = current->next;
	}
}

int	main(int argc, char **argv)
{
	FILE	*fp;
	t_alpha	*head;

	if (argc != 2)
		fprintf(stderr, "Invalid Usage: ./a.out file");
	fp = fopen(argv[1], "r");
	head = read_file(fp);
	fclose(fp);
	sort_list(&head);
	lst_clear(&head);
}

/* First, create a collection of n
 initial Huffman trees, each of which is a single leaf
 node containing one of the letters. Put the n
 partial trees onto a priority queue organized
 by weight (frequency). Next, remove the first two trees
 (the ones with lowest weight) from the priority queue.
 Join these two trees together to create a new tree whose root
 has the two trees as children, and whose weight is the sum of
 the weights of the two trees. Put this new tree back into the priority queue.
 This process is repeated until all of the partial Huffman trees have been combined
 into one. */
