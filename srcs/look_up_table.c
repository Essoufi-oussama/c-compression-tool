#include "header.h"


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
		fclose(data->fp);
		fprintf(stderr, "Malloc failure\n");
		clear_tree(data->root);
		exit(1);
	}
	populate_table(data, data->root, 0, 0);
}