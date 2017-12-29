#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>
#include <string.h>

const char* program_name;
int master();

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

    child_pid = childSpeak();

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
            char ** info = NULL;
            int llen;
            int counter = 0;
            char *isJ; //per verifica j
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

                    counter++;
            }
            for (int i = 0; i<counter; i++) {
                    isJ = strchr(info[i],'j');
                    if( isJ!= NULL){
                        printf("New Job! \n");
                    }
                    printf("%s \n",info[i]);
            }

        }
        //return 0;
 }

