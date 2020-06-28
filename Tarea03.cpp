/*
-11807, 66, 12720
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define EXCEL_FILE "universities_followers.csv"

using namespace std;

struct TwitterAccount {
/*

    University: Universidad a la que sigue el usuario en la red social.
    User ID: Número identificador del usuario en Twitter. Este campo es único.
    User Name: Nombre de usuario en Twitter. Este campo es único.
    Number Tweets: Número de tweets publicados por el usuario.
    Friends Count: Número de cuentas que el usuario sigue en la red social.
    Followers Count: Número de seguidores que tiene el usuario en la red social.
    Created At: Fecha en la que el usuario creó su cuenta en la red social.

*/
string university;
unsigned long long user_id;
string user_name;
unsigned int tweets_n;
unsigned int friends_count;
unsigned int followers_count;
string created_at;
	
} typedef TwitterAccount;

// Create TwitterAccount
TwitterAccount TwitterAccount_FromString(string line) {
	TwitterAccount new_account;
	string tmp_s;
	long double tmp_d;

	istringstream csvStream(line);

	// university
	getline(csvStream,new_account.university ,',');

	// user id
	getline(csvStream,tmp_s,',');
	tmp_d = stold(tmp_s);
	new_account.user_id = tmp_d;

	// user name
	getline(csvStream,new_account.user_name,',');
		
	// Number tweets
	getline(csvStream,tmp_s,',');
	new_account.tweets_n = stoul(tmp_s);

	// Friends count
	getline(csvStream,tmp_s,',');
	new_account.friends_count = stoul(tmp_s);

	// Followers count
	getline(csvStream,tmp_s,',');
	new_account.followers_count = stoul(tmp_s);

	// Created at
	getline(csvStream,new_account.created_at);

	return new_account;
}

void print_TwitterAccount(TwitterAccount &account) {
	cout << account.university << endl << account.user_id << endl << account.user_name << endl << account.tweets_n << endl << account.friends_count << endl << account.followers_count << endl << account.created_at << endl;
}

vector<TwitterAccount> readFileToVector(string file_name) {
	ifstream file;
	string tmp;

	vector<TwitterAccount> twitter_accounts;

	file.open(file_name);
	getline(file, tmp);

	while(getline(file, tmp) && tmp.size()!=0)
		twitter_accounts.push_back(TwitterAccount_FromString(tmp));
	file.close();

	return twitter_accounts;
}


#include <map>
#include <unordered_map>
#include "MyHashTable.cpp"
#include <ctime>
#include <cstdlib>

#include "twitterStringEncoding.cpp"

#include <tuple>
#include <cmath>

tuple<float,float, unsigned> sd_avr_and_biggest(vector<unsigned> c) {
	unsigned biggest=0;
	float avr = 0;

	for(auto &i : c) {
		if (biggest<i) biggest = i;
		avr += i;
	}
	avr /= c.size();

	float sd = 0;

	for(auto &i : c)
		sd += (i-avr)*(i-avr);
	sd /= c.size();
	sd = sqrt(sd);

	return {sd, avr, biggest};
}

template <class T>
T random_key_under(T &max) {
	return rand()%max;
}

template <>
string random_key_under(string &max) {
	string k = "";

	for (int i=0, j; i<10; i++) {
		j = rand()%63;
		if (j<10) k += char('0'+j);
		else if (j<36) k += char('A'+j-10);
		else if (j==36) k += '_';
		else k += char('a'+j-37);
	}

	return k;
}

template <class T>
void testWithArbitraryKey(vector<TwitterAccount> data, T (*select_key)(TwitterAccount), int present_indexes, int not_present) {
	// Data test compared to balanced tree
	map<T, TwitterAccount> balanced_map;
	MyHashTable_SC<T, TwitterAccount> my_hash_table(twitter_string_encode);
	MyHashTable_OA<T, TwitterAccount> my_hash_table_oa(twitter_string_encode);
	MyHashTable_OA<T, TwitterAccount> my_hash_table_oa_dh(twitter_string_encode, false);
	unordered_map<T, TwitterAccount> hash_table;

	double map_t, mymap_t, myavlmap_t, hash_t, myhash_t, myhash_oa_t, myhash_oa_dh_t;
	clock_t t;

	cout << "Containers initialized" << endl;

	int l=0;
	unsigned long long intervalos[] = {1000, 5000, 10000, 15000 , 20000, data.size()+1};

	// Populate times!
	cout << "Tiempo de lectura de map:" << endl;
	t = clock();
	for (int i=0; i<data.size(); i++) {
		balanced_map.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}

	cout << "Tiempo de lectura de unordered_map:" << endl;
	t = clock();
	l=0;
	for (int i=0; i<data.size(); i++) {
		hash_table.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}

	

	

	cout << "Tiempo de lectura de SC hash:" << endl;
	t = clock();
	l=0;
	for (int i=0; i<data.size(); i++) {
		my_hash_table.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}

	cout << "Tiempo de lectura de OA hash linear:" << endl;
	t = clock();
	l=0;
	for (int i=0; i<data.size(); i++) {
		my_hash_table_oa.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}

	cout << "Tiempo de lectura de OA hash double:" << endl;
	t = clock();
	l=0;
	for (int i=0; i<data.size(); i++) {
		my_hash_table_oa_dh.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}

	cout << "Data inserted: " << balanced_map.size() << "\t" << hash_table.size();
	cout << "\t" << my_hash_table.size() << "\t" << my_hash_table_oa.size() << "\t" << my_hash_table_oa_dh.size() << endl << endl;
	///////////////////////////////////////////////////////////////////////////////////

	// Set random indexes for test
	vector<int> index(present_indexes);
	srand(time(0));
	for (auto &it :index)
		it = rand()%data.size();
	cout << "Indexes for data to be found CREATED!" << endl;

	// Set indexes not found
	srand(time(0));
	vector<T> index_not_present(not_present);
	T max = (*balanced_map.rbegin()).first;
	for (auto &it : index_not_present) {
		T k = random_key_under(max);
		while (hash_table.find(k) != hash_table.end())
			k = random_key_under(max);
		it = k;
	}
	cout << "Keys not presented CREATED!" << endl << endl;

	cout.precision(5);


	// Test find function

	vector<int> intervals;
	int count;
	for (int i=1000; i<present_indexes+1; i+=1000)
		intervals.push_back(i);

	cout << "intervals created: " << intervals.size() << endl << endl;


	// MY HASH TABLE SEPARATE CHAINING
	cout << "testing SC" << endl;
	t = clock();
	l=0, count =0;
	for (auto &it : index) {
		my_hash_table[select_key(data[it])];
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myhash_t = ((double)t)/CLOCKS_PER_SEC;

	// MY HASH TABLE OPEN ADRESS LINEAR PROBING
	cout << "testing LP" << endl;
	t = clock();
	l=0,count=0;
	for (auto &it : index) {
		my_hash_table_oa[select_key(data[it])];
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myhash_oa_t = ((double)t)/CLOCKS_PER_SEC;

	// MY HASH TABLE OPEN ADRESS DOUBLE HASHING
	cout << "testing DH" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index){
		my_hash_table_oa_dh[select_key(data[it])];
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myhash_oa_dh_t = ((double)t)/CLOCKS_PER_SEC;

	// MAP
	cout << "testing map" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index) {
		if (balanced_map.find(select_key(data[it])) == balanced_map.end())
			cout << "NOT FOUND" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	map_t = ((double)t)/CLOCKS_PER_SEC;

	// UNORDERED MAP
	cout << "testing unordered_map" << endl;
	t = clock();
	l=0, count=0;
	for (auto it : index){
		if (hash_table.find(select_key(data[it])) == hash_table.end())
			cout << "NOT FOUND" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	hash_t = ((double)t)/CLOCKS_PER_SEC;



	cout << "Find " << index.size() << " elements that are contained." << endl;
	cout << "In map: " << map_t << " seconds" << endl;
	cout << "In MyUnbalancedMap: " << mymap_t << " seconds" << endl;
	cout << "In MyAVLMap: " << myavlmap_t << " seconds" << endl;
	cout << "In unordered_map: " << hash_t << " seconds" << endl;
	cout << "In MyHashTable_SC: " << myhash_t << " seconds" << endl;
	cout << "In MyHashTable_OA: " << myhash_oa_t << " seconds" << endl;
	cout << "In MyHashTable_OA DH: " << myhash_oa_dh_t << " seconds" << endl << endl;


	///////////////////////////////////////////////////////////////////////
	/// Not found indexes





	// MY HASH TABLE SEPARATE CHAINING
	cout << "testing SC" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present){
		if (my_hash_table.find(it) != my_hash_table.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myhash_t = ((double)t)/CLOCKS_PER_SEC;

	// MY HASH TABLE OPEN ADRESS LINEAR PROBING
	cout << "testing LP" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present){
		if (my_hash_table_oa.find(it) != my_hash_table_oa.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myhash_oa_t = ((double)t)/CLOCKS_PER_SEC;

	// MY HASH TABLE OPEN ADRESS DOUBLE HASHING
	cout << "testing DH" << endl;
	t = clock();
	l=0, count=0;
	for (int i=0; i<index_not_present.size(); i++) {
		// cout << i << "\t" << index_not_present[i] << endl;
		if (my_hash_table_oa_dh.find(index_not_present[i]) != my_hash_table_oa_dh.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myhash_oa_dh_t = ((double)t)/CLOCKS_PER_SEC;

	// MAP
	cout << "testing map" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present) {
		if (balanced_map.find(it) != balanced_map.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	map_t = ((double)t)/CLOCKS_PER_SEC;

	// UNORDERED MAP
	cout << "testing unordered_map" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present) {
		if (hash_table.find(it) != hash_table.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	hash_t = ((double)t)/CLOCKS_PER_SEC;



	cout << "Find " << index.size() << " elements that are not contained." << endl;
	cout << "In map: " << map_t << " seconds" << endl;
	cout << "In MyUnbalancedMap: " << mymap_t << " seconds" << endl;
	cout << "In MyAVLMap: " << myavlmap_t << " seconds" << endl;
	cout << "In unordered_map: " << hash_t << " seconds" << endl;
	cout << "In MyHashTable_SC: " << myhash_t << " seconds" << endl;
	cout << "In MyHashTable_OA: " << myhash_oa_t << " seconds" << endl;
	cout << "In MyHashTable_OA DH: " << myhash_oa_dh_t << " seconds" << endl << endl;

	// Check unbalancedmap data
	for(auto it : index)
		if (balanced_map[select_key(data[it])].user_id != my_hash_table[select_key(data[it])].user_id || 
			balanced_map[select_key(data[it])].user_id != my_hash_table_oa[select_key(data[it])].user_id) 
			cout << "ERROR!" << endl;

	auto lp_statistics = sd_avr_and_biggest(my_hash_table_oa.get_clusters());
	auto dh_statistics = sd_avr_and_biggest(my_hash_table_oa_dh.get_clusters());
	auto sc_statistics = sd_avr_and_biggest(my_hash_table.get_clusters());

	auto temp = my_hash_table.get_clusters();
	int total_SC_collisions = -temp.size();
	for (auto &s : temp)
		total_SC_collisions += s;


	cout << "Load factor for hash tables: \nSTD HashTable: " << hash_table.load_factor() << endl;
	cout << "MyHashTable_SC: " << my_hash_table.load_factor() << endl;
	cout << "MyHashTable_OA: " << my_hash_table_oa.load_factor() << endl;
	cout << "MyHashTable_OA DH: " << my_hash_table_oa_dh.load_factor() << endl;
	cout << "MyHashTable_SC collisions: " << total_SC_collisions << endl;	
	cout << "MyHashTable_OA LP collisions: " << my_hash_table_oa.collisions() << endl;
	cout << "MyHashTable_OA DH collisions: " << my_hash_table_oa_dh.collisions() << endl << endl;

	cout << "Linear Probing:\n-avr: " << get<1>(lp_statistics) << endl << "-sd: ";
	cout << get<0>(lp_statistics) << endl << "-biggest: " << get<2>(lp_statistics) << endl << "[";
	// for(auto &it : my_hash_table_oa.get_clusters())
	// 	cout << it << ", ";
	cout << "]" << endl << endl;

	cout << "Double Hashing:\n-avr: " << get<1>(dh_statistics) << endl << "-sd: ";
	cout << get<0>(dh_statistics) << endl << "-biggest: " << get<2>(dh_statistics) << endl << "[";
	// for(auto &it : my_hash_table_oa_dh.get_clusters())
	// 	cout << it << ", ";
	cout << "]" << endl << endl;

	cout << "Separate Chaining:\n-avr: " << get<1>(sc_statistics) << endl << "-sd: ";
	cout << get<0>(sc_statistics) << endl << "-biggest: " << get<2>(sc_statistics) << endl << "[";
	// for(auto &it : my_hash_table.get_clusters())
	// 	cout << it << ", ";
	cout << "]" << endl << endl;

	cout << "Tamanio hash SC:" << my_hash_table.size_of() << endl;
	cout << "Tamanio hash LP:" << my_hash_table_oa.size_of() << endl;
	cout << "Tamanio hash DH:" << my_hash_table_oa_dh.size_of() << endl;
}

// KEY Accesser
unsigned long long id_user_key(TwitterAccount a) { return a.user_id; }
string user_name_key(TwitterAccount a) { return a.user_name; }

#include <list>
int main() {
	vector<TwitterAccount> data = readFileToVector(EXCEL_FILE);

	int inside_users_N = 400000;
	int not_inside_users_N = 400000;
	
	cout << "N of accounts: " << data.size() << endl;

	cout << endl << "================================================" << endl;
	cout << "USING USER ID: " << endl;
	testWithArbitraryKey(data, id_user_key, inside_users_N, not_inside_users_N);


	cout << endl << "================================================" << endl;
	cout << "USING USER NAME: " << endl;
	testWithArbitraryKey(data, user_name_key, inside_users_N, not_inside_users_N);

	// cout << sizeof(TwitterAccount) << endl;

	return 0;
}
