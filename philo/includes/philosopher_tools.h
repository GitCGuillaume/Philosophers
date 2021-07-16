/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_tools.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 13:31:45 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/16 11:45:14 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_TOOLS_H
# define PHILOSOPHER_TOOLS_H

long int	math_time(void);
int			argv_ok(char *str);
int			check_args(int argc, char **argv);
int			ft_atoi(const char *str);
int			ft_str_is_digit(char *str);

#endif
