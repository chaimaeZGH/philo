/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/17 15:41:02 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void philo_eat(t_philo *philo)
{
        printf("%d is eating\n", philo->philo_id);
    write_state(philo->philo_id, "is eating\n");
        philo->last_meal_time = get_time_ms();
        eat_count++;
        smart_sleep(philo, philo->data->time_to_eat);

}

void philo_sleep(t_philo *philo)
{
        printf("Philosopher %d is sleeping\n", philo->philo_id);
        smart_sleep(philo, philo->data->time_to_sleep);
}

void philo_think()
{
    printf("Philosopher %d is thinking\n", philo->philo_id);
    
}

void    take_forks(t_philo *philo)
{
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

}

void    release_forks(t_philo *philo)
{
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
}

