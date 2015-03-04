all: peeler

COMMONS := peeler-common.h overrides.h elapsed_time.h
OBJECTS := peeler peeler-spreads peeler-avg peeler-multimap peeler-list
INPUTS  := $(OBJECTS:.cpp)

CC := g++
CFLAGS := --std=c++11

peeler: peeler.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-spreads: peeler-spreads.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-avg: peeler-avg.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-multimap: peeler-multimap.cpp $(COMMONS)
	$(CC) $(CFLAGS) $< -o $@

peeler-optim: peeler.cpp $(COMMONS)
	$(CC) -O9 $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJECTS)


run: peeler
	./peeler words moby

time: peeler
	time ./peeler words moby

both: peeler peeler-optim
	time ./peeler words moby
	time ./peeler-optim words moby

run-optim: peeler-optim
	./peeler-o words moby

run-multi: peeler-multimap
	./peeler-multimap words moby

spread.txt: peeler-spreads
	./peeler-spreads words moby | sort -n

averages.txt: peeler-avg
	./peeler-avg words moby > avgs.txt
	awk -f ./average.awk < avgs.txt

data: spread.txt averages.txt


%PHONY: clean data time run both
