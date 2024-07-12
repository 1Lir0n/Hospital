#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//date structure inculding date and time
typedef struct Date{
	int Year; //4 digits since 1900 till now
	int Month; //1-12
	int Day; //1-end of month
	int Hour; //24 format 0-23
	int Min; //0-59
}Date;

//doctor structure name license number and numder of patients
typedef struct Doc{
	char* Name; //full name, first and last. only letters and spaces
	char nLicense[7]; //license number 6 digits + \0 
	int nPatients; //number of current patients max of 4
}Doc;

//visit structure for patient visit 
typedef struct Visit{
	Date tArrival; //date and time of arrival to the clinic
	Date tDismissed; //date and time of leaving the clinic
	float Duration; //amount of time in the clinic in hour.min
	Doc* Doctor; //the attending doctor of the patient
	char* Summary; //summary of visit if none then NULL
}Visit;

//patient structure name id allergies and visits
typedef struct Patient {
	char* Name; //patient full name only letters and spaces
	char ID[10]; //patient ID number 9+\0
	unsigned char Allergies; //bitwise. for full list reference Allergies.txt File. can have multiple allergies
	struct StackVisit* Visits; //dynamic stack of visits
	int nVisits; //number of visits to the clinic
}Patient;

#endif // !STRUCTS_H
