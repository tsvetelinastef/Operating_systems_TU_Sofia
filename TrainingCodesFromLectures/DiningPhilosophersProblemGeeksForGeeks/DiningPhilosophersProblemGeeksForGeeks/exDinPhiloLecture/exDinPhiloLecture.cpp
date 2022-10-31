﻿#include <iostream>

/*
Dining Philosophers
Five Philosophers stay on circle table. 
Each of them spends your life in sequence from thinking
and eating. In the center there is a large bowl on
the table spaghetti. Every philosopher has need two
forks to eat a serving of spaghetti.
0 1 3 2 4 0 1 2 3 4
One fork is placed between every pair of
philosophers they are agreed that each will use
only the forks that are immediate left and right.
..............................................................
Пет философа стоят на кръгла маса. Всеки от тях прекарва
живота си в последователност от мислене и ядене.
В центъра на масата има голяма купа спагети.
Всеки филосог има нужда от две вилици, за да изяде
порция спагети.
0 1 3 2 4 0 1 2 3 4
По една вилица е сложена между всяка двойка философи те
са се съгласили, че всеки ще използва само вилиците,
които са непосредствено вляво и вдясно
.............................................................
===============================================================

Всяка вилица е споделен ресурс с възможни действия 
взимам и оставям. Когато е гладен, всеки ФИЛ трябва
първо да вземе лявата и дясната вилица, преди да 
започне да яде.
.......................................................
Проблемът
- Проектиране на алгоритъм, който ще позволи на философите
да се нахранят.
-> Някои двама философи не могат да използват една и съща
вилица по едно и също време(взаимно изключване)
-> Нито един философ не трябва да умира от глад(избягване на 
deadlock и недостиг на ресурси)
*/

//program diningPhilosophers
// solution with semaphores
semaphore fork[5] = { 1 };
int i;
void philosopher(int i)
{
	while (true) {
		think();
		wait(fork[i]);
		wait(fork[(i + 1) mod 5]);
		eat();
		signal(fork[(i + 1) mod 5]);
		signal(fork[i]);
	}
}

void main()
{
	parbegin(philosopher(0), philosopher(1), philosopher(2), philosopher(3), philosopher(4));

}



// that was first Solution to the Dinning Philosophers Problem

// escaping of deadlock

