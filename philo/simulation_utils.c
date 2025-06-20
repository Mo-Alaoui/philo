#include "philo.h"

void destroy_all(t_engine *engine, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&engine->forks[count]);
	pthread_mutex_destroy(&engine->write_lock);
	pthread_mutex_destroy(&engine->meal_lock);
}

void print_status(t_philo *philo, char *status)
{
	size_t time;

	pthread_mutex_lock(philo->mutexes.write_lock);
	time = get_current_time() - philo->times.born_time;
	printf("%ld %d %s\n", time, philo->id, status);
	pthread_mutex_unlock(philo->mutexes.write_lock);
}