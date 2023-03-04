#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
#include "threads/interrupt.h"
#include "userprog/process.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

void init (void);
void halt(void);
int open(const char *name);
bool create(const char *file, unsigned initial_size);
void close(int fd);
int read (int fd, const void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
tid_t exec(const char *cmd_line); 
void exit(int status);
int wait(tid_t pid);
void seek(int fd, unsigned position);
unsigned tell(int fd);
int filesize(int fd);
bool remove(const char *file);

/*        validation            */
void validate_pointer(const void *vaddr, unsigned size);
void validate_string(const char *str);
bool check_bounds(const void *vaddr);
#endif /* userprog/syscall.h */
