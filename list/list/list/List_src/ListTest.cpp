#include <iostream>
#include <cstdio>
#include "list.h"

using namespace std;
void visit(int & n) {
	printf("%d ", n);
}

int main() {

	int num[] = {9,7,8,1,3,2,4,6,5,0};
	cout << "============list test============" << endl;
	List<int> L;
	for (int i = 0; i < 10; i++)
		L.insertAsFirst(num[i]);
	L.traverse(visit);
	cout << endl;

	cout << "============list remove============" << endl;
	List<int> L1 = L;
	L1.traverse(visit);
	cout << endl;
	for (int i = 0; i < 5; i++)
		L1.remove(L1.first());
	L1.traverse(visit);
	cout << endl;

	cout << "============list sort============" << endl;
	L.sort();
	L.traverse(visit);
	cout << endl;

	cout << "============list deduplicate============" << endl;
	for (int i = 0; i < 5; i++)
		L.insertAsFirst(num[i]);
	L.traverse(visit);
	cout << endl;
	L.deduplicate();
	L.traverse(visit);
	cout << endl;

	cout << "============list uniquify============" << endl;
	for (int i = 0; i < 8; i++)
		L.insertAsFirst(num[i]);
	L.sort();
	L.traverse(visit);
	cout << endl;
	L.uniquify();
	L.traverse(visit);
	cout << endl;
}