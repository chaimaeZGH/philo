/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/19 22:43:09 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	unsigned long long	result;
	int					sign;
	int					i;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		if (result > 9223372036854775807)
			return (-1);
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (result * sign);
}

void	ft_putlong(unsigned long tim)
{
	char	a;

	if (tim >= 0 && tim <= 9)
	{
		a = tim + '0';
		write (1, &a, 1);
		return ;
	}
	if (tim > 9)
	{
		ft_putlong (tim / 10);
		ft_putlong (tim % 10);
	}
	if (tim < 0)
	{
		write (1, "-", 1);
		ft_putlong(tim * -1);
	}
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
	write(1, "\n", 1);
}

void	write_state(int id, unsigned long tim, char *str)
{
	ft_putlong(tim);
	write(1, " ", 1);
	ft_putlong((unsigned long)id);
	write(1, " ", 1);
	ft_putstr(str);
}

unsigned long	time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL));
}
