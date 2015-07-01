// datestring.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <crtdbg.h>  
#include<queue>

#define Max 100   //缓冲队列的大小
queue<RowBean*>        q;  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/


void *consumer(void* args)  
{  
	ODBCDao dao;
	dao.getConn();
	RowBean *beanobject;
	int size=0;
	while(true)
	{
		pthread_mutex_lock(&mutex);
		size=q.size();
		if(size>0)
		{
			beanobject=q.front();
			q.pop();
			pthread_cond_broadcast(&cond);
			pthread_mutex_unlock(&mutex);
			dao.insertbulkParamInto(beanobject);
			delete beanobject;
		}
		else 
		{
			pthread_cond_broadcast(&cond);
			pthread_cond_wait(&cond,&mutex);/*解锁mutex，并等待cond改变*/
			pthread_mutex_unlock(&mutex);
			
		}
		
	}

	return NULL;  
}  

void *producer(void* args)
{
	while(true)
	{
		pthread_mutex_lock(&mutex);
		int size=q.size();
		if( size>= Max)  
		{  
			pthread_cond_broadcast(&cond); 
			pthread_cond_wait(&cond,&mutex);/*解锁mutex，并等待cond改变*/ 
			pthread_mutex_unlock(&mutex); 
			
			
		}  
		else
		{
			RowBean *beanobject=new RowBean;
			q.push(beanobject);
			pthread_cond_broadcast(&cond);  
			pthread_mutex_unlock(&mutex);
		}
	}
}
static char msg[] = "I received a msg";
int len;
void show_msg(int signo)
{
//      write(STDERR_FILENO, msg, len);
        exit(0);
}
int intervaltime=0;
void * settimer(void *args)
{
        
        while ( 1 )
        {
                Sleep(intervaltime*1000);
                //sigqueue(getpid(), 50, tsval);
				exit(0);
        }
}
int _tmain(int argc, _TCHAR* argv[])
{
	
		if(argc<4)
        {
                printf("%s","please enter argv[1],argv[2],argv[3]");
                return 0;
        }
        printf("%s %s %s %d",argv[1],argv[2],argv[3],Max);
        intervaltime=atoi(argv[1]);
        printf("intervaltime=%d",intervaltime);
        int produernum=2;
        produernum=atoi(argv[2]);
		pthread_t proid[30];  
		for(int i=0;i<produernum;i++)
        {
                pthread_create(&proid[i], NULL, producer, NULL);
        }
        int consumernum=5;
        consumernum=atoi(argv[3]);
		pthread_t conid[30];
        for(int i=0;i<consumernum;i++)
        {
                pthread_create(&conid[i], NULL,consumer , NULL);
        }
		pthread_t sid;
        pthread_create(&sid, NULL,settimer , NULL);
		for(int i=0;i<produernum;i++)
        {
                pthread_join(proid[i],NULL);
        }
		 for(int i=0;i<consumernum;i++)
        {
                pthread_join(conid[i],NULL);
        }
		//for(;;)Sleep(1);
		char* pChars = new char[10]; 
		_CrtDumpMemoryLeaks(); 
		cout<<"thread main"<<endl;

	
}



