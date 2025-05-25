#include "../header.h"

void	update_frequencies(t_alpha **map, uint8_t c, t_data *data)
{
	if (!map[c])
		map[c] = add_non_leaf(c, map, data);
	else
		map[c]->freq += 1;
}

void	read_file(t_data *data)
{
	t_alpha	*map[256] = {0};
	int		j;
	uint8_t	c;

	j = 0;
	while (!feof(data->fp))
	{
		fread(&c, sizeof(uint8_t), 1, data->fp);
		if (!feof(data->fp))
			update_frequencies(map, (uint8_t) c, data);
	}
	data->frequencies = malloc(sizeof(t_alpha *) * (data->n_freq));
	if (data->frequencies == NULL)
	{
		fclose(data->fp);
		fatal_error_handle(map, 1);
	}
	for (int i = 0; i < 256; i++)
	{
		if (map[i])
		{
			data->frequencies[j] = map[i];
			j++;
		}
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		fprintf(stderr, "Invalid Usage: ./encode file\n");
		return (1);
	}
	memset(&data, 0, sizeof(t_data));
	data.fp = fopen(argv[1], "rb");
	if (data.fp == NULL)
	{
		perror("fopen\n");
		return (1);
	}
	check_empty_file(data.fp);
	read_file(&data);
	fclose(data.fp);
	sort_array(&data);
	build_huffman_tree(&data);
	build_lookup_table(&data);
	encode(&data, argv[1]);
	clear_tree(data.root);
	clear_lookup_table(&data);
	return (0);
}
