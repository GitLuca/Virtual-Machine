#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct semaphore {
	int count;
	queueType queue;
	};

void semWait (semaphore s){
	s.count--;
	if ()
	}
