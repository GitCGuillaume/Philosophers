/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 16:45:09 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/05 15:45:55 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_tools_bonus.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strdup(const char *s1)
{
	char		*ptr_char;
	size_t		i;

	i = 0;
	ptr_char = malloc(ft_strlen(s1) + 1);
	if (ptr_char == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		*(ptr_char + i) = s1[i];
		i++;
	}
	*(ptr_char + i) = '\0';
	return (ptr_char);
}
