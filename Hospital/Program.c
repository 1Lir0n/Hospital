#include "func.h"

int main() {
    DoctorNode* doctorList = NULL;
    loadDoctors(&doctorList);

    // Print the loaded doctors
    printDoctors(doctorList);


    printf("\n\n");

    pTree* patientTree = createPatientTree();
    loadPatients(doctorList ,&patientTree);
    
    printAllPatients(&patientTree);
    // Clean up
    deleteDoctorList(&doctorList);
    return 0;
}