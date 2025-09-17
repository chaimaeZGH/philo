/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/17 16:42:04 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void ft_putstr(char *str)
{
    int i = 0;
    while (str[i])
        write(1, &str[i++], 1);
    write(1, "\n", 1);
}

void write_state(int id,unsigned long tim,char *str)
{
    ft_putlong(tim);
    write(1," ",1);
    ft_putlong((unsigned long)id);
    write(1," ",1);
    ft_putstr(str);
}

unsigned long get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL);
}

int check_death(t_philo *philo)
{
    unsigned long now = get_time_ms();
    if ((int)(now - philo->last_meal_time) > philo->data->time_to_die && philo->data->simulation_running)
    {
        pthread_mutex_lock(philo->data->writing);
        if (philo->data->simulation_running)
            write_state(philo->philo_id, (get_time_ms() - philo->data->start_time), "died");
        pthread_mutex_unlock(philo->data->writing);
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

void *monitor_routine(void *arg) 
{
    t_philo *philos = (t_philo *)arg;
    t_data *data = philos[0].data;
    
    while (data->simulation_running == 1)
    {
        for (int i = 0; i < data->number_of_philo; i++)
        {
            unsigned long now = get_time_ms();
            if ((int)(now - philos[i].last_meal_time) > data->time_to_die) 
            {
                pthread_mutex_lock(data->writing);
                if (data->simulation_running == 1)
                {
                    write_state(philos[i].philo_id, (get_time_ms() - data->start_time), "died");
                    data->simulation_running = 0;
                }
                pthread_mutex_unlock(data->writing);
                return NULL;
            }
        }
        usleep(1000);
    }
    return NULL;
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;
    int eat_count = 0;

    if (data->number_of_philo == 1)
    {
        pthread_mutex_lock(data->writing);
        write_state(philo->philo_id, (get_time_ms() - data->start_time), "is thinking");
        pthread_mutex_unlock(data->writing);
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(data->writing);
        write_state(philo->philo_id, (get_time_ms() - data->start_time), "picked up left fork");
        pthread_mutex_unlock(data->writing);
        smart_sleep(philo, data->time_to_die + 10);
        pthread_mutex_unlock(philo->left_fork);
        return NULL;
    }
    if (philo->philo_id % 2 == 1)
        smart_sleep(philo, 1);
    while (data->simulation_running == 1 && (data->times_must_eat == -1 || eat_count < data->times_must_eat))
    {
        if (data->simulation_running == 0)
            break;
        pthread_mutex_lock(data->writing);
        if (data->simulation_running == 1)
            write_state(philo->philo_id, (get_time_ms() - data->start_time), "is thinking");
        pthread_mutex_unlock(data->writing);

        if (data->simulation_running == 0)
            break;
        pthread_mutex_lock(philo->left_fork);
        if (data->simulation_running == 0)
        {
            pthread_mutex_unlock(philo->left_fork);
            break;
        }
        pthread_mutex_lock(data->writing);
        if (data->simulation_running == 1)
            write_state(philo->philo_id, (get_time_ms() - data->start_time), "picked up left fork");
        pthread_mutex_unlock(data->writing);
        pthread_mutex_lock(philo->right_fork);
        if (data->simulation_running == 0)
        {
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            break;
        }
        pthread_mutex_lock(data->writing);
        if (data->simulation_running == 1)
            write_state(philo->philo_id, (get_time_ms() - data->start_time), "picked up right fork");
        pthread_mutex_unlock(data->writing);
        pthread_mutex_lock(data->writing);
        if (data->simulation_running == 1)
            write_state(philo->philo_id, (get_time_ms() - data->start_time), "is eating");
        pthread_mutex_unlock(data->writing);
        philo->last_meal_time = get_time_ms();
        eat_count++;
        smart_sleep(philo, data->time_to_eat);
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        if (data->simulation_running == 0)
            break;
        pthread_mutex_lock(data->writing);
        if (data->simulation_running == 1)
            write_state(philo->philo_id, (get_time_ms() - data->start_time), "is sleeping");
        pthread_mutex_unlock(data->writing);

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
    pthread_mutex_t writ;
    my_data.number_of_philo = ft_atoi(av[1]);
    my_data.time_to_die = ft_atoi(av[2]);
    my_data.time_to_eat = ft_atoi(av[3]);
    my_data.time_to_sleep = ft_atoi(av[4]);
	my_data.simulation_running = 1;
    my_data.start_time = get_time_ms();
    pthread_mutex_init(&writ, NULL);
    my_data.writing = &writ;
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
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor_routine, philos);
	i = 0;
	while (i < my_data.number_of_philo)
		pthread_join(philos[i++].thread, NULL);
    pthread_join(monitor_thread, NULL);
	free(philos);
	// atexit(ll);
}
