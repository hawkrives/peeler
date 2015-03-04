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
vector<u_int64_t> areAnagramsPass;
vector<u_int64_t> areAnagramsFail;
vector<u_int64_t> hashTimes;
double findAnagramsCycles;
double dictionaryCheckCycles;

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

	Dictionary(string &filename);  // constructor

	bool inWordArray(string &s);   // single query
	void check(string &filename);  // multiple queries

	long countAnagrams();
	vector<string> findAnagrams(string &s);
};

void getWords(string &filename, unordered_map<int, vector<string> > &m) {
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

Dictionary::Dictionary(string &filename) {
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

void Dictionary::check(string &filename) {
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

	dictionaryCheckCycles = elapsed_time();
}

bool areAnagrams(string &s1, string &s2, bool s1_sorted=false, bool s2_sorted=false) {
	u_int64_t start = get_timer();
	if (s1.size() != s2.size()) {
		areAnagramsFail.push_back(get_timer() - start);
		return false;
	}

	string s1_sort = s1;
	if (!s1_sorted) {
		sort(s1_sort.begin(), s1_sort.end());
	}

	string s2_sort = s2;
	if (!s2_sorted) {
		sort(s2_sort.begin(), s2_sort.end());
	}

	for (int i = 0; i < s1_sort.size(); i++) {
		char ch1 = s1_sort[i];
		char ch2 = s2_sort[i];
		if (ch1 != ch2) {
			areAnagramsFail.push_back(get_timer() - start);
			return false;
		}
	}
	areAnagramsPass.push_back(get_timer() - start);
	return true;
}

vector<string> Dictionary::findAnagrams(string &s) {
	cerr << "looking for anagrams of " << s << endl;

	start_timer();  // from elapsed_time.h

	int hash = hashString(s);
	string sorted = s;
	sort(sorted.begin(), sorted.end());
	vector<string> matches;

	try {
		vector<string> &possibilities = hashTable.at(hash);
		int size = possibilities.size();

		for (size_t i = 0; i < size; i++) {
			if (areAnagrams(sorted, possibilities[i], true)) {
				matches.push_back(possibilities[i]);
			}
		}
	} catch (...) {}

	findAnagramsCycles = elapsed_time();

	return matches;
}

long Dictionary::countAnagrams() {
	long count = 0;

	for (auto hash : hashTable) {
		vector<string> &possibilities = hash.second;
		if (!possibilities.size()) {
			continue;
		}

		string& test = possibilities.at(0);
		auto iter = possibilities.begin() + 1;

		for (; iter != possibilities.end(); iter++) {
			if (areAnagrams(test, *iter)) {
				cout << test << " <-> " << *iter << endl;
				count++;
			}
		}
	}

	return count;
}

unordered_map<string, string> get_args(int argc, char **argv) {
	string dictionaryFile = argv[1];
	string inputFile = argv[2];
	unordered_map<string, string> args = {
		{"dictionaryFile", dictionaryFile}
	};

	for (int i = 0; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-w" && i < argc - 1) {
			args.emplace("-w", argv[i+1]);
			inputFile = "";
			i++;
			continue;
		}
		else if (arg == "-f" && i < argc - 1) {
			inputFile = argv[i+1];
			i++;
			continue;
		}
	}

	args.emplace("inputFile", inputFile);

	return args;
}

int main(int argc, char **argv) {
	if ( argc < 3 ) {
		cerr << "Usage: peeler dictionaryFile [[-f] inputFile] [-w anagramWord]" << endl;
		exit(0);
	}

	unordered_map<string, string> args = get_args(argc, argv);

	Dictionary d(args.at("dictionaryFile"));

	auto word = args.find("-w");
	auto inputFile = args.find("inputFile");
	if (word != args.end()) {
		// d.countAnagrams();
		// d.findAnagrams(word->second);
	}
	else if (inputFile != args.end()) {
		d.check(inputFile->second);
	}
	else {
		cerr << "Usage: peeler dictionaryFile [inputFile] [-w anagramWord]" << endl;
		exit(0);
	}

	long count = d.countAnagrams();
	cout << count << " anagrams" << endl;

	/*
	cout << endl << "stats:" << endl;

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

	// merge the hit times and the miss times into another vector
	std::sort(areAnagramsPass.begin(), areAnagramsPass.end());
	std::sort(areAnagramsFail.begin(), areAnagramsFail.end());
	vector<u_int64_t> areAnagramsTimes;
	merge(
		areAnagramsPass.begin(), areAnagramsPass.end(),
		areAnagramsFail.begin(), areAnagramsFail.end(),
		back_inserter(areAnagramsTimes));

	cout << endl;
	cout << "  [areAnagrams] average cycles overall: "  << average(areAnagramsTimes)  << endl;
	cout << "  [areAnagrams] total hits: "              << areAnagramsPass.size()     << endl;
	cout << "  [areAnagrams] hit cycles: "              << sum(areAnagramsPass)       << endl;
	cout << "  [areAnagrams] average cycles per hit: "  << average(areAnagramsPass)   << endl;
	cout << "  [areAnagrams] total misses: "            << areAnagramsFail.size()   << endl;
	cout << "  [areAnagrams] miss cycles: "             << sum(areAnagramsFail)     << endl;
	cout << "  [areAnagrams] average cycles per miss: " << average(areAnagramsFail) << endl;

	cout << endl;
	cout << "  [hashItem] average cycles: " << average(hashTimes)     << endl;

	cout << endl;
	cout << "  [findAnagrams] cycles: "     << findAnagramsCycles << endl;
	cout << "  [Dictionary::check] cycles " << dictionaryCheckCycles << endl;
	*/
}
