#pragma once
#ifndef PATIENTS_H
#define PATIENTS_H
#include <stdbool.h>

struct Patient* createPatient(char* name, char* id, char* allergies);
struct Visit* createVisit(struct Date arrival,struct Date dismissed, float duration,struct Doc* doctor, char* summary);
void pushVisit(struct StackVisit** stack, struct Visit* visit);

bool only_letters_and_spaces(char* text);

//capitalize words in string
void capitalize(char str[]);

//checks if a char is a digit via the ASCII table
bool isDigit(char digit);

//checks if the provided id is valid (9 digits)
bool valid_id(char Id[]);

//clear cache text
void clear();

#endif // !PATIENTS_H
