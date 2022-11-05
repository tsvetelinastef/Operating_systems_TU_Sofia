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

/* Проблема със Вечерящите философи дефинира К на
брой философа, който седят около кръгла маса със
по една чиния и по една вилица за двете си ръце.
Между тях има по една вилица. Философа може да се
храни само, когато хване двете вилици – отляво и от
дясно ! Философ неможе да се храни когато е със
една вилица ! Философите прекарват живота си хранейки
се и мислеки през целия си живот */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define N_PHILOSOPHERS 5
#define LEFT (ph_num + N_PHILOSOPHERS - 1) % N_PHILOSOPHERS
#define RIGHT (ph_num + 1) % N_PHILOSOPHERS
pthread_mutex_t mutex;
pthread_cond_t condition[N_PHILOSOPHERS];
enum { THINKING, HUNGRY, EATING} state[N_PHILOSOPHERS];
int phil_num[N_PHILOSOPHERS];
void* philosophing(void* arg);
void pickup_forks(int ph_num);
void return_forks(int ph_num);
void test(int ph_num);
int main(int argc, char* argv[]) {
	pthread_t ph_thread[N_PHILOSOPHERS];
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < N_PHILOSOPHERS; i++) {
		pthread_cond_init(&condition[i], NULL);
		phil_num[i] = i;
	}
	for (int i = 0; i < N_PHILOSOPHERS; i++) {
		pthread_create(&ph_thread[i], NULL, philosophing, &phil_num[i]);
		printf("Philosopher #%d sits on the table.\n", i + 1);
		sleep(1);
	}
	for (int i = 0; i < N_PHILOSOPHERS; i++) {
		pthread_join(ph_thread[i], NULL);
		pthread_mutex_destroy(&condition[i]);
		return(0);
	}

	void* philosophing(void* arg) {
		while (1) {
			int* ph_num = arg;
			printf("Philosopher #%d starts thinking.\n", *ph_num + 1);
			sleep(2);
			pickup_forks(*ph_num);
			return_forks(*ph_num);
		}
	}
	void pickup_forks(int ph_num) {
		pthread_mutex_lock(&mutex);
		printf("Philosopher #%d is HUNGRY. She tries to grab her forks.\n", ph_num + 1);
		state[ph_num] = HUNGRY;
		test(ph_num);
		while (state[ph_num] != EATING)
			pthread_cond_wait(&condition[ph_num], &mutex);
		pthread_mutex_unlock(&mutex);
	}
	void return_forks(int ph_num) {
		pthread_mutex_lock(&mutex);
		printf("Philosopher #%d puts down chopsticks. Now she asks her neighbors if they are hungry.\n", ph_num + 1);
		state[ph_num] = THINKING;
		test(LEFT);
		test(RIGHT);
		pthread_mutex_unlock(&mutex);
	}
	void test(int ph_num) {
		if (state[ph_num] == HUNGRY && state[LEFT] != EATING
			&& state[RIGHT] != EATING) {
			printf("Philosopher #%d starts EATING.\n", ph_num + 1);
			state[ph_num] = EATING;
			sleep(3);
			pthread_cond_signal(&condition[ph_num]);
		}
	}


	/* Алгоритъм на Банкера е познат още като
	Алгоритъм за Избягване на Мъртва хватка.
	Разработен е от Дейкстра. Той е предназначен
	да проверява безопасното състояние, когато
	се изисква ресурс. Прави се аналогия с банка,
	когато клиента изисква да изтегли пари !
	Базира се на сума в банковата сметка за да се
	отпуснат пари на клиента. Банкерът не може
	да даде повече пари от това, което клиентът е
	поискал, както и ако клиента иска повече
	няма как да даде повече от наличните пари в
	банката ! Тъй като алгоритъма използва
	банкова аналогия – това е така наречения
	алгоритъм на банкера !*/

#include <stdio.h>
	int main() { // P0, P1, P2, P3 are the Process names here
		int n, m, i, j, k;
		n = 5; // number of processes
		m = 3; // number of resources
		int alloc[5][3] = { { 0, 1, 0},
							{ 2, 0, 0},
							{ 3, 0, 2},
							{ 2, 1, 1},
							{ 0, 0, 2} };
		int max[5][3] = { { 7, 5, 3},
						  { 3, 2, 2},
						  { 9, 0, 2},
						  { 2, 2, 2},
						  { 4, 3, 3} };
		int avail[3] = { 3, 3, 2 }; // available resources
		int f[n], ans[n], ind = 0;
		for (k = 0; k < n; k++) {
			f[k] = 0;
		}
		int need[n][m];
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++)
				need[i][j] = max[i][j] - alloc[i][j];
		}
		int y = 0;
		for (k = 0; k < 5; k++) {
			for (i = 0; i < n; i++) {
				if (f[i] == 0) {
					int flag = 0;
					for (j = 0; j < m; j++) {
						if (need[i][j] > avail[j]) {
							flag = 1;
							break;
						}
					}
					if (flag == 0) {
						ans[ind++] = i;
						for (y = 0; y < m; y++)
							avail[y] += alloc[i][y];
						f[i] = 1;
					}
				}
			}
		}
		printf("Following is the SAFE Sequence\n");
		for (i = 0; i < n - 1; i++)
			printf(" P%d ->", ans[i]);
		printf(" P%d", ans[n - 1]);
		return(0);
}

#include <stdio.h>
	int current[5][5], maximum_claim[5][5], available[5];
	int allocation[5] = { 0, 0, 0, 0, 0 };
	int maxres[5], running[5], safe = 0;
	int counter = 0, i, j, exec, resources, processes, k = 1;

	int main() {
		printf("\nEnter number of processes: ");
		scanf("%d", &processes);

		for (i = 0; i < processes; i++) {
			running[i] = 1;
			counter++;
		}
		printf("\nEnter number of resources: ");
		scanf("%d", &resources);

		printf("\nEnter Claim Vector:");
		for (i = 0; i < resources; i++) {
			scanf("%d", &maxres[i]);
		}
		printf("\nEnter Allocated Resource Table:\n");
		for (i = 0; i < processes; i++) {
			for (j = 0; j < resources; j++) {
				scanf("%d", &maximum_claim[i][j]);
			}
		}
		printf("\nThe Claim Vector is: ");
		for (i = 0; i < resources; i++) {
			printf("\t%d", maxres[i]);
		}
		printf("\nThe Allocated Resource Table:\n");
		for (i = 0; i < processes; i++)
		{
			for (j = 0; j < resources; j++) {
				printf("\t%d", current[i][j]);
			}
			printf("\n");
		}
		printf("\nThe Maximum Claim Table:\n");
		for (i = 0; i < processes; i++) {
			for (j = 0; j < resources; j++) {
				printf("\t%d", maximum_claim[i][j]);
			}
			printf("\n");
		}
		for (i = 0; i < processes; i++) {
			for (j = 0; j < resources; j++) {
				allocation[j] += current[i][j];
			}
		}
		printf("\nAllocated resources:");
		for (i = 0; i < resources; i++) {
			printf("\t%d", allocation[i]);
		}
		for (i = 0; i < resources; i++) {
			available[i] = maxres[i] - allocation[i];
		}
		printf("\nAvailable resources:");
		for (i = 0; i < resources; i++) {
			printf("\t%d", available[i]);
		}
		printf("\n");

		while (counter != 0) {
			safe = 0;
			for (i = 0; i < processes; i++)
			{
				if (running[i])
				{
					exec = 1;
					for (j = 0; j < resources; j++)
					{
						if (maximum_claim[i][j] - current[i][j] > available[j])
						{
							exec = 0;
							break;
						}
					}
					if (exec) {
						printf("\nProcess%d is executing\n", i + 1);
						running[i] = 0;
						counter--;
						safe = 1;
						for (j = 0; j < resources; j++)
						{
							available[j] += current[i][j];
						}
						break;
					}
				}
			}
			if (!safe)
			{
				printf("\nThe processes are in unsafe state.\n");
				break;
			}
			else
			{
				printf("\nThe process is in safe state");
				printf("\nAvailable vector:");
				for (i = 0; i < resources; i++) {
					printf("\t%d", available[i]);
				}
				printf("\n");
			}
		}
		return 0;

		// Пример задача на C и C++ за семафори
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
		sem_t mutex;
		void* thread(void* arg) { // wait
			sem_wait(&mutex);
			printf("\nEntered..\n");

			// critical section 
			sleep(4);
			// signal
			printf("\nJust Existing...\n");
			sem_post(&mutex);
		}
		int main() {
			sem_init(&mutex, 0, 1);
			pthread_t t1, t2;
			pthread_create(&t1, NULL, thread, NULL);
			sleep(2);
			pthread_create(&t2, NULL, thread, NULL);
			pthread_join(t1, NULL);
			pthread_join(t2, NULL);
			sem_destroy(&mutex);
			return 0;
		}



		// Пример задача на C и C++ за Semaphore между процеси
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#define EXIT_FAILURE -1 // A number not equal to 0
#define EXIT_SUCCESS 0
		const char* semName = "asdfsd";
		void parent(void) {
			sem_t* sem_id = sem_open(seemName, O_CREAT, 0600, 0);
			if (sem_id == SEM_FAILED) {
				perror("Parent : [sem_open] Failed\n");
				return;
			}
			printf("Parent :Wait for Child to Print\n");
			if (sem_wait(sem_id) < 0)
				printf("Parent : [sem_wait] Failed\n");
			printf("Parent : Child Printed! \n");
			if (sem_close(sem_id) != 0) {
				perror("Parent : [sem_close] Failed\n"); return;
			}
			if (sem_unlink(semName) < 0) {
				printf("Parent : [sem_unlink] Failed\n"); return;
			}
		}
		void child(void) {
			sem_t* sem_open(semName, O_CREAT, 0600, 0);
			if (sem_id == SEM_FAILED) {
				perror("Child : [sem_open] Failed\n"); return;
			}
			printf("Child : I am done! Release Semaphore\n");
			if (sem_post(sem_id) < 0)
				printf("Child : [sem_post] Failed \n");
		}
		int main(int argc, char* argv[]) {
			pid_t pid;
			pid = fork();
			if (pid < 0) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (!pid) {
				int status;
				parent();
				wait(&status);
				printf("Parent : Done with sem_open \n");
			}
			return 0;
		}
		/*
		sem_open() : Connects to, &optinally creates,
		a named semaphore(like sem_init())

		sem_unlink() : Ends connection to an open
		semaphore & causes the semaphore to be
		removed when the last process closes
		it(like sem_destroy())

		sem_wait() : Wait for the semaphore to acquire

		sem_post(): Release semaphore
		*/

		// ==========================================================================

		/* Проблема Producer consumer е познат
		като проблема с ограничения буфер -
		bounded buffer. При този проблем
		имаме два процеса, producer и
		consumer, който споредят общ буфер
		със фиксиран размер. Producer работи
		като осигурява данни или неща , който
		ги поставя в буфер. Consumer работи,
		като трябва да премахне данни от
		буфера и ги консумира. Трябва да се
		уверим, че Producer не произвежда
		данни, когато буферът е пълен, а
		consumer не премахва данни, когато
		буферът е празен.
		Producer трябва да заспи , когато е пълен.
		Следващият път когато consumer премахва данни и
		уведомява producer и producer стартира
		данните отново. Consumer би трябвало да заспи,
		когато буфера е празен. В следващата ситуация
		когато producer добавя данни , той
		уведомява consumer и тои започва да
		консумира данни */

#include <pthread.h> #include <semaphore.h> 
#include <stdlib.h> #include <stdio.h>
#define MaxItems 5 // Max items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer 
		sem_t empty;
		sem_t full;
		int in = 0; int out = 0; int buffer[BufferSize];
		pthread_mutex_t mutex;
		void* producer(void* pno) {
			int item;
			for (int i = 0; i < MaxItems; i++) {
				item = rand(); // produce an random item
				sem_wait(&empty);
				pthread_mutex_lock(&mutex);
				buffer[in] = item;
				printf("Producer %d: Insert Item %d at %d\n", *((int*)pno), buffer[in], in);
				in = (in + 1) % BufferSize;
				pthread_mutex_unlock(&mutex);
				sem_post(&full);
			}
		}
		void* consumer(void* cno) {
			for (int i = 0; i < MaxIteams; i++) {
				sem_wait(&full);
				pthread_mutex_lock(&mutex);
				int item = buffer[out];
				printf("Consumer %d: Remove Item %d from %d\n", *((int*)cno), item, out);
				out = (out + 1) % BufferSize;
				pthread_mutex_unlock(&mutex);
				sem_post(&empty);
			}
		}
		int main() {
			pthread_t pro[5], con[5];
			pthread_mutex_init(&mutex, NULL);
			sem_init(&empty, 0, BufferSize);
			sem_init(&full, 0, 0);
			int a[5] = { 1, 2, 3, 4, 5 }; // just used for numbering the producer and consumer
			for (int i = 0; i < 5; i++) {
				pthread_create(&pro[i], NULL, (void*)producer, (void*)&a[i]);
			}
			for (int i = 0; i < 5; i++) {
				pthread_create(&pro[i], NULL, (void*)producer, (void*)&a[i]);
			}
			for (int i = 0; i < 5; i++) {
				pthread_join(pro[i], NULL);
			}
			for (int i = 0; i < 5; i++) {
				pthread_join(con[i], NULL);
			}
			pthread_mutex_destroy(&mutex);
			sem_destroy(&empty);
			sem_destroy(&full);
			return 0;
		}


#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdlio.h>

		/* This program provides a possible solution for producer-consumer
		problem using mutex and semaphore. I have used 5 producers and
		5 onsumers to demonstrate the solution. You can always play with these */

#define MaxItems 5 // Maximim items a producer or a cosumer can consume
#define BufferSize 5 // Size of the buffer

		sem_t empty;
		sem_t full;
		int in = 0;
		int out = 0;
		int buffer[BufferSize];
		pthread_mutex_t mutex;

		void* producer(void* pno)
		{
			int items;
			for (int i = 0; i < MaxItems; i++) {
				item = rand(); // procedure an random item
				sem_wait(&empty);
				pthread_mutex_lock(&mutex);
				buffer[in] = item;
				printf("Producer %d: Insert Item %d at %d\n", *((int*)pno), buffer[in], in);
				in = (in + 1) % BufferSize;
				pthread_mutex_unlock(&mutex);
				sem_post(&full);
			}
		}
		void* consumer(void* cno)
		{
			for (int i = 0; i < MaxItems; i++) {
				sem_wait(&full);
				pthread_mutex_lock(&mutex);
				int item = buffer[out];
				printf("Consumer %d: Remove Item %d from %d\n", *((int*)cno), item, out);
				out = (out + 1) % BufferSize;
				pthread_mutex_unlock(&mutex);
				sem_post(&empty);
			}
		}
		int main()
		{
			pthread_t pro[5], con[5];
			pthread_mutex_init(&mutex, NULL);
			sem_init(&empty, 0, BufferSize);
			sem_init(&full, 0, 0);

			int a[5] = { 1, 2, 3, 4, 5 }; // Just used for numbering the producer and consumer
			for (int i = 0; i < 5; i++) {
				pthread_create(&pro[i], NULL, (void*)producer, (void*)&a[i]);
			}
			for (int i = 0; i < 5; i++) {
				pthread_create(&con[i], NULL, (void*)consumer, (void*)&a[i]);
			}
			for (int i = 0; i < 5; i++) {
				pthread_join(pro[i], NULL);
			}
			for (int i = 0; i < 5; i++) {
				pthread_join(con[i], NULL);
			}


			pthread_mutex_destroy(&mutex);
			sem_destroy(&empty);
			sem_desroy(&full);

			return 0;


			/* Проблема Readers Writers
		Нека за пример да дадем База от данни БД, която е
		споделена между няколко конкурентни процеса. Някой
		от тези процеси искат да четат базата, но някой от тях
		искат да обновят – Update ( операции като четене и
		запис ! ) в БД. Разграничаваме тези два вида процеси,
		като се отнасяме към първите като читатели, а към
		вторите като писатели. Очевидно е, че ако двама четци
		имат достъп до споделените данни едновременно,
		няма да има неблагоприятни ефекти. Ако обаче писател
		и някакъв друг процес (или четец, или писател) имат
		достъп до базата данни едновременно, може да
		настъпи хаос.
		За да гаратираме синхронизация между процесите
		трябва процесите да имат изключителен достъп до БД.
		Този проблем със синхронизизрането между процесите
		се начича Читатели и Писачи - Readers Writers
		Проблемът читатели-писатели има няколко вариации, като всички
		включват приоритети. Най-простият, посочен като първият проблем
		на читателите и писателите, изисква да не се чака читател, освен ако
		писателят вече е получил разрешение да използва споделения
		обект. С други думи, никой читател не трябва да чака други читатели
		да свършат, просто защото писателят чака.
		*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
			sem_t wrt;
			pthread_mutex_t mutex;
			int cnt = 1;
			int numreader = 0;
			void* writer(void* wno) {
				sem_wait(&wrt);
				cnt = cnt * 2;
				printf("Writer %d modified cnt to %d\n", (*((int*)wno)), cnt);
				sem_post(&wrt);
			}
			void* reader(void* rno) {
				pthread_mutex_lock(&mutex); // reader acquire the lock before modifying numreader
				numreader++;
				if (numreader == 1) {
					sem_wait(&wrt); // if this id the first reader, then it will block the writer
				}
				pthread_mutex_unlock(&mutex); // reading section
				printf("Reader %d: read cnt as %d\n", *((int*)rno), cnt);
				pthread_mutex_lock(&mutex); // reader acquire the lock before modifying numreader
				numreader--;
				if (numreader == 0) {
					sem_post(&wrt); // If this is the last reader, it will wake up the writer
				}
				pthread_mutex_unlock(&mutex);
			}
			int main() {
				pthread_t read[10], write[5];
				pthread_mutex_init(&mutex, NULL);
				sem_init(&wrt, 0, 1);
				int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; // just used for numbering the producer and consumer
				for (int i = 0; i < 10; i++) {
					pthread_create(&read[i], NULL, (void*)reader, (void*)&a[i]);
				}
				for (int i = 0; i < 5; i++) {
					pthread_create(&write[i], NULL, (void*)writer, (void*)&a[i]);
				}
				for (int i = 0; i < 10; i++) {
					pthread_join(read[i], NULL);
				}
				for (int i = 0; i < 5; i++) {
					pthread_join(write[i], NULL);
				}
				pthread_mutex_destroy(&mutex);
				sem_destroy(&wrt);
				return 0;
			}


			/*  Задачи за изпълнение
			1. Да се реализира списъчен елемент, описващ блок памет.
			2. Да се имплементира функция за заделяне на памет по описания алгоритъм за заделяне.
			3. Да се имплементира функция за освобождаване на памет по описания алгоритъм за освобождаване.
			4. Да се имплементира функция за дефрагментация на свободната памет, която да се извиква от функцията за освобождаване на памет.
			5. (Незадължителна) Да се реализира функция за преоразмеряване на зает блок памет.
			Среди за използване
			Под Debian: QTCreator – локално на машините
			Езици за програмиране: C и/или C++
			Литература
			Първоначално заделяне на памет за използване от създадените
			функции за реализиране на списъка от блокове памет.

			void initFreeSpace(int sizeInBytes)
			{
			memListRoot = calloc(1, sizeInBytes);
			//Попълване на структурата на свободната памет
			...
			memListRoot->nextItem = NULL;
			}  */

#include <stdio.h>
#include "memory.h"
			MemoryItem* memListRoot;
			void initFreeSpace(unsigned sizeInBytes) {
				memListRoot = calloc(1, sizeInBytes);
				if (memListRoot == NULL) return;
				memListRoot->size = sizeInBytes - sizeof(MemoryItem);
				memListRoot->nextItem = tempItem;
				memListRoot->allocated = 1;
				break;
			}
	else
	if (currentItem->size >= sizeInBytes)
		result = (char*)currentItem + sizeof(memoryItem);
		currentItem->allocated = 1;
		break;
		}
		currentItem = currentItem->nextItem;
	}
	return result;

	void userFree(void* userAllocation) {
		MemoryItem* userBlockMemory;
		char* tempPtr;
		if (userAllocation == NULL) return;
		userBlockMemory = (MemoryItem*)((char*)userAllocation - sizeof(MemoryItem));
		userBlockMemory->allocation = 0;
		defragmentMemory();
	}
	void defragmentMemory() {
		MemoryItem* currentItem = memListRoot;
		while (currentItem != NULL) {
			if (currentItem->allocated == 0) {
				if (currentItem->nextItem != NULL) {
					if (currentItem->nextItem->allocated == 0) {
						currentItem->size = currentItem->size + sizeof(MemoryItem) + currentItem->nextItem->size;
						currentItem->nextItem = currentItem->nextItem->nextItem;
					}
					else
						currentItem = currentItem->nextItem;
				}
				else   break;
			}
			else currentItem = currentItem->nextItem;
		}
	}
	void printMemoryStatus() {
		MemoryItem* currentItem = memListRoot;
		printf("Begin list: \n");
		while (currentItem != NULL) {
			printf("%p: Size: %u, Allocated: %d\n", currentItem, currentItem->size, currentItem->allocated);
			currentItem = currentItem->nextItem;
		}
		printf("End list\n\n");
	}

// ==============================================
#ifndef USER_MEMORY
#define USER_MEMORY
	typedef struct MemoryItem {
		unsigned int size;
		struct MemoryItem* nextItem;
		unsigned char allocated;
		// void* userData;
	}
	MemoryItem;
	void initFreeSpace(int sizeInBytes);
	void cleanUpSpace();
	void* userAllocation(int sizeInBytes);
	void defragmentMemory();
	void userFree(void* userAllocation);
	void printMemoryStatus();
#endif
// ===========================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#define kB 1024
#define MB 1024*1024
#define GB 1024*1024*1024
	typedef struct List {
		int val;
		struct List* next;
	}
	List;

	List* new_List(int val) {
		List* result = userAllocation(sizeof(List));
		if (result != NULL) {
			result->val = val;
			result->next = NULL;
		}
		return result;
	}
	int main() {
		List* root, * current;
		char* msg, * msg2;
		initFreeSpace(512 * MB);

		printMemoryStatus();

		root = new_List(5);
		printMemoryStatus();

		current = new_List(5);
		printMemoryStatus();

		userFree(root);
		printMemoryStatus();

		userFree(root);
		printMemoryStatus();
		msg = userAllocation(12000);
		// strcpy(msg, HELLO WORLD!");
		printMemoryStatus();
		userFree(current);
		userFree(msg);
		printMemoryStatus();
		msg = userAllocation(10000);
		printMemoryStatus();
		cleanUpSpace();
		return 0;
	}



		
	


