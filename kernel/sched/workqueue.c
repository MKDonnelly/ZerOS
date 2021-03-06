#include <kernel/sched/workqueue.h>

#include <arch/x86/pmode/spinlock.h>
#include <kernel/sched/sched.h>
#include <kernel/task.h>
#include <kernel/mm/heap.h>

tasklet_t *tasklet_create( void (*function)(), void *data){
   tasklet_t *new_tasklet = k_malloc( sizeof(tasklet_t), 0 );
   new_tasklet->function = function;
   new_tasklet->data = data;
   
   return new_tasklet;
}

workqueue_t *workqueue_create(){
   workqueue_t *workqueue = k_malloc( sizeof(workqueue_t), 0);
   workqueue->tasklet_list = list_create(tasklet_t, tasklet_list);
   spinlock_init(&workqueue->workqueue_lock);
   return workqueue;
}

void workqueue_add(workqueue_t *workqueue, tasklet_t *new_task){
   spinlock_acquire( &workqueue->workqueue_lock );

   list_pushfront(workqueue->tasklet_list, new_task);

   spinlock_release(&workqueue->workqueue_lock);
}

tasklet_t *workqueue_get(workqueue_t *workqueue){
   spinlock_acquire( &workqueue->workqueue_lock );
   
   tasklet_t *return_tasklet = NULL;
   if( list_len(workqueue->tasklet_list) > 0 ){
      return_tasklet = list_rmfront(workqueue->tasklet_list, tasklet_t);
   }

   spinlock_release( &workqueue->workqueue_lock );
   return return_tasklet;
}

void workqueue_worker(workqueue_t *workqueue){
   while(1){
      tasklet_t *tasklet = workqueue_get( workqueue );

      //No tasks to run, yield cpu
      if( tasklet == NULL ){
         current_scheduler->scheduler_yield_task();
      }else{
         tasklet->function();
      }
   }
}

void workqueue_worker_spawn(workqueue_t *workqueue){
   current_scheduler->scheduler_add_task( 
        ktask_create( workqueue_worker, workqueue, NULL ) );
}
