/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:09 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/17 17:17:43 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_input
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_meals;
}					t_input;

typedef struct s_philos
{
	int				number;
	pthread_t		thread_id;
	int 			state; // 0: not yet started, 1: taken a fork, 2: eating, 3: sleeping, 4: thinking, 5: dead
	int				meals_eaten;
	int				locked;
	pthread_mutex_t	left_fork;
}					t_philos;

typedef struct s_data
{
	pthread_mutex_t lock; // which general mutexes do i need?
	int 			alive; // 1: all alive, 0: somebody died
	pthread_mutex_t	alive_mutex;
	t_philos		*philos;
	t_input			input;
}					t_data;

// libft_helpers
int					ft_atoi(const char *nptr);
void				error_exit(char *error_msg, int errnum);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);

// main

#endif