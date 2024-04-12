#include <stdio.h>
#include <stdlib.h>

typedef struct ListElem {
    struct ListElem *next, *prev; // указатель на следующий и предыдущий элементы списка
    int val;
} ListElem;

class Queue {
public: 
    ListElem* head;
    ListElem* tail;
    int count;

public:
	Queue(){
	this->head = NULL;
	this->tail = NULL;
	this->count = 0;
	}

	~Queue(){
	while (this->count > 0)
	    this->pop();
	}

	void pushHead(int val) { // кладет элемент в очередь (в голову списка)
		ListElem* nw = (ListElem*)malloc(sizeof(ListElem));
		nw->next = NULL;
		nw->prev = NULL;
		nw->val = val;
		if (this->count == 0) {
		    this->head = nw;
		    this->tail = nw;
		} else {
		    nw->next = this->head;
		    this->head->prev = nw;
		    this->head = nw;
		}
		count++;
	}

	void pop(){ 
	        if (this->count > 0) {
	            ListElem* del = this->tail;
	            this->tail = this->tail->prev;
	            free(del);
	            count--;
	        }
	}

	bool isEmpty(){ 
        	return (count == 0) ? true : false;
	}

	int getHead(){ 
        	return this->head ? this->head->val : 0;
	}

	int getTail(){ 
        	return this->tail ? this->tail->val : 0;
	}
};
