#pragma once
#ifndef FUNC_H
#define FUNC_H
#include "structs.h"
#include "ptree.h"
#include "pline.h"
#include "svisit.h"
#include "dsll.h"
#include "patients.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//loads patients data from file into binary search tree
void loadPatients(DoctorNode* head, pTree** root);

//loads doctors data from file into SSL
void loadDoctors(DoctorNode** head);

//loads patients in line into queue
pQueue loadLine();

//updates all the files necessary
void updateFiles();

//search patients via ID
Patient* searchPatient();

//assignDoctor2case
Doc* assignDoctor2case(DoctorNode* head) {

//prints an error accordinly 
void displayError(char* errorType);


void printDoctors(DoctorNode* doc);

#endif // !FUNC_H
