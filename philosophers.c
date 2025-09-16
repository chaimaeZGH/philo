/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/16 20:33:29 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL);
}
int check_death(t_philo *philo)
{
    unsigned long now = get_time_ms();
    if ((int)(now - philo->last_meal_time) > philo->data->time_to_die)
    {
        printf("Philosopher %d died\n", philo->philo_id);
        philo->data->simulation_running = 0;
        return 1;
    }
    return 0;
}

void smart_sleep(t_philo *philo, int ms)
{
    unsigned long start = get_time_ms();
    while (philo->data->simulation_running && (int)(get_time_ms() - start) < ms)
    {
        if (check_death(philo))
            break;
        usleep(1000);
    }
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;
    int eat_count = 0;

    if (data->number_of_philo == 1)
    {
        printf("Philosopher %d is thinking\n", philo->philo_id);
        pthread_mutex_lock(philo->left_fork);
        printf("Philosopher %d picked up left fork\n", philo->philo_id);
        smart_sleep(philo, data->time_to_die);
        pthread_mutex_unlock(philo->left_fork);
        printf("Philosopher %d died\n", philo->philo_id);
        data->simulation_running = 0;
        // free(philo);
        return NULL;
    }

    if (philo->philo_id % 2 == 1)
        smart_sleep(philo, 1);

    while (data->simulation_running && (data->times_must_eat == -1 || eat_count < data->times_must_eat))
    {
        if (check_death(philo))
            break;
        printf("Philosopher %d is thinking\n", philo->philo_id);

        pthread_mutex_lock(philo->left_fork);
        if (check_death(philo))
        {
            pthread_mutex_unlock(philo->left_fork);
            break;
        }
        printf("Philosopher %d picked up left fork\n", philo->philo_id);

        pthread_mutex_lock(philo->right_fork);
        if (check_death(philo))
        {
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            break;
        }
        printf("Philosopher %d picked up right fork\n", philo->philo_id);

        // Eat
        printf("Philosopher %d is eating\n", philo->philo_id);
        philo->last_meal_time = get_time_ms();
        eat_count++;
        smart_sleep(philo, data->time_to_eat);

        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);

        if (check_death(philo))
            break;
        printf("Philosopher %d is sleeping\n", philo->philo_id);
        smart_sleep(philo, data->time_to_sleep);
    }
    return NULL;
}

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
		if (result > 9223372036854775807 && sign == -1)
			return (-1);
		if (result >= 9223372036854775807 && sign == 1)
			return (-1);
		i++;
	}
    if(str[i]!='\0')
        return -1;
	return (result * sign);
}

void ll()
{
	system("leaks philosophers");
}

int main(int ac, char **av)
{
    t_data  my_data;
    int i;
    
    i = 0;
    if (ac < 5 || ac > 6)
    {
        printf("invalid number of argssss");
        return (1);
    }
    if (ft_atoi(av[2]) < 60 || ft_atoi(av[3]) < 60
		|| ft_atoi(av[4]) < 60 || (ft_atoi(av[1]) > 200 || ft_atoi(av[1]) < 1 ))
        return (printf("invalid number of argssss"),1);
    if (ac == 6 && ft_atoi(av[5]) <= 0)
		return (printf("B ERROR INPUT NC"),1);
    my_data.number_of_philo = ft_atoi(av[1]);
    my_data.time_to_die = ft_atoi(av[2]);
    my_data.time_to_eat = ft_atoi(av[3]);
    my_data.time_to_sleep = ft_atoi(av[4]);
	my_data.simulation_running = 1;
    if (ac == 6)
        my_data.times_must_eat = ft_atoi(av[5]);
    else
        my_data.times_must_eat = -1;

    pthread_mutex_t forks[my_data.number_of_philo];
    while (i < my_data.number_of_philo)
        pthread_mutex_init(&forks[i++], NULL);
    t_philo *philos;
    philos = malloc(sizeof(t_philo) * my_data.number_of_philo);
	if (!philos)
		return (0);
	i = 0;
	while (i < my_data.number_of_philo)
	{
		philos[i].philo_id = i+1;
		philos[i].data = &my_data;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % my_data.number_of_philo];
		philos[i].last_meal_time = get_time_ms();
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	i = 0;

	while (i < my_data.number_of_philo)
		pthread_join(philos[i++].thread, NULL);
	i=0;
	free(philos);
	atexit(ll);
}
