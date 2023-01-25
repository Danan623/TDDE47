#include <stdio.h>
#include "singlelist.h"
#include <stdlib.h>

int main() {
    struct list_item root;
    root.value = -1;
    root.next = NULL;
    append(&root, 5);
    append(&root, 7);
    prepend(&root, 3);
    input_sorted(&root, 6);
    input_sorted(&root, 2);
    print(&root);
    clear(&root);
    print(&root);
    append(&root, 8);
    clear(&root);
  //  append(&root, 3);
    print(&root);
    return 0;
};