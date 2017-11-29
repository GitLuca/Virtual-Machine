/*of course IT IS NOT WORKING 
 * NOPE
 * NOPE
 * NOPE */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const int n = 10; //number of process
semaphore s =1;
struct semaphore {
	int count;
	queueType queue;
	};

void semWait (semaphore s){
	s.count--;
	if (s.count < 0){
		/*place this processi in s.queue*/ 
		/*block this process*/
	}
}
void semSignal (semaphore s){
	s.count++;
	if (s.count <= 0){
		/*place this processi in s.queue*/ 
		/*block this process*/
	}
}
void process(int i){
	whhile(true){
		semWait(s);
		/*critical section*/
		semSignal(s);
		/*remainder*/
	}
}
void main (){
	//parbegin(process(1)... process(n));
	pid_t child_pid;
	child_pid = fork();
}
