#pragma once

typedef int Rank;

#define ListNodePosi(T) ListNode<T>*

template <typename T>
struct  ListNode
{
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ;

	ListNode() {};
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {} 
	~ListNode() {};
	ListNodePosi(T) insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);
};

template <typename T> //��e������ǰ�ڵ�֮ǰ�����ڵ�ǰ�ڵ������б������ڱ�ͷ�ڵ�header��
ListNodePosi(T) ListNode<T>::insertAsPred(T const & e)
{
	ListNodePosi(T) x = new ListNode(e, pred, this);
	pred->succ = x, pred = x;
	return x;
}
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const & e)
{
	ListNodePosi(T) x = new ListNode(e, this, succ);
	succ->pred = x, succ = x;
	return x;
}
