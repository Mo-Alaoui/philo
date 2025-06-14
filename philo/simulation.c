#include "philo.h"

int all_philos_eat(t_philo *philos)
{
	int finished = 0;
	int i = -1;

	if (philos[0].must_eat == -1)
		return (0);
	while (++i < philos[0].philo_count)
	{
		pthread_mutex_lock(philos->mutexes.meal_lock);
		if (philos[i].meals_eaten >= philos[i].must_eat)
			finished++;
		pthread_mutex_unlock(philos->mutexes.meal_lock);
	}
	if (finished == philos[0].philo_count)
	{
		pthread_mutex_lock(philos->mutexes.write_lock);
		return (1);
	}
	return (0);
}

void philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->mutexes.left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->mutexes.right_fork);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(philo->mutexes.meal_lock);
	print_status(philo, "is eating");
	philo->times.last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->mutexes.meal_lock);

	ft_usleep(philo->times.eat);

	pthread_mutex_unlock(philo->mutexes.left_fork);
	pthread_mutex_unlock(philo->mutexes.right_fork);

	print_status(philo, "is sleeping");
	ft_usleep(philo->times.sleep);

	print_status(philo, "is thinking");
}

void *start_routine(void *ptr)
{
	t_philo *philo = (t_philo *)ptr;

	if (philo->id % 2 == 0)
		ft_usleep(1);

	pthread_mutex_lock(philo->mutexes.meal_lock);
	philo->times.born_time = get_current_time();
	philo->times.last_meal = get_current_time();
	pthread_mutex_unlock(philo->mutexes.meal_lock);

	while (1)
		philo_routine(philo);
	return (NULL);
}

void *observer(void *ptr)
{
	t_philo *philos = (t_philo *)ptr;
	int i;

	while (1)
	{
		i = -1;
		while (++i < philos[0].philo_count)
		{
			pthread_mutex_lock(philos->mutexes.meal_lock);
			if (get_current_time() - philos[i].times.last_meal > philos[i].times.die)
			{
				pthread_mutex_unlock(philos->mutexes.meal_lock);
				print_status(&philos[i], "died");
				pthread_mutex_lock(philos->mutexes.write_lock);
				return (NULL);
			}
			pthread_mutex_unlock(philos->mutexes.meal_lock);
		}
		if (all_philos_eat(philos))
			return (NULL);
	}
	return (NULL);
}

void start_simulation(t_engine *engine, int count)
{
	pthread_t observer_id;
	int i = -1;

	if (pthread_create(&observer_id, NULL, &observer, engine->philos) != 0)
		destroy_all(engine, "Thread error\n", count, 1);

	while (++i < count)
	{
		if (pthread_create(&engine->philos[i].thread_id, NULL, start_routine, &engine->philos[i]) != 0)
			destroy_all(engine, "Thread error\n", count, 1);
	}

	if (pthread_join(observer_id, NULL) != 0)
		destroy_all(engine, "Join error\n", count, 1);

	i = -1;
	while (++i < count)
	{
		if (pthread_detach(engine->philos[i].thread_id) != 0)
			destroy_all(engine, "Detach error\n", count, 1);
	}
}