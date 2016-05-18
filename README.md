# hamuel-pintos
Git Repo for pintos for use in ICCS222

#Project 1
## Hamuel and Karn Team
- - -
Threads documentation will be up soon read at the DESIGNDOC in the thread folder but before then you can also 
read this, this is some idea before implementing

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
    we know that list_elem is a sleeper type therefore we can do but we should cast it just in case
    struct sleeper as = (struct sleeper *)a
    struct sleeper bs = (struct sleeper *)b
    int_t64 a_timeleft = timer_elasped(a.start) - a.ticks
    int_t64 b_timeleft = timer_elasped(b.start) - b.ticks

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


