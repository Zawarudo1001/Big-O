#include <iostream>
#include <stack>
#include <time.h>

using namespace std;

unsigned long int nop = 0;

//нужно отдельно написать адаптер, содержащий функции setElement(position) и getElement(pos)

class Adapter
{
	stack<int>* st;

public:
	Adapter() {
		st = new stack<int>();
	}

	int size() {					//1
		nop++;
		return st->size();
	}

	bool isEmpty() {				//1
		nop++;
		return st->empty();
	}

	int getElement(int ind) {				// + 2(size) + 1(return) + 3(if) + 2(tmpSt1) + 1(pointers) + 1(element) + (2 + 5n)(for loop) + 1(delete)
											// 2 + 1 + 3 + 2 + 1 + 1 + 2 + 1 + 5n = 5n + 13

		int size = st->size(); nop += 2;		//assignment + size() call

		if ((ind >= 0) and (ind < size)) {

			stack<int>* tmpSt1 = new stack<int>(); nop += 2;		//tmpSt1 assignment and operator new 

			*tmpSt1 = *st; nop++;

			int element = 0; nop++;			//assignment element

			nop++;		//initilisator
			nop++;		//condition
			for (int i = 0; i <= ind; i++) {			//max iterations = n
				element = tmpSt1->top(); nop += 2;		//assignment + top() call
				tmpSt1->pop(); nop++;			//pop() call
				nop++;		//condition
				nop++;		//iterator
			}

			delete tmpSt1; nop++;

			nop++;		//return
			return element;

		}	nop += 3;			//if 2 conditions + AND

		nop++;		//return 1 value
		return -1;
	}

	bool setElement(int ind, int element) {				//2(size) + 2(st2) + 3(if) + 1(return) + 1(delete) + (2 + 5n)(for loop) + 1(pop) + 2(push) + (2 + 6n)(while loop)
														// 2 + 2 + 3 + 1 + 1 + 2 + 1 + 2 + 2 + 5n + 6n = 11n + 16

		int size = st->size(); nop += 2;		//size assignment and size() call

		stack<int>* st2 = new stack<int>(); nop += 2;		//st2 assignment and operator new

		if ((ind >= 0) and (ind < size)) {

			nop++;		//initilisator
			nop++;		//condition
			for (int i = 0; i <= ind; i++) {
				st2->push(st->top());	//push call with 1 arg, top call
				nop += 2;
				st->pop();
				nop++;		//pop call
				nop++;		//condition
				nop++;		//iterator
			}

			st2->pop(); nop++;
			st->push(element);  nop += 2;

			nop += 2;			//condition; inverter + empty call()
			while (!st2->empty()) {
				st->push(st2->top());
				nop += 3;		//push() call with 1 arg and top() call
				st2->pop();
				nop++;		//pop() call
				nop += 2;		//condition; inverter + empty call()
			}
			
			delete st2; nop++;
			nop++;			//return 1 value
			return true;
		} nop += 3;			//if 2 conditions + AND


		delete st2; nop++;
		
		nop++;		//return 1 value
		return false;
	}

	void push(int x)		//2
	{
		st->push(x);
		nop += 2;
	}

	void pop() {			//1
		st->pop();
		nop += 1;
	}

	int top() {				//1
		nop += 1;
		return st->top();
	}

	void display() {			//2(tmpSt1) + 2(size) + (2 + 8n)(for loop) + 1(delete) + 2(tmpSt2) + (2 + 6n) (for loop) + 3(delete, st, cout)
								//2 + 2 + 2 + 1 + 2 + 2 + 3 + 8n + 6n = 14 + 14n


		stack<int>* tmpSt1 = new stack<int>(); nop += 2;		//assignment + operator new

		int size = st->size(); nop += 2;		//assignment + size() call

		nop++;		//initilisation
		nop++;		//condition

		for (int i = 0; i < size; i++) {
			int element = st->top(); nop += 2;		//assignment + top() call
			cout << " " << element; nop++;	
			st->pop(); nop++;
			tmpSt1->push(element); nop += 2;		//call push() with 1 arg

			nop++;		//condition
			nop++;		//iterator
		}

		delete(st); nop++;

		stack<int>* tmpSt2 = new stack<int>(); nop += 2;		//tmpSt2 assignment + operation new

		nop++;		//initilisation
		nop++;		//condition
		for (int i = 0; i < size; i++) {
			tmpSt2->push(tmpSt1->top()); nop += 3;		//top() call no args + push call 1 arg()
			tmpSt1->pop(); nop++;

			nop++;		//condition
			nop++;		//iterator
		}
		

		delete tmpSt1; nop++;
		st = tmpSt2; nop++;

		cout << '\n'; nop++;
		
	}

};

class Sorter{
public:
	Sorter() {}
	~Sorter() {};
	// сортировка вставками

	//getElement = 5n + 13
	//setElement = 11n + 16


	void insertionSort(Adapter &st, int size)		// 2(key,j) +	2 + n * (		5n+13+1(key)   +  2(j)  +  5n + 16 + n*(5n + 16 + 1 + 5n + 13 + 11n + 16)(while loop) + 11n+16(setElem)	+ 2(condition + iterator)	)

													// 4 + n * (	5n + 14 + 2 + 5n + 16 + n*(5n + 16 + 1 + 5n + 13 + 11n + 16) + 11n + 16 + 2			)

													// 4 + n * (	10n + 32 + n * (21n + 46) + 11n + 8) = 4 + n * (21n + 40 + n * (21n + 46))    = 4 + n(21n + 40 + 21n^2 + 46n) = 4 + 21n^2 + 40n + 21n^3 + 46n^2 = 21n^3 + 67n^2 + 40n + 4 

	{
		
		int key = 0;
		int j = 0;
		nop += 2;		//variables assignment

		nop++;		//initilisation
		nop++;		//condition
		for (int i = 1; i < size; i++)
		{
			key = st.getElement(i); nop += 2;				//5n + 13 + 1			//SetElem + key assignment
			j = i - 1; nop+=2;								//2

			nop += 5;		//condition						//1 + 1 + 1 + (5n + 13) = 5n + 16, if st.getElement(j) <= key -> break else j for i in range(i)
															//max iterations = n
			while (j >= 0 && st.getElement(j) > key)
			{
				st.setElement(j + 1, st.getElement(i)); nop += 4;		//getElement() with 1 arg call + setElement with 2 args call
				j--; nop++;
				nop += 5;		//condition
			}												//(5n + 15 + n*(5n + 15 + 1 + 5n + 13 + 11n + 16)

			st.setElement(j + 1, key); nop += 3;		//setElement call + 2 args

			nop++;		//condition
			nop++;		//iterator
		}

	}

};


//		F(n) = 21n^3 + 67n^2 + 40n + 4
//		O(F(n)) = n^3

int main()
{
	Sorter sortmanager;

	for (int n = 300; n <= 3000; n += 300) {
		Adapter *st =new Adapter;

		srand(time(NULL));
		for (int i = 0; i < n; i++) {
			st->push(rand());
		}

		const clock_t beginTime = clock();
		sortmanager.insertionSort(*st, st->size());
		cout << endl;
		cout << "Size: " << st->size() << endl;
		cout << "Time(sec): " << float(clock() - beginTime) / CLOCKS_PER_SEC << endl;
		cout << "NOP: " << nop << endl;
		nop = 0;
		delete st;
	}
	return 0;
}
