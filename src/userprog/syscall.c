#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
struct file_des{
  struct list_elem elem;
  int fd;
  struct file *fd_file;
};

static void syscall_handler (struct intr_frame *);

void shutdown_power_off(void);
uint8_t input_getc (void);
struct file *filesys_open (const char *name);
struct list fd_list;

static bool
is_in(int x){
  struct list_elem *e;
  for (e = list_begin (&fd_list); e != list_end (&fd_list);
           e = list_next (e))
  {
        struct file_des *f = list_entry (e, struct file_des, elem);
        if (f->fd == x) {
            return true;
        }else{
            return false;
        }
 
  }
  return false;
}

static struct file_des *
find_fd(uint8_t fd){
  struct list_elem *e;
  for(e = list_begin (&fd_list); e != list_end (&fd_list); e = list_next(e)){
    struct file_des *f = list_entry (e, struct file_des, elem);
    if (f->fd == fd){
      return f;
    }else{
      return NULL;
    }
  }
  return NULL;
}

/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */
static int
get_user (const uint8_t *uaddr)
{
  static int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}
 
/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}
 

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  list_init(&fd_list);
  printf("Init syscall\n");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf("In the syscall_handler\n");
  //retreive caller stack pointer
  uint32_t *stack_ptr = f->esp;
  //retreive return address
  uint32_t *return_val = f->eax;
  //retreive sys call number
  if ((uint32_t)stack_ptr < (uint32_t)PHYS_BASE){
    printf("successful stack_ptr\n");
    sys_num = get_user(stack_ptr);
  }else{
    printf("Invalid User Address\n");
    thread_exit();
  }

  printf("sys_num %jd\n", sys_num);
  if (sys_num == SYS_HALT){
    printf("Calling sys halt\n");
  	 shutdown_power_off ();
  }

  if (sys_num == SYS_EXIT){
    printf("Calling sys exit\n");
  	thread_exit();
  }

  if (sys_num == SYS_READ){
    stack_ptr--;
    uint8_t fd = get_user(stack_ptr);
    stack_ptr--;
    char *buf = (char *) get_user(stack_ptr);
    stack_ptr--
    uint8_t size = get_user(stack_ptr);

    if (fd == 0){
      for (uint8_t i = 0; i < size; i++){
        buf[i] = input_getc();
      }
    }else{
      struct file des *get_fd = find_fd(fd);
      unsigned int size_c = 0;
      while(size_c <= size){
        size_c+=file_read(get_fd->fd_file, buf, size_c);
    }
  }
}

if (sys_num == SYS_OPEN){
    stack_ptr--;
    char* file_name = (char *) get_user(stack_ptr);
    int finding_fd = 2;
   
    while (is_in(finding_fd)) {
      finding_fd++;
    }
 
    struct file_des adding_file;
    adding_file.fd = finding_fd;
    adding_file.fd_file = (struct file*)filesys_open(file_name);
    list_insert(list_end(&fd_list),&adding_file.elem);
    put_user(return_val, finding_fd);
  }

  if (sys_num == SYS_WRITE){
    stack_ptr--;
    int fd = get_user(stack_ptr);
    stack_ptr--;
    char *buf = (char *) get_user(stack_ptr);
    stack_ptr--;
    unsigned int size = get_user(stack_ptr);
    //find file descriptor in the list
    if (fd == 1){
      putbuf(buf, size);
      put_user(return_val, size);
    }else{
      struct file_des *get_fd = find_fd(fd);
      unsigned int size_c = 0;
      while(size_c <= size){
        size_c+=file_write(get_fd->fd_file, buf, size_c);
      }
      put_user(return_val, size_c);
    }
 
  }

  printf ("system call!\n");
  thread_exit ();
}
