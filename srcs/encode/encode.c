#include "../header.h"

void	write_header_section(t_data *data)
{
	uint8_t		c;
	uint32_t	freq;

	data->out_fp = fopen("output", "wb");
	if (data->out_fp == NULL)
	{
		perror("fopen");
		clear_tree(data->root);
		clear_lookup_table(data);
		exit(1);
	}
	for (int i = 0; i < 256; i++)
	{
		if (data->table[i])
		{
			c = (uint8_t)data->table[i]->c;
			freq = data->table[i]->freq;
		}
		else
		{
			c = i;
			freq = 0;
		}
		fwrite(&c, sizeof(uint8_t), 1, data->out_fp);
		fwrite(&freq, sizeof(uint32_t), 1, data->out_fp);
	}
}

void	rw_encoded(t_data *data)
{
	uint8_t		buffer[BUFFER_SIZE];
	int			buffer_index = 0;
	uint32_t	bits_buffer = 0;
	int			bit_count = 0;
	int			c;
	uint8_t		byte;

	while ((c = fgetc(data->fp)) != EOF)
	{
		bit_count += data->table[c]->bits;
		bits_buffer = (bits_buffer << data->table[c]->bits) | data->table[c]->code;
		while (bit_count >= 8)
		{
			bit_count -= 8;
			byte = (bits_buffer >> bit_count) & 0xFF;
			buffer[buffer_index++] = byte;
			if (buffer_index == BUFFER_SIZE)
			{
				fwrite(buffer, BUFFER_SIZE, 1, data->out_fp);
				buffer_index = 0;
			}
		}
	}
    if (bit_count > 0)
    {
        byte = (bits_buffer << (8 - bit_count)) & 0xFF;
        buffer[buffer_index++] = byte;
    }
    if (buffer_index > 0)
        fwrite(buffer, buffer_index, 1, data->out_fp);
    fclose(data->fp);
    fclose(data->out_fp);
}

void	encode(t_data *data, char *file)
{
	data->fp = fopen(file, "r");
	if (data->fp == NULL)
	{
		perror("fopen\n");
		clear_tree(data->root);
		clear_lookup_table(data);
		exit (1);
	}
	write_header_section(data);
	rw_encoded(data);
}