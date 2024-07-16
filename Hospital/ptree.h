#pragma once
#ifndef PTREE_H
#define PTREE_H 
#include "structs.h"

//patient leaf in binary tree
typedef struct pInTree {
	Patient* tpatient;
	struct pInTree* left;
	struct pInTree* right;
} pInTree;

//patient binary tree
typedef struct pTree {
	pInTree* root;
} pTree;

// Function declarations


// Create and initialize a new patient binary tree
pTree* createPatientTree();

// Destroy the patient binary tree
void destroyPatientTree(pTree* tree);

// Insert a new patient into the binary tree
pInTree* insertPatient(pTree* tree, Patient* patient);

// Find a patient in the binary tree by ID
pInTree* findPatient(pTree* tree, const char* id);

// Delete a patient from the binary tree by ID
void deletePatient(pTree* tree, const char* id);

// Perform in-order traversal of the tree and print each patient
void printAllPatients(pTree* node);


#endif // !PTREE_H
