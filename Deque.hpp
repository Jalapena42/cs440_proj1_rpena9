#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Deque_int_Iterator {
	int* arr;
	int beginIdx;
	int idx;
	int length;

	void (*inc)(Deque_int_Iterator*);
	void (*dec)(Deque_int_Iterator*);
	int (*deref)(Deque_int_Iterator*);
} Deque_int_Iterator;

bool Deque_int_Iterator_equal(Deque_int_Iterator it1, Deque_int_Iterator it2){
	return (it1.arr == it2.arr && it1.idx == it2.idx);
}

void Deque_int_Iterator_increment(Deque_int_Iterator* it){
	it->idx++;
}

void Deque_int_Iterator_decrement(Deque_int_Iterator* it){
	it->idx--;
}

int Deque_int_Iterator_dereference(Deque_int_Iterator* it){
	return it->arr[(it->idx + it->beginIdx)%it->length];
}

typedef struct Deque_int {
	int length;
	int frontIdx;
	int backIdx;
	int maxSize;
	char type_name[strlen("Deque_int")+1];

	bool (*compare)(const int &, const int &);
	void (*dtor)(Deque_int*);
	void (*clear)(Deque_int*);
	
	int * arr;	
	std::size_t (*size)(Deque_int*);
	bool (*empty)(Deque_int*);

	Deque_int_Iterator (*begin)(Deque_int*);
	Deque_int_Iterator (*end)(Deque_int*);

	int &(*at)(Deque_int*, int);
	int (*front)(Deque_int*);
	int (*back)(Deque_int*);
	void (*push_back)(Deque_int*, int);
	void (*push_front)(Deque_int*, int);
	void (*pop_back)(Deque_int*);
	void (*pop_front)(Deque_int*);

	void (*sort)(Deque_int*, Deque_int_Iterator it1, Deque_int_Iterator it2);
} Deque_int;

std::size_t Deque_int_size(Deque_int *dq){
	return dq->length;
}

void Deque_int_dtor(Deque_int *dq){
	if(dq->arr != NULL) free(dq->arr);
}

void Deque_int_clear(Deque_int *dq){
	dq->length = 0;
	dq->maxSize = 0;
}

bool Deque_int_empty(Deque_int *dq){
	return (dq->length == 0);
}

bool Deque_int_equal(Deque_int dq1, Deque_int dq2){
	if(dq1.length == dq2.length){
		for(int i = 0; i < dq1.length; i++){
			if(dq1.compare(dq1.arr[(dq1.frontIdx+i)%dq1.maxSize], dq2.arr[(dq2.frontIdx + i)%dq2.maxSize]) || dq1.compare(dq2.arr[(dq2.frontIdx + i)%dq2.maxSize], dq1.arr[(dq1.frontIdx+i)%dq1.maxSize])) return false;
		}
		return true;
	} else {
		return false;
	}
}

void Deque_int_push_back(Deque_int *dq, int val) {
	if(dq->length == dq->maxSize) {
		int oldSize = dq->maxSize;
		dq->maxSize = dq->maxSize == 0 ? 32:2*dq->maxSize;
		int* newArr = (int*) malloc(sizeof(int) * dq->maxSize);
		// Fill new array with values from old array, starting from frontIdx
		for(int i = 0; i < dq->length; i++){
			newArr[i] = dq->arr[(dq->frontIdx + i)%oldSize];
		}
		newArr[dq->length] = val;
		// Free the old array so it can be replaced
		if(dq->arr != NULL) free(dq->arr);
		dq->arr = newArr;
		// Set new indices
		dq->frontIdx = 0;
		dq->backIdx = dq->length;
	} else {
		dq->backIdx = (dq->backIdx + 1)%dq->maxSize;
		dq->arr[dq->backIdx] = val;
	}
	dq->length++;
}

void Deque_int_push_front(Deque_int *dq, int val) {
	if(dq->length == dq->maxSize) {
		int oldSize = dq->maxSize;
		dq->maxSize = dq->maxSize == 0 ? 32:2*dq->maxSize;
		int* newArr = (int*) malloc(sizeof(int) * dq->maxSize);
		// Fill new array with values from old array, starting from frontIdx, leaving space for front insert
		for(int i = 0; i < dq->length; i++){
			newArr[i+1] = dq->arr[(dq->frontIdx + i)%oldSize];
		}
		newArr[0] = val;
		// Free the old array so it can be replaced
		if(dq->arr != NULL) free(dq->arr);
		dq->arr = newArr;
		// Set new indices
		dq->frontIdx = 0;
		dq->backIdx = dq->length;
	} else {
		dq->frontIdx = (dq->frontIdx-1 + dq->maxSize)%dq->maxSize;
		dq->arr[dq->frontIdx] = val;
	}
	dq->length++;
}

int Deque_int_back(Deque_int *dq) {
	if(dq->length != 0){
		return dq->arr[dq->backIdx];
	} else {
		assert(false);
	}
}

int Deque_int_front(Deque_int *dq) {
	if(dq->length != 0){
		return dq->arr[dq->frontIdx];
	} else {
		assert(false);
	}
}

void Deque_int_pop_back(Deque_int *dq){
	if(dq->length != 0){
		dq->backIdx = ((dq->backIdx - 1) + dq->maxSize)%dq->maxSize;
		dq->length--;
	} else {
		assert(false);
	}
}

void Deque_int_pop_front(Deque_int *dq){
	if(dq->length != 0){
		dq->frontIdx = (dq->frontIdx + 1)%dq->maxSize;
		dq->length--;
	} else {
		assert(false);
	}
}

int& Deque_int_at(Deque_int *dq, int idx){
	return dq->arr[(idx + dq->frontIdx + dq->maxSize)%dq->maxSize];
}

Deque_int_Iterator Deque_int_begin(Deque_int* dq){
	Deque_int_Iterator it;
	it.arr = dq->arr;
	it.idx = 0;
	it.beginIdx = dq->frontIdx;
	it.length = dq->maxSize;
	it.inc = &Deque_int_Iterator_increment;
	it.dec = &Deque_int_Iterator_decrement;
	it.deref = &Deque_int_Iterator_dereference;
	return it;
}

Deque_int_Iterator Deque_int_end(Deque_int* dq){
	Deque_int_Iterator it;
	it.arr = dq->arr;
	it.idx = dq->length;
	it.beginIdx = dq->frontIdx;
	it.length = dq->maxSize;
	it.inc = &Deque_int_Iterator_increment;
	it.dec = &Deque_int_Iterator_decrement;
	it.deref = &Deque_int_Iterator_dereference;
	return it;
}

void Deque_int_ctor(Deque_int *dq, bool (*cmp)(const int &, const int &)){
	strcpy(dq->type_name, "Deque_int");
	dq->length = 0;
	dq->frontIdx = 0;
	dq->backIdx = 0;
	dq->maxSize = 0;
	dq->arr = (int*) malloc(sizeof(int) * dq->maxSize);
	dq->compare = cmp;
	dq->size = &Deque_int_size;
	dq->empty = &Deque_int_empty;
	dq->push_front = &Deque_int_push_front;
	dq->push_back = &Deque_int_push_back;
	dq->back = &Deque_int_back;
	dq->front = &Deque_int_front;
	dq->pop_back = &Deque_int_pop_back;
	dq->pop_front = &Deque_int_pop_front;
	dq->begin = &Deque_int_begin;
	dq->end = &Deque_int_end;
	dq->dtor = &Deque_int_dtor;
	dq->clear = &Deque_int_clear;
	dq->at = &Deque_int_at;
}

#endif
