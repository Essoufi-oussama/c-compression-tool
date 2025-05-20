
#include "header.h"

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

void clear_tree_error(t_alpha *root)
{
	if (root == NULL)
		return ;
	clear_tree_error(root->left);
	if (root->is_leaf)
		free(root);
}

void clear_tree(t_alpha *root)
{
	if (root == NULL)
		return;
	clear_tree(root->left);
	clear_tree(root->right);
	free(root);
}

void clear_array(t_alpha **frequencies, int i)
{
	for (int j = 0; j < i; j++)
	{
		if (frequencies[j]->is_leaf)
			clear_tree(frequencies[j]);
		else
			free(frequencies[j]);
	}
	free(frequencies);
}

void clear_lookup_table(t_table **table, int i)
{
	for (int j = 0; j < i; j++)
	{
		free(table[j]);
	}
	free(table);
}

t_alpha	*add_non_leaf(char c, t_alpha **map, FILE *fp, int *i)
{
	t_alpha	*new;

	(*i)++;
	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		fclose(fp);
		fatal_error_handle(map);
	}
	new->c = c;
	new->freq = 1;
	new->is_leaf = 0;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void	update_frequencies(t_alpha **map, char c, FILE *fp, int *i)
{
	if (!map[(unsigned char) c])
		map[(unsigned char) c] = add_non_leaf(c, map, fp, i);
	else
		map[(unsigned char) c]->freq += 1;
}

t_alpha	**read_file(FILE *fp, int *i)
{
	char	buffer[BUFFER_SIZE] = {0};
	t_alpha *map[256] = {0};
	int j = 0;
	t_alpha **frequency_array;
	char c;

	while ((c = fgetc(fp)) != EOF)
		update_frequencies(map, c, fp, i);
	frequency_array = malloc(sizeof(t_alpha *) * (*i));
	if (frequency_array == NULL)
		fatal_error_handle(map);
	for (int i = 0; i < 256; i++)
	{
		if (map[i])
		{
			frequency_array[j] = map[i];
			j++;
		}
	}
	return (frequency_array);
}

void	sort_array(t_alpha **frequency_array, int n)
{
	for(int i = 0; i < n; i++)
	{
		int	lowest_index = i;
		for (int j = i + 1; j < n; j++)
		{
			if (frequency_array[j]->freq < frequency_array[lowest_index]->freq)
				lowest_index = j;
		}
		t_alpha *tmp = frequency_array[i];
		frequency_array[i] = frequency_array[lowest_index];
		frequency_array[lowest_index] = tmp;
	}
}

void	add_leaf(t_alpha **frequencies, int i, int n)
{
	t_alpha *new;

	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		write(2, "Malloc error\n", 14);
		clear_array(frequencies + i, n);
		exit (1);
	}
	new->right = frequencies[i + 1];
	new->left  = frequencies[i];
	new->freq = new->right->freq + new->left->freq;
	new->is_leaf = 1;
	new->c = 0;
	frequencies[i + 1] = new;
	sort_array(frequencies + (i + 1), n - (i + 1));

}

t_alpha *build_huffman_tree(t_alpha **frequencies, int n)
{
	t_alpha *root = NULL;
	

	if (n > 1)
	{
		for (int i = 0; i < n - 1; i++)
			add_leaf(frequencies, i, n);
	}
	root = frequencies[n - 1];
	free(frequencies);
	return (root);
}

t_table	*create_new_table_entry(t_alpha *original_root,t_table **table, t_alpha *current, int n, int result, int depth)
{
	t_table *new;

	new = malloc(sizeof(t_table));
	if (new == NULL)
	{
		fprintf(stderr, "Malloc error\n");
		clear_tree(original_root);
		for (int i = 0; i < n; i++)
			free(table[i]);
		free(table);
		exit(1);
	}
	new->c = current->c;
	new->freq = current->freq;
	new->code = result;
	new->bits = depth;
	return (new);
}

void	populate_table(t_alpha *original_root, t_alpha *current ,t_table **table, int *i, int result, int depth)
{
	if (current == NULL)
		return;
	if (!current->is_leaf)
	{
		table[*i] = create_new_table_entry(original_root, table, current, *i, result, depth);
		(*i)++;
	}
	populate_table(original_root, current->left, table, i, result << 1 | 0, depth + 1);
	populate_table(original_root, current->right, table, i, result << 1 | 1, depth + 1);
}

t_table **build_lookup_table(t_alpha *root, int n)
{
	t_table **table;
	int i = 0;
	
	table = malloc(sizeof(t_table *) * n);
	if (table == NULL)
	{
		fprintf(stderr, "Malloc failure\n");
		clear_tree(root);
	}
	populate_table(root, root, table, &i, 0, 0);
	return (table);
}

// need to check for empty file

// void	print_lokout_table(t_table **table, int n)
// {
// 	printf("char	freq	code  	Num_bits   \n");
// 	for (int i = 0; i < n; i++)
// 	{
// 		printf("%c	 %d	  %d	%d\n",table[i]->c ,table[i]->freq ,table[i]->code ,table[i]->bits);
// 	}
// }

int	main(int argc, char **argv)
{
	FILE	*fp;
	t_alpha	**frequencies;
	int		i = 0;
	t_alpha *root ;
	t_table **table = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "Invalid Usage: ./a.out file\n");
		return (1);
	}
	fp = fopen(argv[1], "r");
	frequencies = read_file(fp, &i);
	fclose(fp);
	sort_array(frequencies, i);
	root = build_huffman_tree(frequencies, i);
	table = build_lookup_table(root, i);
	clear_tree(root);
	// print_lokout_table(table, i);
	clear_lookup_table(table, i);

}
