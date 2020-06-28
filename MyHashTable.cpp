/*
Ignacio Osorio W.
iosorio@udec.cl

Implementation of Hash Table.
	- Separate Chaining
	- Open Adress
		- Linear Probing
		- Double Hashing
*/

#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#define DEFAULT_CONTAINER_SIZE 7
#define RESIZE_FACTOR 1.6
#define DEFAULT_MAX_LOAD_FACTOR_OA 0.6
#define DEFAULT_MAX_LOAD_FACTOR_SC 0.9

#include <cstddef>
#include <list>
#include <vector>
#include <utility>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <typeinfo>
#include <string>

#include "MyDoubleLinkedList.cpp"

using namespace std;

// Easy function to check if number n is prime
bool is_it_prime(unsigned int n) {
	int d = 3;

	if (n%2==0)
		return false;

	while(d<=n/d) {
		if (n%d==0) return false;
			
		d += 2;
	}

	return true;
}

// Very bad encoding function
unsigned long long encode(string &s) {
		unsigned long long encoded = 0;

		for (auto &c : s)
			encoded = int(c)+256*encoded;

		return encoded;
	}

// template <class K>
// unsigned int hash_function(K k, unsigned i=0) {
	
// }

// template <>
// unsigned int hash_function<string>(string k, unsigned i=0) {
	
// }


// =======================================================
// Hash table with separate chaining
template<class K, class T>
class MyHashTable_SC {
private:
	unsigned long long (*encoder)(string&);
	unsigned int _capacity;
	unsigned int _size;
	float _max_load_factor;
	MyDoubleLinkedList<pair<K,T>> *buckets;

	// Hashing parameters
	unsigned int h;


	void set_default_container() {
		buckets = new MyDoubleLinkedList<pair<K,T>>[DEFAULT_CONTAINER_SIZE];
		_capacity = DEFAULT_CONTAINER_SIZE;
		h = new_hash(_capacity);
	}

	unsigned int next_prime(unsigned int start_point) {
		if (start_point%2 == 0) start_point += 1;

		while (!is_it_prime(start_point)) {
			start_point += 2;
		}

		return start_point;
	}

	unsigned int hash(string &k) {
		return (encoder(k)*h)%_capacity;
	}

	unsigned int hash(char &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(unsigned char &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(short &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(unsigned short &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(int &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(unsigned int &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(long &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(unsigned long &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(long long &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(unsigned long long &k) {
		return (k*h)%_capacity;
	}

	unsigned int hash(float &k) {
		return unsigned (k*h)%_capacity;
	}

	unsigned int hash(double &k) {
		return unsigned (k*h)%_capacity;
	}

	unsigned int hash(long double &k) {
		return unsigned(k*h)%_capacity;
	}

	unsigned int new_hash(unsigned int start_point) {
		return next_prime(start_point+rand()%1000);
	}

public:
	MyHashTable_SC() {
		// For a bit of randomness
		srand(time(0));
		// if (typeid(K) == typeid(string)) hash = &MyHashTable_SC<K,T>::string_hash;
		// else hash = &MyHashTable_SC<K,T>::normal_hash;
		encoder = &encode;

		_size = 0;
		_max_load_factor = DEFAULT_MAX_LOAD_FACTOR_SC;
		set_default_container();
	}

	MyHashTable_SC(unsigned long long (*e)(string&)) {
		// For a bit of randomness
		srand(time(0));
		// if (typeid(K) == typeid(string)) hash = &MyHashTable_SC<K,T>::string_hash;
		// else hash = &MyHashTable_SC<K,T>::normal_hash;
		encoder = e;

		_size = 0;
		_max_load_factor = DEFAULT_MAX_LOAD_FACTOR_SC;
		set_default_container();
	}

	~MyHashTable_SC() {
		clear();
	}

	void clear() {
		for (int i=0; i<_capacity; i++)
			buckets[i].clear();

		delete[] buckets;

		_size = 0;

		set_default_container();
	}

	MyDoubleLinkedList<pair<K,T>>* end() {
		return buckets+_capacity;
	}

	unsigned int bucket_count() {
		return _capacity;
	}

	unsigned int size() {
		return _size;
	}

	unsigned int bucket_size(unsigned int i) {
		return buckets[i].size();
	}

	// ****************** use hash function to return key hashed
	unsigned int bucket(K k) {
		return hash(k);
	}

	float load_factor() {
		return ((float) _size)/_capacity;
	}

	float max_load_factor() {
		return _max_load_factor;
	}

	void max_load_factor(float lf) {
		_max_load_factor = lf;
	}

	void rehash(unsigned int new_capacity) {
		if (((float) _size)/new_capacity > _max_load_factor) {
			cout << "Did not rehash, because it violates the maximum load factor policy." << endl;
			return;
		}

		int old_capacity = _capacity;
		h = new_hash(new_capacity);
		_capacity = new_capacity;

		MyDoubleLinkedList<pair<K,T>> *tmp = new MyDoubleLinkedList<pair<K,T>>[new_capacity];

		// Rehash every single value saved and clean old data
		for (int i=0; i<old_capacity; i++) {
			for (auto it=buckets[i].begin(); it!=buckets[i].end(); it++)
				tmp[hash(it->first)].push_back(*it);
			
			buckets[i].clear();
		}
		delete[] buckets;

		buckets = tmp;
	}

	// Insertion
	void insert(pair<K,T> new_value) {
		unsigned int b = bucket(new_value.first); 

		auto it=buckets[b].begin();

		// No repeated keys
		for (auto it=buckets[b].begin(); it!=buckets[b].end(); it++)
			if (it->first == new_value.first) return;

		buckets[b].push_back(new_value);
		_size++;

		if (_max_load_factor<load_factor()) rehash(next_prime(RESIZE_FACTOR*_capacity));
	}

	// Deletion
	void erase(K &k) {
		unsigned int b = hash(k);

		for (auto it=buckets[b].begin(); it!=buckets[b].end(); it++)
			if (it->first==k) {
				buckets[b].erase(it);
				_size--;
			}
	}

	MyDoubleLinkedList<pair<K,T>>* find(K &k) {
		unsigned int b = hash(k);

		for (auto &it : buckets[b])
			if (it.first == k) return buckets+b;

		return buckets+_capacity;
	}

	T &operator[] (K k) {
		unsigned int b = hash(k);

		for (auto &it : buckets[b])
			if (it.first == k) return it.second;

		buckets[b].push_back(make_pair(k, T()));
		_size++;

		if (_max_load_factor<load_factor()) { 
			rehash(next_prime(RESIZE_FACTOR*_capacity));

			// If rehash, it needs to find the created value on the new hash table
			b = hash(k);
			auto it = buckets[b].begin();
			while (it->first != k) it++;
			return it->second;
		}
		else return buckets[b].rbegin()->second;
	}

	vector<unsigned> get_clusters() {
		vector<unsigned> clusters;

		for (int i=0; i<_capacity; i++)
			if (buckets[i].size()) clusters.push_back(buckets[i].size());

		return clusters;
	}

	unsigned long long size_of() {
		unsigned long long mem_size = 0;

		mem_size += sizeof(*this);
		mem_size += sizeof(typename MyDoubleLinkedList<pair<K,T>>::Node)*_size;
		mem_size += sizeof(MyDoubleLinkedList<pair<K,T>>)*_capacity;

		return mem_size;
	}

	// For iteration and return purpouse
	// class iterator {
	// public:
	// 	list<pair<K,T>> *list_container;
	// 	typename list<pair<K,T>>::iterator it;

	// 	iterator() {
	// 		list_container = nullptr;
	// 		it = nullptr;
	// 	}

	// 	iterator(list<pair<K,T>>& l, list<pair<K,T>> &i) {
	// 		list_container = &l;
	// 		it = i;
	// 	}

	// 	// prefix operator ++
	// 	// iterator operator++ () {
	// 	// 	if (it->r_child != NULL) {
	// 	// 		it = it->r_child->lower_bound();
	// 	// 		return *this;	
	// 	// 	}
	// 	// 	else {
	// 	// 		Node *parent = it->parent;
	// 	// 		while (parent!=NULL) {
	// 	// 			// If its right child
	// 	// 			if (parent->r_child == it) {
	// 	// 				it = parent;
	// 	// 				parent = parent->parent;
	// 	// 			}
	// 	// 			// If its left child
	// 	// 			else {
	// 	// 				it = parent;
	// 	// 				return *this;
	// 	// 			}
	// 	// 		}

	// 	// 		it = NULL;
	// 	// 		return *this;
	// 	// 	} 
	// 	// }

	// 	// postfix operator ++
	// 	// iterator operator++ ( int ) {
	// 	// 	iterator r(it);
	// 	// 		if (it->r_child != NULL) {
	// 	// 		it = it->r_child->lower_bound();
	// 	// 		return r;	
	// 	// 	}
	// 	// 	else {
	// 	// 		Node *parent = it->parent;
	// 	// 		while (parent!=NULL) {
	// 	// 			// If its right child
	// 	// 			if (parent->r_child == it) {
	// 	// 				it = parent;
	// 	// 				parent = parent->parent;
	// 	// 			}
	// 	// 			// If its left child
	// 	// 			else {
	// 	// 				it = parent;
	// 	// 				return r;
	// 	// 			}
	// 	// 		}

	// 	// 		it = NULL;
	// 	// 		return r;
	// 	// 	} 
	// 	// }

	// 	T& operator*() {
	// 		return it->second;
	// 	}

	// 	bool operator!= (const iterator &b) {
	// 		return it!=b.it;
	// 	}

	// 	bool operator== (const iterator &b) {
	// 		return it==b.it;
	// 	}
	// };

	// iterator end() {
	// 	return iterator(NULL);
	// }

	// iterator begin() {
	// 	if (_size==0) return iterator(NULL);
		
	// 	for (auto &l : buckets)
	// 		if (l.size()) return iterator(l.begin());
	// }
};


template<class K, class V>
class MyHashTable_OA {
private:
	enum Mark {ACTIVE, EMPTY, DELETED};

	unsigned long long (*encoder)(string&);	
	unsigned int _capacity;
	unsigned int _size;
	float _max_load_factor;
	pair<K,V> *buckets;
	Mark *states;

	unsigned _collisions;

	unsigned int (MyHashTable_OA<K,V>::* hash)(K&, unsigned int);

	// Hashing parameters
	unsigned int h, h2;


	void set_default_container() {
		states = new Mark[DEFAULT_CONTAINER_SIZE];
		buckets = new pair<K,V>[DEFAULT_CONTAINER_SIZE];
		_capacity = DEFAULT_CONTAINER_SIZE;
		h = new_hash(_capacity);
		h2 = new_hash(h+1);
		_collisions = 0;

		for (int i=0; i<DEFAULT_CONTAINER_SIZE; i++)
			states[i] = EMPTY;
	}

	unsigned int next_prime(unsigned int start_point) {
		if (start_point%2 == 0) start_point += 1;

		while (!is_it_prime(start_point)) {
			start_point += 2;
		}

		return start_point;
	}

	// Linear Probing
	unsigned int hash_LP(string &k, unsigned i) {
		return (encoder(k)*h+i)%_capacity;
	}

	unsigned int hash_LP(char &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(unsigned char &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(short &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(unsigned short &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(int &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(unsigned int &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(long &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(unsigned long &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(long long &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(unsigned long long &k, unsigned i) {
		return (k*h+i)%_capacity;
	}

	unsigned int hash_LP(float &k, unsigned i) {
		return unsigned (k*h+i)%_capacity;
	}

	unsigned int hash_LP(double &k, unsigned i) {
		return unsigned (k*h+i)%_capacity;
	}

	unsigned int hash_LP(long double &k, unsigned i) {
		return unsigned(k*h+i)%_capacity;
	}


	// Double Hashing
	unsigned int hash_DH(string &k, unsigned i) {
		unsigned long long encoded = encoder(k);
		return ((encoded*h)%_capacity+i*((encoded*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(char &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(unsigned char &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(short &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(unsigned short &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(int &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(unsigned int &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(long &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(unsigned long &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(long long &k, unsigned i) {
		return unsigned ((k*h)%_capacity+i*((k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(unsigned long long &k, unsigned i) {
		unsigned int h1_k = (k*h)%_capacity;
		unsigned int h2_k = ((k*h2)%(_capacity-1)+1);
		// cout << "h1: " << h1_k << "\th2: " << h2_k << endl;
		return unsigned (h1_k+i*h2_k)%_capacity;
	}

	unsigned int hash_DH(float &k, unsigned i) {
		return (unsigned(k*h)%_capacity+i*(unsigned(k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(double &k, unsigned i) {
		return (unsigned(k*h)%_capacity+i*(unsigned(k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int hash_DH(long double &k, unsigned i) {
		return (unsigned(k*h)%_capacity+i*(unsigned(k*h2)%(_capacity-1)+1))%_capacity;
	}

	unsigned int new_hash(unsigned int start_point) {
		return next_prime(start_point+rand()%1000);
	}

	// unsigned int linear_probing() {

	// }

	// unsigned int double_hashing() {

	// }
	
	// unsigned int cuadratic_probing() {
		
	// }

public:
	MyHashTable_OA(bool linear_probing = true) {
		// For randomness
		srand(time(0));
		if (linear_probing)
			hash = MyHashTable_OA<K,V>::hash_LP;
		else
			hash = MyHashTable_OA<K,V>::hash_DH;
		encoder = &encode;

		_size = 0;
		_max_load_factor = DEFAULT_MAX_LOAD_FACTOR_OA;
		set_default_container();
	}

	MyHashTable_OA(unsigned long long (*e)(string&), bool linear_probing = true) {
		// For randomness
		srand(time(0));
		if (linear_probing)
			hash = MyHashTable_OA<K,V>::hash_LP;
		else
			hash = MyHashTable_OA<K,V>::hash_DH;
		encoder = e;

		_size = 0;
		_max_load_factor = DEFAULT_MAX_LOAD_FACTOR_OA;
		set_default_container();
	}
		
	~MyHashTable_OA() {
		clear();
	}

	void clear() {
		delete[] buckets;

		_size = 0;

		set_default_container();
	}

	unsigned int bucket_count() {
		return _capacity;
	}

	unsigned int size() {
		return _size;
	}

	float load_factor() {
		return float(_size)/_capacity;
	}

	float max_load_factor() {
		return _max_load_factor;
	}

	unsigned collisions() {
		return _collisions;
	}

	bool is_bucket_empty(unsigned i) {
		return states[i] == EMPTY;
	}

	pair<K,V>* end() {
		return buckets+_capacity;
	}

	vector<unsigned> get_clusters() {
		vector<unsigned> clusters;
		unsigned c = 0;

		for(int i=0; i<_capacity; i++) {
			if (!is_bucket_empty(i))
				c++;
			else if (c!=0) {
				clusters.push_back(c);
				c=0;
			}
		}

		if (c!=0) {
			if (!is_bucket_empty(0)) clusters[0] += c;
			else clusters.push_back(c);
		}

		return clusters;
	}

	void max_load_factor(float lf) {
		if (lf>=1) {
			cout << "Did not reash, load factor can not be equal or greater tan 1." << endl;
			return;
		}
		_max_load_factor = lf;
	}

	void rehash(unsigned int new_capacity) {
		if (((float) _size)/new_capacity > _max_load_factor) {
			cout << "Did not rehash, because it violates the maximum load factor policy." << endl;
			return;
		}

		int old_capacity = _capacity;
		h = new_hash(new_capacity);
		h2 = new_hash(h+1);
		_capacity = new_capacity;
		_collisions = 0;

		pair<K,V> *tmp_buckets = new pair<K,V>[new_capacity];
		Mark *tmp_states = new Mark[new_capacity];

		for (int i=0; i<new_capacity; i++)
			tmp_states[i] = EMPTY;

		// Rehash every single value saved and clean old data
		for (unsigned int i=0,j,b; i<old_capacity; i++)
			if (states[i] == ACTIVE) {
				b=(this->*hash)(buckets[i].first, 0);
				for(j=1; tmp_states[b]==ACTIVE; b=(this->*hash)(buckets[i].first, j++), _collisions++);
				tmp_states[b] = ACTIVE;
				tmp_buckets[b] = buckets[i];
			}
		delete[] buckets;
		delete[] states;

		buckets = tmp_buckets;
		states = tmp_states;
	}

	pair<K,V>* find(K &k) {
		unsigned int b = (this->*hash)(k,0);
		for (unsigned int i=1; states[b]!=EMPTY; b=(this->*hash)(k,i++)) {
			if (i>_capacity*2) cout << k << endl;
			if (buckets[b].first==k) return &buckets[b];
		}

		return buckets+_capacity;
	}

	// Insertion
	void insert(pair<K,V> new_value) {
		unsigned int b=(this->*hash)(new_value.first, 0), collisions_i=0;
		for(unsigned int i=1; states[b]==ACTIVE; b=(this->*hash)(new_value.first, i++), collisions_i++) {
			if (i>_capacity) cout << new_value.first << "\t" << b << "\t" << i << endl;
			// No repeated keys
			if (buckets[b].first==new_value.first) return;
		}

		states[b] = ACTIVE;
		buckets[b] = new_value;
		_size++;
		_collisions += collisions_i;

		if (_max_load_factor<load_factor()) rehash(next_prime(RESIZE_FACTOR*_capacity));
	}

	// Deletion
	void erase(K k) {
		unsigned int b = (this->*hash)(k, 0);
		for(unsigned int i=1; states[b]!=EMPTY; b=(this->*hash)(k, i++))
			if (buckets[b].first == k) break;

		if (states[b] == ACTIVE) {
			states[b] = DELETED;
			_size--;
		}
	}

	V &operator[] (K k) {
		unsigned int b = (this->*hash)(k, 0);
		for(unsigned int i=1; states[b]!=EMPTY; b=(this->*hash)(k, i++))
			if (buckets[b].first == k) return buckets[b].second;

		states[b] = ACTIVE;
		_size++;

		if (_max_load_factor<load_factor()) {
			rehash(next_prime(RESIZE_FACTOR*_capacity));

			// If rehash then, then it has to find the created node
			b = (this->*hash)(k, 0);
			for(unsigned int i=1; buckets[b].first == k; b=(this->*hash)(k, i++));
			return buckets[b].second;
		}
		else return buckets[b].second;
	}

	unsigned long long size_of() {
		unsigned long long mem_size = 0;

		mem_size += sizeof(*this);
		mem_size += sizeof(pair<K,V>)*_capacity;
		mem_size += sizeof(Mark)*_capacity;

		return mem_size;
	}
};

#endif

// #include <unordered_map>

// #include <cmath>

// pair<float,float> sd_and_avr(vector<unsigned> c) {
// 	float avr = 0;

// 	for(auto &i : c)
// 		avr += i;
// 	avr /= c.size();

// 	float sd = 0;

// 	for(auto &i : c)
// 		sd += (i-avr)*(i-avr);
// 	sd /= c.size();
// 	sd = sqrt(sd);

// 	return make_pair(sd, avr);
// }

// int main() {
// 	unordered_map<int,int> u_map;
// 	MyHashTable_SC<int,int> table;
// 	MyHashTable_OA<int,int> table_OA;

// 	int n=20;

// 	for (int i=0, k; i<n; i++) {
// 		k = rand()%100;
// 		u_map.insert(make_pair(k, k+200));
// 		table.insert(make_pair(k, k+200));
// 		table_OA.insert(make_pair(k, k+200));		
// 	}

// 	// for (auto it=u_map.begin(); it!=u_map.end(); it++)
// 	// 	cout << it->first << "\t" << it->second << endl;
// 	for (auto it=u_map.begin(); it!=u_map.end(); it++)
// 		cout << "key: " << it->first << "\t" << u_map[it->first] << "\t" << table[it->first] << "\t" << table_OA[it->first] << endl;

// 	vector<unsigned> clusters = table_OA.get_clusters();

// 	unsigned biggest = 0;
// 	for (auto &c : clusters)
// 		if (biggest<c) biggest = c;

// 	pair<float,float> sd_avr = sd_and_avr(clusters);

// 	cout << "Number of clusters: " << clusters.size() << endl;
// 	cout << "Avr size of clusters: " << sd_avr.second << endl;
// 	cout << "Standar deviation: " << sd_avr.first << endl;
// 	cout << "Biggest cluster: " << biggest << endl;


// 	return 0;
// }

// int main() {
// 	MyHashTable_SC<string, int> my_map;
// 	my_map["hola"] = 15;

// 	cout << my_map["hola"] << endl;
// 	return 0;
// }