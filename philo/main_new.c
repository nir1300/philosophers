/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/03/03 18:02:34 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(char *error_msg, int errnum)
{
    printf("%s\n", error_msg);
    exit(errnum);
}

void	join_threads(t_data *data)
{
	int	i;
	int	error;

	i = data->input.nb_philo - 1;
	while (i >= 0)
	{
		error = pthread_join(data->philos[i].thread_id, NULL);
		if (error != 0)
			printf("Thread of philosopher number %i could not be joined", i);
		i--;
	}
	pthread_join(data->monitor, NULL);
}

void	destroy_mutexes(t_data *data)
{
	int	i;
	int	error;

	i = data->input.nb_philo - 1;
	while (i >= 0)
	{
        error = pthread_mutex_destroy(&data->forks[i]);
        if (error != 0)
            printf("Fork number %i could not be destroyed", i);
		i--;
	}
	error = pthread_mutex_destroy(&data->output);
        if (error != 0)
            printf("Fork number %i could not be destroyed", i);
}


long	get_time(long start_time)
{
	struct timeval	tv;
	int				error;

	error = gettimeofday(&tv, NULL);
	if (error == -1)
		return (-1);
	return (((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start_time);
}

long	get_starttime(void)
{
	struct timeval	tv;
	int				error;

	error = gettimeofday(&tv, NULL);
	if (error == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_states(t_philos *philos, t_data *data, int state)
{
	pthread_mutex_lock(&data->output);
	if (state == 1)
		printf("%lu %i has taken a fork\n", get_time(data->start_time), philos->number);
	if (state == 2)
		printf("%lu %i is eating\n", get_time(data->start_time), philos->number);
	if (state == 3)
		printf("%lu %i is sleeping\n", get_time(data->start_time), philos->number);
	if (state == 4)
		printf("%lu %i is thinking\n", get_time(data->start_time), philos->number);
	if (state == 5)
		printf("%lu %i died\n", get_time(data->start_time), philos->number);
	pthread_mutex_unlock(&data->output);
}

void	error_handling(int argc, char *argv[])
{
	if (argc < 5)
		error_exit("Argument number invalid", 1);
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0
	|| (argc == 6 && ft_atoi(argv[5]) <= 0))
		error_exit("Input variables must be positive", 1);
}

void	initialize(int argc, char *argv[], t_data *data)
{
	data->input.nb_philo = ft_atoi(argv[1]);
	data->input.time_to_die = ft_atoi(argv[2]);
	data->input.time_to_eat = ft_atoi(argv[3]);
	data->input.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->input.nb_meals = ft_atoi(argv[5]);
	else
		data->input.nb_meals = -1;
	data->alive = 1;
	data->stop = 0;
	data->philos = ft_calloc(data->input.nb_philo, sizeof(t_philos));
	data->forks = malloc(data->input.nb_philo * sizeof(pthread_mutex_t));
	if (!data->philos || !data->forks)
		error_exit("Allocation failed", 1);
	init_philos(argv, data);
	pthread_mutex_init(&data->output, NULL);
}

void	init_philos(char *argv[], t_data *data)
{
	int i = 0;
	while (i < data->input.nb_philo)
	{
		data->philos[i].input.nb_philo = ft_atoi(argv[1]);
		data->philos[i].input.time_to_die = ft_atoi(argv[2]);
		data->philos[i].input.time_to_eat = ft_atoi(argv[3]);
		data->philos[i].input.time_to_sleep = ft_atoi(argv[4]);
		data->philos[i].data = data;
		data->philos[i].number = i + 1;
		data->philos[i].state = 0;
		data->philos[i].i = i;
		data->philos[i].meals_eaten = 0;
		i++;
	}
}

void take_fork(t_philos *philo, t_data *data) 
{
	if (philo->number % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->i]);
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
		// pthread_mutex_lock(&data->output);
		// printf("Nb philo: %i\n, left fork: %d\n, right fork: %d\n", philo->number, philo->i, (philo->i + 1) % data->input.nb_philo);
		// pthread_mutex_unlock(&data->output);
	}
	else
	{
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
		pthread_mutex_lock(&data->forks[philo->i]);
	}
	print_states(philo, data, 1);
}

void eat(t_philos *philo, t_data *data) 
{
	print_states(philo, data, 2);
	pthread_mutex_lock(&data->output);
	philo->meals_eaten++;
	// pthread_mutex_unlock(&data->output);
	usleep(philo->input.time_to_eat * 1000);
	// pthread_mutex_lock(&data->output);
	philo->time_since_eaten = get_time(data->start_time);
	pthread_mutex_unlock(&data->output);
}

void put_down_fork(t_philos *philo, t_data *data)
{
	pthread_mutex_unlock(&data->forks[philo->i]);
	pthread_mutex_unlock(&data->forks[(philo->i+1) % data->input.nb_philo]);
}

void sleepy(t_philos *philo, t_data *data)
{
	print_states(philo, data, 3);
	usleep(philo->input.time_to_sleep * 1000);
	if (data->input.nb_philo % 2 != 0)
		print_states(philo, data, 4);
}

void	*philo_routine(void *var)
{
	t_philos *philo = (t_philos *)var;
	t_data	*data = philo->data;
	if (philo->number % 2 == 0)
		usleep(1000);
	while (!data->stop)
	{
		take_fork(philo, data);
		eat(philo, data);
		put_down_fork(philo, data);
		sleepy(philo, data);
	}
	return (NULL);
}


void	create_threads(t_data *data)
{
	int	error;
	int	i;

	i = 0;
	while (i < data->input.nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}

	i = 0;
	data->threads_finished = 0;
	while (i < data->input.nb_philo)
	{
		error = pthread_create(&data->philos[i].thread_id, NULL, &philo_routine, (void *)&data->philos[i]);
		if (error != 0)
			error_exit("Thread could not be created", error);
		i++;
	}
	data->threads_finished = 1;
}

void	free_data(t_data *data)
{
	free(data->philos);
	free(data->forks);
}

void	exit_sim(t_data *data)
{
	join_threads(data);
	destroy_mutexes(data);
	free_data(data);
	exit(0);
}

void	*monitoring(void *smt)
{
	t_data *data = (t_data *)smt;
	t_philos *philo = data->philos;
	int nb = data->input.nb_meals;
	int i = 0;
	long last_meal_time; get_time(data->start_time);

	while (data->alive)
	{
		i = 0;
		while (i < nb)
		{
			last_meal_time = get_time(data->start_time) - philo[i].time_since_eaten;
			if (last_meal_time > philo[i].input.time_to_eat)
			{
				print_states(&philo[i], data, 5);
				data->alive = 0;
				exit_sim(data);
			}
			i++;
		}
		i = 0;
		while (i < nb)
		{
			if (philo[i].meals_eaten >= data->input.nb_meals)
				i++;
			else
				break;
		}
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	data.start_time = get_starttime();
	error_handling(argc, argv);
	initialize(argc, argv, &data);
	create_threads(&data);
	pthread_create(&data.monitor, NULL, &monitoring, (void *)&data);
	exit_sim(&data);
}
