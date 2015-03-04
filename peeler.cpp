#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
using namespace std;
#include "peeler-common.h"

vector<u_int64_t> inArrayHits;
vector<u_int64_t> inArrayMisses;
vector<u_int64_t> hashTimes;
double totalCycles;

int hashString(string &input) {
	u_int64_t start = get_timer();

	size_t len = input.size();
	size_t quadtrant = len / 4;

	if (len < 4) {
		return asciiify(input);
	}

	string str1 = input.substr(quadtrant*0, quadtrant);
	string str2 = input.substr(quadtrant*1, quadtrant);
	string str3 = input.substr(quadtrant*2, quadtrant);
	string str4 = input.substr(quadtrant*3, len);

	int hash1 = asciiify(str1) * seeds[0];
	int hash2 = asciiify(str2) * seeds[1];
	int hash3 = asciiify(str3) * seeds[2];
	int hash4 = asciiify(str4) * seeds[3];

	int hash_sum = hash1 + hash2 + hash3 + hash4;

	int result = hash_sum % nextPrime;

	hashTimes.push_back(get_timer() - start);

	return result;
}

struct Dictionary {
	unordered_map<int, vector<string> > hashTable;

	Dictionary(const char *filename);  // constructor

	bool inWordArray(string &s);       // single query
	void check(const char *filename);  // multiple queries
};

void getWords(const char *filename, unordered_map<int, vector<string> > &m) {
	ifstream f(filename);

	if ( !f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}

	string s;
	cerr << "reading/hashing file" << endl;

	while ( f >> s ) {
		int hash = hashString(s);

		try {
			vector<string> &current = m.at(hash);
			current.push_back(s);
		} catch (...) {
			m.emplace(hash, vector<string> {s});
		}
	}

	cerr << "done with file; " << m.size() << " items." << endl;
}

Dictionary::Dictionary( const char *filename ) {
	getWords(filename, hashTable);
}

bool Dictionary::inWordArray(string &s) {
	u_int64_t start = get_timer();

	int hash = hashString(s);

	try {
		vector<string> &possibilities = hashTable.at(hash);
		int size = possibilities.size();

		for (size_t i = 0; i < size; i++) {
			if (possibilities[i] == s) {
				inArrayHits.push_back(get_timer() - start);
				return true;
			}
		}
	} catch (...) {}

	inArrayMisses.push_back(get_timer() - start);

	return false;
}

void Dictionary::check( const char *filename ) {
	vector<string> query;
	getWords(filename, query);
	inArrayHits.reserve(query.size());
	inArrayMisses.reserve(query.size());

	cerr << "checking " << filename << endl;
	start_timer();  // from elapsed_time.h

	int counter = 0;
	int n = query.size();
	for (int i = 0; i < n; i++) {
		if ( !inWordArray(query[i]) ) {
			counter++;
		}
	}

	cerr << "Misspelled " << counter << " words." << endl;

	totalCycles = elapsed_time();
}

int main(int argc, char **argv) {
	if ( argc != 3 ) {
		cerr << "Usage: spellCheck dictionaryFile inputFile" << endl;
		exit(0);
	}
	Dictionary d(argv[1]);
	d.check(argv[2]);

	cout << endl << "stats:" << endl;

	cout << "  [peeler] total cycles " << totalCycles << endl;

	// c++ unsorted_map hashtable stats
	cout << endl;
	cout << "  [hashTable internal] avg items/bucket: " << d.hashTable.load_factor() << endl;
	cout << "  [hashTable internal] max items/bucket: " << d.hashTable.max_load_factor() << endl;
	cout << "  [hashTable internal] bucket count: " << d.hashTable.bucket_count() << endl;

	// our hashtable stats
	vector<long> hashSizes;
	hashSizes.reserve(d.hashTable.size());
	for (auto item : d.hashTable) hashSizes.push_back(item.second.size());

	auto minMaxSizes = minmax_element(hashSizes.begin(), hashSizes.end());
	long minItemsPer = *minMaxSizes.first;
	long maxItemsPer = *minMaxSizes.second;
	long itemsPerHash = average(hashSizes);
	long countForMinItemsPer = count(hashSizes.begin(), hashSizes.end(), minItemsPer);
	long countForMaxItemsPer = count(hashSizes.begin(), hashSizes.end(), maxItemsPer);

	cout << endl;
	cout << "  [hashTable] key count: " << d.hashTable.size() << endl;
	cout << "  [hashTable] avg items per hash: " << itemsPerHash << endl;
	cout << "  [hashTable] most items per hash: " << maxItemsPer << endl;
	cout << "  [hashTable] number of hashes of that size: " << countForMaxItemsPer << endl;
	cout << "  [hashTable] min items per hash: " << minItemsPer << endl;
	cout << "  [hashTable] number of hashes of that size: " << countForMinItemsPer << endl;

	// cycle counts

	// merge the hit times and the miss times into another vector
	std::sort(inArrayHits.begin(), inArrayHits.end());
	std::sort(inArrayMisses.begin(), inArrayMisses.end());
	vector<u_int64_t> inArrayTimes;
	merge(
		inArrayHits.begin(),   inArrayHits.end(),
		inArrayMisses.begin(), inArrayMisses.end(),
		back_inserter(inArrayTimes));

	cout << endl;
	cout << "  [inWordArray] average cycles overall: "  << average(inArrayTimes)  << endl;
	cout << "  [inWordArray] total hits: "              << inArrayHits.size()     << endl;
	cout << "  [inWordArray] hit cycles: "              << sum(inArrayHits)       << endl;
	cout << "  [inWordArray] average cycles per hit: "  << average(inArrayHits)   << endl;
	cout << "  [inWordArray] total misses: "            << inArrayMisses.size()   << endl;
	cout << "  [inWordArray] miss cycles: "             << sum(inArrayMisses)     << endl;
	cout << "  [inWordArray] average cycles per miss: " << average(inArrayMisses) << endl;

	cout << endl;
	cout << "  [hashItem] average cycles: "         << average(hashTimes)     << endl;
}
