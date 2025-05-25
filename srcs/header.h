#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 4096
#define MAX_BITS 16

typedef struct s_alpha
{
	bool			is_leaf;
	uint8_t			c;
	uint32_t		freq;
	struct s_alpha	*right;
	struct s_alpha	*left;
}					t_alpha;

typedef struct s_table
{
	unsigned char	c;
	unsigned int	freq;
	int				code;
	char			bits;
}					t_table;

typedef struct s_data
{
	FILE			*fp;
	FILE			*out_fp;
	t_alpha			**frequencies;
	int				n_freq;
	t_alpha			*root;
	t_table			**table;
}					t_data;

char				*strjoin(char *s1, char *s2);
char				*ft_strdup(const char *s1);
void				clear_lookup_table(t_data *data);
void				clear_array(t_alpha **frequencies, int i);
void				clear_tree(t_alpha *root);
void				clear_tree_error(t_alpha *root);
void				fatal_error_handle(t_alpha **map, int flag);
void				encode(t_data *data, char *str);
void				build_lookup_table(t_data *data);
void				sort_array(t_data *data);
t_alpha				*add_non_leaf(uint8_t c, t_alpha **map, t_data *data);
void				add_leaf(t_data *data, int i);
void				build_huffman_tree(t_data *data);
void				decode(t_data *data);
void				check_empty_file(FILE *fp);
