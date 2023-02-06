#include <stdio.h>
#include "singlelist.h"
#include <stdlib.h>

int main() {
  struct list_item root;
    root.value = -1; // set first node (root node) to value -1
    root.next = NULL; // set next node to null

    append(&root, 10);
    append(&root, 9);
    append(&root, 8);
    append(&root, 7);
    append(&root, 6);
    append(&root, -5);

    print(&root);

    prepend(&root,-4);
    print(&root);

    input_sorted(&root,-3);
    print(&root);

   clear(&root);
    append(&root,10);
    print(&root);

    
    return 0;
};