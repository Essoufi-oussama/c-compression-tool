#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#define BUFFER_SIZE 100

typedef struct s_alpha
{
	bool			is_leaf;
	unsigned char	c;
	unsigned int	freq;
	struct s_alpha	*right;
	struct s_alpha	*left;
}					t_alpha;

typedef struct s_table
{
	unsigned char	c;
	unsigned int	freq;
	int				code;
	char			bits;
}	t_table;

typedef struct s_data
{
	FILE	*fp;
	t_alpha	**frequencies;
	int		n_freq;
	t_alpha	*root ;
	t_table	**table;
} t_data;

char				*strjoin(char *s1, char *s2);
char				*ft_strdup(const char *s1);
