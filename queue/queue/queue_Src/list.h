#pragma once
#include "listNode.h"
#include <iostream>

template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); } //less than
template <typename T> static bool lt(T& a, T& b) { return a < b; } //less than
template <typename T> static bool eq(T* a, T* b) { return eq(*a, *b); } //equal
template <typename T> static bool eq(T& a, T& b) { return a == b; } //equal

template <typename T> class List { //列表模板类

private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //规模、头哨兵、尾哨兵

protected:
	void init(); //列表创建时的初始化
	int	 clear(); //清除所有节点
	void copyNodes(ListNodePosi(T) p, int n); //复制列表中自位置p起的n项
	void merge(ListNodePosi(T)& p, int, List<T>& L, ListNodePosi(T) q, int m); //归并
	void mergeSort(ListNodePosi(T)& p, int n); //对从p开始连续的n个节点归并排序
	void selectionSort(ListNodePosi(T) p, int n); //对从p开始连续的n个节点选择排序
	void insertionSort(ListNodePosi(T) p, int n); //对从p开始连续的n个节点插入排序

public:
	// 构造函数
	List() { init(); } //默认
	List(List<T> const& L); //整体复制列表L
	List(List<T> const& L, Rank r, int n); //复制列表L中自第r项起的n项
	List(ListNodePosi(T) p, int n); //复制列表中自位置p起的n项
									// 析构函数
	~List(); //释放（包含头、尾哨兵在内的）所有节点
			 // 只读访问接口
	Rank size() const { return _size; } //规模
	bool empty() const { return _size <= 0; } //判空
	T& operator[] (Rank r) const; //重载，支持循秩访问（效率低）
	ListNodePosi(T) first() const { return header->succ; } //首节点位置
	ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
	bool valid(ListNodePosi(T) p) //判断位置p是否对外合法
	{
		return p && (trailer != p) && (header != p);
	} //将头、尾节点等同于NULL
	ListNodePosi(T) find(T const& e) const //无序列表查找
	{
		return find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //无序区间查找
	ListNodePosi(T) search(T const& e) const //有序列表查找
	{
		return search(e, _size, trailer);
	}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //有序区间查找
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //在p及其n-1个后继中选出最大者
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //整体最大者
																		   // 可写访问接口
	ListNodePosi(T) insertAsFirst(T const& e); //将e当作首节点插入
	ListNodePosi(T) insertAsLast(T const& e); //将e当作末节点插入
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //将e当作p的后继插入（After）
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //将e当作p的前驱插入（Before）
	T remove(ListNodePosi(T) p); //删除合法位置p处的节点,返回被删除节点
	void merge(List<T>& L) { merge(first(), size, L, L.first(), L._size); } //全列表归并
	void sort(ListNodePosi(T) p, int n); //列表区间排序
	void sort() { sort(first(), _size); } //列表整体排序
	int deduplicate(); //无序去重
	int uniquify(); //有序去重
	void reverse(); //前后倒置
					// 遍历
	void traverse(void(* visit) (T&)); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
	template <typename VST> //操作器
	void traverse(VST& visit); //遍历，依次实施visit操作（函数对象，可全局性修改）
}; //List


template <typename T> void List<T>::init() { //列表初始化，在创建列表对象时统一调用
	header = new ListNode<T>; //创建头哨兵节点
	trailer = new ListNode<T>; //创建尾哨兵节点
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	_size = 0; //记录规模
}

template <typename T> int List<T>::clear() { //清空列表
	int oldSize = _size;
	while (0 < _size) remove(header->succ); //反复删除首节点，直至列表变空
	return oldSize;
}

template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{
	init();
	while (n--) { insertAsLast(p->data); p = p->succ; }
}

template<typename T>//有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
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
		p = p->succ; remove(p->pred); //转向下一节点
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

template <typename T> T List<T>::remove(ListNodePosi(T) p) { //删除合法节点p，返回其数值
	T e = p->data; //备份待删除节点的数值（假定T类型可直接赋值）
	p->pred->succ = p->succ; p->succ->pred = p->pred; //后继、前驱
	delete p; _size--; //释放节点，更新规模
	return e; //返回备份的数值
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
	ListNodePosi(T) p = header; ListNodePosi(T) q = trailer; //头、尾节点
	for (int i = 1; i < _size; i += 2) //（从首、末节点开始）由外而内，捉对地
		swap((p = p->succ)->data, (q = q->pred)->data); //交换对称节点的数据项
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

template<typename T>//在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到 不大于e最后者
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
