#pragma once
#ifndef DSSL_H
#define DSSL_H
#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"

// A structure to represent a node in a singly linked list of doctors
typedef struct DoctorNode {
    Doc data; // The doctor data
    struct DoctorNode* next; // Pointer to the next node
} DoctorNode;

// Function declarations for singly linked list operations

// Create a new doctor node
DoctorNode* createDoctorNode(Doc data);  

// Append a doctor to the end of the list
void appendDoctor(DoctorNode** head, Doc data);

// Delete the entire doctor list
void deleteDoctorList(DoctorNode** head);  

// Find a doctor by license number
DoctorNode* findDoctor(DoctorNode* head, const char* license);

// Delete a doctor by license number
void deleteDoctor(DoctorNode** head, const char* license); 

#endif // DSSL_H
