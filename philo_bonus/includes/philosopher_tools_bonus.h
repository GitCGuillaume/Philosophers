/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_tools_bonus.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 12:00:54 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/17 18:09:54 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_TOOLS_BONUS_H
# define PHILOSOPHER_TOOLS_BONUS_H

# include <stdlib.h>

int			is_dying(long int current_time,
				long int time_simulation, long int time_to);
long int	math_time(void);
int			argv_ok(char *str);
int			check_args(int argc, char **argv);
ssize_t			ft_atoi(const char *str);
char		*ft_strdup(const char *s1);
char		*ft_itoa(int n);
int			ft_str_is_digit(char *str);
char		*ft_strjoin(char const *s1, char const *s2);
int			check_values(char **argv, int argc);

#endif
