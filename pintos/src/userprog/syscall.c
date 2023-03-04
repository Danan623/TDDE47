#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/off_t.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

#define STDIN_FILENO 0 // if fd = 0 - std input
#define STDOUT_FILENO 1 // if fd = 1 - std output

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

//This function serves as the handler for system calls made by the user program
// Interrupt frame, f, contains information about the system call made
static void
syscall_handler (struct intr_frame *f UNUSED) // interrupt frame arg. which contains the info about the system call that was made
{
  validate_pointer(f->esp, sizeof(int)); // check if the stack pointer is a valid user pointer

  // Get the syscall number that corresponds to the requested system call
  int syscall_num = *((int*) f->esp);
  
  // Check if the system call number is valid (syscall.nr.h defines the valid system call numbers)
  /*
  if(syscall_num < 0 || syscall_num > 12){
    // Return error code -1
    f->eax = -1;  //eax is the return value of the system call
    return;
  }
  */
  

  // File descriptor number
  int fd; // = *(int*) (f->esp + 4);

  // Address of user space buffer for reading/writing
  const void *buffer; // = *(void**) (f->esp + 8);

  // Size of the third argument object
  unsigned size_arg3; // = *(unsigned*) (f->esp + 12);

  // Address of file name
  const char *file; // = *((char**)(f->esp + 4));

  // Size of the second argument object
  unsigned size_arg2; // = *((unsigned*)(f->esp + 8));

  // cmd line argument
  const char *cmd_line; // = *((char**)(f->esp + 4));
  
  // tid argument
  tid_t tid; // = *((tid_t*)(f->esp + 4));

  switch(syscall_num) {
    case SYS_HALT:
    // Shuts down the system, using the power off function declared in threads/init.h
    halt();
    break;

    case SYS_EXIT: // Terminates the current process and returns the status to the kernel (0 if exited normally)
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number
    exit(fd); // exit the current process
    //exit(thread_current()->status);
    break;

    case SYS_EXEC:
    // Executes the program with the given command line
    validate_pointer(f->esp + 4, sizeof(char*)); // check if the stack pointer is a valid user pointer
    cmd_line = *((char**)(f->esp + 4)); // get the command line argument
    validate_string(cmd_line); // check if the command line argument is a valid user string
    
    f->eax = exec(cmd_line);
    break;

    case SYS_WAIT:
    // Wait for a child process to die
    validate_pointer(f->esp + 4, sizeof(tid_t)); // check if the stack pointer is a valid user pointer
    tid = *((tid_t*)(f->esp + 4)); // get the tid argument
    
    f->eax = wait(tid);
    break;

    case SYS_CREATE:
    // Create a file, returns true on success and false on failure
    validate_pointer(f->esp + 4, sizeof(char*)); // check if the stack pointer is a valid user pointer
    file = *((char**)(f->esp + 4)); // get the address of the file name
    validate_string(file); // check if the file name is a valid user string
    validate_pointer(f->esp + 8, sizeof(unsigned)); // check if the stack pointer is a valid user pointer
    size_arg2 = *((unsigned*)(f->esp + 8)); // get the size of the arg2 object

    f->eax = create(file, size_arg2);
    break;

    case SYS_REMOVE:
    // Delete a file and returns true on success and false on failure
    validate_pointer(f->esp + 4, sizeof(char*)); // check if the stack pointer is a valid user pointer
    file = *((char**)(f->esp + 4)); // get the address of the file name
    validate_string(file); // check if the file name is a valid user string

    f->eax = remove(file);
    break;

    case SYS_OPEN:
    // Open a file, returns the file descriptor on success and -1 on failure
    validate_pointer(f->esp + 4, sizeof(char)); // check if the stack pointer is a valid user pointer
    file = *((char**)(f->esp + 4)); // get the address of the file name
    validate_string(file); // check if the file name is a valid user string

    f->eax = open(file);
    break;

    case SYS_FILESIZE:
    // Obtain the size of a file in bytes, returns the size on success and -1 on failure
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number

    f->eax = filesize(fd);
    break;

    case SYS_READ:
    // Read from a file, returns the number of bytes read on success and -1 on failure
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number
    validate_pointer(f->esp + 8, sizeof(char)); // check if the stack pointer is a valid user pointer
    buffer = *(void**) (f->esp + 8); // get the address of the user space buffer for reading/writing
    validate_string(buffer);
    validate_pointer(f->esp + 12, sizeof(unsigned)); // check if the stack pointer is a valid user pointer
    size_arg3 = *(unsigned*) (f->esp + 12); // get the size of the arg3 object

    f->eax = read(fd, buffer, size_arg3);
    break;

    case SYS_WRITE:
    // Write to a file, returns the number of bytes written on success and -1 on failure
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number
    validate_pointer(f->esp + 8, sizeof(const char)); // check if the stack pointer is a valid user pointer
    buffer = *(void**) (f->esp + 8); // get the address of the user space buffer for reading/writing
    validate_string(buffer);
    validate_pointer(f->esp + 12, sizeof(unsigned)); // check if the stack pointer is a valid user pointer
    size_arg3 = *(unsigned*) (f->esp + 12); // get the size of the arg3 object

    f-> eax = write(fd, buffer, size_arg3);
    break;

    case SYS_SEEK:
    // Change the position in a file  (offset bytes from the start of the file)
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number
    validate_pointer(f->esp + 8, sizeof(unsigned)); // check if the stack pointer is a valid user pointer
    size_arg2 = *(unsigned*) (f->esp + 8); // get the size of the arg2 object

    seek(fd, size_arg2);
    break;

    case SYS_TELL:
    // Report the current position in a file (offset from the start of the file)
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number

    break;
    
    case SYS_CLOSE:
    // Close a file
    validate_pointer(f->esp + 4, sizeof(int)); // check if the stack pointer is a valid user pointer
    fd = *(int*) (f->esp + 4); // get the file descriptor number

    close(fd);
    break;
  }
}

//sleep the parent until child finishes and return the child’s exit status to the parent
int wait(tid_t child_tid) {
  return process_wait(child_tid); //Wait for child process to finish and return exit status
}
void halt() {
  power_off();  //Power off
}
/* terminate the current user program, returning status to the kernel*/
void exit(int status) {
  struct thread *thread = thread_current();
  thread->family->exit_status = status; // Set exit status of current thread
  char *token, *save_ptr;
  token = strtok_r (thread->name, " ", &save_ptr);
  printf("%s: exit(%d)\n", token, status);
  thread_exit();  

}

tid_t exec(const char *cmd_line) {
  return process_execute(cmd_line); //Execute process with input cmd_line
}
//Creates a new file called file initially initial size bytes in size. Returns true
//if successful, false otherwise.
bool create(const char *file, unsigned initial_size){
  return filesys_create(file, initial_size);   //Return true if file was created, false if not.
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
  return -1;
}

/*Closes file descriptor fd. Exiting or terminating a process implicitly closes all
its open file descriptors, as if by calling this function for each one.*/
void close(int fd) {
  if(fd < 0 || fd > 129){ //Return if invalid filedescriptor.
    return -1;
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
  if(fd < 0 || fd == STDOUT_FILENO || fd > 129){
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
  if (fd < 0 || fd == STDIN_FILENO || fd > 129) {
    // return -1 if the file descriptor is invalid
    return -1; 
  }

  if(size <= 0) {
    return 0;
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

void validate_pointer(const void *vaddr, unsigned size) {
  long size2 = (int) size; 
  if (check_bounds(vaddr)) //If not valid user space address and page is not null
  {
    exit(-1);
  }
  void *page_next = pg_round_up(vaddr); //Get next page boundary
  size2 -= page_next - vaddr; //Decrement size of bytes to validate
  vaddr = page_next; //Move ptr to next page boundary
  while (size2 > 0) //Validate all bytes within a page
  {
    if (check_bounds(vaddr)){ //If not valid user space address and page is not null
      exit(-1); 
    }
    vaddr += 4096; //Move ptr to next page boundary  (changes page one page is 4kB)
    size2 -= 4096; //Decrement size of bytes to validate
  }
}
//Check if valid userspace address and if page mapped to current thread is not null
bool check_bounds(const void *vaddr) { 
    
  if (!is_user_vaddr(vaddr) || pagedir_get_page(thread_current()->pagedir, vaddr) == NULL){
    return true;
  }
  return false;
}
//Validate string
void validate_string(const char *ptr){
  validate_pointer(ptr, sizeof(char));
  while (*ptr != '\0'){ //Until we find null terminator
    validate_pointer(ptr, sizeof(char));
    ptr +=sizeof(char);
  }
}
//seek - sets position in a file for read and write system calls.
void seek(int fd, unsigned position) {
  
  // check if fd is invalid
  if (fd < 0 || fd > 129) {
    return; 
  }
  struct thread *thread = thread_current(); //Get current thread
  struct file *file = thread->file_descriptor[fd]; //Get file pointer
 
  // check if file is empty
  if (file == NULL) {
    return; 
  }
  
  // check if position is greater than file length
  if (position > file_length(file)){
    position = file_length(file); //Set position to end of file if greater than file length
    file_seek(file, position); //Set position in file
  }else{
    file_seek(file, position); //Set position in file
  }
  
}
//tell - returns the position of the next byte to be read or written in open file fd.
unsigned tell(int fd) {
 
  // check if fd is invalid
  if (fd < 0 || fd > 129 ) {
    return -1; 
  }
  struct thread *thread = thread_current(); //Get current thread
  struct file *file = thread->file_descriptor[fd]; //Get file pointer
  
  // check if file is empty
  if (file == NULL) {
    return -1; 
  }
  return file_tell(file); //Return position in file
}
//filesize - returns the size, in bytes, of the file open as fd.
int filesize(int fd) {
 
  // check if fd is invalid
  if (fd < 0 || fd > 129) {
    return -1; 
  }

  struct thread *thread = thread_current(); //Get current thread
  struct file *file = thread->file_descriptor[fd]; //Get file pointer'
  
  // check if file is empty
  if (file == NULL) {
    return -1; 
  }
  return file_length(file); //Return file size
}

//remove - deletes the file.
bool remove(const char *file) {
  
  return filesys_remove(file); //Remove file
}


