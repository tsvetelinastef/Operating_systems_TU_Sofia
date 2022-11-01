/* Да се създаде програма, която стартира 3 нишки.
Всяка нишка трябва да изведе даден текст на екрана 
10 пъти.Нишките изпълняват една и съща функция. */

#include <pthread.h>
#include <stdio.h>
void* print(void* s) {
	printf(“ % s”, s);
}
Int main() {
	pthread_t th1, th2, th3;
	int ret1, ret2, ret3;
	char s1[] = “\n zdrasti 1 \n”;
	char s2[] = “\n zdrasti 2 \n”;
	char s3[] = “\n zdrasti 3 \n”;
	ret1 = pthread_create(&th1, NULL, print, s1);
	ret2 = pthread_create(&th2, NULL, print, s2);
	ret3 = pthread_create(&th3, NULL, print, s3);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	return 0;
}

//  Пример задача на C++ за работа с МUTEX
#include <iostream> // std::cout
#include <thread>  // std::thread
#include <mutex> // std::mutex
std::mutex mtx;
void print_block(int n, char c) {
	mtx.lock();
	std::cout << "text here " << c;
	for (int i = 0; i < n; ++i) {
		std::cout << c;
	}
	std::cout << '\n';
	mtx.unlock();
}

int main()
{
	std::thread th1(print_block, 50, '*');
	std::thread th2(print_block, 50, '$');

	th1.join();
	th2.join();

	return 0;
}


/* Да се напише програма, в която три нишки обслужват басейн.
Първата нишка на всеки 30 милисек. влива 10 л, втората на всеки
70 милисек. излива 50 литра, третата излива 300 литра всяка
секунда. Капацитетът на басейна е 5000 литра. Когато басейнът е
празен или пълен съотвенитните нишки заспиват за 10 секунди.
Пълнененето и изпразването става в критична секция. */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;
mutex mutexPool;
int currentPoolCapacity = 0;
int maxPoolCapacity = 5000;
void threadFunction(int updateVolume, int sleepInMilliseconds) {
	bool sleepFlag = false;
	while (true) {
		mutexPool.lock();
		if ((currentPoolCapacity + updateVolume > maxPoolCapacity)
			|| (currentPoolCapacity + updateVolume < 0))
			sleepFlag = true;
		else
			currentPoolCapacity += updateVolume;
		mutexPool.unlock();
		if (sleepFlag) {
			this_thread::sleep_for(chrono::seconds(10));
			sleepFlag = true;
		}
		else
		{
			this_thread::sleep_for(chrono::milliseconds(sleepInMilliseconds));

		}
	}
}
int main() {
	int timeRunning = 0;
	thread* t1, * t2, * t3;
	t1 = new thread(threadFunction, 0, 30);
	t2 = new thread(threadFunction, -50, 70);
	t3 = new thread(threadFunction, -300, 1000);
	while (true)
	{
		mutexPool.lock();
		printf("Time: %d -> Pool Capacity: %d\n", timeRunning, currentPoolCapacity);
		mutexPool.unlock();
		timeRunning++;
		this_thread::sleep_for(chrono::seconds(1));
	}
	t1->join(); t2->join(); t3->join();

}


