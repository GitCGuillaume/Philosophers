/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 14:05:29 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/02 10:05:01 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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
		write(2, "Error\nNot enough parameters.\n", 29);
		return (0);
	}
	if (argc > 6)
	{
		write(2, "Error\nToo much parameters\n", 26);
		return (0);
	}
	while (argc > i)
	{
		if (argv_ok(argv[i]) == 0)
		{
			write(2, "Error\n", 6);
			return (0);
		}
		i++;
	}
	return (1);
}
