#include <stdio.h>
#include <stdlib.h>
#include "singlelist.h"

//put x at the end of the list
void append(struct list_item *first, int x){
    struct list_item *link = (struct list_item*) malloc(sizeof(struct list_item)); //allocate memory for pointer between structs
    struct list_item *ptr = first; //ptr to root item

    while(ptr->next != NULL) { //while there are still items to iterate over
       ptr = ptr->next;
    }

    ptr->next = link; //Set link-address for previous node to new node.
    link->value = x; //Set value in new node to x.
    link->next = NULL; //Set link-adress for new node to null.
};
//put x at the beginning of the list
void prepend(struct list_item *first, int x){
    struct list_item *link = (struct list_item*) malloc(sizeof(struct list_item)); //Create new node
    link->value = x; //Assign value to node

    struct list_item *ptr = first; //Set pointer to root nodes adress.
    link->next = first->next; //Set what new node points to the same as what root points to.
    //first->next = link; //Make root point to new node.
    ptr->next = link; //Make root point to new node.
};
//prints all elements in the list
void print (struct list_item *first){
    struct list_item *temp = first; //set pointer to root

    while(temp != NULL) { //while we have items to iterate over
        printf("%d - ", temp->value); //write value of node
        temp = temp->next; // go to next node
    }
    printf("\n");
    free(temp); //deallocate memory for the node

};
//find the first element in the list larger than x and input x right before that element
void input_sorted(struct list_item *first, int x) {
    struct list_item *ptr = first; //set pointer to root
    while (x > ptr->next->value) { //always ignore root so ptr->next and this nodes value so ->value
        ptr = ptr->next; //go to next node
    }
    prepend(ptr, x); //reuse prepend
};
//free everything dynamically allocated
void clear (struct list_item *first){
    struct list_item *ptr = first->next; //ignore root
    struct list_item *prev; //ptr to save previous address
    first->next = NULL; //clear first node after root
    while (ptr != NULL) { //while we still have items to iterate over
        prev = ptr; //save address
        ptr = ptr->next; //go to next node
        free(prev); //deallocate memory
    } 
};