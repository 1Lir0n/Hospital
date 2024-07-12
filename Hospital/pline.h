#pragma once
#ifndef PLINE_H
#define PLINE_H
#include "structs.h"

// Queue node structure
typedef struct pInLine {
    Patient* lpatient;
    struct pInLine* next;

}pInLine;

// Queue structure
typedef struct pQueue {
    pInLine* front;
    pInLine* rear;
    int size;
} pQueue;

// Function declarations
pQueue* createPatientQueue();
void destroyPatientQueue(pQueue* queue);
void enqueuePatient(pQueue* queue, Patient* patient);
Patient* dequeuePatient(pQueue* queue);
Patient* peekPatientQueue(pQueue* queue);
int isPatientQueueEmpty(pQueue* queue);
int getPatientQueueSize(pQueue* queue);

#endif // !PLINE_H
