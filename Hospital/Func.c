#include "func.h"

void parseDate(char* dateStr, Date* date) {
    sscanf(dateStr, "%d/%d/%d %d:%d", &date->Day, &date->Month, &date->Year, &date->Hour, &date->Min);
}

float parseDuration(char* durationStr) {
    int hours, mins;
    sscanf(durationStr, "%d:%d", &hours, &mins);
    return hours + mins / 60.0;
}

bool only_letters_and_spaces(char* text) {
    while (*text != '\0') {
        // Check if the character is an uppercase letter (A-Z)
        if ((*text >= 'A' && *text <= 'Z') ||
            // Check if the character is a lowercase letter (a-z)
            (*text >= 'a' && *text <= 'z') ||
            // Check if the character is a space
            (*text == ' ')) {
            text++;
        }
        else {
            return false;
        }
    }
    return true;
}

void loadPatients(pTree** root)
{
    char* filename = "Patients.txt";
    FILE* file = fopen(filename, "r");
    pInTree* patient = (*root)->root;
    if (!file) {
        perror("Failed to open file");
        return;
    }
    char line[512];
    char name[100], id[10], allergies=0b00000000;
    Date arrive;
    char arrival[20], dismissed[20], duration[10], doctor[50], summary[256];
    Date tArrival, tDismissed;
    float durationVal;
    // Skip the first two lines (header and separator)
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {

        char* token = strtok(line, "."); //go after the index+"."

        token = strtok(line, ";");
        if (token != NULL) {
            strcpy(name, token);
            token = strtok(NULL, ";");
            strcpy(id, token);
            token = strtok(NULL, ";");
            while ((token = strtok(NULL, ","))) {
                if (token != NULL) {
                    if (strcmp(token, "Sulfa") == 0)
                        allergies | 0b00000010;
                    else if (strcmp(token, "Penicillin") == 0)
                        allergies | 0b00000001;
                    else if (strcmp(token, "Opioids") == 0)
                        allergies | 0b00000100;
                    else if (strcmp(token, "Anesthetics") == 0)
                        allergies | 0b00001000;
                    else if (strcmp(token, "Eggs") == 0)
                        allergies | 0b00010000;
                    else if (strcmp(token, "Latex") == 0)
                        allergies | 0b001000000;
                    else if (strcmp(token, "Preservatives") == 0)
                        allergies | 0b01000000;
                    else if (strcmp(token, "None") == 0)
                        allergies = 0b00000000;
                    //printf("%s", token);
                }
            }
            Patient* patient = createPatient(name, id, allergies);
        }

        if (strstr(line, "Arrival:")) {
            continue; // Skip until we find a patient record
        }

        while (fgets(line, sizeof(line), file) && line[0] != '=') {
            if (strstr(line, "Arrival:")) {
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(arrival, token);

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(dismissed, token);

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(duration, token);

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(doctor, token);

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(summary, token);

                parseDate(arrival, &tArrival);
                parseDate(dismissed, &tDismissed);
                durationVal = parseDuration(duration);
                Doc* doc = createDoctor(doctor);
                Visit* visit = createVisit(tArrival, tDismissed, durationVal, doc, summary);
                pushVisit(&(patient->Visits), visit);
                patient->nVisits++;
            }
        }

        token = strtok(NULL, ":");
        token = strtok(NULL, "/");
        arrive.Day = token - "0";
        token = strtok(NULL, "/");
        arrive.Month = token - "0";
        token = strtok(NULL, " ");
        arrive.Year = token - "0";
        token = strtok(NULL, ":");
        arrive.Hour = token - "0";
        token = strtok(NULL, " ");
        arrive.Min = token - "0";

        v.tArrival = arrive;


    }
    fclose(file);
}

// Function to load doctors from a file into a singly linked list (assume file is correct)
void loadDoctors(DoctorNode** head) {
    char* filename = "Doctors.txt";
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[100];

    // Skip the first two lines (header and separator)
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {   

        Doc doctor;

        char* token = strtok(line, ";");   

        //name
        if (token != NULL) {
            doctor.Name = token;
            //printf("%s", token);
            token = strtok(NULL, "; ");
        }

        //license number
        if (token != NULL) {
            strcpy(doctor.nLicense, token);
            //printf("%s", token);
            token = strtok(NULL, "; ");
        }

        //number of patients
        if (token != NULL) {
            doctor.nPatients = atoi(token);
            //printf("%s", token);
            token = strtok(NULL, ";");
        }

        // Append the doctor to the list
        appendDoctor(head, doctor);
    }
    fclose(file);
}
