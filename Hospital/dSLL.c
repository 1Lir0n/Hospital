#include "dsll.h"

// Create a new doctor node
DoctorNode* createDoctorNode(Doc data) {
    DoctorNode* newNode = (DoctorNode*)malloc(sizeof(DoctorNode));
    if (newNode) {
        newNode->data.Name = _strdup(data.Name); // Duplicate the doctor's name
        strncpy(newNode->data.nLicense, data.nLicense, sizeof(newNode->data.nLicense) - 1);
        newNode->data.nLicense[sizeof(newNode->data.nLicense) - 1] = '\0';
        newNode->data.nPatients = data.nPatients;
        newNode->next = NULL;
    }
    return newNode;
}

// Append a doctor to the end of the list
void appendDoctor(DoctorNode** head, Doc data) {
    DoctorNode* newNode = createDoctorNode(data);
    if (!newNode) return;

    if (*head == NULL) {
        *head = newNode;
    }
    else {
        DoctorNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Delete the entire doctor list
void deleteDoctorList(DoctorNode** head) {
    DoctorNode* current = *head;
    DoctorNode* next;

    while (current != NULL) {
        next = current->next;
        free(current->data.Name); // Free the doctor's name
        free(current);
        current = next;
    }

    *head = NULL;
}

// Find a doctor by license number
DoctorNode* findDoctor(DoctorNode* head, const char* license) {
    DoctorNode* current = head;
    while (current != NULL) {
        if (strcmp(current->data.nLicense, license) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Delete a doctor by license number
void deleteDoctor(DoctorNode** head, const char* license) {
    DoctorNode* temp = *head;
    DoctorNode* prev = NULL;

    // If the doctor to be deleted is the head node
    if (temp != NULL && strcmp(temp->data.nLicense, license) == 0) {
        *head = temp->next;
        free(temp->data.Name);
        free(temp);
        return;
    }

    // Search for the doctor to be deleted, keep track of the previous node
    while (temp != NULL && strcmp(temp->data.nLicense, license) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // If the doctor was not present in the list
    if (temp == NULL) return;

    // Unlink the node from the list
    prev->next = temp->next;

    free(temp->data.Name);
    free(temp);
}
