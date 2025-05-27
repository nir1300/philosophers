/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/05/27 21:58:21 by marvin           ###   ########.fr       */
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
	int nb_philos = data->input.nb_philo;
	i = 0;
	while (i < nb_philos)
	{
		error = pthread_join(data->philos[i].thread_id, NULL);
		if (error != 0)
			printf("Thread of philosopher number %i could not be joined", i);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

void	destroy_mutexes(t_data *data)
{
	int	i;
	int	error;

	t_philos *philo = data->philos;
	i = 0;
	usleep(1000);
	while (i < data->input.nb_philo)
	{
		// if (philo[i].state == 1 || philo[i].state == 2)
		// 	put_down_fork(&philo[i], data);
        error = pthread_mutex_destroy(&data->forks[i]);
        if (error != 0)
            printf("Fork number %i could not be destroyed: error %d\n", i, error);
		i++;
	}
	(void)philo;
	pthread_mutex_destroy(&data->alive);
	if (error != 0)
		printf("Alive mutex could not be destroyed\n");
	pthread_mutex_destroy(&data->access);
	if (error != 0)
		printf("Access mutex could not be destroyed\n");
	error = pthread_mutex_destroy(&data->output);
	if (error != 0)
		printf("Output mutex could not be destroyed\n");
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
	if (ft_atoi(argv[1]) == 1)
		(printf("0 1 died"), exit(1));
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
	data->is_alive = 1;
	data->stop = 0;
	data->philos = ft_calloc(data->input.nb_philo, sizeof(t_philos));
	data->forks = malloc(data->input.nb_philo * sizeof(pthread_mutex_t));
	if (!data->philos || !data->forks)
		error_exit("Allocation failed", 1);
	init_philos(argv, data);
	pthread_mutex_init(&data->output, NULL);
	pthread_mutex_init(&data->access, NULL);
	pthread_mutex_init(&data->alive, NULL);
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
		data->philos[i].time_since_eaten = get_starttime();
		i++;
	}
}


void take_fork(t_philos *philo, t_data *data) 
{
	if (!all_alive(data))
		return ;
	pthread_mutex_lock(&data->access);
	philo->state = 1;
	pthread_mutex_unlock(&data->access);
	usleep(1000 * (philo->number % 2));
	if (philo->number % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->i]);
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
	}
	else
	{
		usleep(1000 * (data->input.time_to_eat / 2));
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
		pthread_mutex_lock(&data->forks[philo->i]);
	}
	if (!all_alive(data))
		return (put_down_fork(philo, data));
	
	print_states(philo, data, 1);
	print_states(philo, data, 1);
}

void eat(t_philos *philo, t_data *data) 
{
	if (!all_alive(data))
		return ;
	pthread_mutex_lock(&data->access);
	philo->meals_eaten++;
	philo->state = 2;
	philo->time_since_eaten = get_starttime();
	pthread_mutex_unlock(&data->access);
	
	print_states(philo, data, 2);
	usleep(philo->input.time_to_eat * 1000);
	pthread_mutex_lock(&data->access);
	philo->time_since_eaten = get_starttime();
	pthread_mutex_unlock(&data->access);
}

int	unlock(t_philos *philo, t_data *data)
{
	int state;
	pthread_mutex_lock(&data->access);
	state = philo->state;
	if (state == 1 || state == 2)
		philo->state = 0;
	pthread_mutex_unlock(&data->access);
	return (state);
}
	
void put_down_fork(t_philos *philo, t_data *data)
{
	if (unlock(philo, data))
	{
		if (philo->number % 2 == 0)
		{
			pthread_mutex_unlock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
			pthread_mutex_unlock(&data->forks[philo->i]);
		}
		else if (philo->number % 2 != 0)
		{
			pthread_mutex_unlock(&data->forks[philo->i]);
			pthread_mutex_unlock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
		}
	}
}

int	all_alive(t_data *data)
{
	pthread_mutex_lock(&data->alive);
	int alive = data->is_alive;
	pthread_mutex_unlock(&data->alive);
	if (!alive)
		return (0);
	return (1);
}

void sleepy(t_philos *philo, t_data *data)
{
	int nb_philo;

	nb_philo = data->input.nb_philo;
	if (!all_alive(data))
		return ;
	print_states(philo, data, 3);
	usleep(philo->input.time_to_sleep * 1000);
	if (!all_alive(data))
		return ;
	if (nb_philo % 2 != 0)
	{
		print_states(philo, data, 4);
		// usleep(data->input.time_to_die - data->input.time_to_sleep);
	}
}

void	ft_usleep(long duration)
{
	long start = get_starttime();
	while (get_starttime() - start < duration)
		usleep(100);
}

void	*philo_routine(void *var)
{
	t_philos *philo = (t_philos *)var;
	t_data	*data = philo->data;
	// if (philo->number % 2 == 0)
	// 	usleep(150);
	if (data->input.nb_philo % 2 != 0)
		usleep(100);
	while (1)
	{
		if (!all_alive(data))
			return (NULL);
		if (data->input.nb_philo % 2 != 0 && philo->number % 2 != 0)
    		usleep(800);
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
	int nb_philos;
	nb_philos = data->input.nb_philo;
	while (i < nb_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}

	i = 0;
	while (i < data->input.nb_philo)
	{
		error = pthread_create(&data->philos[i].thread_id, NULL, &philo_routine, (void *)&data->philos[i]);
		if (error != 0)
			error_exit("Thread could not be created", error);
		// usleep(100);
		i++;
	}
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
	int i = 0;
	int nb_meals = data->input.nb_meals;
	int nb_philos = data->input.nb_philo;
	int meals_eaten;
	int death_time;
	long last_meal_time;
	int total_meals = 0;
	while (1)
	{
		pthread_mutex_lock(&data->alive);
		if (!data->is_alive)
		{
			pthread_mutex_unlock(&data->alive);
			return (NULL);
		}
		pthread_mutex_unlock(&data->alive);
		i = 0;
		while (i < nb_philos)
		{
			pthread_mutex_lock(&data->access);
			last_meal_time = get_starttime() - philo[i].time_since_eaten;
			death_time = philo[i].input.time_to_die;
			pthread_mutex_unlock(&data->access);
			if (last_meal_time > death_time)
			{
				pthread_mutex_lock(&data->alive);
				data->is_alive = 0;
				pthread_mutex_unlock(&data->alive);
				print_states(&philo[i], data, 5);
				return (NULL); //exit_sim(data);
			}	
			i++;
		}
		i = 0;
		total_meals = 0;
		while (nb_meals != -1 && i < nb_philos)
		{
			pthread_mutex_lock(&data->access);
			meals_eaten = philo[i].meals_eaten;
			pthread_mutex_unlock(&data->access);
			if (meals_eaten >= nb_meals)
				total_meals++;
			if (total_meals == nb_philos)
			{
				pthread_mutex_lock(&data->alive);
				data->is_alive = 0;
				pthread_mutex_unlock(&data->alive);
				return (NULL); //exit_sim(data);
			}
			i++;
		}
		usleep(10000);
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
