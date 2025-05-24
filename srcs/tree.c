#include "header.h"

t_alpha	*add_non_leaf(uint8_t c, t_alpha **map, t_data *data)
{
	t_alpha	*new;

	data->n_freq += 1;
	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		fclose(data->fp);
		fatal_error_handle(map, 1);
	}
	new->c = c;
	new->freq = 1;
	new->is_leaf = 0;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void	add_leaf(t_data *data, int i)
{
	t_alpha	*new;

	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		write(2, "Malloc error\n", 14);
		fclose(data->fp);
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