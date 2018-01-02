#ifndef TEST_H__
#define TEST_H__


typedef enum {READY, RUNNING, BLOCKED, EXIT} state;

typedef struct{
    int type_flag;
    int lenght;
    int io_max;
}instruction;

typedef struct {
    int id;
    int arrival_time;
    instruction instr[200];
    state pState;
    int  totalLeght;
    int numbOfInstr;
}Job;

void scheduler_preemptive(Job* job, int quantum);
void scheduler_not_preemptive(Job* jobs, int jobCount);

#endif /* TEST_H__*/


