#include "header.h"

t_alpha	*add_non_leaf(char c, t_alpha **map, t_data *data)
{
	t_alpha	*new;

	data->n_freq += 1;
	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		fclose(data->fp);
		fatal_error_handle(map);
	}
	new->c = c;
	new->freq = 1;
	new->is_leaf = 0;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void	update_frequencies(t_alpha **map, char c, t_data *data)
{
	if (!map[(unsigned char)c])
		map[(unsigned char)c] = add_non_leaf(c, map, data);
	else
		map[(unsigned char)c]->freq += 1;
}

void	read_file(t_data *data)
{
	char	buffer[BUFFER_SIZE] = {0};
	t_alpha	*map[256] = {0};
	int		j;
	t_alpha	**frequency_array;
	char	c;

	j = 0;
	while ((c = fgetc(data->fp)) != EOF)
		update_frequencies(map, c, data);
	data->frequencies = malloc(sizeof(t_alpha *) * (data->n_freq));
	if (data->frequencies == NULL)
		fatal_error_handle(map);
	for (int i = 0; i < 256; i++)
	{
		if (map[i])
		{
			data->frequencies[j] = map[i];
			j++;
		}
	}
}

void	sort_array(t_data *data)
{
	int		lowest_index;
	t_alpha	*tmp;

	for (int i = 0; i < data->n_freq; i++)
	{
		lowest_index = i;
		for (int j = i + 1; j < data->n_freq; j++)
		{
			if (data->frequencies[j]->freq < data->frequencies[lowest_index]->freq)
				lowest_index = j;
		}
		tmp = data->frequencies[i];
		data->frequencies[i] = data->frequencies[lowest_index];
		data->frequencies[lowest_index] = tmp;
	}
}

void	add_leaf(t_data *data, int i)
{
	t_alpha	*new;

	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		write(2, "Malloc error\n", 14);
		clear_array(data->frequencies + i, data->n_freq);
		exit(1);
	}
	new->right = data->frequencies [i + 1];
	new->left = data->frequencies [i];
	new->freq = new->right->freq + new->left->freq;
	new->is_leaf = 1;
	new->c = 0;
	data->frequencies [i + 1] = new;
	sort_array(data);
}

void	build_huffman_tree(t_data  *data)
{
	if (data->n_freq > 1)
	{
		for (int i = 0; i < data->n_freq - 1; i++)
			add_leaf(data, i);
	}
	data->root = data->frequencies[data->n_freq - 1];
	free(data->frequencies);
}

t_table	*create_new_table_entry(t_data *data, t_alpha *current, int result, int depth)
{
	t_table	*new;

	new = malloc(sizeof(t_table));
	if (new == NULL)
	{
		fprintf(stderr, "Malloc error\n");
		clear_tree(data->root);
		for (int i = 0; i < 256; i++)
			free(data->table[i]);
		free(data->table);
		exit(1);
	}
	new->c = current->c;
	new->freq = current->freq;
	new->code = result;
	new->bits = depth;
	return (new);
}

void	populate_table(t_data *data, t_alpha *current, int result, int depth)
{
	if (current == NULL)
		return ;
	if (!current->is_leaf)
	{
		data->table[current->c] = create_new_table_entry(data, current, result, depth);
	}
	populate_table(data, current->left, result << 1 | 0, depth + 1);
	populate_table(data, current->right, result << 1 | 1, depth + 1);
}

void	build_lookup_table(t_data *data)
{
	data->table = calloc(256, sizeof(t_table *));
	if (data->table == NULL)
	{
		fprintf(stderr, "Malloc failure\n");
		clear_tree(data->root);
		exit(1);
	}
	populate_table(data, data->root, 0, 0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		fprintf(stderr, "Invalid Usage: ./a.out file\n");
		return (1);
	}
	memset(&data, 0, sizeof(t_data));
	data.fp = fopen(argv[1], "r");
	if (data.fp == NULL)
	{
		perror("fopen\n");
		return (1);
	}
	read_file(&data);
	fclose(data.fp);
	sort_array(&data);
	build_huffman_tree(&data);
	build_lookup_table(&data);
	data.fp = fopen(argv[1], "r");
	if (data.fp == NULL)
	{
		perror("fopen\n");
		return (1);
	}
	encode(&data);
	clear_tree(data.root);
	clear_lookup_table(&data);
}
