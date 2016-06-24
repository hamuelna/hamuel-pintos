#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //retreive caller stack pointer
  uint32_t *stack_ptr = f->esp;
  //retreive return address
  uint32_t *return_val = f->eax;
  //retreive sys call number
  uint32_t sys_num = stack_ptr[0]

  if (sys_num == SYS_HALT){
  	 shutdown_power_off ();
  }

  if (sys_num == SYS_EXIT){
  	thread_exit();
  }

  printf ("system call!\n");
  thread_exit ();
}
