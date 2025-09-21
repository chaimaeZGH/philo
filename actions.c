/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/21 21:32:05 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo	*philo)
{
	int			i;
	long long	current_time;
	t_data		*data;

	data = philo->data;
	i = 0;
	while (i < data->number_of_philo)
	{
		pthread_mutex_lock(data->simulation_mutex);
		current_time = time_ms();
		if ((int)(current_time - philo[i].last_meal_time) > data->time_to_die)
		{
			if (data->simulation_running == 1)
			{
				data->simulation_running = 0;
				check_todie(philo[i]);
			}
			return (1);
		}
		pthread_mutex_unlock(data->simulation_mutex);
		i++;
	}
	return (0);
}

int	check_meals(t_philo	*philo)
{
	int		i;
	t_data	*data;

	data = philo->data;
	if (data->times_must_eat == -1)
		return (0);
	i = 0;
	while (i < data->number_of_philo)
	{
		pthread_mutex_lock(data->simulation_mutex);
		if (philo[i].meals_eaten < data->number_of_philo)
		{
			pthread_mutex_unlock(data->simulation_mutex);
			return (0);
		}
		pthread_mutex_unlock(data->simulation_mutex);
		i++;
	}
	pthread_mutex_lock(data->simulation_mutex);
	data->simulation_running = 0;
	pthread_mutex_unlock(data->simulation_mutex);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_death(philo) || check_meals(philo))
			break ;
		usleep(500);
	}
	return (NULL);
}

void	second_pat(t_philo	*philo)
{
	t_data	*data;

	data = philo->data;
	to_write(philo, "is thinking");
	pthread_mutex_lock(philo->left_fork);
	to_write(philo, "has taken a left fork");
	pthread_mutex_lock(philo->right_fork);
	to_write(philo, "has taken a right fork");
	to_write(philo, "is eating");
	pthread_mutex_lock(data->simulation_mutex);
	philo->last_meal_time = time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(data->simulation_mutex);
	smart_sleep(philo, data->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	to_write(philo, "is sleeping");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->number_of_philo == 1)
		return (one_philo(philo), NULL);
	if (philo->philo_id % 2 == 0)
		smart_sleep(philo, data->time_to_eat / 2);
	while (1)
	{
		second_pat(philo);
		smart_sleep(philo, data->time_to_sleep);
		if (is_simulation_running(data) == 0)
			break ;
	}
	return (NULL);
}
