#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Deque_DEFINE(T) \
typedef struct Deque_##T##_Iterator { \
	T* arr;	\
	int beginIdx; \
	int idx; \
	int length; \
	void (*inc)(Deque_##T##_Iterator*); \
	void (*dec)(Deque_##T##_Iterator*); \
	T& (*deref)(Deque_##T##_Iterator*); \
} Deque_##T##_Iterator; \
bool Deque_##T##_Iterator_equal(Deque_##T##_Iterator it1, Deque_##T##_Iterator it2){ \
	return (it1.arr == it2.arr && it1.idx == it2.idx); \
} \
void Deque_##T##_Iterator_increment(Deque_##T##_Iterator* it){ \
	it->idx++; \
} \
void Deque_##T##_Iterator_decrement(Deque_##T##_Iterator* it){ \
	it->idx--; \
} \
T& Deque_##T##_Iterator_dereference(Deque_##T##_Iterator* it){ \
	return it->arr[(it->idx + it->beginIdx)%it->length]; \
} \
typedef struct Deque_##T { \
	int length; \
	int frontIdx; \
	int backIdx; \
	int maxSize; \
	char type_name[strlen("Deque_"#T)+1]; \
	bool (*compare)(const T &, const T &); \
	void (*dtor)(Deque_##T *); \
	void (*clear)(Deque_##T *); \
	T* arr;	 \
	std::size_t (*size)(Deque_##T *); \
	bool (*empty)(Deque_##T *); \
	Deque_##T##_Iterator (*begin)(Deque_##T *); \
	Deque_##T##_Iterator (*end)(Deque_##T *); \
	T& (*at)(Deque_##T *, int); \
	T& (*front)(Deque_##T *); \
	T& (*back)(Deque_##T *); \
	void (*push_back)(Deque_##T *, T); \
	void (*push_front)(Deque_##T *, T); \
	void (*pop_back)(Deque_##T *); \
	void (*pop_front)(Deque_##T *); \
	void (*sort)(Deque_##T *, Deque_##T##_Iterator, Deque_##T##_Iterator); \
} Deque_##T; \
std::size_t Deque_##T##_size(Deque_##T *dq){ \
	return dq->length; \
} \
void Deque_##T##_dtor(Deque_##T *dq){ \
	if(dq->arr != NULL) free(dq->arr); \
} \
void Deque_##T##_clear(Deque_##T *dq){ \
	dq->length = 0; \
	dq->maxSize = 0; \
} \
bool Deque_##T##_empty(Deque_##T *dq){ \
	return (dq->length == 0); \
} \
bool Deque_##T##_equal(Deque_##T dq1, Deque_##T dq2){ \
	if(strcmp(dq1.type_name, dq2.type_name) == 0 && dq1.length == dq2.length){ \
		for(int i = 0; i < dq1.length; i++){ \
			if(dq1.compare(dq1.arr[(dq1.frontIdx+i)%dq1.maxSize], dq2.arr[(dq2.frontIdx + i)%dq2.maxSize]) || dq1.compare(dq2.arr[(dq2.frontIdx + i)%dq2.maxSize], dq1.arr[(dq1.frontIdx+i)%dq1.maxSize])) return false; \
		} \
		return true; \
	} else { \
		return false; \
	} \
} \
void Deque_##T##_push_back(Deque_##T *dq, T val) { \
	if(dq->length == dq->maxSize) { \
		int oldSize = dq->maxSize; \
		dq->maxSize = dq->maxSize == 0 ? 32:2*dq->maxSize; \
		T* newArr = (T*) malloc(sizeof(T) * dq->maxSize); \
		for(int i = 0; i < dq->length; i++){ \
			newArr[i] = dq->arr[(dq->frontIdx + i)%oldSize]; \
		} \
		newArr[dq->length] = val; \
		if(dq->arr != NULL) free(dq->arr); \
		dq->arr = newArr; \
		dq->frontIdx = 0; \
		dq->backIdx = dq->length; \
	} else { \
		dq->backIdx = (dq->backIdx + 1)%dq->maxSize; \
		dq->arr[dq->backIdx] = val; \
	} \
	dq->length++; \
} \
void Deque_##T##_push_front(Deque_##T *dq, T val) { \
	if(dq->length == dq->maxSize) { \
		int oldSize = dq->maxSize; \
		dq->maxSize = dq->maxSize == 0 ? 32:2*dq->maxSize; \
		T* newArr = (T*) malloc(sizeof(T) * dq->maxSize); \
		for(int i = 0; i < dq->length; i++){ \
			newArr[i+1] = dq->arr[(dq->frontIdx + i)%oldSize]; \
		} \
		newArr[0] = val; \
		if(dq->arr != NULL) free(dq->arr); \
		dq->arr = newArr; \
		dq->frontIdx = 0; \
		dq->backIdx = dq->length; \
	} else { \
		dq->frontIdx = (dq->frontIdx-1 + dq->maxSize)%dq->maxSize; \
		dq->arr[dq->frontIdx] = val; \
	} \
	dq->length++; \
} \
T& Deque_##T##_back(Deque_##T *dq) { \
	if(dq->length != 0){ \
		return dq->arr[dq->backIdx]; \
	} else { \
		assert(false); \
	} \
} \
T& Deque_##T##_front(Deque_##T *dq) { \
	if(dq->length != 0){ \
		return dq->arr[dq->frontIdx]; \
	} else { \
		assert(false); \
	} \
} \
void Deque_##T##_pop_back(Deque_##T *dq){ \
	if(dq->length != 0){ \
		dq->backIdx = ((dq->backIdx - 1) + dq->maxSize)%dq->maxSize; \
		dq->length--; \
	} else { \
		assert(false); \
	} \
} \
void Deque_##T##_pop_front(Deque_##T *dq){ \
	if(dq->length != 0){ \
		dq->frontIdx = (dq->frontIdx + 1)%dq->maxSize; \
		dq->length--; \
	} else { \
		assert(false); \
	} \
} \
T& Deque_##T##_at(Deque_##T *dq, int idx){ \
	return dq->arr[(idx + dq->frontIdx + dq->maxSize)%dq->maxSize]; \
} \
int Deque_##T##_sort_compare(const void* a, const void* b, void* d){ \
	Deque_##T * dq = (Deque_##T *) d; \
	T i1 = *(T*) a; \
	T i2 = *(T*) b; \
	return dq->compare(i1, i2); \
} \
void Deque_##T##_sort(Deque_##T *dq, Deque_##T##_Iterator it1, Deque_##T##_Iterator it2){ \
	int num2sort = (it2.idx - it1.idx); \
	int startIdx = it1.idx; \
	int endIdx = it2.idx; \
	T* unsortedArr = (T*) malloc(sizeof(T) * num2sort); \
	for(int i = 0; i < num2sort; i++){ \
		unsortedArr[i] = it1.deref(&it1); \
		it1.inc(&it1); \
	} \
	qsort_r(unsortedArr, num2sort, sizeof(T), Deque_##T##_sort_compare, dq);\
	int j = num2sort-1; \
	for(Deque_##T##_Iterator it = dq->begin(dq); !Deque_##T##_Iterator_equal(it, dq->end(dq)); it.inc(&it)){ \
		if(it.idx >= startIdx && it.idx < endIdx) { \
			dq->arr[(it.idx + it.beginIdx)%it.length] = unsortedArr[j]; \
			j--; \
		} \
	} \
	free(unsortedArr); \
} \
Deque_##T##_Iterator Deque_##T##_begin(Deque_##T * dq){ \
	Deque_##T##_Iterator it; \
	it.arr = dq->arr; \
	it.idx = 0; \
	it.beginIdx = dq->frontIdx; \
	it.length = dq->maxSize; \
	it.inc = &Deque_##T##_Iterator_increment; \
	it.dec = &Deque_##T##_Iterator_decrement; \
	it.deref = &Deque_##T##_Iterator_dereference; \
	return it; \
} \
Deque_##T##_Iterator Deque_##T##_end(Deque_##T * dq){ \
	Deque_##T##_Iterator it; \
	it.arr = dq->arr; \
	it.idx = dq->length; \
	it.beginIdx = dq->frontIdx; \
	it.length = dq->maxSize; \
	it.inc = &Deque_##T##_Iterator_increment; \
	it.dec = &Deque_##T##_Iterator_decrement; \
	it.deref = &Deque_##T##_Iterator_dereference; \
	return it; \
} \
void Deque_##T##_ctor(Deque_##T *dq, bool (*cmp)(const T &, const T &)){ \
	strcpy(dq->type_name, "Deque_"#T); \
	dq->length = 0; \
	dq->frontIdx = 0; \
	dq->backIdx = 0; \
	dq->maxSize = 0; \
	dq->arr = (T*) malloc(sizeof(T) * dq->maxSize); \
	dq->compare = cmp; \
	dq->size = &Deque_##T##_size; \
	dq->empty = &Deque_##T##_empty; \
	dq->push_front = &Deque_##T##_push_front; \
	dq->push_back = &Deque_##T##_push_back; \
	dq->back = &Deque_##T##_back; \
	dq->front = &Deque_##T##_front; \
	dq->pop_back = &Deque_##T##_pop_back; \
	dq->pop_front = &Deque_##T##_pop_front; \
	dq->begin = &Deque_##T##_begin; \
	dq->end = &Deque_##T##_end; \
	dq->dtor = &Deque_##T##_dtor; \
	dq->clear = &Deque_##T##_clear; \
	dq->at = &Deque_##T##_at; \
	dq->sort = &Deque_##T##_sort; \
} 
#endif
