#include "StdAfx.h"
#include "block_queue.h"

block_queue<T>::block_queue(T)
{
}
template<class T> block_queue<int>::block_queue(int max_size = 1000)  
{  
	if(max_size <= 0)  
	{  
		exit(-1);  
	}  

	m_max_size = max_size;  
	m_array = new T[max_size];  
	m_size = 0;  
	m_front = -1;  
	m_back = -1;  

	m_mutex = new pthread_mutex_t;  
	m_cond = new pthread_cond_t;  
	pthread_mutex_init(m_mutex, NULL);  
	pthread_cond_init(m_cond, NULL);  
}  
void block_queue:: clear()  
{  
	pthread_mutex_lock(m_mutex);  
	m_size = 0;  
	m_front = -1;  
	m_back = -1;  
	pthread_mutex_unlock(m_mutex);  
}  
bool block_queue::full()const  
{  
	pthread_mutex_lock(m_mutex);  
	if(m_size >= m_max_size)  
	{  
		pthread_mutex_unlock(m_mutex);  
		return true;  
	}  
	pthread_mutex_unlock(m_mutex);  
	return false;  
}  

bool block_queue::empty()const  
{  
	pthread_mutex_lock(m_mutex);  
	if(0 == m_size)  
	{  
		pthread_mutex_unlock(m_mutex);  
		return true;  
	}  
	pthread_mutex_unlock(m_mutex);  
	return false;  
}  

bool block_queue::front(T& value)const  
{  
	pthread_mutex_lock(m_mutex);  
	if(0 == m_size)  
	{  
		pthread_mutex_unlock(m_mutex);  
		return false;  
	}  
	value = m_array[m_front];  
	pthread_mutex_unlock(m_mutex);  
	return true;  
}  

bool block_queue::back(T& value)const  
{  
	pthread_mutex_lock(m_mutex);  
	if(0 == m_size)  
	{  
		pthread_mutex_unlock(m_mutex);  
		return false;  
	}  
	value = m_array[m_back];  
	pthread_mutex_unlock(m_mutex);  
	return true;  
}  

int block_queue::size()const  
{  
	int tmp = 0;  
	pthread_mutex_lock(m_mutex);  
	tmp = m_size;  
	pthread_mutex_unlock(m_mutex);  
	return tmp;  
}  

int block_queue::max_size()const  
{  
	int tmp = 0;  
	pthread_mutex_lock(m_mutex);  
	tmp = m_max_size;  
	pthread_mutex_unlock(m_mutex);  
	return tmp;  
}  

bool block_queue::push(const T& item)  
{  
	pthread_mutex_lock(m_mutex);  
	if(m_size >= m_max_size)  
	{  
		pthread_cond_broadcast(m_cond);  
		pthread_mutex_unlock(m_mutex);  
		return false;  
	}  

	m_back = (m_back + 1) % m_max_size;  
	m_array[m_back] = item;  

	m_size++;  
	pthread_cond_broadcast(m_cond);  
	pthread_mutex_unlock(m_mutex);  

	return true;  
}  
template<class T>
bool block_queue<T>::pop(T& item)  
{  
	pthread_mutex_lock(m_mutex);  
	while(m_size <= 0)  
	{  
		if(0 != pthread_cond_wait(m_cond, m_mutex))  
		{  
			pthread_mutex_unlock(m_mutex);  
			return false;  
		}  
	}  

	m_front = (m_front + 1) % m_max_size;  
	item = m_array[m_front];  
	m_size--;  
	pthread_mutex_unlock(m_mutex);  
	return true;  
}  
template<class T>
bool block_queue::pop(T& item, int ms_timeout)  
{  
	struct timespec t = {0,0};  
	struct timeval now = {0,0};  
	gettimeofday(&now, NULL);  
	pthread_mutex_lock(m_mutex);  
	if(m_size <= 0)  
	{  
		t.tv_sec = now.tv_sec + ms_timeout/1000;  
		t.tv_nsec = (ms_timeout % 1000)*1000;  
		if(0 != pthread_cond_timedwait(m_cond, m_mutex, &t))  
		{  
			pthread_mutex_unlock(m_mutex);  
			return false;  
		}  
	}  

	m_front = (m_front + 1) % m_max_size;  
	item = m_array[m_front];  
	m_size--;  
	pthread_mutex_unlock(m_mutex);  
	return true;  
}  
block_queue::~block_queue(void)
{
	pthread_mutex_lock(m_mutex);  
	if(m_array != NULL)  
		delete  m_array;  
	pthread_mutex_unlock(m_mutex);  

	pthread_mutex_destroy(m_mutex);  
	pthread_cond_destroy(m_cond);  

	delete m_mutex;  
	delete m_cond;  
}
