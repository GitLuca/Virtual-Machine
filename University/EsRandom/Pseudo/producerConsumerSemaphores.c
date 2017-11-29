/*program producerconsumer*/
semaphore n =0, s=1;
void producer(){
	while (true){
		produce();
		semWait(s);
		append();
		semSignal(s);
		semSignal(n);
	}
}

void consumer(){
	while(true){
		semWait(n);
		semWait(s);
		take();
		semSignal(s);
		consume();
	}
}

void main(){
	parbegin(producer,consumer);
}
