/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/19 23:13:02 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		philos[i].meals_eaten = 0;
		pthread_mutex_init(&philos[i].meal_locker, NULL);
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
