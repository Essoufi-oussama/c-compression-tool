#include "../header.h"


void    search_tree(t_alpha *root, uint32_t byte, int *depth, int bits_count, int *c)
{
    t_alpha *current = root;
    *depth = 0;
    *c = 257;

    while (current != NULL && *depth < bits_count)
    {
        if (!current->is_leaf)
        {
            *c = current->c;
            return ; 
        }
        uint8_t bit = (byte >> (bits_count - 1 - *depth)) & 0x01;
        (*depth)++;
        if (bit == 0)
            current = current->left;
        else
            current = current->right;
    }
    *depth = 0;
}

void    decode(t_data *data)
{
    char    buffer[BUFFER_SIZE];
    uint32_t buffer_byte = 0;
    int j = 0;
    uint8_t byte;
    int bits_count = 0;

    while (fread(&byte, sizeof(uint8_t), 1, data->fp) == 1)
    {
        buffer_byte = (buffer_byte << 8) | byte;
        bits_count += 8;
        while (bits_count > 0)
        {
            int c = 257;
            int depth = 0;
            search_tree(data->root , buffer_byte, &depth, bits_count, &c);
            if (c != 257 && depth > 0)
            {
                buffer[j++] = (char) c;
                if (j == BUFFER_SIZE)
                {
                    fwrite(buffer, BUFFER_SIZE, 1, data->out_fp);
                    j = 0;
                }
                bits_count -= depth;
                buffer_byte = buffer_byte & ((1U << bits_count) - 1);
            }
            else
                break;
        }
    }
    if (j > 0)
        fwrite(buffer, j, 1, data->out_fp);
}
