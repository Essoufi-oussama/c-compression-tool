#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define BUFFER_SIZE 100

typedef struct s_alpha
{
	bool			is_leaf;
	unsigned char	c;
	unsigned int	freq;
	struct s_alpha	*right;
	struct s_alpha	*left;
}					t_alpha;

char				*strjoin(char *s1, char *s2);
char				*ft_strdup(const char *s1);
