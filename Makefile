ENCODE_SRCS= srcs/cleanup.c srcs/encode/encode.c srcs/encode/main.c srcs/utils.c srcs/look_up_table.c srcs/tree.c 
ENCODE_OBJS = $(ENCODE_SRCS:%.c=%.o)
HEADER = srcs/header.h
DECODE_SRCS= srcs/cleanup.c srcs/decode/main.c srcs/utils.c srcs/look_up_table.c srcs/tree.c  srcs/decode/decode.c
DECODE_OBJS = $(DECODE_SRCS:%.c=%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: encode decode

encode: $(ENCODE_OBJS)
	$(CC) $(CFLAGS) $(ENCODE_OBJS) -o $@

decode: $(DECODE_OBJS)
	$(CC) $(CFLAGS) $(DECODE_OBJS) -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ENCODE_OBJS) 
	rm -rf $(DECODE_OBJS)
fclean: clean
	rm -rf encode

re: fclean all

.PHONY: clean