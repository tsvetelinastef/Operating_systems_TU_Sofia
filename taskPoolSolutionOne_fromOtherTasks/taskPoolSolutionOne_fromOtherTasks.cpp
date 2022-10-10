#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define true 1
#define false 0
typedef int bool;

typedef struct
{
	int updateVolume;
	int sleepTimeMilliseconds;
}
ThreadData;

pthread_mutex_t mutexPool = PTHREAD_MUTEX_INITIALIZER;
int currentPoolCapacity = 0;
const int maxPoolCapacity = 5000;

ThreadData* new_ThreadData(int updateVol, int sleeMillisec)
{
	ThreadData* result = calloc(1, sizeof(ThreadData));
	result->updateVolume = updateVol;
	result->sleepTimeMilliseconds = sleeMillisec;
	return result;
}

void* threadFunction(void* data)
{
    ThreadData* update = (ThreadData*)data;
    bool sleepFlag = false;
    while (true)
    {
        pthread_mutex_lock(&mutexPool);
        if ((currentPoolCapacity + update->updateVolume > maxPoolCapacity)
            || (currentPoolCapacity + update->updateVolume < 0))
            sleepFlag = true;
        else
            currentPoolCapacity += update->updateVolume;
        pthread_mutex_unlock(&mutexPool);

        if (sleepFlag)
        {
            sleep(10);
            sleepFlag = false;
        }
        else
        {
            if (update->sleepTimeMilliseconds > 1000)
                sleep(update->sleepTimeMilliseconds / 1000);
            else
                usleep(update->sleepTimeMilliseconds * 1000);
        }
    }
}
int main()
{
    pthread_t t1, t2, t3;
    int timeRunning = 0;
    ThreadData* thrData1 = new_ThreadData(10, 30);
    ThreadData* thrData2 = new_ThreadData(-50, 70);
    ThreadData* thrData3 = new_ThreadData(-300, 1000);

    pthread_create(&t1, NULL, threadFunction, thrData1);
    pthread_create(&t2, NULL, threadFunction, thrData2);
    pthread_create(&t3, NULL, threadFunction, thrData3);

    while (true)
    {
        pthread_mutex_lock(&mutexPool);
        printf("Time: %d -> Pool Capacity: 
            % d\n",timeRunning,currentPoolCapacity); 
            pthread_mutex_unlock(&mutexPool);
        timeRunning++;
        sleep(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
