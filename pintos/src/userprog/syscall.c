#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/off_t.h"
#define STDIN_FILENO 0 // if fd = 0 - std input
#define STDOUT_FILENO 1 // if fd = 1 - std output
#define STDERR_FILENO 2 // if fd = 2 - std error

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

// This function serves as the handler for system calls made by the user program
// Interrupt frame, f, contains information about the system call made
static void
syscall_handler (struct intr_frame *f UNUSED) // interrupt frame arg. which contains the info about the system call that was made
{
  // Get the syscall number that corresponds to the requested system call
  int syscall_num = *(int*) f->esp;

  // File descriptor number
  int fd = *(int*) (f->esp + 4);

  // Address of user space buffer for reading/writing
  const void *buffer = *(void**) (f->esp + 8);

  // Size of the third argument object
  unsigned size_arg3 = *(unsigned*) (f->esp + 12);

  // Address of file name
  const char *file = *((char**)(f->esp + 4));

  // Size of the second argument object
  unsigned size_arg2 = *((unsigned*)(f->esp + 8));

  switch(syscall_num) {
    case SYS_HALT:
    // Shuts down the system, using the power off function declared in threads/init.h
    halt();
    break;
    case SYS_EXIT:
    // Terminates the current process
    exit();
    break;
    case SYS_EXEC:
    // Start another process
    break;
    case SYS_WAIT:
    // Wait for a child process to die
    break;
    case SYS_CREATE:
    // Create a file, returns true on success and false on failure
    f->eax = create(file, size_arg2);
    break;
    case SYS_REMOVE:
    // Delete a file
    break;
    case SYS_OPEN:
    // Open a file, returns the file descriptor on success and -1 on failure
    f->eax = open(file);
    break;
    case SYS_FILESIZE:
    // Obtain the size of a file
    break;
    case SYS_READ:
    // Read from a file, returns the number of bytes read on success and -1 on failure
    f->eax = read(fd, buffer, size_arg3);
    break;
    case SYS_WRITE:
    // Write to a file, returns the number of bytes written on success and -1 on failure
    f-> eax = write(fd, buffer, size_arg3);
    break;
    case SYS_SEEK:
    // Change the position in a file
    break;
    case SYS_TELL:
    // Report the current position in a file
    break;
    case SYS_CLOSE:
    // Close a file
    close(fd);
    break;
  }
}

void halt() {
  power_off();
}

void exit() {
  struct thread* thread = thread_current();
  thread_exit();
}

//Creates a new file called file initially initial size bytes in size. Returns true
//if successful, false otherwise.
bool create(const char *file, unsigned initial_size){
  return filesys_create(file, initial_size);
}

int open (const char *name) {
  struct file *file = filesys_open(name); //Returns file with input name, returns -1 if no file with name exists.
  if (file == NULL){
    return -1; //Return if file didnt exist.
  } 
  struct thread* thread = thread_current(); //Get current thread
  for (int i = 2; i < 130; i++) { // 0 and 1 reserved for terminal. Iterate over 2-129 to find first free index.
    if (thread->file_descriptor[i] == NULL) { //If free index exists.
      thread->file_descriptor[i] = file ; //Place file in the index.
      return i; //Return index int.
    }
  }
}

/*Closes file descriptor fd. Exiting or terminating a process implicitly closes all
its open file descriptors, as if by calling this function for each one.*/
void close(int fd) {
  if(fd < 0){ //Return if invalid filedescriptor.
    return;
  }
  struct thread *thread = thread_current(); //Get current thread
  struct file *file = thread->file_descriptor[fd]; //Get file pointer
  thread->file_descriptor[fd] = NULL; //Set the index to null
  file_close(file); //Close the file at given index fd
}

/**
int read (int fd, const void *buffer, unsigned size) :
This function reads number of bytes (the size) from the file open as file descriptor "fd"
and stores it in the buffer pointed to by "buffer".

The function returns the number of bytes read, 
or -1 if the file could not be read due to a condition other than end of file.

If "fd" is 0, the function reads from the keyboard using the "input_getc" function
defined in the file "devices/input.h".
*/
int read (int fd, const void *buffer, unsigned size){

  // Check if file descriptor is invalid
  if(fd < 0 || fd == STDOUT_FILENO || fd >= 130){
    // Return error code -1
    return -1; 
  }

  int bytes_read = 0;

  // Read from keyboard if file descriptor is 0
  if (fd == STDIN_FILENO) {
    // Read number of bytes from keyboard
    while (bytes_read < (int)size) {
      // Store the read character in the buffer
      ((uint8_t*)buffer)[bytes_read++] = input_getc();
    }
    // Return number of bytes read
    return size;
  }

  // Get the current thread
  struct thread *thread = thread_current();
  // Get the file associated with the file descriptor
  struct file *file = thread->file_descriptor[fd];

  // Check if the file is empty
  if(file == NULL){
    // Return error code -1
    return -1;
  }
  
  // Read from the file
  bytes_read = file_read(file, buffer, size);
    
  // Return number of bytes actually read
  return bytes_read;
}


/**
int write (int fd, const void *buffer, unsigned size) - 
writes size bytes from buffer to the open file referred to by the file descriptor fd.
It returns the number of bytes actually written or -1 if the file could not be written.

The function implements the behavior of writing as many bytes as possible up to end-of-file
and returns the number of bytes written or -1 if no bytes could be written.

If fd = STDOUT_FILENO, the system call writes to the console. 
*/
int write (int fd, const void *buffer, unsigned size) {
  // check if the file descriptor is invalid (less than 0), standard input (STDIN_FILENO), or greater than or equal to 130
  if (fd < 0 || fd == STDIN_FILENO || fd >= 130) {
    // return -1 if the file descriptor is invalid
    return -1; 
  }

  // if the file descriptor is equal to standard output (STDOUT_FILENO), write to the console
  if (fd == STDOUT_FILENO) {
    // write to console using putbuf function
    putbuf((char*)buffer, size); 
    // return the number of bytes written
    return size; 
  }
  // get the current thread
  struct thread *thread = thread_current();
  // get the file associated with the file descriptor
  struct file *file = thread->file_descriptor[fd];
  if (file == NULL) return -1; //if file doesnt exist we dont want to write to it
  // write to the file and get the number of bytes written
  int bytes_written = file_write(file, buffer, (off_t)size);
  // return the number of bytes written
  return bytes_written;
}