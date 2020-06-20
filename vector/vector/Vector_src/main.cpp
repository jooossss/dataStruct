#include<iostream>
#include<cstdio>
#include "Vector.h"

int main() {
	int num[] = { 10,6,4,1,2,9,8,6,2,1 };
	cout << "============= test =============" << endl;
	Vector<int> V (num,10);

	for (int i = 0; i < V.size(); i++) { //依次查找向量中元素，当然成功
		int e = num[i];
		Rank r = V.find(e);
		if (r < 0) printf(" : not found until rank V[%d] <> %d", r, e);
		else printf(" : found at rank V[%d] = %d", r, V[r]);
		printf("\n");
	}
	cout << "============= test  deduplicate=============" << endl;
	V.deduplicate();
	for (int i = 0; i < V.size(); i++) { //依次查找向量中元素，当然成功
		int e = num[i];
		Rank r = V.find(e);
		if (r < 0) printf(" : not found until rank V[%d] <> %d", r, e);
		else printf(" : found at rank V[%d] = %d", r, V[r]);
		printf("\n");
	}
	cout << "============= test  sort=============" << endl;
	V.sort();
	for (int i = 0; i < V.size(); i++) { //依次查找向量中元素，当然成功
		 printf(" : found at rank V[%d] = %d\n", i,V[i]);
	}


	cout << "============= test  V1=V =============" << endl;
	Vector<int> V1 = V;
	for (int i = 0; i < V1.size(); i++) { //依次查找向量中元素，当然成功
		printf(" : found at rank V1[%d] = %d\n", i, V1[i]);
	}

	cout << "============= test  V1 inster =============" << endl;
	for (int i = 0; i < 10; i++) 
		V1.insert(num[i]);
	for (int i = 0; i < V1.size(); i++) { //依次查找向量中元素，当然成功
		printf(" : found at rank V1[%d] = %d\n", i, V1[i]);
	}

	cout << "============= test  V1 unsort =============" << endl;
	V1.unsort();
	for (int i = 0; i < V1.size(); i++) { //依次查找向量中元素，当然成功
		printf(" : found at rank V1[%d] = %d\n", i, V1[i]);
	}
}