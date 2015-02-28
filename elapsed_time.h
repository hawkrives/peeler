#ifndef _ELAPSED_TIME_
#define _ELAPSED_TIME_

#include <iostream>
#include <stdlib.h>
using std::cerr;
using std::endl;

u_int64_t start_time;  // might have to use uint64_t

inline
u_int64_t read_TSC()
{
  u_int64_t i;
  __asm__ __volatile__ ("rdtsc" : "=&A" (i));
  return i;
}

inline
void start_timer()
{
  start_time = read_TSC();
}

inline
double elapsed_time()
{
  u_int64_t curr_time = read_TSC();
  double elapsed = curr_time - start_time;
//  if ( elapsed < 0 ) cerr << "Warning:  negative number of cycles in elapsed_time!" << endl;
  return elapsed;
}

#endif
