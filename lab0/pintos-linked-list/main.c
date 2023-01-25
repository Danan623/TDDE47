#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	char *name;
	struct list_elem elem;
};

//Insert at the end of list
void insert (struct list *student_list) {
	char *ptr = (char*) malloc(sizeof(char)*20); //Allocate memory for storing the name in student.
	struct student *newstudent = malloc(sizeof(struct student)); //Allocate memory for student struct.
	scanf("%s", ptr); //Assign input from terminal to name (ptr)
	newstudent->name = ptr;  //Set structs name to the input name.
	list_push_back (student_list, &newstudent->elem); //Insert into list.
}

//Find student in list and free from list.
void delete (struct list *student_list) {
	char delname[20]; //Stack variable.
	printf("Enter name to delete: ");
	scanf("%s", delname); //Assign name to variable from terminal input.
	struct list_elem *temp; //ptr to iterate through list.

	 for (temp = list_begin (student_list); temp != list_end (student_list);temp = list_next (temp))
        {
	   struct student *comparename = list_entry(temp, struct student, elem); //get student struct
        if (strcmp(comparename->name,delname) == 0) { //compare input name with student structs name
			list_remove(temp); //attach current->prev to current->next
			free(comparename->name); //dealloc memory
			free(comparename); //dealloc memory
			return; //return if match found and done
		}
        }
	printf("No match found! \n");
}
//Print student list.
void list (struct list *student_list) {
	if (!list_empty(student_list)){ //While we still have items to print
		struct list_elem *temp; //initial pointer
		for (temp = list_begin (student_list); temp != list_end(student_list); temp = list_next(temp)) {
			struct student *std = list_entry (temp, struct student, elem); //get student struct from pointer with list_entry
			printf("%s \n", std->name); //print 
		
		}
	} else {
		printf("Empty \n");
	}
}

void quit (struct list *student_list) {
	while (!list_empty (student_list))
     {
       struct list_elem *ptr = list_pop_front (student_list); //Get front element
	   struct student *std = list_entry(ptr, struct student, elem); //get struct student from ptr
	   free(std->name); //free student structs name memalloc
		free(std); //free student struct memalloc
     }
	// free(student_list);
	 exit(0);
}

int main() {
	struct list student_list;
	list_init (&student_list);
	int opt;

	do {
		printf("Menu:\n");
		printf("1 - Insert student\n");
		printf("2 - Delete student\n");
		printf("3 - List students\n");
		printf("4 - Exit\n");
		scanf("%d", &opt);
		switch (opt) {
			case 1:
				{
					insert(&student_list);
					break;
				}
			case 2:
				{
					delete(&student_list);
					break;
				}
			case 3:
				{
					list(&student_list);
					break;
				}

			case 4:
				{
					quit(&student_list);
					break;
				}
			default:
				{
					printf("Quit? (1/0):\n");
					scanf("%d", &opt);
					if (opt)
						quit(&student_list);
					break;
				}
		}
	} while(1);
		
	return 0;
}
