#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
#include "threads/interrupt.h"
void init (void);
void halt(void);
int open(const char *name);
bool create(const char *file, unsigned initial_size);
void close(int fd);
int read (int fd, const void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);

#endif /* userprog/syscall.h */
