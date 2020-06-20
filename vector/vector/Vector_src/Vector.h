#pragma once
#include "Fib.h" //����fibSearch

using namespace std;

typedef int Rank; //��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����


//����ģ����
template<typename T> class Vector
{
protected:
	Rank _size; int _capacity;  T* _elem; //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
	void expand(); //�ռ䲻��ʱ����
	void shrink(); //װ�����ӹ�Сʱѹ��
	//bool bubble(Rank lo, Rank hi); //ɨ�轻��
	void bubbleSort(Rank lo, Rank hi); //���������㷨
	Rank max(Rank lo, Rank hi); //ѡȡ���Ԫ��
	void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
	void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
	void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
	void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩
	Rank partition(Rank lo, Rank hi); //��㹹���㷨
	void quickSort(Rank lo, Rank hi); //���������㷨
	void shellSort(Rank lo, Rank hi); //ϣ�������㷨
public:
	Vector (int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //����Ϊc����ģΪs������Ԫ�س�ʼΪv
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

	~Vector() { delete [] _elem; };
	// ֻ�����ʽӿ�
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e) const
	{	return (0 >= _size) ? -1 : search(e, 0, _size);	}
	Rank search(T const& e, Rank lo, Rank hi) const;
	// ��д���ʽӿ�
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

	//����
	void traverse(void(*)(T&)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST> void traverse(VST&); //������ʹ�ú������󣬿�ȫ�����޸ģ�
};

template <typename T> //Ԫ������
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

template <typename T> //����������������ǰ��ֹ�棩
void Vector<T>::bubbleSort(Rank lo, Rank hi) { //assert: 0 <= lo < hi <= size
	for (bool sorted = false; sorted = !sorted; )
		for (Rank i = lo; i < hi - 1; i++)
			if (_elem[i] > _elem[i + 1]) //��������
				swap(_elem[i], _elem[i + 1]), sorted = false;
	//������ʹ�ֲ����򡪡���Ȼ�����˻�����ȷ������������
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
	for (Rank i = 0, j = 0, k = 0; j < lb;) //�鲢��������B��C��Ԫ����ȡ����С��
		A[i++] = (lc <= k || B[j] <= C[k]) ? B[j++] : C[k++];//�������A��
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
//	T* A = _elem + lo; Rank n = hi - lo; heapify(A, n); //�����������佨��һ����ȫ����ѣ�O(n)
//	while (0 < --n) //������ժ�����Ԫ������������ĺ�׺��ֱ���ѿ�
//	{
//		swap(A[0], A[n]); percolateDown(A, n, 0);
//	} //�Ѷ���ĩԪ�ضԻ���������
//}

template <typename T> //��㹹���㷨��ͨ������Ԫ��λ�ù�������[lo, hi)����㣬����������
Rank Vector<T>::partition(Rank lo, Rank hi) { //�汾B1���汾B�ĵȼ���ʽ����ֱ��ת����汾A�ȼ۵İ汾A1
	swap(_elem[lo], _elem[lo + rand() % (hi - lo)]); //��ѡһ��Ԫ������Ԫ�ؽ���
	hi--; T pivot = _elem[lo]; //����Ԫ��Ϊ��ѡ��㡪�������Ͻ�������Ч�����ѡȡ
	while (lo < hi) { //�����������˽�������м�ɨ��
		while ((lo < hi) && (pivot < _elem[hi])) //�ڴ���pivot��ǰ����
			hi--; //������չ�Ҷ�������
		if (lo < hi) _elem[lo++] = _elem[hi]; //������pivot�߹������������
		while ((lo < hi) && (_elem[lo] < pivot)) //��С��pivot��ǰ����
			lo++; //������չ���������
		if (lo < hi) _elem[hi--] = _elem[lo]; //��С��pivot�߹����Ҷ�������
	} //assert: lo == hi
	_elem[lo] = pivot; //�����ݵ�����¼����ǰ����������֮��
	return lo; //����������
}


template <typename T> //������������
void Vector<T>::quickSort(Rank lo, Rank hi) { //0 <= lo < hi <= size
											  /*DSA*/ //printf ( "\tQUICKsort [%3d, %3d)\n", lo, hi );
	if (hi - lo < 2) return; //��Ԫ��������Ȼ���򣬷���...
	Rank mi = partition(lo, hi); //��[lo, hi)�ڹ������
	quickSort(lo, mi); //��ǰ׺�ݹ�����
	quickSort(mi + 1, hi); //�Ժ�׺�ݹ�����
}

template<typename T> //����ϣ������
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

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //���
	if (_elem) delete[] _elem; //�ͷ�ԭ������
	copyFrom(V._elem, 0, V.size()); //���帴��
	return *this; //���ص�ǰ��������ã��Ա���ʽ��ֵ
}

template <typename T> T Vector<T>::remove(Rank r) { //ɾ����������Ϊr��Ԫ�أ�0 <= r < size
	T e = _elem[r]; //���ݱ�ɾ��Ԫ��
	remove(r, r + 1); //��������ɾ���㷨����Ч�ڶ�����[r, r + 1)��ɾ��
	return e; //���ر�ɾ��Ԫ��
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


template <typename T> void Vector<T>::sort(Rank lo, Rank hi) { //��������[lo, hi)����
	switch (rand() % 3) {
	case 0:  bubbleSort(lo, hi); break; //��������
	case 1:  selectionSort(lo, hi); break; //ѡ������
	case 2:  mergeSort(lo, hi); break; //�鲢����
	//case 4:  heapSort(lo, hi); break; //������
	//case 5:  quickSort(lo, hi); break; //��������
	//default: shellSort(lo, hi); break; //ϣ������
	} //���ѡ���㷨�Գ�ֲ��ԡ�ʵ��ʱ���Ӿ���������ص����ȷ��������
}

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) { //�ȸ��������������[lo, hi)
	T* V = _elem + lo; //��������_elem[lo, hi)������һ����V[0, hi - lo)
	for (Rank i = hi - lo; i > 0; i--) //�Ժ���ǰ
		swap(V[i - 1], V[rand() % i]); //��V[i - 1]��V[0, i)��ĳһԪ���������
}

template <typename T> int Vector<T>::deduplicate() { //ɾ�������������ظ�Ԫ�أ���Ч�棩
	int oldSize = _size; //��¼ԭ��ģ
	Rank i = 1; //��_elem[1]��ʼ
	while (i < _size) //��ǰ�����һ�����Ԫ��_elem[i]
		if (find(_elem[i], 0, i) < 0) //��ǰ׺[0,i)��Ѱ����֮��ͬ�ߣ�����һ����
			i++; //������ͬ�������������
		else
			remove(i); //����ɾ����ǰԪ��
	return oldSize - _size; //��ɾ��Ԫ������
}

template <typename T> void Vector<T>::traverse(void(*visit) (T&)) //��������ָ�����
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
} //��������

template <typename T> template <typename VST> //Ԫ�����͡�������
void Vector<T>::traverse(VST& visit) //���������������
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
} //��������



template<class T> Rank binSearch(T& A, T e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (hi + lo) >> 1;
		e < A[lo] ? hi = mi : lo = mi + 1;//���һ����� lo = mi + 1;
	}
	return --lo;//�ϸ�������ִ��
}
template <class T> Rank fibSearch(T& A, T e, Rank lo, Rank hi) {
	Fib fib(hi - lo);
	while (lo < hi) {
		while (hi - lo < fib.get()) fib.prev();
		Rank mi = lo + fib.get() - 1;
		//�ܽ�
		//����ǳ�ʼֵ�����䳤�ȣ�һ��Ҫ��һ
		//�����[lo,hi)���䣬���䳤�� = hi - lo
		//forѭ����i = 0(���)i<_size���ҿ���,��ѭ��[0,_size)�����䳤�� = _size,ѭ����ô��Ρ�
		//whileѭ����lo<hi ==> 0<hi - lo��ѭ����hi = lo��ֹ//whileѭ����1< hi - lo��ѭ����[lo,hi����hi = lo+1
		//���whileѭ������Ǽ���0��1��2�����������䳤�Ⱦ��Ǽ�
		//����ָ�벻ͬ��_elem+lo,����ָ��lo���λ��
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
