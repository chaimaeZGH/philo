/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/21 21:32:02 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL));
}

void	smart_sleep(t_philo *philo, int ms)
{
	unsigned long	start;
	t_data			*data;

	data = philo->data;
	start = time_ms();
	while (is_simulation_running(data) && (int)(time_ms() - start) < ms)
		usleep(500);
}

void	to_write(t_philo *philo, char *str)
{
	t_data			*data;

	data = philo->data;
	pthread_mutex_lock(data->writing);
	if (is_simulation_running(data))
		write_state(philo->philo_id, (time_ms() - data->start_time), str);
	pthread_mutex_unlock(data->writing);
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
	smart_sleep(philo, data->time_to_die + 3);
	pthread_mutex_unlock(philo->left_fork);
}

void	check_todie(t_philo	philo)
{
	t_data	*data;

	data = philo.data;
	pthread_mutex_unlock(data->simulation_mutex);
	pthread_mutex_lock(data->writing);
	write_state(philo.philo_id, (time_ms() - data->start_time), "died");
	pthread_mutex_unlock(data->writing);
}
