/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:09 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/06/16 02:02:57 by nrumpfhu         ###   ########.fr       */
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
	t_input			input;
	int				i;
	pthread_t		thread_id;
	int				state;
	int				meals_eaten;
	long			time_since_eaten;
	struct s_data	*data;
}					t_philos;

typedef struct s_data
{
	pthread_t		monitor;
	pthread_mutex_t	output;
	pthread_mutex_t	*forks;
	pthread_mutex_t	access;
	pthread_mutex_t	alive;
	int				is_alive;
	int				mutex_init;
	int				thread_init;
	long			start_time;
	t_philos		*philos;
	t_input			input;
}					t_data;

// libft_helpers
int					ft_atoi(const char *nptr);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
void				ft_putendl(char *str);
int					ft_strcmp(char *s1, char *s2);
int					ft_strlen(char *str);

// init
int					error_handling(int argc, char *argv[]);
int					initialize(int argc, char *argv[], t_data *data);
void				init_philos(char *argv[], t_data *data);

// time
long				get_time(long start_time);
long				get_starttime(void);
void				print_states(t_philos *philos, t_data *data, int state);

// threads
int					create_mutexes(t_data *data);
int					create_threads(t_data *data);
int					join_threads(t_data *data);
int					destroy_mutexes(t_data *data);

// monitoring
void				*monitoring(void *smt);

// routine helpers
void				one_philo(t_data *data, t_philos *philo);
void				stagger_start(t_data *data, t_philos *philo);

// helpers
int					all_alive(t_data *data);
int					unlock(t_philos *philo, t_data *data);
void				forks_even(t_data *data, t_philos *philo, int take);
void				take_odd(t_data *data, t_philos *philo);

// routine
void				take_fork(t_philos *philo, t_data *data);
void				eat(t_philos *philo, t_data *data);
void				put_down_fork(t_philos *philo, t_data *data);
void				sleepy(t_philos *philo, t_data *data);
void				*philo_routine(void *var);
void				ft_usleep(long duration);

// exit
void				free_data(t_data *data);
int					exit_sim(t_data *data, char *msg);

#endif
