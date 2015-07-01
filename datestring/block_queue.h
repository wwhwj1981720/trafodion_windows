#pragma once
//#ifndef BLOCK_QUEUE_H  
//#define BLOCK_QUEUE_H  


 
template<class T> class block_queue
{
	pthread_mutex_t *m_mutex;  
	pthread_cond_t *m_cond;  
	T *m_array;  
	int m_size;  
	int m_max_size;  
	int m_front;  
	int m_back;  
public:
	block_queue(T);
	//block_queue(int);
	void clear() ;
	bool full()const;
	bool front(T& value)const ;
	bool empty()const;
	bool back(T& value)const;
	bool push(const T& item) ;
	int size()const ;
	int max_size()const ;
	bool pop(T& item) ;
	bool pop(T& item, int ms_timeout)  ;
	~block_queue(void);
};

