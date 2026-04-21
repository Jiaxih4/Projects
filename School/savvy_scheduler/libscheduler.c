/**
 * savvy_scheduler
 * CS 341 - Fall 2023
 */
#include "libpriqueue/libpriqueue.h"
#include "libscheduler.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "print_functions.h"

double tt[4];

/**
 * The struct to hold the information about a given job
 */
typedef struct _job_info {
    int id;
    double t[6];

    /* TODO: Add any other information and bookkeeping you need into this
     * struct. */
} job_info;

void scheduler_start_up(scheme_t s) {
    switch (s) {
    case FCFS:
        comparision_func = comparer_fcfs;
        break;
    case PRI:
        comparision_func = comparer_pri;
        break;
    case PPRI:
        comparision_func = comparer_ppri;
        break;
    case PSRTF:
        comparision_func = comparer_psrtf;
        break;
    case RR:
        comparision_func = comparer_rr;
        break;
    case SJF:
        comparision_func = comparer_sjf;
        break;
    default:
        printf("Did not recognize scheme\n");
        exit(1);
    }
    priqueue_init(&pqueue, comparision_func);
    pqueue_scheme = s;
    // Put any additional set up code you may need here
    for (int i = 0; i < 4; i++) {
        tt[i] = 0;
    }
}


static int break_tie(const void *a, const void *b) {
    return comparer_fcfs(a, b);
}

int helper(const void *a, const void *b, int lev) {
    job_info * ja = ((job*)a)->metadata;
    job_info * jb = ((job*) b)->metadata;
    if (ja->t[lev] < jb->t[lev]) {
        return -1;
    } else if (ja->t[lev] > jb->t[lev]) {
        return 1;
    }
    if (lev == 0) {
        return 0;
    } else {
        return break_tie(a, b);
    }
}

int comparer_fcfs(const void *a, const void *b) {
    // TODO: Implement me!
    int output = helper(a, b, 0);
    return output;
    //done
}

int comparer_ppri(const void *a, const void *b) {
    // Complete as is
    return comparer_pri(a, b);
}

int comparer_pri(const void *a, const void *b) {
    // TODO: Implement me!
    int output = helper(a, b, 1);
    return output;
}

int comparer_psrtf(const void *a, const void *b) {
    // TODO: Implement me!
    int output = helper(a, b, 2);
    return output;
}

int comparer_rr(const void *a, const void *b) {
    // TODO: Implement me!
    int output = helper(a, b, 3);
    return output;
}

int comparer_sjf(const void *a, const void *b) {
    // TODO: Implement me!
    int output = helper(a, b, 4);
    return output;
}

// Do not allocate stack space or initialize ctx. These will be overwritten by
// gtgo
void scheduler_new_job(job *newjob, int job_number, double time,
                       scheduler_info *sched_data) {
    // TODO: Implement me!
    job_info * ji = calloc(1,sizeof(job_info));
    ji->id = job_number;
    ji->t[0] = time;
    ji->t[1] = sched_data->priority;
    ji->t[2] = sched_data->running_time;
    ji->t[3] = -1;
    ji->t[4] = sched_data->running_time;
    ji->t[5] = -1;
    newjob->metadata = ji;
    priqueue_offer(&pqueue, newjob);
}

int checker(scheme_t s) {
    if (pqueue_scheme == s) {
        job* j = priqueue_poll(&pqueue);
        priqueue_offer(&pqueue, j);
        return 1;
    }
    return 0;
}

job *scheduler_quantum_expired(job *job_evicted, double time) {
    // TODO: Implement me!
    if (!job_evicted) {
        return priqueue_peek(&pqueue);
    }
    job_info * ji = job_evicted->metadata;
    if (ji->t[5] < 0) {
        ji->t[5] = time - 1;
    }
    ji->t[3] = time;
    ji->t[2] -= 1;

    if (checker(PPRI) == 1) {
        return priqueue_peek(&pqueue);
    }
    if (checker(PSRTF) == 1) {
        return priqueue_peek(&pqueue);
    }
    if (checker(RR) == 1) {
        return priqueue_peek(&pqueue);
    }
    return job_evicted;
}

void scheduler_job_finished(job *job_done, double time) {
    // TODO: Implement me!
    job_info * ji = job_done->metadata;
    tt[1] += time - ji->t[0] - ji->t[4];
    tt[2] += time - ji->t[0];
    tt[3] += ji->t[5] - ji->t[0];
    tt[0]++;
    free(ji);
    priqueue_poll(&pqueue);


}

static void print_stats() {
    fprintf(stderr, "turnaround     %f\n", scheduler_average_turnaround_time());
    fprintf(stderr, "total_waiting  %f\n", scheduler_average_waiting_time());
    fprintf(stderr, "total_response %f\n", scheduler_average_response_time());
}

double helper2(int lev) {
    return tt[lev] / tt[0];
}

double scheduler_average_waiting_time() {
    // TODO: Implement me!

    return helper2(1);
}

double scheduler_average_turnaround_time() {
    // TODO: Implement me!
    return helper2(2);
}

double scheduler_average_response_time() {
    // TODO: Implement me!
    return helper2(3);
}

void scheduler_show_queue() {
    // OPTIONAL: Implement this if you need it!
}

void scheduler_clean_up() {
    priqueue_destroy(&pqueue);
    print_stats();
}
