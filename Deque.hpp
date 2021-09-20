#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Deque_int_Iterator {
	void (*inc)(Deque_int_Iterator*);
	void (*dec)(Deque_int_Iterator*);
	int (*deref)(Deque_int_Iterator*);
} Deque_int_Iterator;

bool Deque_int_Iterator_equal(Deque_int_Iterator it1, Deque_int_Iterator it2){
	assert(false);
}

typedef struct Deque_int {
	bool (*ctor)(int, int);
	bool (*dtor)(Deque_int*);
	void (*clear)(Deque_int*);
	
	char* type_name;
	int ** arr;	
	std::size_t (*size)(Deque_int*);
	bool (*empty)(Deque_int*);

	Deque_int_Iterator (*begin)(Deque_int*);
	Deque_int_Iterator (*end)(Deque_int*);

	int (*at)(Deque_int*, int);
	int (*front)(Deque_int*);
	int (*back)(Deque_int*);
	void (*push_back)(Deque_int*, int);
	void (*push_front)(Deque_int*, int);
	void (*pop_back)(Deque_int*);
	void (*pop_front)(Deque_int*);

	void (*sort)(Deque_int*, Deque_int_Iterator it1, Deque_int_Iterator it2);
} Deque_int;

void Deque_int_ctor(Deque_int *dq, bool (*ctor)(const int &, const int &)){
	assert(false);
}

bool Deque_int_equal(Deque_int dq1, Deque_int dq2){
	assert(false);
}

#endif
