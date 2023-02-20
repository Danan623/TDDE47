#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
#include "threads/interrupt.h"
#include "userprog/process.h"

void init (void);
void halt(void);
int open(const char *name);
bool create(const char *file, unsigned initial_size);
void close(int fd);
int read (int fd, const void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
tid_t exec(const char *cmd_line); 
void exit(int status);

#endif /* userprog/syscall.h */
