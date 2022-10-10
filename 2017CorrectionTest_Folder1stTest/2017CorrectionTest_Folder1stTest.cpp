/*
To write a multi-threaded/ многонишкова
program,which starts 4 threads.
която стартира 4 нишки. 
Each of the threads aims to check
value from their shared memory and should
make a decision depending on this value.
Each thread has a parameter k.
When the value in shared memory does not
divides an integer by k, then the string
increments the shared cell by one
and falls asleep in k seconds. If the value
of the common variable is a multiple of k, it
the same is incremented
with k+1, and the thread sleeps for 1 second,
but if the value of the shared variable
is also divisible by the square of k, the thread
adds 2k+1 and doesn't sleep.



Да се напише многонишкова програма,
която стартира 4 нишки.
Всяка от нишките има за цел да 
проверява стойност от от общата им
памет и трябва да 
вземе решение в зависимост от тази стойност.
Всяка нишка притежава параметър k.
Когато стойносста в общатата памет не се 
дели целочислено на k, то нишката 
инкрементира с единица споделената клетка 
и заспива за k секунди. Ако пък стойността
на общата променлива е кратна на k, то 
същата се инкрементира
с k+1, а нишката заспива за 1 секунда, 
но ако стойността на споделената променлива
се дели и на квадрата на k, то нишката
прибавя 2k+1 и не заспива.
===========================================
Стойностите на k за четирите нишки са 
съответно 3, 5, 7, 17.
Нишките работят до безкрай.
*/


#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex mtx;
int data = 3;

void thread_func(int k)
{
	while (true)
	{
		mtx.lock();
		if (data % k != 0) 
		{
			data++;
			this_thread::sleep_for(chrono::seconds(k));
		}

		else if ((data % k == 0) && (data % (k*k) == 0))
		{
			data += (2 * k + 1);
		}

		else if (data % k == 0) 
		{
			data += (k + 1);
			this_thread::sleep_for(chrono::seconds(1));
		}
		mtx.unlock();
	}
}

int main()
{
	thread t1(thread_func, 3);
	thread t2(thread_func, 5);
	thread t3(thread_func, 7);
	thread t4(thread_func, 17);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}

