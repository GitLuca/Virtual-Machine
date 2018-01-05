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

/**shortest job next**/
void scheduler_not_preemptive(Job *jobs, int jobCount){
    int clock1 = 0;
    int clock2 = 0;
    int jobDone = 0;
    bool th1 = false;
    bool th2 = false;
    pthread_t thread1;
    pthread_t thread2;
    args thread1_args;
    args thread2_args;

    //printf("Jobs non ordinati\n");
    /*for (int i = 0; i<jobCount; i++){
        printf("Job numero %d, con lunghezza %d\n", jobs[i].id,jobs[i].totalLeght);

    }*/
    sort(jobs, jobCount);

    //printf("Jobs ordinati\n");
    //printf("Prova %d\n",jobs[0].id);
    /*for (int i = 0; i<jobCount; i++){
        printf("Job numero %d, con lunghezza %d\n", jobs[i].id,jobs[i].totalLeght);

    }*/
    int i = 0;
    while(jobDone < jobCount){

        printf("clock1: %d\n", clock1);
        printf("clock2: %d\n", clock2);

        for (i; i<jobCount; i++){
            if(jobs[i].arrival_time >= clock1 && !th1){

                //printf("job numero %d va in core 1\n",jobs[i].id);
                /**metto argomenti**/
                thread1_args.job = jobs[i];
                thread1_args.clock = clock1;
                th1 = true;
                printf("Verso il core1\n");
                pthread_create (&thread1, NULL, &core1, &thread1_args);
                break;

            }else if(jobs[i].arrival_time >= clock2 && !th1){
                //printf("job numero %d va in core 2\n",jobs[i].id);
                 /**metto argomenti**/
                thread2_args.job = jobs[i];
                thread2_args.clock = clock2;
                th2 = true;
                pthread_create (&thread1, NULL, &core2, &thread2_args);
                break;

            }
            if(th1){
                pthread_join (thread1, NULL); //Qua?
                th1 = false;
                clock1 = thread1_args.clock;
                //printf("clock1 DOPO THREAD: %d\n", clock1);
                jobDone++;
            }
            if(th2){
                pthread_join (thread2, NULL); //Qua?
                th2 = false;
                clock2 = thread2_args.clock;
                //printf("clock2 DOPO THREAD: %d\n", clock2);
                jobDone++;

            }

        }
        clock1++;
        clock2++;


    }

}

void* core1 (void* parameters){
    args* th_args = (args*) parameters;

    (*th_args).clock += 10; /**STUPID**/
    printf("core 1 esegue job: %d\n", (*th_args).job.id);
    sleep(2);
    return NULL;
}

void* core2 (void* parameters){
    args* th_args = (args*) parameters;

    (*th_args).clock += 10; /**STUPID**/
    printf("core 2 esegue job: %d\n", (*th_args).job.id);
    //sleep(2);
    return NULL;
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
