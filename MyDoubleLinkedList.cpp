#ifndef MY_DOUBLE_LINKED_LIST
#define MY_DOUBLE_LINKED_LIST
#include <iostream>

template <class T> 
class MyDoubleLinkedList {
public:

	class Node {
	public:
		Node *next;
		Node *prev;

		T value;

		Node() {
			next = NULL;
			prev = NULL;
			value = 0;
		}

		Node(T val) {
			next = NULL;
			prev = NULL;

			value = val;
		}
	};

	class iterator {
	public:
		Node *it;

		iterator() {
			it = NULL;
		}

		iterator(Node *i) {
			it = i;
		}

		// prefix operator ++
		iterator operator++ () {
			it = it->next;
			return iterator(it);	
		}

		// postfix operator ++
		iterator operator++ ( int ) {
			iterator r(it);
			it=it->next;
			return r;	
		}

		// prefix operator --
		iterator operator-- () {
			it = it->prev;
			return iterator(it);
		}

		// postfix operator --
		iterator operator-- ( int ) {
			iterator r(it);
			it = it->prev;
			return r;
		}

		T& operator*() const {
			return it->value;
		}

		T* operator->() const {
			return &(it->value);
		}

		bool operator!= (const iterator &b) {
			return it!=b.it;
		}


	};

	size_t _size;

	Node *_head;
	Node *_tail;

	MyDoubleLinkedList() {
		_size = 0;
		_head = NULL;
		_tail = NULL;
	}

	void push_back(T value) {
		Node *n = new Node(value);

		if (_size++==0) {
			_head = n;
		}
		else {
			_tail->next = n;
			n->prev = _tail;
		}
		_tail = n;
	}

	void pop_back() {
		Node *t = _tail;
		if (_tail != NULL) _tail = _tail->prev;
		erase(iterator(t));
	}

	void pop_front() {
		Node *t = _head;
		if (_head != NULL) _head = _head->next;
		erase(iterator(t));
	}

	void erase(iterator it) {
		if (_size == 0)	return;
		else if (_size == 1) {
			_head = NULL;
			_tail = NULL;
		}

		if (it.it->prev != NULL) it.it->prev->next = it.it->next;
		if (it.it->next != NULL) it.it->next->prev = it.it->prev;

		_size--;

		delete(it.it);
	}

	void clear() {
		while (_size)
			pop_back();
	}

	T &front() {
		return _head->value;
	}

	T &back() {
		return _tail->value;
	}

	size_t size() {
		return _size;
	}

	bool empty() { return size==0; }

	iterator begin() {
		return iterator(_head);
	}

	iterator end() {
		return iterator(NULL);
	}

	iterator rbegin() {
		return iterator(_tail);
	}
};

// using namespace std;

// int main() {
// 	MyDoubleLinkedList<int> l;
// 	int stoper = 0;

// 	for (int i=0; i<25; i++)
// 		l.push_back(i);

// 	cout << "size: " << l.size() << endl;


// 	// for(MyDoubleLinkedList<int>::iterator it=l.begin(); it!=l.end(); ++it)
// 	// 	cout << *it << endl;

// 	// l.pop_front();
// 	l.clear();

// 	cout << "size:" << l.size() << endl;

// 	// for(auto it=l.begin(); it!=l.end(); it++) {
// 	// 	// if (stoper++==l.size()+3) break;
// 	// 	cout << *it << endl;
// 	// }

// 	return 0;
// }

#endif