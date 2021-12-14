/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_tools_bonus.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 12:00:54 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 17:01:04 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_TOOLS_BONUS_H
# define PHILOSOPHER_TOOLS_BONUS_H

# include <stdlib.h>

long int	math_time(void);
int			argv_ok(char *str);
int			check_args(int argc, char **argv);
ssize_t		ft_atoi(const char *str);
int			ft_str_is_digit(char *str);
int			check_values(char **argv, int argc);

#endif
