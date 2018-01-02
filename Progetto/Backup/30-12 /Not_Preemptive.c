#include "Job.h"
#include "Processes.h"
#include <pthread.h>

/**shortest job next**/
void scheduler_not_preemptive(Job* jobs[], int jobCount){

    //Job readyJob[jobCount] = jobs[]; //HA SENSO?????

    /*pthread_t thread1;
    pthread_create (&thread1, NULL, &core1, &thread1_args);*/

}

void* core1 (void* parameters)
{
    /* Cast pointer to the right type. */
    //struct char_print_parms* p = (struct char_print_parms*) parameters;

    return NULL;
}

void sort (Job *readyJob[], int size){
    int i,j;
    Job temp;
    for(i=0; i<size; i++)
    {
        /*
         * Place the currently selected element array[i]
         * to its correct place.
         */
        for(j=i+1; j<size; j++)
        {
            /*
             * Swap if currently selected array element
             * is not at its correct position.
             */
            if((*readyJob[i]).totalLeght > (*readyJob[j]).totalLeght)
            {
                temp     = (*readyJob[i]);
                (*readyJob[i]) = (*readyJob[j]);
                (*readyJob[j]) = temp;
            }
        }
    }
}
