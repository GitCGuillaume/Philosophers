/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 14:05:29 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/03 12:02:56 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	argv_ok(char *str)
{
	int	result;

	result = ft_str_is_digit(str);
	return (result);
}

int	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc < 5)
	{
		printf("Error\nNot enough parameters.\n");
		return (1);
	}
	if (argc > 6)
	{
		printf("Error\nToo much parameters.\n");
		return (1);
	}
	while (argc > i)
	{
		if (argv_ok(argv[i]) == 0)
		{
			printf("Error\n");
			return (1);
		}
		i++;
	}
	return (0);
}
