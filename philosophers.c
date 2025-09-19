/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/19 12:28:56 by mac              ###   ########.fr       */
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
	unsigned long	start;
	t_data			*data;

	data = philo->data;
	start = time_ms();
	while (is_simulation_running(data) && (int)(time_ms() - start) < ms)
	{
		if (check_death(philo))
			break ;
		usleep(1000);
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

t_data	make_data(int ac, char **av)
{
	t_data	my_data;

	my_data.number_of_philo = ft_atoi(av[1]);
	my_data.time_to_die = ft_atoi(av[2]);
	my_data.time_to_eat = ft_atoi(av[3]);
	my_data.time_to_sleep = ft_atoi(av[4]);
	my_data.simulation_running = 1;
	my_data.philosophers_finished = 0;
	my_data.start_time = time_ms();
	if (ac == 6)
		my_data.times_must_eat = ft_atoi(av[5]);
	else
		my_data.times_must_eat = -1;
	return (my_data);
}

t_philo	*creat_philo(t_data *my_data, pthread_mutex_t *forks)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * my_data->number_of_philo);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < my_data->number_of_philo)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = my_data;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % my_data->number_of_philo];
		philos[i].last_meal_time = time_ms();
		philos[i].meals_eaten = 0;
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	return (philos);
}

t_philo	*second_parth(t_data *my_data)
{
	t_philo			*philos;
	pthread_t		monitor_thread;
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * (my_data->number_of_philo));
	if (!forks)
		return (NULL);
	i = 0;
	while (i < my_data->number_of_philo)
		pthread_mutex_init(&forks[i++], NULL);
	philos = creat_philo(my_data, forks);
	if (!philos)
		return (NULL);
	pthread_create(&monitor_thread, NULL, monitor_routine, philos);
	i = 0;
	while (i < my_data->number_of_philo)
		pthread_join(philos[i++].thread, NULL);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < my_data->number_of_philo)
		pthread_mutex_destroy(&forks[i++]);
	free(forks);
	return (philos);
}

// void ll()
// {
// 	system("leaks philosophers");
// }

int	chek_vars(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (printf("invalid number of argssss"), 1);
	if (ft_atoi(av[2]) < 60 || ft_atoi(av[3]) < 60
		|| ft_atoi(av[4]) < 60 || (ft_atoi(av[1]) > 200 || ft_atoi(av[1]) < 1))
		return (printf("invalid number of argssss"), 1);
	if (ac == 6 && ft_atoi(av[5]) < 0)
		return (printf("invalid number of argssss"), 1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data			my_data;
	pthread_mutex_t	writ;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	simulation_mutex;
	t_philo			*philos;

	if (chek_vars(ac, av) == 1)
		return (1);
	my_data = make_data(ac, av);
	pthread_mutex_init(&writ, NULL);
	pthread_mutex_init(&meal_mutex, NULL);
	pthread_mutex_init(&simulation_mutex, NULL);
	my_data.writing = &writ;
	my_data.meal_check = &meal_mutex;
	my_data.simulation_mutex = &simulation_mutex;
	philos = second_parth(&my_data);
	if (philos != NULL)
		free(philos);
	pthread_mutex_destroy(&writ);
	pthread_mutex_destroy(&meal_mutex);
	return (0);
}
