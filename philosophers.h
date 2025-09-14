/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/14 16:31:53 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include<stdio.h>
#include <stdbool.h>

typedef struct s_philo
{
    pthread_t thread;
    int philo_id;
    int time_to_die ; 
    int time_to_eat; 
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
}					t_philo;

#endif