#include <stdio.h>
#include <stdlib.h>
#include "singlelist.h"

    /*
    puts x at the end of the list
    */
    void append(struct list_item *first, int x){
        // allokera minne : skapa en instans av structen och använd malloc för att allokera mer minne vid varje insättning, strl = strl av structen efter ny insättning
        // skapa en instans av root minnet för att veta vart "HEAD" är dvs position 0
        // loopa från position 0 till "struct.length", sätt därefter in det nya element på den tomma positionen (pointer == null)
        // return

        // no need to continue if the list is empty
        if(first == NULL){
            return;
        } 
        // create an instance of the struct "list_item", use malloc to allocate more memory for every append. size = size of the struct.
        struct list_item *new_node = (struct list_item*)malloc(sizeof(struct list_item));
        // create an instance ... , first is the root node (pos 0 in the list). this will give us the entry to the list in memory. 
        struct list_item *parent_node = first; // root node
        // assign the argument x to value and set the next node to empty (null) in the list
        new_node->value = x;
        new_node->next = NULL;

        // iterate from position 0 to end of list
        while(parent_node->next != NULL){    
            parent_node = parent_node->next;  
        }
        // assign next node = new node (value = x , next = null)
        parent_node->next = new_node;
        
        return; // it's a void so we return nothing

    }
    /*
    puts x at the beginning of the list
    */
    void prepend(struct list_item *first, int x){
    // allocate memory for one more node
    struct list_item *new_child = (struct list_item*)malloc(sizeof(struct list_item));
    // new_child value
    new_child->value = x;
    // assign the root node's ->next node to the new_child node 
    new_child->next = first->next;
    // assign new_child node to the old node's -> next node 
    first->next = new_child;
    return;
    }
    /*
    prints all elements in the list
    */
    void print(struct list_item *first){
        // skapa en instans till listan
        // iterera från pos 0 till end
        // printa ut varje pos
        // return
        struct list_item *root_node = first->next;
        printf("%s","[");
        while(root_node != NULL){
            printf(" %d",root_node->value);
            root_node = root_node->next;
        }
        printf("%s"," ]");
        
        return;
    }
    
    /*
    input sorted: find the first element in the list larger than x 
    and input x right before that element
    */
    void input_sorted(struct list_item *first, int x){
        // not sure if we want to check this, the first should be assigned.. it's static
        if(first == NULL){
            return;
        }
        struct list_item *parent_node = first; 
        // we will iterate and check one step ahead before moving to next node
        // that will give us the advantage of prepend
        while((parent_node->next) != NULL && (parent_node->next)->value < x){
            parent_node = parent_node->next;
        }
        // if next mem address is empty or element is bigger then we should assign the new node there 
        prepend(parent_node,x);
        return;
    }
    /*
    free everything dynamically allocated
    */
    void clear(struct list_item *first){
        // create instance variable and assign position 1 in the list to it
        struct list_item *parent_node = first->next;
        // create a tmp instance variable
        struct list_item *tmp_node;
        // iterate from 1 to end of list
        while(parent_node != NULL){
            // save next mem location of the parent node
            tmp_node = parent_node->next;
            // free all the dynamically allocated mem of the list
            free(parent_node);
            // assign next mem location to the parent node
            parent_node = tmp_node;
        }
        // "free" the allocated memory of position 1
        first->next = NULL;
        // note : the root node (position 0) is a static allocated node and should not be freed

        return;
}