#include "func.h"

void parseDate(char* dateStr, Date* date) {

    if (strcmp(dateStr, "") != 0) {
        sscanf(dateStr, "%d/%d/%d %d:%d", &date->Day, &date->Month, &date->Year, &date->Hour, &date->Min);
    }
    else {
        date->Day = 0;
        date->Month = 0;
        date->Year = 0;
        date->Hour = 0;
        date->Min = 0;
    }
}

float parseDuration(char* durationStr) {
    int hours, mins;
    if (strcmp(durationStr, "")!=0) {
        sscanf(durationStr, "%d:%d", &hours, &mins);
        return  hours + (mins / 100.0);
    }
    else return 0;
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
void loadPatients(DoctorNode* head, pTree* tree) {
    char* filename = "Patients.txt";
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[512];
    // Skip the first two lines (header and separator)
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Initialize variables for patient data
        char name[100], id[10], arrival[20], dismissed[20], duration[10], doctor[50], summary[256];
        Date tArrival = { 0 }, tDismissed = { 0 };
        float durationVal = 0.0;
        char allergies = 0b00000000;
        StackVisit* visitStack = NULL;

        // Tokenize and parse the line for patient data
        char* token = strtok(line, ".");
        token = strtok(NULL, ";");
        strcpy(name, token ? token : "");
        if (strcmp(name, "") == 0) break;//end of list
        token = strtok(NULL, ";");
        strcpy(id, token ? token : "");

        token = strtok(NULL, ",");

        while (token!= NULL) {
            // Check if the last character is '\n'
            if (strlen(token) > 0 && token[strlen(token) - 1] == '\n') {
                // Replace '\n' with '\0' (null terminator)
                token[strlen(token) - 1] = '\0';
            }
            //printf("%s|", token);
            if (strcmp(token, "Sulfa") == 0) allergies |= 0b00000010;
            else if (strcmp(token, "Penicillin") == 0) allergies |= 0b00000001;
            else if (strcmp(token, "Opioids") == 0) allergies |= 0b00000100;
            else if (strcmp(token, "Anesthetics") == 0) allergies |= 0b00001000;
            else if (strcmp(token, "Eggs") == 0) allergies |= 0b00010000;
            else if (strcmp(token, "Latex") == 0) allergies |= 0b00100000;
            else if (strcmp(token, "Preservatives") == 0) allergies |= 0b01000000;
            else if (strcmp(token, "None") == 0) allergies = 0b00000000;
            token = strtok(NULL, ",");

        }
 
        // Create the patient with no visits
        Patient* patient = createPatient(name, id, allergies);

        // Read visits for the patient
        while (fgets(line, sizeof(line), file) && line[0] != '=') {
            if (strstr(line, "Arrival:")) {
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(arrival, token ? token : "");

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(dismissed, token ? token : "");

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(duration, token ? token : "");

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(doctor, token ? token : "");

                DoctorNode* doctorNode = findDoctor(head, doctor);
                Doc* doc = doctorNode ? &(doctorNode->data) : NULL;

                fgets(line, sizeof(line), file);
                token = strtok(line, ":");
                token = strtok(NULL, "\n");
                strcpy(summary, token ? token : "");

                parseDate(arrival, &tArrival);
                parseDate(dismissed, &tDismissed);
                durationVal = parseDuration(duration);

                Visit* visit = createVisit(tArrival, tDismissed, durationVal, doc, summary);
                pushVisit(&visitStack, visit);
            }
        }

        // Associate visits with the patient
        patient->Visits = visitStack;

        // Insert the patient into the tree
        insertPatient(tree, patient);
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

//print a list of all docotrs
void printDoctors(DoctorNode* doc) {
    DoctorNode* current = doc;

    while (current) {
        printf("Full Name: %s, License Number: %s, Number of Patients: %d\n",
            current->data.Name, current->data.nLicense, current->data.nPatients);
        current = current->next;
    }
}

