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
	int core;
}args;

void scheduler_not_preemptive(Job *jobs, int jobCount);
void sort (Job *readyJob, int size);
void* core (void* parameters);

int rand_lim(int limit);

/**
0 = READY
1 = RUNNING
2 = DONE
**/

int core_status[2];
int jobDone = 0;
pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_mutex_t lock4;


/**shortest job next**/
void scheduler_not_preemptive(Job *jobs, int jobCount){

    int clock1 = 0;
    int clock2 = 0;
    pthread_t thread[2];
    args thread_args[2];
    //args thread2_args;
    printf("JOBCOUNT %d\n", jobCount);


    /**inizializza il MUTEX che mi servirà per il jobdone++**/
    if (pthread_mutex_init(&lock1, NULL) != 0)
    {
        printf("\n mutex init failed\n");

    }
    if (pthread_mutex_init(&lock2, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }
    if (pthread_mutex_init(&lock3, NULL) != 0)
    {
        printf("\n mutex init failed\n");

    }
    if (pthread_mutex_init(&lock4, NULL) != 0)
    {
        printf("\n mutex init failed\n");

    }
    core_status[0]=0;
    core_status[1]=0;
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
        }
        sort(jobs, jobCount);

/*******************************************************************************************************/
    while(jobDone < jobCount){

        for (int i = 0; i<jobCount; i++){
            //printf("CORE1 %d\n", core_status[0]);
            //printf("CORE222222222222222222222222222222 %d\n", core_status[1]);

            //fprintf(stderr,"Ciclo for %d\n", i);
            if(core_status[0] == 0  && jobs[i].pState == READY){

                /**metto argomenti**/

                pthread_mutex_lock(&lock2);
                jobs[i].pState = RUNNING;
                thread_args[0].job = jobs[i];
                thread_args[0].clock = clock1;
                thread_args[0].core = 0;
                core_status[0] = 1;

                pthread_mutex_unlock(&lock2);

                printf("Numero %d va verso il core %d\n", thread_args[0].job.id, thread_args[0].core);
                //printf("Job %d ha pState %d\n", i ,jobs[i].pState);
                pthread_create (&thread[0], NULL, &core, (void*)&thread_args[0]);

            }else if(core_status[1] == 0  && jobs[i].pState == READY){


                /**metto argomenti**/
                 pthread_mutex_lock(&lock2);
                jobs[i].pState = RUNNING;
                thread_args[1].job = jobs[i];
                thread_args[1].clock = clock2;
                thread_args[1].core = 1;
                core_status[1] = 1;
                pthread_mutex_unlock(&lock2);

                printf("Numero %d va verso il core %d\n", thread_args[1].job.id, thread_args[1].core);
                //printf("Job %d ha pState %d\n", i ,jobs[i].pState);
                pthread_create (&thread[1], NULL, &core, (void*)&thread_args[1]);
            }

            /**devo far si che i thread abbiano delle variabili run, ready, done**/



        }
        if(core_status[0] == 2){

            pthread_mutex_lock(&lock3);
            pthread_join (thread[0], NULL);
            core_status[0] = 0;
            clock1 += thread_args[0].clock;
            jobs[thread_args[0].job.id] = thread_args[0].job;
            //(void*)&thread_args[0]
            //printf("CORE 0 STATUS: %d\n",core_status[0]);

            printf("CORE 0 ha lasciato il JOB %d ha pState %d e istrDone %d\n", thread_args[0].job.id, jobs[thread_args[0].job.id].pState, jobs[thread_args[0].job.id].instrDone);
            if(thread_args[0].job.pState == BLOCKED ){
                printf("job %d MESSO IN READY \n", thread_args[0].job.id);
                jobs[thread_args[0].job.id].pState = READY;
                thread_args[0].job.pState = READY;

            }
            pthread_mutex_unlock(&lock3);


        }
         if(core_status[1] == 2){
            pthread_mutex_lock(&lock4);
            pthread_join (thread[1], NULL);
            core_status[1] = 0;
            clock2 += thread_args[1].clock;
            jobs[thread_args[1].job.id] = thread_args[1].job;
            //printf("CORE 1 STATUS: %d\n",core_status[1]);

            printf("CORE 1 ha lasciato il JOB %d ha pState %d e istrDone %d\n", jobs[thread_args[1].job.id].id, jobs[thread_args[1].job.id].pState, jobs[thread_args[1].job.id].instrDone);
             if(thread_args[1].job.pState == BLOCKED ){
                printf("job %d MESSO IN READY\n", thread_args[1].job.id);
                thread_args[1].job.pState = READY;
                jobs[thread_args[0].job.id].pState = READY;
            }
            pthread_mutex_unlock(&lock4);
        }



    }


    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    pthread_mutex_destroy(&lock4);
}

void* core (void* parameters){


    args* th_args = (args*) parameters;
    int clock = (*th_args).clock;
    int coreNumb = (*th_args).core;

    int count = 0; //per poter riprendere un job se è stato interrotto
    //printf("job numero:%d cxon numero istruzioni %d, pState %d e lunghezza %d IStrDone %d\n", (*th_args).job.id, (*th_args).job.numbOfInstr,(*th_args).job.pState, (*th_args).job.totalLeght,(*th_args).job.instrDone);

    if((*th_args).job.instrDone != 0){
        count = (*th_args).job.instrDone;
    }
    //printf("COUNT 1 del Job %d: %d\n",(*th_args).job.id, count);
    for (; count < (*th_args).job.numbOfInstr; count++){


        if((*th_args).job.instr[count].type_flag == 0){ //se l'itruzione è non bloccante

            int lenght = (*th_args).job.instr[count].lenght;
            fprintf(stderr,"core %d ha eseguito un istruzione n. %d del job: %d\n",coreNumb, count, (*th_args).job.id);
            (*th_args).job.instrDone++;
            for (int j=0; j<lenght; j++){
                clock++;
            }

        }else if((*th_args).job.instr[count].type_flag == 1){

            clock++;
            int max = (*th_args).job.instr[count].io_max;
            (*th_args).job.instrDone = (count+1);
            /*espediente per fare da 1 a io_max*/
            int rnd = rand_lim(max);

            /*se è bloccato metto lo stato BLOCKED e l'arrival_time lo setto al clock attuale
            aggiungendo il numero di cicli per cui deve restare bloccato (rnd) ANCHE SE  POTREBBE ESSERE SBAGLIATO
            ESSENDOCI DUE CICLI DI CLOCK DIVERSI*/

            printf("core %d ha eseguito l' istruzione n. %d del job: %d ma è stato bloccato\n",coreNumb, count, (*th_args).job.id);
            (*th_args).job.pState = BLOCKED;
            //printf("Job %d ha pState %d\n", (*th_args).job.id, (*th_args).job.pState);
            (*th_args).job.arrival_time = (*th_args).clock + rnd;
            break;

        }else{
            fprintf(stderr, "Errore nella lettura del type_flag");
            (*th_args).job.pState = BLOCKED;
        }
    }

    if((*th_args).job.numbOfInstr == count){
        /*completa procedura con job concluso*/
        pthread_mutex_lock(&lock1);
        jobDone++;
        fprintf(stderr,"CORE %d ha FINITO il job: %d\n",coreNumb, (*th_args).job.id);
        (*th_args).job.pState = EXIT;
        pthread_mutex_unlock(&lock1);


    }

    (*th_args).clock += clock;
    //printf("clock1: %d\n", (*th_args).clock);
    /*char ch = 'Y';
    for(int i =0; i<(800*l); i++){
        fputc (ch, stderr);
        //fprintf(stderr, "X");
    }*/
    printf("job numero:%d con numero istruzioni %d, pState %d e lunghezza %d IStrDone %d\n", (*th_args).job.id, (*th_args).job.numbOfInstr,(*th_args).job.pState, (*th_args).job.totalLeght,(*th_args).job.instrDone);

    /**Completa job con mutex e status del core**/
    core_status[coreNumb] = 2;

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
