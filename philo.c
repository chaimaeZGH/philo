/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czghoumi <czghoumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:07:22 by czghoumi          #+#    #+#             */
/*   Updated: 2025/09/14 11:16:15 by czghoumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int main(int ac, char **av)
// {
    
//     if (ac < 5 || ac > 6)
//     {
//          printf("invalid number of args");
//          return (1);
//     }
//     int number_of_philosophers = atoi(av[1]); 
//     int time_to_die = atoi(av[2]); 
//     int time_to_eat = atoi(av[3]); 
//     int time_to_sleep = atoi(av[4]);
//     int number_of_times_each_philosopher_must_eat;
//     if (ac == 6)
//         number_of_times_each_philosopher_must_eat = atoi(av[5]);
    
// }



typedef struct s_check
{
	int			i;
	pthread_mutex_t	*lock;
}				t_check;

void *thread_func(void *arg) 
{
    t_check* data = (t_check*)arg;
    pthread_mutex_lock(data->lock);
    printf("Thread %d has locked the mutex\n", data->i);
    pthread_mutex_unlock(data->lock);
    return NULL;
}

int main() 
{
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    t_check data1;
    t_check data2;
    
    data1.i=id1;
    data1.lock=&lock;
    data2.i=id2;
    data2.lock=&lock;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, thread_func, &data1);
    pthread_create(&t2, NULL, thread_func, &data2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
