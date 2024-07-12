#include "svisit.h"

StackVisit* newNode(Visit data)
{
    StackVisit* stackVisit =
        (StackVisit*)
        malloc(sizeof(StackVisit));
    stackVisit->data = data;
    stackVisit->next = NULL;
    return stackVisit;
}

int isEmpty(StackVisit* root)
{
    return !root;
}

void push(StackVisit** root, Visit data)
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
    Visit popped = temp->data;
    free(temp);

    return popped;
}

Visit peek(StackVisit* root)
{
    if (isEmpty(root)) {
        Visit emptyVisit = { 0 }; // Return an empty visit if the stack is empty
        return emptyVisit;
    }
    return root->data;
}
