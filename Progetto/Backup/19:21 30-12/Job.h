#ifndef TEST_H__
#define TEST_H__

typedef struct{
    int type_flag;
    int lenght;
    int io_max;
}instruction;

typedef struct {
    int id;
    int arrival_time;
    instruction instr[200];
    int  totalLeght;
    int numbOfInstr;
}Job;

#endif /* TEST_H__*/


