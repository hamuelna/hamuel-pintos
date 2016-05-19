# hamuel-pintos
Git Repo for pintos for use in ICCS222

#Project 1
## Hamuel and Karn Team
- - -
Threads documentation will be up soon read at the DESIGNDOC in the thread folder but before then you can also 
read this, this is some idea before implementing
- - - 
###Question 1 (Alarm Clock)

  _Introduction_
  ------------------------
   when the thread is started, the thread sleep we first do a thread_block on it initially
  and wake it up after it is time when it need to wake up.

  when we sleep the thread using the thread block we put it in some queue i.e. some list
  when we add the sleeping thread into the queue we need to add it into an ordered position

  our list should be ordered by ticks left, that mean the thread that is nearly ready to wake up 
  should be at the head of the list because we will wake up the thread that is at the head of the
  tail first 
  
  _Structure_
  ------------------------
  we need a struct to keep the ticks and the thread_id of the timersleep to unblock it at the right time
  i.e.
  <pre><code>
  struct sleeper {
    int_t64 start;
    int_t64 ticks;
    thread *this_th;
  }
  </code></pre>

  Therefore we can check it by if timer_elasped(start) < sleeper[i].th_id, therefore we will always 
  check the head only which will take O(1) although it might take O(n) when we add the thread into the queue
  when it is time to wake up we unblock_thread which will add the thread into the ready queue ready to be
  executed!

  we must not waste CPU cycle to let a thread run when it is not suppose to wake up yet therefore ONLY the
  ready to be awoken up thread must be in the READY QUEUE if not it must be in the Waiting Queue when the thread
  is in sleeping mode we put it in the "Ordered Waiting Queue" where head timer_elasped(start) minus ticks the one
  that is closer to zero should be near the head of the list

  When we remove the thread from the waiting queue we use list_pop_front to remove the list from the beginning of
  the list

  _Implementation_
  -------------------------
  we make global variable call struct list *wait_list = malloc(sizeof(struct list)); 
  and init the list init_list(wait_list)
  to append to the list we can do list_insert_ordered(wait_list, &sleeper, less, aux) which runs in O(n)
  we need to make a new function to compare the data using list_less_func
  <pre><code>
  list_less_func my_comparator(const struct list_elem *a,const struct list_elem *b,void *aux) {

    //we know that list_elem is a sleeper type therefore we can do but we should cast it just in case

    struct sleeper as = (struct sleeper *)a
    struct sleeper bs = (struct sleeper *)b
    int_t64 a_timeleft = timer_elasped(as.start) - as.ticks
    int_t64 b_timeleft = timer_elasped(bs.start) - bs.ticks

    //we ordered the list by the amount of time that is left before it need to wake up

    if(a_timeleft < b_timeleft)
      return true
    else
      return false
  }
  </code></pre>

  aux data can be elem = (sleeper *) some_elts
  
  we can use thread_current() function to keep the pointer of the thread and put it in the global variable struct
  i.e. sleeper.this_th = thread_current()

  we can use sema_down() or thread_block to sleep the thread initially but before we sleep the thread we need to 
  add it into the waiting queue with all of the important essential information otherwise we cannot wake up
  the thread when we need to actually wake it up

  we constantly check the head by using list_begin(struct list *list) which will return the beginning of the list

  to properly do this we must prevent the possible race condition that could happen to the global variable wait_list
  using thread_block or sema_down and sema_up to sync the global variable to avoid race condition

---------------------
###Question 2 (Priority Scheduling)

_Introduction_
- - -

Every thread will now have a priority therefore the thread with more priority will run first, therefore we need
to insert things into the ready queue differently we should insert it into some kind of particular order 
we can use list_insert_ordered or sort after each donation

The biggest problem is that if a higher priority thread is trying to run however it need to wait for a lock that is 
in a thread with a lower priority that will unlikely to get CPU time we must donate our priority number to the thread that is
blocking the thread with the higher priority therefore our higher priority thread don't get block forever 

sample of a donation
Thread A -> 40, Thread B -> 21 if Thread A need to wait for lock in Thread B in order for Thread A to be able to run we need
to give Thread B our priority number, we change thread B to 40 temporary then change it down to 21 when we release the lock 

Solution to priority inversion
We will donate when we run lock_acquire() in synch.c to see who currently hold the lock 
(lock -> holder) will return the thread that is the owner of the lock after that we compare the priority of both thread
(thread _> priority) if the priority of the thread that hold the lock is less than the thread that is trying to acquire the lock we donate the priority to the thread that is currently holding the lock 

After donating the priority we need to set the thread priority that get donated back to its original value by having a 
data structure to keep it the original value before donation this will get trigger in lock_release() by setting priority value back to its original value

Solution to nested priority inversion
we will use a pointer to track our nested donation using the prev_thread pointer, if the thread is waiting i.e. Thread C
need to Wait for Thread B and Thread B need to wait for Thread A unfortunately Thread B has a priority less then Thread C and Thread A has a priority less than Thread B Thread C need to donate to Thread B then Thread B need to donate to Thread A so that all of them get unlock in the end if they are waiting

thread -> prev_thread will point to Thread that is currently waiting (need to be unlock) 

_Structure_
- - -

Here I have taken the original thread struct and modify it here I have remove the variable that does not concern our project
here prev priority will keep the previous priority number 

<pre><code>

struct thread
  {
    /* Owned by thread.c. */
    int priority;                       /* Priority. */
    struct list_elem allelem;           /* List element for all threads list. */
    int prev_priority; /* we add this to keep track of our previous priority number */
    thread *prev_thread; /*keep the 
    /* Shared between thread.c and synch.c. */
    struct list_elem elem;              /* List element. */
  };

</code>
</pre>

_Implementation_
- - -

For normal donation (try to handle donation)
the only time when we need to donate is when we do lock_acquire this is how 
we could implement lock_acquire this way 

<pre><code>
	if(lock -> holder != NULL){
		thread *curthread = lock -> holder
		thread *newthread = thread_current()
		if(curthread -> priority < newthread -> priority ) {
			//if the current thread have the priority less than the new thread (which mean it need to wait)
			//time for the new thread to donate to current thread 
			curthread -> prev_priority = curthread -> priority
			curthread -> priority = newthread -> priority
			newthread -> prev_thread = curthread
			curthread -> prev_thread = NULL
		}else{
			newthread -> prev_priority = NULL
		}
	}
</code>
</pre>

lock_release change the low priority thread number back to it original number

<pre><code>
	thread *my_thread = lock -> holder
	if(my_thread -> prev_priority != NULL ){
		my_thread -> priority = my_thread -> prev_priority
		my_thread -> prev_priority = NULL
	}
	lock -> holder = NULL
</code></pre>


---------------------
###Question 3
