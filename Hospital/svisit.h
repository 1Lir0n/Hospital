#pragma once
#ifndef SVISIT_H
#define SVISIT_H
#include "structs.h"

// A structure to represent a stack of visits
typedef struct StackVisit {
    Visit data;
    struct StackVisit* next;
} StackVisit;


// Function declarations for stack operations

// Create a new stack node
StackVisit* newNode(Visit data);  

// Push a visit onto the stack
void push(StackVisit** root, Visit data);

// Check if the stack is empty
int isEmpty(StackVisit* root);  

// Pop a visit from the stack
Visit pop(StackVisit** root);

// Peek at the top visit on the stack
Visit peek(StackVisit* root); 

#endif // SVISIT_H
