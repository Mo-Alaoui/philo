#include "philo.h"

void init_philos(t_engine *engine, t_philo *philos, t_mutex *forks, char **argv)
{
	int i = -1;

	while (++i < ft_atoi(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].times.die = ft_atoi(argv[2]);
		philos[i].times.eat = ft_atoi(argv[3]);
		philos[i].times.sleep = ft_atoi(argv[4]);
		philos[i].times.last_meal = get_current_time();
		philos[i].times.born_time = get_current_time();
		philos[i].must_eat = -1;
		if (argv[5])
			philos[i].must_eat = ft_atoi(argv[5]);
		philos[i].meals_eaten = 0;
		philos[i].philo_count = ft_atoi(argv[1]);
		philos[i].mutexes.left_fork = &forks[i];
		if (i == 0)
			philos[i].mutexes.right_fork = &forks[philos[i].philo_count - 1];
		else
			philos[i].mutexes.right_fork = &forks[i - 1];
		philos[i].mutexes.write_lock = &engine->write_lock;
		philos[i].mutexes.meal_lock = &engine->meal_lock;
	}
}

void init_forks(t_engine *engine, t_mutex *forks, int count)
{
	int i = -1;
	while (++i < count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			destroy_all(engine, "Mutex error\n", i, 1);
	}
}

void init_engine(t_engine *engine, t_philo *philos, t_mutex *forks)
{
	engine->forks = forks;
	engine->philos = philos;
	if (pthread_mutex_init(&engine->write_lock, NULL) != 0
		|| pthread_mutex_init(&engine->meal_lock, NULL) != 0)
		destroy_all(engine, "Mutex error\n", -1, 1);
}

void check_args(int argc, char **argv)
{
	long num;
	int i;
	
	i = 0;
	if (argc < 5 || argc > 6)
		error_exit("Arg count error\n", 1);
	while (++i < argc)
	{
		num = ft_atoi(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
			error_exit("Invalid arg\n", 1);
		else if (i == 5 && (num < 0 || num > INT_MAX))
			error_exit("Invalid arg\n", 1);
		else if (i != 1 && i != 5 && (num < 60 || num > INT_MAX))
			error_exit("Invalid arg\n", 1);
	}
}

int main(int argc, char **argv)
{
	t_philo philos[PHILO_MAX_COUNT];
	t_mutex forks[PHILO_MAX_COUNT];
	t_engine engine;

	check_args(argc, argv);
	init_engine(&engine, philos, forks);
	init_forks(&engine, forks, ft_atoi(argv[1]));
	init_philos(&engine, philos, forks, argv);
	start_simulation(&engine, philos[0].philo_count);
	destroy_all(&engine, NULL, philos[0].philo_count, 0);
	return (0);
}
