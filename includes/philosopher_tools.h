/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_tools.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 13:31:45 by gchopin           #+#    #+#             */
/*   Updated: 2021/05/24 15:09:12 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_TOOLS_H
# define PHILOSOPHER_TOOLS_H

int	argv_ok(char *str);
int	check_args(int argc, char **argv);
int	ft_atoi(const char *str);
int	ft_str_is_digit(char *str);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *str, int fd);
void	ft_putnbr_fd(int n, int fd);

#endif
