#include "func.h"

int main() {
    DoctorNode* doctorList = NULL;
    loadDoctors(&doctorList);

    // Print the loaded doctors
    DoctorNode* current = doctorList;
    while (current) {
        printf("Full Name: %s, License Number: %s, Number of Patients: %d\n",
            current->data.Name, current->data.nLicense, current->data.nPatients);
        current = current->next;
    }

    // Clean up
    deleteDoctorList(&doctorList);

    printf("\n\n");

    pTree* root = NULL;
    loadPatients(&root);

    //preOrderPrint(root->root);


    return 0;
}