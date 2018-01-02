#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Job.h"
//#include "Processes.h"

void scheduler_not_preemptive(Job *jobs, int jobCount);
void sort (Job *readyJob, int size);

/**shortest job next**/
void scheduler_not_preemptive(Job *jobs, int jobCount){
    int clock1 = 0;
    int clock2 = 0;
    int jobDone = 0;
    printf("Jobs non ordinati\n");
    for (int i = 0; i<jobCount; i++){
        printf("Job numero %d, con lunghezza %d\n", jobs[i].id,jobs[i].totalLeght);

    }
    sort(jobs, jobCount);

    printf("Jobs ordinati\n");
    //printf("Prova %d\n",jobs[0].id);
    for (int i = 0; i<jobCount; i++){
        printf("Job numero %d, con lunghezza %d\n", jobs[i].id,jobs[i].totalLeght);

    }
    while(jobDone < jobCount){
        for (int i = 0; i<jobCount; i++){
            if(jobs[i])
        }



    }

    /*pthread_t thread1;
    pthread_create (&thread1, NULL, &core1, &thread1_args);*/

}

/*void* core1 (void* parameters)
{
    // Cast pointer to the right type.
    //struct char_print_parms* p = (struct char_print_parms*) parameters;

    return NULL;
}*/

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
