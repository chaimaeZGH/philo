/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/21 21:35:29 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <time.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>

typedef struct s_data
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_philo;
	int				times_must_eat;
	pthread_mutex_t	*simulation_mutex;
	pthread_mutex_t	*writing;
	int				simulation_running;
	unsigned long	start_time;

}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	unsigned long	last_meal_time;
	int				meals_eaten;
}					t_philo;

int				check_death(t_philo	*philo);
int				check_meals(t_philo	*philo);
void			*monitor_routine(void *arg);
void			second_pat(t_philo	*philo);
void			*philo_routine(void *arg);
unsigned long	time_ms(void);
void			smart_sleep(t_philo *philo, int ms);
void			to_write(t_philo *philo, char *str);
void			one_philo(t_philo *philo);
void			check_todie(t_philo	philo);
int				is_simulation_running(t_data *data);
int				ft_atoi(const char *str);
void			ft_putlong(unsigned long tim);
void			ft_putstr(char *str);
void			write_state(int id, unsigned long tim, char *str);
t_data			make_data(int ac, char **av);
t_philo			*creat_philo(t_data *my_data, pthread_mutex_t *forks);
t_philo			*second_part(t_data *my_data);
int				chek_vars(int ac, char **av);
#endif