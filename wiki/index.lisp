(h1 "peeler")

(p "Matthew Johnson and Hawken Rives' speel checker, for Algorithms (Spring '15).")

(ul
	(li (a 'href "Peeler/buckets" "Spread of words in buckets"))
	(li "Query efficiency:" (a 'href "#efficiency" "down below"))
	(li "Code!" (a 'href "Peeler/cpp" "file") (a 'href "http://stogit.cs.stolaf.edu/rives/peeler" "git repo")))

(h2 'id "efficiency" "query efficiency")

(pre "
make averages.txt

hits: 205960
cycles: 1361647559
average: 6611.223339

miss: 5823
cycles: 40509118
average: 6956.743603")
