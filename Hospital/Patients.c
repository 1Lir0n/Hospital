
#include "patients.h"
#include "structs.h"
#include "svisit.h"
#include "ptree.h"

Patient* createNewPatient(char* name, char id[10], unsigned char allergies) {
    Patient* patient = (Patient*)malloc(sizeof(Patient));
    patient->Name = _strdup(name);
    strcpy(patient->ID, id);
    strcpy(patient->Allergies, allergies);
    patient->Visits = NULL;
    patient->nVisits = 0;
    return patient;
}

void clear() {
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);//remove any lingering data in line
}

void capitalize(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        //check first character is lowercase alphabet
        if (i == 0) {
            if ((str[i] >= 'a' && str[i] <= 'z'))
                str[i] = str[i] - 32; //subtract 32 to make it capital
            continue; //continue to the loop
        }
        if (str[i] == ' ') //check space
        {
            //if space is found, check next character
            ++i;
            if (str[i] == '\0') return;
            //check next character is lowercase alphabet
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = str[i] - 32; //subtract 32 to make it capital
                continue; //continue to the loop
            }
        }
        else {
            //all other uppercase characters should be in lowercase
            if (str[i] >= 'A' && str[i] <= 'Z')
                str[i] = str[i] + 32; //subtract 32 to make it small/lowercase
        }
    }
}

bool valid_id(char Id[]) {
    if (strlen(Id) > 10 - 1) {
        return false;
    }
    //checks if the char is comprised of only digits
    for (int i = 0; i < 10 - 1; i++)
    {
        if (!isdigit(Id[i]))
            return false;
    }
    return true;
}

bool isDigit(char digit) {

    if (digit > 47 && digit < 58) {

        return true;
    }
    else return false;
}


//add Patient to tree
void addPatient(struct pInTree root) {

    printf("Enter Patient's Full Name: ");
    char buffer[256] = { 0 };
    char* name;
    char id[10];

    //NAME 

    do {
        printf("Enter member's full name: \n");
        fgets(buffer, sizeof(buffer), stdin); // Read full name from user input
        buffer[strcspn(buffer, "\n")] = '\0';
        if (!only_letters_and_spaces(buffer)) {
            printf("Please input only letters and spaces.\n\n");
        }
    } while (!only_letters_and_spaces(buffer));
    name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));

    // Copy the name from the buffer to the allocated memory
    strcpy(name, buffer);
    capitalize(name);

    //ID

    char idBuff[11] = { 0 };
    do {
        idBuff[0] = '\0';
        printf("Enter ID:\n");
        scanf("%11s", idBuff);
        clear();
        idBuff[strcspn(id, "\n")] = '\0';

        if (!valid_id(idBuff)) {
            printf("Please enter a 9 digit number.\n\n");
        }
    } while (!valid_id(idBuff));

    if (searchPatient(root,id)) {
        printf("An account with this Id already exist. Index(starts from 0):%s\n", searchPatient(root, id));
        return;
    }
    strcpy(id, idBuff);


}
Visit* createVisit(Date arrival, Date dismissed, float duration, Doc* doctor, char* summary) {
    Visit* visit = (Visit*)malloc(sizeof(Visit));
    visit->tArrival = arrival;
    visit->tDismissed = dismissed;
    visit->Duration = duration;
    visit->Doctor = doctor;
    visit->Summary = summary ? _strdup(summary) : NULL;
    return visit;
}

void pushVisit(StackVisit** stack, Visit* visit) {
    StackVisit* newNode = (StackVisit*)malloc(sizeof(StackVisit));
    newNode->data = *visit;
    newNode->next = *stack;
    *stack = newNode;
}
