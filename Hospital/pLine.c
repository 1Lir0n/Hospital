#include "pline.h"

// Create and initialize a new patient queue
pQueue* createPatientQueue() {
    pQueue* queue = (pQueue*)malloc(sizeof(pQueue));
    if (queue) {
        queue->front = NULL;
        queue->rear = NULL;
        queue->size = 0;
    }
    return queue;
}

// Destroy the patient queue and free all memory
void destroyPatientQueue(pQueue* queue) {
    if (queue) {
        while (!isPatientQueueEmpty(queue)) {
            dequeuePatient(queue);
        }
        free(queue);
    }
}

// Add a patient to the end of the queue
void enqueuePatient(pQueue* queue, Patient* patient) {
    if (!queue) return;

    pInLine* newNode = (pInLine*)malloc(sizeof(pInLine));
    if (newNode) {
        newNode->lpatient = patient;
        newNode->next = NULL;
        if (isPatientQueueEmpty(queue)) {
            queue->front = newNode;
        }
        else {
            queue->rear->next = newNode;
        }
        queue->rear = newNode;
        queue->size++;
    }
}

// Remove and return the patient at the front of the queue
Patient* dequeuePatient(pQueue* queue) {
    if (!queue || isPatientQueueEmpty(queue)) return NULL;

    pInLine* frontNode = queue->front;
    Patient* patient = frontNode->lpatient;
    queue->front = frontNode->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(frontNode);
    queue->size--;
    return patient;
}

// Peek at the patient at the front of the queue without removing them
Patient* peekPatientQueue(pQueue* queue) {
    if (queue && !isPatientQueueEmpty(queue)) {
        return queue->front->lpatient;
    }
    return NULL;
}

// Check if the patient queue is empty
int isPatientQueueEmpty(pQueue* queue) {
    return queue ? queue->size == 0 : 1;
}

// Get the number of patients in the queue
int getPatientQueueSize(pQueue* queue) {
    return queue ? queue->size : 0;
}
