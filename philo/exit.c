/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:45:45 by marvin            #+#    #+#             */
/*   Updated: 2025/06/16 00:37:05 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_data(t_data *data)
{
	free(data->philos);
	free(data->forks);
}

int	exit_sim(t_data *data, char *msg)
{
	join_threads(data);
	destroy_mutexes(data);
	free_data(data);
	if (msg)
	{
		ft_putendl(msg);
		return (1);
	}
	return (0);
}

void	ft_putendl(char *msg)
{
	int	i;

	i = 0;
	while (msg[i])
		write(2, &msg[i++], 1);
	write(2, "\n", 1);
}
