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

/**
0 = READY
1 = RUNNING
2 = DONE
**/
int core1_status = 0;
int core2_status = 0;
int jobDone = 0;
pthread_mutex_t lock;

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

    sort(jobs, jobCount);
    printf("LISTA JOBS:\n");
    for(int c; c<jobCount; c++){
        jobs[c].pState = READY;
        printf("Job numero %d, con lunghezza %d e arrival time %d\n", jobs[c].id,jobs[c].totalLeght, jobs[c].arrival_time);
        if(jobs[c].pState == READY){
            printf("ready\n");
        }
    }

    /**inizializza il MUTEX che mi servirà per il jobdone++**/
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");

    }


    while(jobDone < jobCount){

        //printf("clock1: %d\n", clock1);
       // printf("clock2: %d\n", clock2);

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

                pthread_create (&thread1, NULL, &core1, &thread1_args);
                //break;

            }else if(core2_status == 0  && jobs[i].pState == READY){


                /**metto argomenti**/
                jobs[i].pState = RUNNING;
                thread2_args.job = jobs[i];
                thread2_args.clock = clock2;
                core2_status = 1;
                printf("Numero %d va verso il core2\n", jobs[i].id);

                pthread_create (&thread2, NULL, &core2, &thread2_args);
                //break;

            }

            /**devo far si che i thread abbiano delle variabili run, ready, done**/

                if(core1_status == 2){
                    pthread_join (thread1, NULL);
                    core1_status = 0;
                    clock1 += thread1_args.clock;
                    jobDone++;
                    printf("Chiuso thread 1\n");
                }
                if(core2_status == 2){
                    pthread_join (thread2, NULL);
                    core2_status = 0;
                    clock2 += thread2_args.clock;
                    printf("Chiuso thread 2\n");
                    jobDone++;

                }


        }

    }
    pthread_mutex_destroy(&lock);

}

void* core1 (void* parameters){
    args* th_args = (args*) parameters;
    int clock = (*th_args).clock;

    int l =  (*th_args).job.totalLeght;
    while((*th_args).job.arrival_time > clock){
        clock++;
    }
    //sleep(l);
    (*th_args).clock += l;
    printf("clock1: %d\n", (*th_args).clock);
    /*char ch = 'Y';
    for(int i =0; i<(800*l); i++){
        fputc (ch, stderr);
        //fprintf(stderr, "X");
    }*/
    core1_status = 2;

    pthread_mutex_lock(&lock);
    jobDone++;
    pthread_mutex_unlock(&lock);

    fprintf(stderr,"core 1 ha eseguito job: %d\n", (*th_args).job.id);
    (*th_args).job.pState = EXIT;
    pthread_exit(NULL);

}

void* core2 (void* parameters){
    args* th_args = (args*) parameters;
    int clock = (*th_args).clock;

    int l =  (*th_args).job.totalLeght;
    while((*th_args).job.arrival_time > clock){
        clock ++;
    }
    //sleep(l);
    (*th_args).clock += l;
    printf("clock2: %d\n", (*th_args).clock);
    /*char ch = 'X';
    for(int i =0; i<(800*l); i++){

        fputc (ch, stderr);
        //fprintf(stderr, "Y");
    }*/
    core2_status = 2;

    pthread_mutex_lock(&lock);
    jobDone++;
    pthread_mutex_unlock(&lock);

    fprintf(stderr,"core 2 ha eseguito job: %d\n", (*th_args).job.id);
    (*th_args).job.pState = EXIT;

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
