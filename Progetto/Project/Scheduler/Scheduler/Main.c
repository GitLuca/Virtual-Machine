#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>
#include <string.h>

#include "Job.h"
//#include "Processes.h"

const char* program_name;
int master();
int childSpeak();
void calculateLenght(Job* job);

void print_help(FILE* stream, int exit_code)
{
    fprintf(stream, "Usage: %s options [inputfile ...]\n", program_name);
    fprintf(stream,
            " -op --output-preemption             Output of preemption scheduler\n"
            " -on --output-no-preemption          Output of no-preemption scheduler\\n"
            " -i --input                          Input file with jobs list"
            " -q --quantum                        Quantum lenght(preempt. scheduler)");
    exit(exit_code);
}


int main(int argc, char* argv[])
{
    //int quantum=0; //lo leggerò poi da linea


    /****Creazione processi*****/

    int child_status, child_pid;

    child_pid = master();

    waitpid(child_pid, &child_status, 0); // wait for the child whose PID is equal to child_pid
    /*
     * if you have only one child you can use:
     * wait (&child_status);
     */

    if(WIFEXITED(child_status)) {
        printf("The child process exited normally with exit code %d.\n", WEXITSTATUS(child_status));
    }
    else {
        printf("The child process exited abnormally with code %d.\n", WEXITSTATUS(child_status));
    }

    printf("Done with the main program.\n");
    /**fine creazione processi**/

    return 0;
}

int childSpeak(){
        pid_t child_pid;

        child_pid = fork();
        if(child_pid != 0) {
            return child_pid; // if I am the parent, return the child pid

        }else{
            printf("This is the child\n");
            exit(EX_OSERR);
        }

}
int master(){

        pid_t child_pid;

        child_pid = fork();
        if(child_pid != 0) {
            return child_pid; // if I am the parent, return the child pid

        }else{

            FILE * infile;
            char line[121];
            char ** info = NULL; //per ora è dove salvo le istruzioni
            int llen;
            int counter = 0;
            char *isJ; //per verifica j
            int jobCount = 0;

            const char s[2] = ",";
            char *token;


            infile = fopen("01.dat","r");
            if(infile == NULL){
              printf("Error!");
              exit(1);
            }
            while (fgets(line,120,infile)) {

            // Allocate memory for pointer to line just added
                info = realloc(info,(counter+1) * sizeof(char *));
            // And allocate memory for that line itself!
                llen = strlen(line);
                info[counter] = calloc(sizeof(char),llen+1);
            // Copy the line just read into that memory
                strcpy(info[counter],line);
                isJ = strchr(info[counter],'j');
                if( isJ != NULL){
                    jobCount++;
                }
                counter++;
            }

            Job jobs[jobCount];
            int k = -1; //per il numero dei jobs
            int h = -1; //per il sottonumero delle istruzioni per ogni job

            for (int i = 0; i<counter; i++) {
                isJ = strchr(info[i],'j');


                if( isJ != NULL){

                    if(k != -1){
                        //printf("Job NUMERO: %d\n",k);
                        jobs[k].numbOfInstr = h ;
                        calculateLenght(&jobs[k]);
                         //printf( "Job number %d  has IstrNumb %d\n", k, jobs[k].numbOfInstr);
                    }
                    /**NUOVO JOB**/
                    h = -1; //riazzero il numero delle istruzione per un nuovo j
                    k++;

                   /* get the first token */
                    token = strtok(info[i], s);
                    //ID
                    token = strtok(NULL, s);
                    jobs[k].id = atoi(token);

                    //Arrival time
                    token = strtok(NULL, s);
                    jobs[k].arrival_time = atoi(token);
                    //all'inizio setto instrDone a 0
                    jobs[k].instrDone = 0;
                    //printf( "Job number %d with id %d and arrival time %d\n", k, jobs[k].id, jobs[k].arrival_time);

                }else{

                    /**NUOVA ISTRUZIONE**/
                    token = strtok(info[i], s);
                    //TYPE FLAG
                    token = strtok(NULL, s);
                    jobs[k].instr[h].type_flag = atoi(token);

                    //LENGHT
                    token = strtok(NULL, s);
                    jobs[k].instr[h].lenght = atoi(token);

                    //IO_MAX
                    token = strtok(NULL, s);
                    jobs[k].instr[h].io_max = atoi(token);
                    //printf( "Istruction number %d with type flag %d,lenght %d and I/O max %d \n", h, jobs[k].instr[h].type_flag, jobs[k].instr[h].lenght, jobs[k].instr[h].io_max );
                }
                h++;
                //controllo per l'ultimo job che non ha una j dopo
                if(i == (counter -1)){
                    jobs[k].numbOfInstr = h;
                    //printf( "Job number %d  has IstrNumb %d\n", k, jobs[k].numbOfInstr);
                    calculateLenght(&jobs[k]);
                }

            }

            /**ora si passano i job agli scheduler**/
            scheduler_not_preemptive(jobs, jobCount);
            //printf("Numero jobs: %d",jobCount);
            exit(EX_OSERR);
        }
        //return 0;
 }

 void calculateLenght(Job* job){
    int max =  (*job).numbOfInstr;
    int count;
    for (int i = 0; i < max; i++){
        count += (*job).instr[i].lenght;
    }
    (*job).totalLeght = count;
    //printf("Lunghezza istruzioni del job %d è: %d\n",(*job).id,(*job).totalLeght);

 }

