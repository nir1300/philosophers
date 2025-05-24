/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:05:39 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/27 21:50:50 by nrumpfhu         ###   ########.fr       */
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

	i = 0;
	while (i < data->input.nb_philo)
	{
		error = pthread_join(data->philos[i].thread_id, NULL);
		if (error != 0)
			printf("Thread of philosopher number %i could not be joined", i);
		i++;
	}

}

void	destroy_mutexes(t_data *data)
{
	int	i;
	int	error;

	i = 0;
	while (i < data->input.nb_philo)
	{
        error = pthread_mutex_destroy(&data->forks[i]);
        if (error != 0)
            printf("Fork number %i could not be destroyed", i);
		i++;
	}
}