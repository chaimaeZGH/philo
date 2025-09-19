/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/19 12:35:15 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    ft_usleep(size_t milliseconds)
{
    size_t    start;

    start = time_ms();
    while ((time_ms() - start) < milliseconds)
        usleep(500);
}

int is_simulation_running(t_data *data)
{
    int running;
    pthread_mutex_lock(data->simulation_mutex);
    running = data->simulation_running;
    pthread_mutex_unlock(data->simulation_mutex);
    return running;
}

// void	*monitor_routine(void *arg)
// {
// 	t_philo			*philos;
// 	t_data			*data;
// 	int				i;
// 	unsigned long	now;
	
// 	philos = (t_philo *)arg;
// 	data = philos[0].data;
// 	while (is_simulation_running(data) == 1)
// 	{
// 		if (data->times_must_eat != -1)
// 		{
// 			pthread_mutex_lock(data->meal_check);
// 			if (data->philosophers_finished >= data->number_of_philo)
// 			{
// 				pthread_mutex_lock(data->simulation_mutex);		
// 				data->simulation_running = 0;
// 				pthread_mutex_unlock(data->simulation_mutex);
				
// 				pthread_mutex_unlock(data->meal_check);
// 				return NULL;
// 			}
// 			pthread_mutex_unlock(data->meal_check);
// 		}
// 		i = 0;
// 		while (i < data->number_of_philo)
// 		{
// 			now = time_ms();
// 			pthread_mutex_lock(data->writing);
// 			unsigned long last_meal = philos[i].last_meal_time;//
// 			pthread_mutex_unlock(data->writing);//
// 			if ((int)(now - last_meal) > data->time_to_die)//
// 			{
// 				pthread_mutex_lock(data->writing);
// 				if (is_simulation_running(data) == 1)
// 				{
// 					write_state(philos[i].philo_id, (time_ms() - data->start_time), "died");
// 					pthread_mutex_lock(data->simulation_mutex);	
// 					data->simulation_running = 0;
// 					pthread_mutex_unlock(data->simulation_mutex);
// 				}
// 				pthread_mutex_unlock(data->writing);
// 				return NULL;
// 			}
// 			i++;
// 		}
// 		ft_usleep(500);
// 	}
// 	return NULL;
// }

static void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(data->simulation_mutex);
	data->simulation_running = 0;
	pthread_mutex_unlock(data->simulation_mutex);
}

static int	check_philosophers_finished(t_data *data)
{
	if (data->times_must_eat == -1)
		return (0);
	pthread_mutex_lock(data->meal_check);
	if (data->philosophers_finished >= data->number_of_philo)
	{
		stop_simulation(data);
		pthread_mutex_unlock(data->meal_check);
		return (1);
	}
	pthread_mutex_unlock(data->meal_check);
	return (0);
}

static int	check_philo_death(t_philo *philos, t_data *data, int i)
{
	unsigned long	now;
	unsigned long	last_meal;

	now = time_ms();
	pthread_mutex_lock(data->writing);
	last_meal = philos[i].last_meal_time;
	pthread_mutex_unlock(data->writing);
	if ((int)(now - last_meal) > data->time_to_die)
	{
		pthread_mutex_lock(data->writing);
		if (is_simulation_running(data) == 1)
		{
			write_state(philos[i].philo_id, (time_ms() - data->start_time), "died");
			stop_simulation(data);
		}
		pthread_mutex_unlock(data->writing);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (is_simulation_running(data) == 1)
	{
		if (check_philosophers_finished(data))
			return (NULL);
		i = 0;
		while (i < data->number_of_philo)
		{
			if (check_philo_death(philos, data, i))
				return (NULL);
			i++;
		}
		ft_usleep(500);
	}
	return (NULL);
}

// void    *philo_routine(void *arg)
// {
// 	t_philo	*philo;
// 	t_data	*data;
	
// 	philo = (t_philo *)arg;
// 	data = philo->data;
// 	if (data->number_of_philo == 1)
// 		return(one_philo(philo), NULL);
// 	if (philo->philo_id % 2 == 1)
// 		smart_sleep(philo, 1);
	
// 	while (is_simulation_running(data) == 1 && (data->times_must_eat == -1 
// 			|| philo->meals_eaten < data->times_must_eat))
// 	{
// 		if (is_simulation_running(data) == 0)
// 			break ;
// 		to_write(philo,"is thinking");
// 		if (is_simulation_running(data) == 0)
// 			break ;
// 		pthread_mutex_lock(philo->left_fork);
// 		if (is_simulation_running(data) == 0)
// 		{
// 			pthread_mutex_unlock(philo->left_fork);
// 			break ;
// 		}
// 		to_write(philo,"has taken a left fork");
// 		pthread_mutex_lock(philo->right_fork);
// 		if (is_simulation_running(data) == 0)
// 		{
// 			pthread_mutex_unlock(philo->right_fork);
// 			pthread_mutex_unlock(philo->left_fork);
// 			break ;
// 		}
// 		to_write(philo,"has taken a right fork");
// 		to_write(philo,"is eating");
// 		pthread_mutex_lock(data->writing);
// 		philo->last_meal_time = time_ms();
// 		pthread_mutex_unlock(data->writing);
// 		smart_sleep(philo, data->time_to_eat);
// 		philo->meals_eaten++;
// 		if (data->times_must_eat != -1 && philo->meals_eaten >= data->times_must_eat)
// 		{
// 			pthread_mutex_lock(data->meal_check);
// 			data->philosophers_finished++;
// 			pthread_mutex_unlock(data->meal_check);
// 		}
// 		pthread_mutex_unlock(philo->right_fork);
// 		pthread_mutex_unlock(philo->left_fork);
// 		if (is_simulation_running(data) == 0)
// 			break ;
// 		to_write(philo,"is sleeping");
// 		smart_sleep(philo, data->time_to_sleep);
// 	}
// 	return NULL;
// }


static void	update_meal_time(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(data->writing);
	philo->last_meal_time = time_ms();
	pthread_mutex_unlock(data->writing);
}

static void	increment_meals_eaten(t_philo *philo, t_data *data)
{
	philo->meals_eaten++;
	if (data->times_must_eat != -1 && philo->meals_eaten >= data->times_must_eat)
	{
		pthread_mutex_lock(data->meal_check);
		data->philosophers_finished++;
		pthread_mutex_unlock(data->meal_check);
	}
}

static int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	to_write(philo, "has taken a left fork");
	pthread_mutex_lock(philo->right_fork);
	to_write(philo, "has taken a right fork");
	return (1);
}

static void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

static int	philo_eat(t_philo *philo, t_data *data)
{
	if (!take_forks(philo))
		return (0);
	to_write(philo, "is eating");
	update_meal_time(philo, data);
	smart_sleep(philo, data->time_to_eat);
	increment_meals_eaten(philo, data);
	put_down_forks(philo);
	return (1);
}

static int	should_continue(t_philo *philo, t_data *data)
{
	return (is_simulation_running(data) == 1 &&
		(data->times_must_eat == -1 || philo->meals_eaten < data->times_must_eat));
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->number_of_philo == 1)
		return (one_philo(philo), NULL);
	if (philo->philo_id % 2 == 1)
		smart_sleep(philo, 1);
	while (should_continue(philo, data))
	{
		to_write(philo, "is thinking");
		if (!philo_eat(philo, data))
			break ;
		to_write(philo, "is sleeping");
		smart_sleep(philo, data->time_to_sleep);
	}
	return (NULL);
}


