semaphore fork [5] ={1};
int i;
void philosopher (int i){
	while(true){
	think();
	wait(fork[i]);
	wait(fork[i] mod 5);
	eat
)}
}
