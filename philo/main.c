#include "philo.h"

void init_philos(t_engine *engine, t_philo *philos, t_mutex *forks, char **argv)
{
	int i;
	
	i = -1;
	while (++i < ft_atol(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].times.die = ft_atol(argv[2]);
		philos[i].times.eat = ft_atol(argv[3]);
		philos[i].times.sleep = ft_atol(argv[4]);
		philos[i].times.last_meal = get_current_time();
		philos[i].times.born_time = get_current_time();
		philos[i].must_eat = -1;
		if (argv[5])
			philos[i].must_eat = ft_atol(argv[5]);
		philos[i].meals_eaten = 0;
		philos[i].philo_count = ft_atol(argv[1]);
		philos[i].mutexes.left_fork = &forks[i];
		if (i == 0)
			philos[i].mutexes.right_fork = &forks[philos[i].philo_count - 1];
		else
			philos[i].mutexes.right_fork = &forks[i - 1];
		philos[i].mutexes.write_lock = &engine->write_lock;
		philos[i].mutexes.meal_lock = &engine->meal_lock;
	}
}

int init_forks(t_engine *engine, t_mutex *forks, int count)
{
	int i = -1;
	while (++i < count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{		
			printf("Mutex error\n");
			destroy_all(engine, i);
			return (1);
		}
	}
	return (0);
}

int init_engine(t_engine *engine, t_philo *philos, t_mutex *forks)
{
	engine->forks = forks;
	engine->philos = philos;
	if (pthread_mutex_init(&engine->write_lock, NULL) != 0
		|| pthread_mutex_init(&engine->meal_lock, NULL) != 0)
	{
		printf("Mutex error\n");
		destroy_all(engine, -1);
		return (1);
	}
	return (0);
}

int check_args(int argc, char **argv)
{
	long num;
	int	ret, (i);
	
	i = 0, (ret = 0);
	if (argc < 5 || argc > 6)
	{
		printf("Arg count error\n");
		return (1);
	}
	while (++i < argc)
	{
		num = ft_atol(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
			ret = 1;
		else if (i == 5 && (num < 0 || num > INT_MAX))
			ret = 1;
		else if (i != 1 && i != 5 && (num < 60 || num > INT_MAX))
			ret = 1;
		if (ret == 1)
		{
			printf("Invalid arg\n");
			return (1);
		}
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_philo philos[PHILO_MAX_COUNT];
	t_mutex forks[PHILO_MAX_COUNT];
	t_engine engine;

	if (check_args(argc, argv))
		return (1);
	if (init_engine(&engine, philos, forks))
		return (1);
	if (init_forks(&engine, forks, ft_atol(argv[1])))
		return (1);
	init_philos(&engine, philos, forks, argv);
	if (start_simulation(&engine, philos[0].philo_count))
		return (1);
	destroy_all(&engine, philos[0].philo_count);
	return (0);
}
