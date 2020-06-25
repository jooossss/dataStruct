#pragma once
#include "Fib.h" //用于fibSearch

using namespace std;

typedef int Rank; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）


//向量模板类
template<typename T> class Vector
{
protected:
	Rank _size; int _capacity;  T* _elem; //规模、容量、数据区
	void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
	void expand(); //空间不足时扩容
	void shrink(); //装填因子过小时压缩
	//bool bubble(Rank lo, Rank hi); //扫描交换
	void bubbleSort(Rank lo, Rank hi); //起泡排序算法
	Rank max(Rank lo, Rank hi); //选取最大元素
	void selectionSort(Rank lo, Rank hi); //选择排序算法
	void merge(Rank lo, Rank mi, Rank hi); //归并算法
	void mergeSort(Rank lo, Rank hi); //归并排序算法
	void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
	Rank partition(Rank lo, Rank hi); //轴点构造算法
	void quickSort(Rank lo, Rank hi); //快速排序算法
	void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
	Vector (int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

	~Vector() { delete [] _elem; };
	// 只读访问接口
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e) const
	{	return (0 >= _size) ? -1 : search(e, 0, _size);	}
	Rank search(T const& e, Rank lo, Rank hi) const;
	// 可写访问接口
	T& operator[] (Rank r);
	const T& operator[] (Rank r) const;
	Vector<T> & operator= (Vector<T> const&);
	T remove(Rank r);
	int remove(Rank lo, Rank hi);
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }
	void sort(Rank lo, Rank hi);
	void sort() { sort(0, _size); }
	void unsort(Rank lo, Rank hi);
	void unsort() { unsort(0, _size); }
	int deduplicate();
	int uniquify();

	//遍历
	void traverse(void(*)(T&)); //遍历（使用函数指针，只读或局部性修改）
	template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
};

template <typename T> //元素类型
void Vector<T>::copyFrom (T const* A, Rank lo ,Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

template<typename T>
void Vector<T>::expand()
{
	if (_size < _capacity)return;
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink()
{
	if (_capacity < DEFAULT_CAPACITY << 1) return;
	if (_capacity < _size << 2) return;
	T* oldElem = _elem; _elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}

template <typename T> //向量的起泡排序（提前终止版）
void Vector<T>::bubbleSort(Rank lo, Rank hi) { //assert: 0 <= lo < hi <= size
	for (bool sorted = false; sorted = !sorted; )
		for (Rank i = lo; i < hi - 1; i++)
			if (_elem[i] > _elem[i + 1]) //若逆序，则
				swap(_elem[i], _elem[i + 1]), sorted = false;
	//经交换使局部有序——当然，至此还不能确定已整体有序
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi)
{
	Rank mx = hi;
	while (lo < hi--)
	{
		if ((_elem[hi] > _elem[mx]))
			mx = hi;
	}
	return mx;
}

template<typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi)
{
	while (lo < --hi)
	{
		swap(_elem[max(lo, hi)], _elem[hi]);
	}
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
	T* A = _elem + lo;
	int lb = mi - lo; T* B = new T[lb];
	for (Rank i = 0; i < lb; i++) B[i] = A[i];
	int lc = hi - mi; T* C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; j < lb;) //归并：反复从B和C首元素中取出更小者
		A[i++] = (lc <= k || B[j] <= C[k]) ? B[j++] : C[k++];//将其归入A中
	delete[] B;
}

template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2) return;
	int mi = (hi + lo) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}
template<typename T>
inline void Vector<T>::heapSort(Rank lo, Rank hi)
{
}
//
//template <typename T> void Vector<T>::heapSort(Rank lo, Rank hi) { //0 <= lo < hi <= size
//	T* A = _elem + lo; Rank n = hi - lo; heapify(A, n); //将待排序区间建成一个完全二叉堆，O(n)
//	while (0 < --n) //反复地摘除最大元并归入已排序的后缀，直至堆空
//	{
//		swap(A[0], A[n]); percolateDown(A, n, 0);
//	} //堆顶与末元素对换，再下滤
//}

template <typename T> //轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
Rank Vector<T>::partition(Rank lo, Rank hi) { //版本B1：版本B的等价形式，可直接转至与版本A等价的版本A1
	swap(_elem[lo], _elem[lo + rand() % (hi - lo)]); //任选一个元素与首元素交换
	hi--; T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
	while (lo < hi) { //从向量的两端交替地向中间扫描
		while ((lo < hi) && (pivot < _elem[hi])) //在大于pivot的前提下
			hi--; //向左拓展右端子向量
		if (lo < hi) _elem[lo++] = _elem[hi]; //不大于pivot者归入左端子向量
		while ((lo < hi) && (_elem[lo] < pivot)) //在小于pivot的前提下
			lo++; //向右拓展左端子向量
		if (lo < hi) _elem[hi--] = _elem[lo]; //不小于pivot者归入右端子向量
	} //assert: lo == hi
	_elem[lo] = pivot; //将备份的轴点记录置于前、后子向量之间
	return lo; //返回轴点的秩
}


template <typename T> //向量快速排序
void Vector<T>::quickSort(Rank lo, Rank hi) { //0 <= lo < hi <= size
											  /*DSA*/ //printf ( "\tQUICKsort [%3d, %3d)\n", lo, hi );
	if (hi - lo < 2) return; //单元素区间自然有序，否则...
	Rank mi = partition(lo, hi); //在[lo, hi)内构造轴点
	quickSort(lo, mi); //对前缀递归排序
	quickSort(mi + 1, hi); //对后缀递归排序
}

template<typename T> //向量希尔排序
void Vector<T>::shellSort(Rank lo, Rank hi)//0 <= lo < hi <= size <= 2^30
{
	for( int d = 0x3FFFFFFF;0<d; d >>= 1)
		for (int j = lo + d; j < hi; j++) {
			T x = _elem[j]; int i = j - d;
			while (lo <= i && _elem[i] > x)
			{
				_elem[i + d] = _elem[i];
				i -= d;
			}
			_elem[i + d] = x;
		}
}

template <typename T> 
Rank Vector<T>::find(T const& e, Rank lo ,Rank hi) const {
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
	return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}

template <typename T>
T& Vector<T>::operator[](Rank r) {
	return _elem[r];
}
template <typename T>
const T& Vector<T>::operator[](Rank r) const {
	return _elem[r];
}

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //深复制
	if (_elem) delete[] _elem; //释放原有内容
	copyFrom(V._elem, 0, V.size()); //整体复制
	return *this; //返回当前对象的引用，以便链式赋值
}

template <typename T> T Vector<T>::remove(Rank r) { //删除向量中秩为r的元素，0 <= r < size
	T e = _elem[r]; //备份被删除元素
	remove(r, r + 1); //调用区间删除算法，等效于对区间[r, r + 1)的删除
	return e; //返回被删除元素
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi) return 0;
	while (hi < _size) _elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}

template <typename T>Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
	_elem[r] = e;
	_size++;
	return r;
}


template <typename T> void Vector<T>::sort(Rank lo, Rank hi) { //向量区间[lo, hi)排序
	switch (rand() % 3) {
	case 0:  bubbleSort(lo, hi); break; //起泡排序
	case 1:  selectionSort(lo, hi); break; //选择排序
	case 2:  mergeSort(lo, hi); break; //归并排序
	//case 4:  heapSort(lo, hi); break; //堆排序
	//case 5:  quickSort(lo, hi); break; //快速排序
	//default: shellSort(lo, hi); break; //希尔排序
	} //随机选择算法以充分测试。实用时可视具体问题的特点灵活确定或扩充
}

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) { //等概率随机置乱区间[lo, hi)
	T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
	for (Rank i = hi - lo; i > 0; i--) //自后向前
		swap(V[i - 1], V[rand() % i]); //将V[i - 1]与V[0, i)中某一元素随机交换
}

template <typename T> int Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
	int oldSize = _size; //记录原规模
	Rank i = 1; //从_elem[1]开始
	while (i < _size) //自前向后逐一考查各元素_elem[i]
		if (find(_elem[i], 0, i) < 0) //在前缀[0,i)中寻找与之雷同者（至多一个）
			i++; //若无雷同则继续考查其后继
		else
			remove(i); //否则删除当前元素
	return oldSize - _size; //被删除元素总数
}

template <typename T> void Vector<T>::traverse(void(*visit) (T&)) //借助函数指针机制
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
} //遍历向量

template <typename T> template <typename VST> //元素类型、操作器
void Vector<T>::traverse(VST& visit) //借助函数对象机制
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
} //遍历向量



template<class T> Rank binSearch(T& A, T e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (hi + lo) >> 1;
		e < A[lo] ? hi = mi : lo = mi + 1;//最后一项必是 lo = mi + 1;
	}
	return --lo;//严格按照语义执行
}
template <class T> Rank fibSearch(T& A, T e, Rank lo, Rank hi) {
	Fib fib(hi - lo);
	while (lo < hi) {
		while (hi - lo < fib.get()) fib.prev();
		Rank mi = lo + fib.get() - 1;
		//总结
		//如果是初始值加区间长度，一定要减一
		//如果是[lo,hi)区间，区间长度 = hi - lo
		//for循环中i = 0(左闭)i<_size（右开）,会循环[0,_size)，区间长度 = _size,循环这么多次。
		//while循环中lo<hi ==> 0<hi - lo即循环到hi = lo终止//while循环中1< hi - lo即循环到[lo,hi），hi = lo+1
		//因此while循环左边是几（0，1，2……），区间长度就是几
		//但是指针不同，_elem+lo,就是指向lo这个位置
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
