#include "queue.h"
#include <iostream>

using namespace std;

int main() {
	int num[] = { 9,8,7,6,5,0,4,3,2,1 };
	Queue<int> q;
	int i = 9;
	while (0 <= i) {
		q.enqueue(num[i--]);
	}
	i = 9;
	while (0 <= i) {
		cout << q.dequeue();
		i--;
	}
}