/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/19 23:20:29 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_death(t_philo *philo)
{
	unsigned long	now;
	int				time;
	t_data			*data;
	unsigned long	tim1;

	data = philo->data;
	now = time_ms();
	tim1 = time_ms() - data->start_time;
	time = (int)(now - philo->last_meal_time);
	if (time > data->time_to_die && is_simulation_running(data))
	{
		pthread_mutex_lock(data->writing);
		if (is_simulation_running(data))
			write_state(philo->philo_id, tim1, "died");
		pthread_mutex_lock(data->simulation_mutex);
		data->simulation_running = 0;
		pthread_mutex_unlock(data->simulation_mutex);
		pthread_mutex_unlock(data->writing);
		return (1);
	}
	return (0);
}

void	smart_sleep(t_philo *philo, int ms)
{
	unsigned long	target;
	t_data			*data;

	data = philo->data;
	target = time_ms() + ms;
	while (time_ms() < target)
	{
		// ! BAD TRIP
		// if (check_death(philo))
		// 	break ;
		usleep(50);
	}
}

void	to_write(t_philo *philo, char *str)
{
	t_data			*data;

	data = philo->data;
	pthread_mutex_lock(data->writing);
	if (is_simulation_running(data))
		write_state(philo->philo_id, (time_ms() - data->start_time), str);
	pthread_mutex_unlock(philo->data->writing);
}

void	one_philo(t_philo *philo)
{
	t_data			*data;
	unsigned long	tim;

	data = philo->data;
	tim = time_ms() - data->start_time;
	pthread_mutex_lock(data->writing);
	write_state(philo->philo_id, tim, "is thinking");
	pthread_mutex_unlock(data->writing);
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(data->writing);
	write_state(philo->philo_id, tim, "has taken a left fork");
	pthread_mutex_unlock(data->writing);
	smart_sleep(philo, data->time_to_die + 10);
	pthread_mutex_unlock(philo->left_fork);
}
