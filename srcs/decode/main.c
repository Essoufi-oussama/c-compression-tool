#include "../header.h"

void    build_frequencies_arr(t_data *data, t_alpha **map)
{
    int j = 0;
    data->frequencies = malloc(sizeof(t_alpha *) * data->n_freq);
    if (data->frequencies == NULL)
    {
        write(2, "malloc error\n", 14);
        fclose(data->fp);
        exit(1);
    }
    for (int i = 0; i < 256; i++)
    {
        if (map[i])
            data->frequencies[j++] = map[i];
    }
}

void    read_header(t_data *data, char *file)
{
    uint8_t c;
    uint32_t freq;
    t_alpha *map[256] = {0};

    data->fp = fopen(file, "r");
    if (data->fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    check_empty_file(data->fp);
    for (int i = 0; i < 256; i++)
    {
        if (!feof(data->fp))
            fread(&c, sizeof(uint8_t), 1, data->fp);
        if (!feof(data->fp))
            fread(&freq, sizeof(uint32_t), 1, data->fp);
        if (feof(data->fp))
            break;
        if (freq != 0)
        {
            map[i] = add_non_leaf(c, map, data);
            map[i]->freq = freq;
        }
    }
    if (feof(data->fp))
    {
        fclose(data->fp);
        write(2, "error not valid file\n", 22);
        fatal_error_handle(map, 0);
    }
    build_frequencies_arr(data, map);
    sort_array(data);
}

int main(int argc, char **argv)
{
    t_data data;
    if (argc != 2)
    {
        fprintf(stderr, "invalid usage ./decode file\n");
        return (1);
    }
    memset(&data, 0, sizeof(t_data));
    read_header(&data, argv[1]);
    build_huffman_tree(&data);
    build_lookup_table(&data);
    data.out_fp = fopen("original", "w");
    if (data.out_fp == NULL)
    {
        perror("fopen");
        clear_tree(data.root);
        return (1);
    }
    decode(&data);
    fclose(data.fp);
    clear_tree(data.root);
}
