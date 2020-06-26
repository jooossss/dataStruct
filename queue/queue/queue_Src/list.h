#pragma once
#include "listNode.h"
#include <iostream>

template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); } //less than
template <typename T> static bool lt(T& a, T& b) { return a < b; } //less than
template <typename T> static bool eq(T* a, T* b) { return eq(*a, *b); } //equal
template <typename T> static bool eq(T& a, T& b) { return a == b; } //equal

template <typename T> class List { //�б�ģ����

private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //��ģ��ͷ�ڱ���β�ڱ�

protected:
	void init(); //�б���ʱ�ĳ�ʼ��
	int	 clear(); //������нڵ�
	void copyNodes(ListNodePosi(T) p, int n); //�����б�����λ��p���n��
	void merge(ListNodePosi(T)& p, int, List<T>& L, ListNodePosi(T) q, int m); //�鲢
	void mergeSort(ListNodePosi(T)& p, int n); //�Դ�p��ʼ������n���ڵ�鲢����
	void selectionSort(ListNodePosi(T) p, int n); //�Դ�p��ʼ������n���ڵ�ѡ������
	void insertionSort(ListNodePosi(T) p, int n); //�Դ�p��ʼ������n���ڵ��������

public:
	// ���캯��
	List() { init(); } //Ĭ��
	List(List<T> const& L); //���帴���б�L
	List(List<T> const& L, Rank r, int n); //�����б�L���Ե�r�����n��
	List(ListNodePosi(T) p, int n); //�����б�����λ��p���n��
									// ��������
	~List(); //�ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
			 // ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return _size <= 0; } //�п�
	T& operator[] (Rank r) const; //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
	ListNodePosi(T) first() const { return header->succ; } //�׽ڵ�λ��
	ListNodePosi(T) last() const { return trailer->pred; } //ĩ�ڵ�λ��
	bool valid(ListNodePosi(T) p) //�ж�λ��p�Ƿ����Ϸ�
	{
		return p && (trailer != p) && (header != p);
	} //��ͷ��β�ڵ��ͬ��NULL
	ListNodePosi(T) find(T const& e) const //�����б����
	{
		return find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //�����������
	ListNodePosi(T) search(T const& e) const //�����б����
	{
		return search(e, _size, trailer);
	}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //�����������
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //��p����n-1�������ѡ�������
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //���������
																		   // ��д���ʽӿ�
	ListNodePosi(T) insertAsFirst(T const& e); //��e�����׽ڵ����
	ListNodePosi(T) insertAsLast(T const& e); //��e����ĩ�ڵ����
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //��e����p�ĺ�̲��루After��
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //��e����p��ǰ�����루Before��
	T remove(ListNodePosi(T) p); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�
	void merge(List<T>& L) { merge(first(), size, L, L.first(), L._size); } //ȫ�б�鲢
	void sort(ListNodePosi(T) p, int n); //�б���������
	void sort() { sort(first(), _size); } //�б���������
	int deduplicate(); //����ȥ��
	int uniquify(); //����ȥ��
	void reverse(); //ǰ����
					// ����
	void traverse(void(* visit) (T&)); //����������ʵʩvisit����������ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST> //������
	void traverse(VST& visit); //����������ʵʩvisit�������������󣬿�ȫ�����޸ģ�
}; //List


template <typename T> void List<T>::init() { //�б��ʼ�����ڴ����б����ʱͳһ����
	header = new ListNode<T>; //����ͷ�ڱ��ڵ�
	trailer = new ListNode<T>; //����β�ڱ��ڵ�
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	_size = 0; //��¼��ģ
}

template <typename T> int List<T>::clear() { //����б�
	int oldSize = _size;
	while (0 < _size) remove(header->succ); //����ɾ���׽ڵ㣬ֱ���б���
	return oldSize;
}

template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{
	init();
	while (n--) { insertAsLast(p->data); p = p->succ; }
}

template<typename T>//�����б�Ĺ鲢����ǰ�б�����p���n��Ԫ�أ����б�L����q���m��Ԫ�ع鲢
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m)
{
	ListNodePosi(T) pp = p->pred;
	while (0 < m) {
		if ((0 < n) && (p->data <= q->data))
		{
			if (q == (p = p->succ)) break;
			n--;
		}
		else
		{
			insertB(p, L.remove((q = q->succ)->pred));
			m--;
		}
	}
	p = pp->succ;
}

template<typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n)
{
	if (n < 2)return;
	int m = n >> 1;
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++) q = q->succ;
	mergeSort(p, m); mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);
}

template<typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T)	tail = p;
	for (int i = 0; i < n; i++) tail = tail->succ;
	while (1 < n) {
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertB(tail, remove(max));
		tail = tail->pred;
		n--;
	}
}

template<typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
	int r = 0;
	while (0 < n--) {
		insertA(search(p->data, r, p), p->data);
		p = p->succ; remove(p->pred); //ת����һ�ڵ�
		r++;
	}
}

template<typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const & e)
{
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) List<T>::
insertAsLast(T const & e)
{
	_size++;
	return trailer->insertAsPred(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const & e)
{
	_size++;
	return p->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const & e)
{
	_size++;
	return p->insertAsPred(e);
}

template <typename T> T List<T>::remove(ListNodePosi(T) p) { //ɾ���Ϸ��ڵ�p����������ֵ
	T e = p->data; //���ݴ�ɾ���ڵ����ֵ���ٶ�T���Ϳ�ֱ�Ӹ�ֵ��
	p->pred->succ = p->succ; p->succ->pred = p->pred; //��̡�ǰ��
	delete p; _size--; //�ͷŽڵ㣬���¹�ģ
	return e; //���ر��ݵ���ֵ
}

template<typename T>
void List<T>::sort(ListNodePosi(T) p, int n)
{
	switch (rand() % 3) {
	case 1: insertionSort(p, n); break; 
	case 2: selectionSort(p, n); break; 
	default: mergeSort(p, n); break; 
	}
}

template<typename T>
int List<T>::deduplicate()
{
	if (_size < 2) return 0;
	int oldSize = _size;
	ListNodePosi(T) p = header;
	Rank r = 0;
	while (trailer != (p = p->succ)) {
		ListNodePosi(T) q = find(p->data,r,p);
		q ? remove(q) : r++;
	}
	return oldSize - _size;
}

template<typename T>
int List<T>::uniquify()
{
	if (_size < 2) return 0;
	ListNodePosi(T) p; ListNodePosi(T) q;
	for(p = header,q = p->succ; trailer != q;  p = q,q = p->succ)
		if (p->data == q->data) { remove(q); q = p; }
	return 0;
}

template<typename T>
void List<T>::reverse()
{
	//Rank r = _size;
	//while (0 < r--) {
	//	ListNodePosi(T) p = header->succ;
	//	insertAsLast(p->data);
	//	p = p->succ;
	//	remove(p->pred);
	//}
	ListNodePosi(T) p = header; ListNodePosi(T) q = trailer; //ͷ��β�ڵ�
	for (int i = 1; i < _size; i += 2) //�����ס�ĩ�ڵ㿪ʼ��������ڣ�׽�Ե�
		swap((p = p->succ)->data, (q = q->pred)->data); //�����Գƽڵ��������
}

template<typename T>
void List<T>::traverse(void(* visit)(T &))
{
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template<typename T>
List<T>::List(List<T> const & L)
{
	copyNodes(L.first(), L._size);
}

template<typename T>
List<T>::List(List<T> const & L, Rank r, int n)
{
	copyNodes(L[r],n )
}

template<typename T>
List<T>::List(ListNodePosi(T) p, int n)
{
	copyNodes(p, n);
}

template<typename T>
List<T>::~List()
{
	clear();
	delete header;
	delete trailer;
}

template<typename T>
T & List<T>::operator[](Rank r) const
{
	ListNodePosi(T) p = first();
	while (r--) p = p->succ;
	return p->data;
}

template<typename T>
ListNodePosi(T) List<T>::find(T const & e, int n, ListNodePosi(T) p) const
{
	while (0< n--) { if (e == (p = p->pred)->data) return p; }
	return NULL;
}

template<typename T>
template<typename VST>
void List<T>::traverse(VST & visit)
{
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data);
}

template<typename T>//�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ� ������e�����
ListNodePosi(T) List<T>::search(T const & e, int n, ListNodePosi(T) p) const
{
	while (0 < n--) {
		if ((p = p->pred)->data <= e) break;
	}
	return p;
}

template<typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) max = p;
	for (ListNodePosi(T) cur = p; 1 < n; n--)
		if (!lt((cur = cur->succ)->data, max->data))
			max = cur;
	return max;
}
