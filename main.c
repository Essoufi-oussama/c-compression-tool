
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
		free(frequencies[j]);
	free(frequencies);
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

	while (!feof(fp))
	{
		char c = fgetc(fp);
		update_frequencies(map, c, fp, i);
	}
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

t_alpha *add_leaf(t_alpha *left, t_alpha **frequencies, int right_i, int n)
{
	t_alpha *new;

	new = malloc(sizeof(t_alpha));
	if (new == NULL)
	{
		write(2, "Malloc error\n", 14);
		clear_tree_error(left);
		clear_array(frequencies, n);
		exit (1);
	}
	if (left == NULL)
		new->freq = frequencies[right_i]->freq;
	else
		new->freq = frequencies[right_i]->freq + left->freq;
	new->is_leaf = 1;
	new->c = 0;
	new->left = left;
	new->right = frequencies[right_i];
	return (new);
}

t_alpha *build_huffman_tree(t_alpha **frequencies, int n)
{
	t_alpha *root = NULL;

	if (n == 1)
		root = add_leaf(root, frequencies, 0, n);
	else
	{
		root = add_leaf(frequencies[0], frequencies, 1, n);
		for (int i = 2; i < n; i++)
			root = add_leaf(root, frequencies, i, n);
	}
	return (root);
}
// need to check for empty file

int	main(int argc, char **argv)
{
	FILE	*fp;
	t_alpha	**frequencies;
	int		i = 0;

	if (argc != 2)
	{
		fprintf(stderr, "Invalid Usage: ./a.out file");
		return (1);
	}
	fp = fopen(argv[1], "r");
	frequencies = read_file(fp, &i);
	fclose(fp);
	sort_array(frequencies, i);
	t_alpha *root =build_huffman_tree(frequencies, i);
	free(frequencies);
	// print_tree(root, 0);
	clear_tree(root);
}
