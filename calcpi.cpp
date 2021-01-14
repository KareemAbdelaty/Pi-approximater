// ======================================================================
// Kareem Abdelaty
// 30075331
// ======================================================================
//
// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pi() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//

#include "calcpi.h"
#include <thread>
//struct to store each threads start/end rows and there a partail count
struct Task{
	int start_row,end_row;
	uint64_t partial_count;
};
//threaded count pi function
void count_pi_threaded(Task *t,double square,int r){
  uint64_t partial_count = 0;	
  for( double x = t->start_row ; x <= t->end_row ; x ++)
    for( double y = -r ; y <= r; y ++)
      if( x*x + y*y <= square) partial_count++;	
	
  t->partial_count = partial_count;
}

uint64_t count_pi(int r, int n_threads)
{
  //setup threads and input
  Task tasks[n_threads];
  std::thread Threads[n_threads];
  int portion = (2*r)/n_threads;
  int start = (-r);
  int end = (-r) + portion;
  double rsq = double(r) * r;
  for(int i=0;i<n_threads;i++){
	  if(i<n_threads-1){
		  tasks[i].start_row = start;
		  tasks[i].end_row = end;
		  tasks[i].partial_count = 0;
		  start = end+1;
		  end = start + portion;
	  }else{
		  tasks[i].start_row = start;
		  tasks[i].end_row = r;
		  tasks[i].partial_count = 0;	  
	  }
	  Threads[i] = std::thread(count_pi_threaded,&tasks[i],rsq,r); 
  }	
  uint64_t count = 0;
  //get answer
  for(int i= 0;i<n_threads;i++){
	  Threads[i].join();
	  count+= tasks[i].partial_count;
  }
  return count;
}



