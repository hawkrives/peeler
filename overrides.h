#ifndef __overrides__
#define __overrides__

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
	out << "[";
	size_t last = v.size() - 1;
	for(size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i != last)
			out << ", ";
	}
	out << "]";
	return out;
}
ostream& operator<< (ostream& out, const pair<int, vector<string> >& p) {
	int hash = p.first;
	vector<string> v = p.second;
	out << hash << " -> ";
	out << "[";
	size_t last = v.size() - 1;
	for(size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i != last)
			out << ", ";
	}
	out << "]";
	return out;
}
ostream& operator<< (ostream& out, const pair<int, int >& p) {
	out << p.second << "\t" << p.first;
	return out;
}
template<typename T>
ostream& operator<< (ostream& out, const array<T, 4>& v) {
	out << "[";
	size_t last = v.size() - 1;
	for(size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i != last)
			out << ", ";
	}
	out << "]";
	return out;
}

#endif // __overrides__
