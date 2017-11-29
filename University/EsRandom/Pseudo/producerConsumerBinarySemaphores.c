int n;
binary-semaphores=1, delay=0;

void producer(){
	while(true){
		produce();
		semWaitB(s);
		append();
		n++;
		if(n==1)
			semSignalB(delay);
		semSignalB(s);
	}
}

void consumer(){
	int m; //local varible
	semWaitB(delay);
	while(true){
		semWaitB(s);
		take();
		n--;
		m=n;
		semSignalB(s);
		consume();
		if(m == 0)
			semWaitB(delay);
	}
}
void main(){
	n=0;
	parbegin(producer, consumer);
}
