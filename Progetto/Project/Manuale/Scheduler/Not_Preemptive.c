#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "Job.h"
//#include "Processes.h"


typedef struct {
    Job job;
	int clock;
}args;

void scheduler_not_preemptive(Job *jobs, int jobCount);
void sort (Job *readyJob, int size);
void* core1 (void* parameters);
void* core2 (void* parameters);
int rand_lim(int limit);

/**
0 = READY
1 = RUNNING
2 = DONE
**/
int core1_status = 0;
int core2_status = 0;
int jobDone = 0;
pthread_mutex_t lock;
int kkk =30;

/**shortest job next**/
void scheduler_not_preemptive(Job *jobs, int jobCount){
    int clock1 = 0;
    int clock2 = 0;

    //bool th1 = false;
    //bool th2 = false;
    pthread_t thread1;
    pthread_t thread2;
    args thread1_args;
    args thread2_args;
    printf("JOBCOUNT %d\n", jobCount);


    /**inizializza il MUTEX che mi servirà per il jobdone++**/
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");

    }

    /*printf("\n *****************************************\n");
    sort(jobs, jobCount);
     for(int i1=0; i1<jobCount; i1++){
        printf("job numero:%d cxon numero istruzioni %d, pState %d e lunghezza %d IStrDone %d\n", jobs[i1].id, jobs[i1].numbOfInstr,jobs[i1].pState, jobs[i1].totalLeght,jobs[i1].instrDone);
        for(int i2 = 0; i2 < jobs[i1].numbOfInstr; i2++ ){
             printf("istruzione numero:%d con type_flag %d, lunghezza %d e io_max %d \n", i2, jobs[i1].instr[i2].type_flag,jobs[i1].instr[i2].lenght,jobs[i1].instr[i2].io_max);
        }
    }*/

    for(int c = 0; c<jobCount; c++){
            jobs[c].pState = READY;
            //printf("Job %d ha pState %d\n", c ,jobs[c].pState);
            //printf("Job numero %d, con lunghezza %d e arrival time %d\n", jobs[c].id,jobs[c].totalLeght, jobs[c].arrival_time);
        }

        /*****************************************************************/
        /*****************************************************************/
    while(jobDone < jobCount){

       sort(jobs, jobCount);

        //printf("LISTA JOBS:\n");
        /*for(int c = 0; c<jobCount; c++){

            printf("Job %d ha pState %d\n", c ,jobs[c].pState);
            printf("CORE1 STATUS %d\n", core1_status );
             printf("CORE22222222222222222222222222222222222222 STATUS %d\n", core1_status );
            if(jobs[c].pState == BLOCKED ){
                printf("job %d MESSO IN READY\n", jobs[c].id);
                jobs[c].pState = READY;
            }


        }*/
        //fprintf(stderr,"JOBDONE %d\n",jobDone);
        for (int i = 0; i<jobCount; i++){


            //fprintf(stderr,"Ciclo for\n");
            if(core1_status == 0  && jobs[i].pState == READY){

                /**metto argomenti**/
                jobs[i].pState = RUNNING;
                //th1 = true;
                core1_status = 1;
                thread1_args.job = jobs[i];
                thread1_args.clock = clock1;

                printf("Numero %d va verso il core1\n", jobs[i].id);
                printf("Job %d ha pState %d\n", i ,jobs[i].pState);
                pthread_create (&thread1, NULL, &core1, (void*)&thread1_args);
                //break;

            }else if(core2_status == 0  && jobs[i].pState == READY){


                /**metto argomenti**/
                jobs[i].pState = RUNNING;
                thread2_args.job = jobs[i];
                thread2_args.clock = clock2;
                core2_status = 1;
                printf("Numero %d va verso il core2\n", jobs[i].id);
                printf("Job %d ha pState %d\n", i ,jobs[i].pState);
                pthread_create (&thread2, NULL, &core2, (void*)&thread2_args);
                //break;

            }

            /**devo far si che i thread abbiano delle variabili run, ready, done**/

            if(core1_status == 2){
                    pthread_join (thread1, (void*)&thread1_args);
                    core1_status = 0;
                    clock1 += thread1_args.clock;
                    //jobDone++;
                    //printf("Chiuso thread 1\n");
                    //printf("core1_status =0\n");
                     printf("JOB BLOCKED %d ha pState %d\n", jobs[i].id, jobs[i].pState);
                     if(jobs[i].pState == BLOCKED ){
                printf("job %d MESSO IN READY\n", jobs[i].id);
                jobs[i].pState = READY;
            }
            }
            if(core2_status == 2){
                    pthread_join (thread2, (void*)&thread2_args);
                    core2_status = 0;
                    clock2 += thread2_args.clock;
                    //printf("Chiuso thread 2\n");
                    //printf("core2_status =0\n");
                    //jobDone++;
                    printf("JOB BLOCKED %d ha pState %d\n", jobs[i].id, jobs[i].pState);
                     if(jobs[i].pState == BLOCKED ){
                printf("job %d MESSO IN READY\n", jobs[i].id);
                jobs[i].pState = READY;
            }

            }


        }

    }
    pthread_mutex_destroy(&lock);

}

void* core1 (void* parameters){

    args* th_args = (args*) parameters;
    int clock = (*th_args).clock;

    int count = 0; //per poter riprendere un job se è stato interrotto
    printf("job numero:%d cxon numero istruzioni %d, pState %d e lunghezza %d IStrDone %d\n", (*th_args).job.id, (*th_args).job.numbOfInstr,(*th_args).job.pState, (*th_args).job.totalLeght,(*th_args).job.instrDone);

    if((*th_args).job.instrDone != 0){
        count = (*th_args).job.instrDone;
    }
    printf("COUNT 1 del Job %d: %d\n",(*th_args).job.id, count);
    for (; count < (*th_args).job.numbOfInstr; count++){


        if((*th_args).job.instr[count].type_flag == 0){ //se l'itruzione è non bloccante

            int lenght = (*th_args).job.instr[count].lenght;
            fprintf(stderr,"core 1 ha eseguito un istruzione n. %d del job: %d\n", count, (*th_args).job.id);
            for (int j=0; j<lenght; j++){
                clock++;
            }

        }else if((*th_args).job.instr[count].type_flag == 1){

            clock++;
            int max = (*th_args).job.instr[count].io_max;
            (*th_args).job.instrDone = count+1;
            /*espediente per fare da 1 a io_max*/
            int rnd = rand_lim(max);

            /*se è bloccato metto lo stato BLOCKED e l'arrival_time lo setto al clock attuale
            aggiungendo il numero di cicli per cui deve restare bloccato (rnd) ANCHE SE  POTREBBE ESSERE SBAGLIATO
            ESSENDOCI DUE CICLI DI CLOCK DIVERSI*/

            printf("core 1 ha eseguito l' istruzione n. %d del job: %d ma è stato bloccato\n", count, (*th_args).job.id);
            (*th_args).job.pState = BLOCKED;
            printf("Job %d ha pState %d\n", (*th_args).job.id, (*th_args).job.pState);
            (*th_args).job.arrival_time = (*th_args).clock + rnd;
            break;

        }else{
            fprintf(stderr, "Errore nella lettura del type_flag");
            (*th_args).job.pState = BLOCKED;
        }
    }

    if((*th_args).job.numbOfInstr == count){
        /*completa procedura con job concluso*/
        pthread_mutex_lock(&lock);
        jobDone++;
        printf("JOBCOUNT ++\n");
        pthread_mutex_unlock(&lock);

        fprintf(stderr,"CORE 1 ha FINITO il job: %d\n", (*th_args).job.id);
        (*th_args).job.pState = EXIT;
    }

    (*th_args).clock += clock;
    //printf("clock1: %d\n", (*th_args).clock);
    /*char ch = 'Y';
    for(int i =0; i<(800*l); i++){
        fputc (ch, stderr);
        //fprintf(stderr, "X");
    }*/

    /**Completa job con mutex e status del core**/
    //printf("core1_status = 2\n");
    core1_status = 2;


    pthread_exit(NULL);

}

void* core2 (void* parameters){
    args* th_args = (args*) parameters;
    int clock = (*th_args).clock;


    int count = 0; //per poter riprendere un job se è stato interrotto

    if((*th_args).job.instrDone != 0){
        count = (*th_args).job.instrDone;
    }
     printf("COUNT 2 del Job %d: %d\n",(*th_args).job.id, count );
    for (; count < (*th_args).job.numbOfInstr; count++){


        if((*th_args).job.instr[count].type_flag == 0){ //se l'itruzione è non bloccante

            int lenght = (*th_args).job.instr[count].lenght;
            printf("core 2 ha eseguito l' istruzione n. %d del job: %d\n", count, (*th_args).job.id);
            for (int j=0; j<lenght; j++){
                clock++;
            }

        }else if((*th_args).job.instr[count].type_flag == 1){

            clock++;
            int max = (*th_args).job.instr[count].io_max;
            (*th_args).job.instrDone = count+1;
            /*espediente per fare da 1 a io_max*/
            int rnd = rand_lim(max);

            /*se è bloccato metto lo stato BLOCKED e l'arrival_time lo setto al clock attuale
            aggiungendo il numero di cicli per cui deve restare bloccato (rnd) ANCHE SE  POTREBBE ESSERE SBAGLIATO
            ESSENDOCI DUE CICLI DI CLOCK DIVERSI*/

            fprintf(stderr,"core 2 ha eseguito istruzione n. %d del job: %d ma è stato bloccato\n", count, (*th_args).job.id);
            (*th_args).job.pState = BLOCKED;
             printf("Job %d ha pState %d\n", (*th_args).job.id , (*th_args).job.pState);
            (*th_args).job.arrival_time = (*th_args).clock + rnd;
            break;
        }else{
            fprintf(stderr, "Errore nella lettura del type_flag");
            (*th_args).job.pState = BLOCKED;
        }
    }

     if((*th_args).job.numbOfInstr == count){
        /*completa procedura con job concluso*/
        pthread_mutex_lock(&lock);
        printf("JOBCOUNT ++\n");+
        jobDone++;
        pthread_mutex_unlock(&lock);

        fprintf(stderr,"CORE 2 ha FINITO il job: %d\n", (*th_args).job.id);
        (*th_args).job.pState = EXIT;
    }

    (*th_args).clock += clock;
    //printf("clock2: %d\n", (*th_args).clock);
    /*char ch = 'X';
    for(int i =0; i<(800*l); i++){

        fputc (ch, stderr);
        //fprintf(stderr, "Y");
    }*/
    //printf("core2_status = 2\n");
    core2_status = 2;

    pthread_exit(NULL);
}

void sort (Job *readyJob, int size){

    int i,j;
    Job temp;
    for(i=0; i<size; i++){

        for(j=i+1; j<size; j++){

            if(readyJob[i].totalLeght > readyJob[j].totalLeght){

                temp = readyJob[i];
                readyJob[i] = readyJob[j];
                readyJob[j] = temp;
            }
        }
    }
}

int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */
    /*cambiato limit e retval se non funziona*/
    limit -=1;
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit);
    retval++;
    return retval;
}
