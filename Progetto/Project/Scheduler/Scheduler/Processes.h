#include "Job.h"

#ifndef TEST_H__
#define TEST_H__

void scheduler_preemptive(Job* job, int quantum, int JobCount);
void scheduler_not_preemptive(Job* jobs, int jobCount);

#endif /* TEST_H__*/
