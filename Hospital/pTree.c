#include "ptree.h"
#include <stdlib.h>
#include <string.h>

// Helper function to create a new tree node
static pInTree* createTreeNode(Patient* patient) {
    pInTree* newNode = (pInTree*)malloc(sizeof(pInTree));
    if (newNode) {
        newNode->tpatient = patient;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

pTree* createPatientTree() {
    pTree* tree = (pTree*)malloc(sizeof(pTree));
    if (tree) {
        tree->root = NULL;
    }
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

void inOrderPrint(pInTree* node) {
    if (node) {
        inOrderPrint(node->left);
        printf("Name:%s, ID:%s", node->tpatient->Name, node->tpatient->ID);
        inOrderPrint(node->right);
    }
}

void preOrderPrint(pInTree* node) {
    if (node) {
        printf("Name:%s, ID:%s", node->tpatient->Name, node->tpatient->ID);
        preOrderPrint(node->left);
        preOrderPrint(node->right);
    }
}

void postOrderPrint(pInTree* node) {
    if (node) {
        postOrderPrint(node->left);
        postOrderPrint(node->right);
        printf("Name:%s, ID:%s",node->tpatient->Name,node->tpatient->ID);
    }
}
