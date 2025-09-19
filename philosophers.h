/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/19 23:22:43 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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
	unsigned long	start_time;
	pthread_mutex_t	*writing;
	pthread_mutex_t	*meal_check;
	pthread_mutex_t	*simulation_mutex;
	int				simulation_running;
	int				philosophers_finished;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	pthread_mutex_t meal_locker; // * for food related stuff so you dont delay the others to update the data of each philo
	unsigned long	last_meal_time;
	int				meals_eaten;
}					t_philo;

void			*monitor_routine(void *arg);
void			*philo_routine(void *arg);
unsigned long	time_ms(void);
void			to_write(t_philo *philo, char *str);
void			one_philo(t_philo *philo);
void			smart_sleep(t_philo *philo, int ms);
void			write_state(int id, unsigned long tim, char *str);
int				is_simulation_running(t_data *data);
void			ft_usleep(size_t milliseconds);
int				ft_atoi(const char *str);
void			ft_putlong(unsigned long tim);
void			ft_putstr(char *str);
void			write_state(int id, unsigned long tim, char *str);
int				check_death(t_philo *philo);
void			to_write(t_philo *philo, char *str);
void			one_philo(t_philo *philo);

#endif