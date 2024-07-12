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
void loadPatients(pTree** root);

//loads doctors data from file into SSL
void loadDoctors(DoctorNode** head);

//loads patients in line into queue
pQueue loadLine();

//updates all the files necessary
void updateFiles();

//search patients via ID
Patient* searchPatient(pInTree root, char* id);

//prints an error accordinly 
void displayError(char* errorType);

#endif // !FUNC_H
