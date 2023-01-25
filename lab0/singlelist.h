//#include <stdio.h>
//#include <stdlib.h>
#ifndef SINGLELIST_H_
#define SINGLELIST_H_


struct list_item {
    int value;
    struct list_item *next;
};

//put x at the end of the list
void append(struct list_item *first, int x);
//put x at the beginning of the list
void prepend(struct list_item *first, int x);
//prints all elements in the list
void print (struct list_item *first);
//find the first element in the list larger than x and input x right before that element
void input_sorted(struct list_item *first, int x);
//free everything dynamically allocated
void clear (struct list_item *first);

#endif