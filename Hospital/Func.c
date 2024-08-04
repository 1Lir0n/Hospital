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


void loadLine(pQueue* queue, pTree* tree) {
    FILE* file = fopen("Line.txt", "r");  // Open the file "Line.txt" for reading
    if (!file) {  // Check if the file was opened successfully
        perror("Failed to open Line.txt");  // Print an error message if the file could not be opened
        return;  // Exit the function if the file could not be opened
    }

    char line[256];  // Buffer to store each line read from the file
    fgets(line, sizeof(line), file);  // Read and discard the first line (header)

    while (fgets(line, sizeof(line), file)) {  // Read each line from the file until the end
        line[strcspn(line, "\n")] = 0;  // Remove the newline character at the end of the line, if present

        int id = atoi(line);  // Convert the line (string) to an integer representing the patient ID
        pTree* current = tree;  // Start searching from the root of the binary search tree

        while (current != NULL) {  // Traverse the binary search tree to find the patient
            int cmp = strcmp(line, current->patient->ID);  // Compare the current node's ID with the target ID
            if (cmp == 0) {  // If the IDs match
                pQueueNode* newNode = (pQueueNode*)malloc(sizeof(pQueueNode));  // Allocate memory for a new queue node
                newNode->patient = current->patient;  // Set the patient pointer of the new node to the current patient
                newNode->next = NULL;  // Set the next pointer of the new node to NULL

                if (queue->rear == NULL) {  // If the queue is empty
                    queue->front = newNode;  // Set the front of the queue to the new node
                    queue->rear = newNode;  // Set the rear of the queue to the new node
                } else {  // If the queue is not empty
                    queue->rear->next = newNode;  // Link the new node at the end of the queue
                    queue->rear = newNode;  // Update the rear of the queue to the new node
                }
                break;  // Exit the inner loop as the patient has been found and enqueued
            } else if (cmp < 0) {  // If the target ID is less than the current node's ID
                current = current->left;  // Move to the left child of the current node
            } else {  // If the target ID is greater than the current node's ID
                current = current->right;  // Move to the right child of the current node
            }
        }
    }

    fclose(file);  // Close the file
}


// Helper function to convert allergies bitmask to string
char* getAllergiesStr(char allergies) {
    static char result[256];
    result[0] = '\0';

    if (allergies & 0b00000001) strcat(result, "Penicillin,");
    if (allergies & 0b00000010) strcat(result, "Sulfa,");
    if (allergies & 0b00000100) strcat(result, "Opioids,");
    if (allergies & 0b00001000) strcat(result, "Anesthetics,");
    if (allergies & 0b00010000) strcat(result, "Eggs,");
    if (allergies & 0b00100000) strcat(result, "Latex,");
    if (allergies & 0b01000000) strcat(result, "Preservatives,");
    if (result[0] == '\0') strcat(result, "None,");
    
    result[strlen(result) - 1] = '\0';  // Remove the trailing comma
    return result;
}

// Function to save patients to Patients.txt
void savePatients(pTree* tree, FILE* file) {
    if (tree == NULL) return;

    savePatients(tree->left, file);  // Save left subtree

    Patient* patient = tree->patient;
    fprintf(file, "%s;%s;%s\n", patient->Name, patient->ID, getAllergiesStr(patient->Allergies));

    StackVisit* visit = patient->Visits;
    while (visit) {
        Visit* v = visit->visit;
        fprintf(file, "Arrival:%02d/%02d/%04d %02d:%02d\n", v->tArrival.Day, v->tArrival.Month, v->tArrival.Year, v->tArrival.Hour, v->tArrival.Min);
        fprintf(file, "Dismissed:%02d/%02d/%04d %02d:%02d\n", v->tDismissed.Day, v->tDismissed.Month, v->tDismissed.Year, v->tDismissed.Hour, v->tDismissed.Min);
        fprintf(file, "Duration:%02d:%02d\n", (int)v->Duration, (int)((v->Duration - (int)v->Duration) * 60));
        fprintf(file, "Doctor:%s\n", v->Doctor->Name);
        fprintf(file, "Summary:%s\n", v->vSummary);
        visit = visit->next;
    }
    fprintf(file, "========================\n");

    savePatients(tree->right, file);  // Save right subtree
}

// Function to save doctors to Doctors.txt
void saveDoctors(DoctorNode* head, FILE* file) {
    DoctorNode* current = head;
    while (current) {
        fprintf(file, "%s;%s;%d\n", current->data.Name, current->data.nLicense, current->data.nPatients);
        current = current->next;
    }
}

// Function to save queue to Line.txt
void saveLine(pQueue* queue, FILE* file) {
    pQueueNode* current = queue->front;
    while (current) {
        fprintf(file, "%s\n", current->patient->ID);
        current = current->next;
    }
}

// Main function to update files
void updateFiles(pTree* patientTree, pQueue* patientQueue, DoctorNode* doctorList) {
    FILE* file;

    // Save Patients
    file = fopen("Patients.txt", "w");
    if (file == NULL) {
        perror("Failed to open Patients.txt");
        return;
    }
    fprintf(file, "Name; ID; Allergies\n");  // Write header
    savePatients(patientTree, file);
    fclose(file);

    // Save Line (Queue)
    file = fopen("Line.txt", "w");
    if (file == NULL) {
        perror("Failed to open Line.txt");
        return;
    }
    fprintf(file, "Patients' IDs in line\n");  // Write header
    saveLine(patientQueue, file);
    fclose(file);

    // Save Doctors
    file = fopen("Doctors.txt", "w");
    if (file == NULL) {
        perror("Failed to open Doctors.txt");
        return;
    }
    fprintf(file, "Full Name; License Number; Number of Patients\n");  // Write header
    saveDoctors(doctorList, file);
    fclose(file);
}

// Function to search for a patient by ID in the binary search tree
Patient* searchPatient(pTree* tree, const char* id) {
    if (tree == NULL) {  // If the tree is empty or we've reached a leaf node
        return NULL;
    }

    int cmp = strcmp(id, tree->patient->ID);  // Compare the given ID with the current node's patient ID

    if (cmp == 0) {  // If the IDs match
        return tree->patient;  // Return the patient
    } else if (cmp < 0) {  // If the given ID is less than the current node's patient ID
        return searchPatient(tree->left, id);  // Recursively search the left subtree
    } else {  // If the given ID is greater than the current node's patient ID
        return searchPatient(tree->right, id);  // Recursively search the right subtree
    }
}


// Function to display an error message based on the error code
void displayError(int errorCode) {
    switch (errorCode) {
        case 1:
            printf("Error: Failed to open file.\n");
            break;
        case 2:
            printf("Error: Memory allocation failed.\n");
            break;
        case 3:
            printf("Error: Patient not found.\n");
            break;
        case 4:
            printf("Error: Invalid input.\n");
            break;
        default:
            printf("Error: Unknown error occurred.\n");
            break;
    }
}

// Function to assign a doctor to a patient case
Doc* assignDoctor2case(DoctorNode* head) {
    if (head == NULL) {
        return NULL;  // No doctors available
    }

    DoctorNode* bestDoctor = head;  // Start with the first doctor as the best candidate

    // Iterate through the doctor list to find the one with the least patients
    for (DoctorNode* current = head->next; current != NULL; current = current->next) {
        if (current->data.nPatients < bestDoctor->data.nPatients) {
            bestDoctor = current;  // Update best doctor if current has fewer patients
        }
    }

    bestDoctor->data.nPatients++;  // Increment the patient count for the assigned doctor
    return &bestDoctor->data;  // Return the pointer to the assigned doctor
}



//menu options


//2

#include "func.h"

// Function to display a patient's allergies
void checkPatientAllergies(pTree* tree, const char* patientID) {
    // Search for the patient in the tree
    Patient* patient = searchPatient(tree, patientID);

    if (patient == NULL) {
        printf("Patient with ID %s not found.\n", patientID);
        return;
    }

    // Display the patient's allergy information
    printf("Allergies for patient %s (ID: %s): %s\n", patient->Name, patient->ID, getAllergiesStr(patient->Allergies));
}






//4 

// Function to display all patient's admissions
void displayPatientAdmissions(Patient* patient) {
    if (patient == NULL) {
        printf("Error: Patient not found.\n");
        return;
    }

    printf("Admissions for patient %s (ID: %s):\n", patient->Name, patient->ID);

    StackVisit* visit = patient->Visits;
    if (visit == NULL) {
        printf("No admissions found for this patient.\n");
        return;
    }

    // Stack to store visits in order to reverse the chronological order
    StackVisit* reversedStack = NULL;

    // Push visits to a temporary stack to reverse the order
    while (visit != NULL) {
        StackVisit* temp = (StackVisit*)malloc(sizeof(StackVisit));
        temp->visit = visit->visit;
        temp->next = reversedStack;
        reversedStack = temp;
        visit = visit->next;
    }

    // Pop from the temporary stack to display visits in reverse order
    while (reversedStack != NULL) {
        Visit* v = reversedStack->visit;
        printf("Arrival: %02d/%02d/%04d %02d:%02d\n", v->tArrival.Day, v->tArrival.Month, v->tArrival.Year, v->tArrival.Hour, v->tArrival.Min);
        printf("Dismissed: %02d/%02d/%04d %02d:%02d\n", v->tDismissed.Day, v->tDismissed.Month, v->tDismissed.Year, v->tDismissed.Hour, v->tDismissed.Min);
        printf("Duration: %02d:%02d\n", (int)v->Duration, (int)((v->Duration - (int)v->Duration) * 60));
        printf("Doctor: %s\n", v->Doctor->Name);
        printf("Summary: %s\n", v->vSummary);
        printf("========================\n");

        // Pop from the temporary stack
        StackVisit* temp = reversedStack;
        reversedStack = reversedStack->next;
        free(temp);
    }
}


//5

// Function to display all patients in line
void displayPatientsInLine(pQueue* queue) {
    if (queue == NULL || queue->front == NULL) {
        printf("No patients in line.\n");
        return;
    }

    printf("Patients in line:\n");

    pQueueNode* current = queue->front;
    while (current != NULL) {
        Patient* patient = current->patient;
        printf("Name: %s\n", patient->Name);
        printf("ID: %s\n", patient->ID);
        printf("Allergies: %s\n", getAllergiesStr(patient->Allergies));
        
        StackVisit* visit = patient->Visits;
        if (visit == NULL) {
            printf("No admissions found for this patient.\n");
        } else {
            printf("Admissions:\n");
            while (visit != NULL) {
                Visit* v = visit->visit;
                printf("  Arrival: %02d/%02d/%04d %02d:%02d\n", v->tArrival.Day, v->tArrival.Month, v->tArrival.Year, v->tArrival.Hour, v->tArrival.Min);
                printf("  Dismissed: %02d/%02d/%04d %02d:%02d\n", v->tDismissed.Day, v->tDismissed.Month, v->tDismissed.Year, v->tDismissed.Hour, v->tDismissed.Min);
                printf("  Duration: %02d:%02d\n", (int)v->Duration, (int)((v->Duration - (int)v->Duration) * 60));
                printf("  Doctor: %s\n", v->Doctor->Name);
                printf("  Summary: %s\n", v->vSummary);
                printf("  ========================\n");
                visit = visit->next;
            }
        }
        current = current->next;
        printf("------------------------------------------------\n");
    }
}




//6

// Function to advance a patient in the line to the front
void advancePatientInLine(pQueue* queue, const char* patientID) {
    if (queue == NULL || queue->front == NULL) {
        printf("No patients in line.\n");
        return;
    }

    pQueueNode *prev = NULL, *current = queue->front, *target = NULL;

    // Find the target patient in the queue
    while (current != NULL) {
        if (strcmp(current->patient->ID, patientID) == 0) {
            target = current;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (target == NULL) {
        printf("Patient with ID %s not found in the line.\n", patientID);
        return;
    }

    // If the patient is already at the front, no need to advance
    if (target == queue->front) {
        printf("Patient is already at the front of the line.\n");
        return;
    }

    // Remove the target node from its current position
    if (prev != NULL) {
        prev->next = target->next;
    }
    if (target == queue->rear) {
        queue->rear = prev;
    }

    // Move the target node to the front of the queue
    target->next = queue->front;
    queue->front = target;

    printf("Patient with ID %s has been moved to the front of the line.\n", patientID);

    // Display the updated queue
    displayPatientsInLine(queue);
}

// Assuming displayPatientsInLine is already implemented as follows:
void displayPatientsInLine(pQueue* queue) {
    if (queue == NULL || queue->front == NULL) {
        printf("No patients in line.\n");
        return;
    }

    printf("Patients in line:\n");

    pQueueNode* current = queue->front;
    while (current != NULL) {
        Patient* patient = current->patient;
        printf("Name: %s\n", patient->Name);
        printf("ID: %s\n", patient->ID);
        printf("Allergies: %s\n", getAllergiesStr(patient->Allergies));
        
        StackVisit* visit = patient->Visits;
        if (visit == NULL) {
            printf("No admissions found for this patient.\n");
        } else {
            printf("Admissions:\n");
            while (visit != NULL) {
                Visit* v = visit->visit;
                printf("  Arrival: %02d/%02d/%04d %02d:%02d\n", v->tArrival.Day, v->tArrival.Month, v->tArrival.Year, v->tArrival.Hour, v->tArrival.Min);
                printf("  Dismissed: %02d/%02d/%04d %02d:%02d\n", v->tDismissed.Day, v->tDismissed.Month, v->tDismissed.Year, v->tDismissed.Hour, v->tDismissed.Min);
                printf("  Duration: %02d:%02d\n", (int)v->Duration, (int)((v->Duration - (int)v->Duration) * 60));
                printf("  Doctor: %s\n", v->Doctor->Name);
                printf("  Summary: %s\n", v->vSummary);
                printf("  ========================\n");
                visit = visit->next;
            }
        }
        current = current->next;
        printf("------------------------------------------------\n");
    }
}


//8

// Function to display all patients assigned to a specific doctor
void displayPatientsAssignedToDoctor(pTree* tree, const char* doctorName) {
    if (tree == NULL) {
        return;
    }

    // Recursively search the tree
    displayPatientsAssignedToDoctor(tree->left, doctorName);

    Patient* patient = tree->patient;
    StackVisit* visit = patient->Visits;

    // Check if any visit was handled by the specified doctor
    bool found = false;
    while (visit != NULL) {
        if (strcmp(visit->visit->Doctor->Name, doctorName) == 0) {
            found = true;
            break;
        }
        visit = visit->next;
    }

    if (found) {
        printf("Patient Name: %s\n", patient->Name);
        printf("Patient ID: %s\n", patient->ID);
        printf("========================\n");
    }

    displayPatientsAssignedToDoctor(tree->right, doctorName);
}


//9
// Function to discharge a patient
void dischargePatient(pTree* tree, const char* patientID) {
    // Search for the patient in the tree
    Patient* patient = searchPatient(tree, patientID);

    if (patient == NULL) {
        printf("Patient with ID %s not found.\n", patientID);
        return;
    }

    // Find the most recent visit
    StackVisit* visit = patient->Visits;
    if (visit == NULL) {
        printf("No admissions found for this patient.\n");
        return;
    }

    Visit* mostRecentVisit = visit->visit;

    // Update the visit details (e.g., set dismissal time)
    // Here, you can set the dismissal time to the current time as an example
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    mostRecentVisit->tDismissed.Year = tm.tm_year + 1900;
    mostRecentVisit->tDismissed.Month = tm.tm_mon + 1;
    mostRecentVisit->tDismissed.Day = tm.tm_mday;
    mostRecentVisit->tDismissed.Hour = tm.tm_hour;
    mostRecentVisit->tDismissed.Min = tm.tm_min;

    printf("Patient %s (ID: %s) has been discharged.\n", patient->Name, patient->ID);
    printf("Discharge details:\n");
    printf("Dismissed: %02d/%02d/%04d %02d:%02d\n", mostRecentVisit->tDismissed.Day, mostRecentVisit->tDismissed.Month, mostRecentVisit->tDismissed.Year, mostRecentVisit->tDismissed.Hour, mostRecentVisit->tDismissed.Min);
    printf("========================\n");

    // Display updated patient information
    printf("Updated patient information:\n");
    printf("Name: %s\n", patient->Name);
    printf("ID: %s\n", patient->ID);
    printf("Allergies: %s\n", getAllergiesStr(patient->Allergies));
    
    visit = patient->Visits;
    if (visit == NULL) {
        printf("No admissions found for this patient.\n");
    } else {
        printf("Admissions:\n");
        while (visit != NULL) {
            Visit* v = visit->visit;
            printf("  Arrival: %02d/%02d/%04d %02d:%02d\n", v->tArrival.Day, v->tArrival.Month, v->tArrival.Year, v->tArrival.Hour, v->tArrival.Min);
            printf("  Dismissed: %02d/%02d/%04d %02d:%02d\n", v->tDismissed.Day, v->tDismissed.Month, v->tDismissed.Year, v->tDismissed.Hour, v->tDismissed.Min);
            printf("  Duration: %02d:%02d\n", (int)v->Duration, (int)((v->Duration - (int)v->Duration) * 60));
            printf("  Doctor: %s\n", v->Doctor->Name);
            printf("  Summary: %s\n", v->vSummary);
            printf("  ========================\n");
            visit = visit->next;
        }
    }
}


//12

// Function to free all visits for a patient
void freeVisits(StackVisit* visit) {
    while (visit != NULL) {
        StackVisit* temp = visit;
        visit = visit->next;
        free(temp->visit);
        free(temp);
    }
}

// Function to free all patients in the tree
void freePatients(pTree* tree) {
    if (tree == NULL) {
        return;
    }

    freePatients(tree->left);
    freePatients(tree->right);

    freeVisits(tree->patient->Visits);
    free(tree->patient->Name);
    free(tree->patient);
    free(tree);
}

// Function to free all doctors in the list
void freeDoctors(DoctorNode* head) {
    while (head != NULL) {
        DoctorNode* temp = head;
        head = head->next;
        free(temp->data.Name);
        free(temp);
    }
}

// Function to free all patients in the queue
void freeQueue(pQueue* queue) {
    while (queue->front != NULL) {
        pQueueNode* temp = queue->front;
        queue->front = queue->front->next;
        free(temp);
    }
    queue->rear = NULL;
}

// Function to close the hospital and delete all data
void closeHospital(pTree** patientTree, pQueue* patientQueue, DoctorNode** doctorList) {
    // Free all patients in the tree
    freePatients(*patientTree);
    *patientTree = NULL;

    // Free all patients in the queue
    freeQueue(patientQueue);

    // Free all doctors in the list
    freeDoctors(*doctorList);
    *doctorList = NULL;

    printf("Hospital closed and all data deleted.\n");
}


//0

// Function to exit the program and save all data
void exitProgram(pTree* patientTree, pQueue* patientQueue, DoctorNode* doctorList) {
    // Save all updated data to the corresponding files
    updateFiles(patientTree, patientQueue, doctorList);

    // Free all allocated memory
    closeHospital(&patientTree, patientQueue, &doctorList);

    printf("Data saved and program exited successfully.\n");
}





