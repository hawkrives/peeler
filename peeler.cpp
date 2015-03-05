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

	string sorted = input;
	sort(sorted.begin(), sorted.end());

	int hash_sum =
		asciiify(sorted.substr(quadtrant*0, quadtrant)) * seeds[0] +
		asciiify(sorted.substr(quadtrant*1, quadtrant)) * seeds[1] +
		asciiify(sorted.substr(quadtrant*2, quadtrant)) * seeds[2] +
		asciiify(sorted.substr(quadtrant*3, len)) * seeds[3];

	int result = hash_sum % nextPrime;

	hashTimes.push_back(get_timer() - start);

	return result;
}

int hashStringLetter(string &input) {
	u_int64_t start = get_timer();

	size_t len = input.size();

	int result = 0;

	for (int i = 0; i < len; i++)
		result += letterHashes[ input[i] ];

	result %= nextPrime;

	hashTimes.push_back(get_timer() - start);

	return result;
}

void getWords(string &filename, unordered_map<int, vector<string> > &m) {
	ifstream f(filename);

	if ( !f.good() ) {
		cerr << "Error:  unable to open " << filename << endl;
		exit(-1);
	}

	string s;
	cerr << "reading/hashing file" << endl;

	while ( f >> s ) {
		int hash = hashStringLetter(s);

		try {
			vector<string> &current = m.at(hash);
			current.push_back(s);
		} catch (...) {
			m.emplace(hash, vector<string> {s});
		}
	}

	cerr << "done with file; " << m.size() << " hashes." << endl;
}

bool areAnagrams(string &s1, string &s2, bool s1_sorted=false, bool s2_sorted=false) {
	u_int64_t start = get_timer();

	// bool result = is_permutation(s1.begin(), s1.end(), s2.begin());
	// if (result) {
	// 	areAnagramsPass.push_back(get_timer() - start);
	// 	return true;
	// }

	// areAnagramsFail.push_back(get_timer() - start);
	// return false;


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

	// cout << s1_sort << " " << s2_sort << endl;

	if (s1_sort == s2_sort) {
		areAnagramsPass.push_back(get_timer() - start);
		return true;
	}

	areAnagramsFail.push_back(get_timer() - start);
	return false;
}

struct Dictionary {
	unordered_map<int, vector<string> > hashTable;

	Dictionary(string &filename) {
		getWords(filename, hashTable);
	}


	/////
	/// Spelling
	///

	bool inWordArray(string &s) {
		u_int64_t start = get_timer();

		int hash = hashStringLetter(s);

		try {
			vector<string> &possibilities = hashTable.at(hash);

			for (auto &word : possibilities) {
				if (word == s) {
					inArrayHits.push_back(get_timer() - start);
					return true;
				}
			}
		} catch (...) {}

		inArrayMisses.push_back(get_timer() - start);

		return false;
	}

	void check(string &filename) {
		vector<string> query;
		getWords(filename, query);
		inArrayHits.reserve(query.size());
		inArrayMisses.reserve(query.size());

		cerr << "checking " << filename << endl;
		start_timer();  // from elapsed_time.h

		int counter = 0;
		for (auto &item : query) {
			if ( !inWordArray(item) ) {
				counter++;
			}
		}

		cerr << "Misspelled " << counter << " words." << endl;

		dictionaryCheckCycles = elapsed_time();
	}


	/////
	/// Anagrams
	///

	vector<string> findAnagrams(string &s) {
		cerr << "looking for anagrams of " << s << endl;

		start_timer();  // from elapsed_time.h

		int hash = hashStringLetter(s);
		string sorted = s;
		sort(sorted.begin(), sorted.end());
		vector<string> matches;

		try {
			vector<string> &possibilities = hashTable.at(hash);

			for (auto &word : possibilities) {
				if (areAnagrams(sorted, word, true)) {
					matches.push_back(word);
				}
			}
		} catch (...) {}

		findAnagramsCycles = elapsed_time();

		return matches;
	}

	long countAnagrams(bool verbose=false) {
		map<string, vector<string> > reSorted;
		long count = 0;

		for (auto hash : hashTable) {
			vector<string> &possibilities = hash.second;
			for (auto &word : possibilities) {
				string sorted = word;
				sort(sorted.begin(), sorted.end());

				try {
					vector<string> &current = reSorted.at(sorted);
					current.push_back(word);
				} catch (...) {
					reSorted.emplace(sorted, vector<string> {word});
				}
			}
		}

		for (auto hash : reSorted) {
			vector<string> &possibilities = hash.second;

			if (possibilities.size() < 2) {
				continue;
			}

			for (auto &word : possibilities) {
				if (verbose)
					cout << word << " ";
			}
			cout << endl;
			count++;
		}

		return count;
	}
};

int main(int argc, char **argv) {
	clock_t tStart = clock();
	string usage = "Usage: peeler dictionaryFile [[-f] inputFile] [-w anagramWord]";

	if ( argc < 3 ) {
		cerr << usage << endl;
		exit(0);
	}

	srand(time(NULL));
	initLetterHashes();

	unordered_map<string, string> args = get_args(argc, argv);

	Dictionary d(args.at("dictionaryFile"));

	auto word = args.find("-w");
	auto countAnagrams = args.find("--count-anagrams");
	auto verbose = args.find("--verbose") != args.end();
	auto inputFile = args.find("inputFile");
	if (countAnagrams != args.end()) {
		cout << countAnagrams << " anagrams" << endl;
		long countAnagrams = d.countAnagrams(verbose);
	}
	else if (word != args.end()) {
		auto matches = d.findAnagrams(word->second);
		cerr << "anagrams: " << matches << endl;
	}
	else if (inputFile != args.end()) {
		d.check(inputFile->second);
	}
	else {
		cerr << usage << endl;
		exit(0);
	}

	double executionTime = (double)(clock() - tStart) / (double)CLOCKS_PER_SEC;

	cout << endl << "stats:";

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
	sort(inArrayHits.begin(), inArrayHits.end());
	sort(inArrayMisses.begin(), inArrayMisses.end());
	vector<u_int64_t> inArrayTimes;
	merge(
		inArrayHits.begin(),   inArrayHits.end(),
		inArrayMisses.begin(), inArrayMisses.end(),
		back_inserter(inArrayTimes));
	if (inArrayTimes.size()) {
		cout << endl;
		cout << "  [inWordArray] average cycles overall: "  << average(inArrayTimes)  << endl;
		cout << "  [inWordArray] total hits: "              << inArrayHits.size()     << endl;
		cout << "  [inWordArray] hit cycles: "              << sum(inArrayHits)       << endl;
		cout << "  [inWordArray] average cycles per hit: "  << average(inArrayHits)   << endl;
		cout << "  [inWordArray] total misses: "            << inArrayMisses.size()   << endl;
		cout << "  [inWordArray] miss cycles: "             << sum(inArrayMisses)     << endl;
		cout << "  [inWordArray] average cycles per miss: " << average(inArrayMisses) << endl;
	}

	// merge the hit times and the miss times into another vector
	sort(areAnagramsPass.begin(), areAnagramsPass.end());
	sort(areAnagramsFail.begin(), areAnagramsFail.end());
	vector<u_int64_t> areAnagramsTimes;
	merge(
		areAnagramsPass.begin(), areAnagramsPass.end(),
		areAnagramsFail.begin(), areAnagramsFail.end(),
		back_inserter(areAnagramsTimes));
	if (areAnagramsTimes.size()) {
		cout << endl;
		cout << "  [areAnagrams] average cycles overall: "  << average(areAnagramsTimes)  << endl;
		cout << "  [areAnagrams] total hits: "              << areAnagramsPass.size()     << endl;
		cout << "  [areAnagrams] hit cycles: "              << sum(areAnagramsPass)       << endl;
		cout << "  [areAnagrams] average cycles per hit: "  << average(areAnagramsPass)   << endl;
		cout << "  [areAnagrams] total misses: "            << areAnagramsFail.size()   << endl;
		cout << "  [areAnagrams] miss cycles: "             << sum(areAnagramsFail)     << endl;
		cout << "  [areAnagrams] average cycles per miss: " << average(areAnagramsFail) << endl;
	}

	cout << endl;
	cout << "  [hashItem] average cycles: " << average(hashTimes)     << endl;

	cout << endl;
	if (areAnagramsTimes.size()) {
		cout << "  [findAnagrams] cycles: "     << findAnagramsCycles << endl;
	}
	if (inArrayTimes.size()) {
		cout << "  [Dictionary::check] cycles: " << dictionaryCheckCycles << endl;
	}

	cout << "  [peeler exec time] seconds: " << executionTime << endl;
	// cout << "  hashInit time: " << hashInit << endl;

	return 0;
}
