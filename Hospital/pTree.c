#include "ptree.h"
#include <stdlib.h>
#include <string.h>
#include "svisit.h"
#include "patients.h"
// Helper function to create a new tree node
static pInTree* createTreeNode(Patient* patient) {
    pInTree* node = (pInTree*)malloc(sizeof(pInTree));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory for TreeNode\n");
        exit(EXIT_FAILURE);
    }
    node->tpatient = patient;  // Initialize patient data
    node->left = NULL;
    node->right = NULL;
    return node;
}

pTree* createPatientTree() {
    pTree* tree = (pTree*)malloc(sizeof(pTree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }
    tree->root = NULL;
    return tree;
}

// Recursively destroy all nodes in the tree
void destroyPatientTreeNodes(pInTree* node) {
    if (node) {
        destroyPatientTreeNodes(node->left);
        destroyPatientTreeNodes(node->right);
        free(node);
    }
}

void destroyPatientTree(pTree* tree) {
    if (tree) {
        destroyPatientTreeNodes(tree->root);
        free(tree);
    }
}

pInTree* insertPatient(pTree* tree, Patient* patient) {
    pInTree** current = &tree->root;
    while (*current) {
        if ((*current)->tpatient != NULL) {
            int cmp = strcmp(patient->ID, (*current)->tpatient->ID);
            if (cmp < 0) {
                current = &(*current)->left;
            }
            else if (cmp > 0) {
                current = &(*current)->right;
            }
            else {
                // Duplicate ID, do not insert
                return NULL;
            }
        }
        else break;
    }
    *current = createTreeNode(patient);
    return *current;
}

// Recursively find a patient node by ID
pInTree* findPatientNode(pInTree* node, const char* id) {
    if (!node) {
        return NULL;
    }
    int cmp = strcmp(id, node->tpatient->ID);
    if (cmp < 0) {
        return findPatientNode(node->left, id);
    }
    else if (cmp > 0) {
        return findPatientNode(node->right, id);
    }
    else {
        return node;
    }
}


pInTree* findPatient(pTree* tree, const char* id) {
    return findPatientNode(tree->root, id);
}

// Helper function to find the minimum node in a subtree
static pInTree* findMin(pInTree* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

// Helper function to delete a node by ID
pInTree* deletePatientNode(pInTree* node, const char* id) {
    if (!node) {
        return NULL;
    }
    int cmp = strcmp(id, node->tpatient->ID);
    if (cmp < 0) {
        node->left = deletePatientNode(node->left, id);
    }
    else if (cmp > 0) {
        node->right = deletePatientNode(node->right, id);
    }
    else {
        if (!node->left) {
            pInTree* rightChild = node->right;
            free(node);
            return rightChild;
        }
        else if (!node->right) {
            pInTree* leftChild = node->left;
            free(node);
            return leftChild;
        }
        else {
            pInTree* minRight = findMin(node->right);
            node->tpatient = minRight->tpatient;
            node->right = deletePatientNode(node->right, minRight->tpatient->ID);
        }
    }
    return node;
}

void deletePatient(pTree* tree, const char* id) {
    if (tree) {
        tree->root = deletePatientNode(tree->root, id);
    }
}
// Function to print patient details (modify as per your structure)
void printPatient(Patient* patient) {
    if (patient == NULL) {
        printf("Patient data is NULL\n");
        return;
    }
    printf("Name: %s, ID:%s, Allergies: %s\n\nVisits:\n", patient->Name, patient->ID, getAllergies(*patient));
    printAllVisits(patient->Visits);
    printf("=========================\n");
}


// Recursive function to perform in-order traversal and print patients
void printPatientsInOrder(pInTree* node) {
    if (node == NULL) return;
    printPatientsInOrder(node->left);
    printPatient(node->tpatient);
    printPatientsInOrder(node->right);
}

// Function to print all patients in the tree
void printAllPatients(pTree* tree) {
    printf("Printing all patients:\n");
    printPatientsInOrder(tree->root);
}
