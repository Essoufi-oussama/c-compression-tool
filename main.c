
#include "header.h"

void    fatal_error_handle(size_t *frequencies)
{
    write(2, "Malloc failure\n", 16);
    free(frequencies);
    exit(1);
}

void update_frequencies(size_t *frequencies, char *line)
{
    for (int i = 0; line[i]; i++)
        frequencies[(unsigned char)line[i]]++;

}


size_t	*read_file(FILE *fp)
{
	char		buffer[BUFFER_SIZE];
	char		*line;
	char		*tmp;
    size_t         *frequencies;

    frequencies = calloc(256, sizeof(size_t));
    if (frequencies == NULL)
    {
        fclose(fp);
        fatal_error_handle(NULL);
    }
	line = NULL;
	while (fgets(buffer, BUFFER_SIZE, fp))
	{
		tmp = ft_strdup(buffer);
		if (!tmp)
			fatal_error_handle(frequencies);
		while (!strchr(tmp, '\n') && !feof(fp))
		{
			if (!fgets(buffer, BUFFER_SIZE, fp))
				break ;
			line = strjoin(tmp, buffer);
			free(tmp);
			if (!line)
			{
				fclose(fp);
				fatal_error_handle(frequencies);
			}
			tmp = line;
		}
        update_frequencies(frequencies, tmp);
		free(tmp);
	}
	return (frequencies);
}

int main(int argc, char **argv)
{
    FILE *fp;

    if (argc != 2)
        fprintf(stderr, "Invalid Usage: ./a.out file");
    fp = fopen(argv[1], "r");
    size_t *arr = read_file(fp);
    fclose(fp);
    printf("%zu    %zu\n", arr['t'], arr['X']);
    free(arr);
}