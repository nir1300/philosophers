/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:09 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/27 18:23:18 by nrumpfhu         ###   ########.fr       */
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

// struct t_data;

typedef struct s_philos
{
	int				number;
	t_input			input;
	int				i;
	pthread_t		thread_id;
	int				state; // 0: not yet started, 1: taken a fork, 2: eating, 3: sleeping, 4: thinking, 5: dead
	int				meals_eaten;
	long			time_since_eaten;
	pthread_mutex_t	output;
	struct s_data	*data;
}					t_philos;

typedef struct s_data
{
	pthread_mutex_t	output;
	pthread_mutex_t	*forks;
	int				alive; // 1: all alive, 0: somebody died
	int				stop;
	long			start_time;
	pthread_mutex_t	alive_mutex;
	int				threads_finished;
	t_philos		*philos;
	t_input			input;
}					t_data;

// libft_helpers
int					ft_atoi(const char *nptr);
void				error_exit(char *error_msg, int errnum);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);

// init
void				error_exit(char *error_msg, int errnum);
void				error_handling(int argc, char *argv[]);
void				free_data(t_data *data);
void				initialize(int argc, char *argv[], t_data *data);
void				init_philos(char *argv[], t_data *data);

// monitoring
long				get_time(long start_time);
long				get_starttime(void);
void				print_states(t_philos *philos, t_data *data, int state);

// cleanup
void	join_threads(t_data *data);
void	destroy_mutexes(t_data *data);

// main

#endif