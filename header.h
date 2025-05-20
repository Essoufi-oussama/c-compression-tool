#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 100

typedef struct s_alpha
{
    unsigned char c;
    size_t freq;
    struct s_alpha *next;
}   t_alpha ;

char    *strjoin(char *s1, char *s2);
char	*ft_strdup(const char *s1);


