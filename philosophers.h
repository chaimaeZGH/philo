/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/16 16:25:03 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct s_data
{
	int time_to_die;
	int time_to_eat;
    int time_to_sleep;
    int number_of_philo;
    int times_must_eat;
    int simulation_running;

}				t_data;


typedef struct s_philo
{
    pthread_t thread;
    int philo_id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data *data;
    unsigned long last_meal_time;
} t_philo;  

#endif