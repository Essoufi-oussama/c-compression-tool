/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:31:35 by oessoufi          #+#    #+#             */
/*   Updated: 2025/05/23 18:08:02 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


void	fatal_error_handle(t_alpha **map)
{
	for (int i = 0; i < 256; i++)
	{
		if (map[i])
			free(map[i]);
	}
	write(2, "Malloc failure\n", 16);
	exit(1);
}

void	clear_tree_error(t_alpha *root)
{
	if (root == NULL)
		return ;
	clear_tree_error(root->left);
	clear_tree_error(root->right);
	if (root->is_leaf)
		free(root);
}

void	clear_tree(t_alpha *root)
{
	if (root == NULL)
		return ;
	clear_tree(root->left);
	clear_tree(root->right);
	free(root);
}

void	clear_array(t_alpha **frequencies, int i)
{
	for (int j = 0; j < i; j++)
	{
		if (frequencies[j]->is_leaf)
			clear_tree(frequencies[j]);
		else
			free(frequencies[j]);
	}
	free(frequencies);
}

void	clear_lookup_table(t_data *data)
{
	for (int j = 0; j < 256; j++)
	{
		free(data->table[j]);
	}
	free(data->table);
}