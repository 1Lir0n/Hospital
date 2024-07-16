#include "svisit.h"

StackVisit* newNode(Visit* data)
{
    StackVisit* node = (StackVisit*)malloc(sizeof(StackVisit));
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

int isEmpty(StackVisit* root)
{
    return !root;
}

void push(StackVisit** root, Visit* data)
{
    StackVisit* stackVisit = newNode(data);
    stackVisit->next = *root;
    *root = stackVisit;
    //debug
    //printf("%d pushed to stack\n", data);
}

Visit pop(StackVisit** root)
{
    if (isEmpty(*root)) {
        Visit emptyVisit = { 0 }; // Return an empty visit if the stack is empty
        return emptyVisit;
    }
    StackVisit* temp = *root;
    *root = (*root)->next;
    Visit popped = *temp->data;
    free(temp);

    return popped;
}

Visit peek(StackVisit* root)
{
    if (isEmpty(root)) {
        Visit emptyVisit = { 0 }; // Return an empty visit if the stack is empty
        return emptyVisit;
    }
    return *root->data;
}

// Function to print a visit
void printVisit(Visit* visit) {
    if (visit == NULL) return;
    printf("Arrival: %02d-%02d-%04d %02d:%02d\n", visit->tArrival->Day, visit->tArrival->Month, visit->tArrival->Year, visit->tArrival->Hour, visit->tArrival->Min);
    if (visit->tDismissed->Day != 0)
        printf("Dismissed: %02d-%02d-%04d %02d:%02d\n", visit->tDismissed->Day, visit->tDismissed->Month, visit->tDismissed->Year, visit->tDismissed->Hour, visit->tDismissed->Min);
    else printf("Dismissed: TBD\n");
    if (visit->Duration != 0)
        printf("Duration: %.2f\n", visit->Duration);
    else printf("Duration: TBD\n");
    printf("Doctor: %s\n", visit->Doctor->Name);
    if (visit->Summary) {
        printf("Summary: %s\n", visit->Summary);
    }
    printf("\n");
}

// Function to print all visits in the stack without destroying it
void printAllVisits(StackVisit* stack) {
    int index = 1;
    StackVisit* current = stack;
    while (current != NULL) {
        printf("%d.", index);
        printVisit(current->data);
        current = current->next;
        index++;
    }
}