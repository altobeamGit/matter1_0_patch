/*
 * sys_arch.c
 *
 *  Created on: 2019-8-10
 *      Author: NANXIAOFENG
 */

#ifndef SYS_ARCH_C_
#define SYS_ARCH_C_

#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "mutex.h"

static sys_mutex_t sys_arch_mutex;

/* Initialize this module(see description in sys.h) */
void sys_init(void)
{
	sys_mutex_new(&sys_arch_mutex);
}

NO_FLASH_FUNC_OS u32_t sys_now(void)
{
	return (xTaskGetTickCount()*portTICK_PERIOD_MS);
}

u32_t sys_jiffies(void)
{
	return xTaskGetTickCount();
}

#if SYS_LIGHTWEIGHT_PROT

NO_FLASH_FUNC_OS sys_prot_t sys_arch_protect(void)
{
	vPortEnterCritical_Internal();
	return 1;

}

/*
  This optional function does a "fast" set of critical region protection to the
  value specified by pval. See the documentation for sys_arch_protect() for
  more information. This function is only required if your port is supporting
  an operating system.
*/
NO_FLASH_FUNC_OS void sys_arch_unprotect(sys_prot_t pval)
{
	( void ) pval;
	vPortExitCritical_Internal();
}
#endif /*SYS_LIGHTWEIGHT_PROT*/


struct sys_timeouts *sys_arch_timeouts(void)
{
	void *pTimeouts;

	pTimeouts = hal_getTimeouts_thread();

	return (struct sys_timeouts *)pTimeouts;
}

/*
 * Prints an assertion messages and aborts execution.
 */
void sys_assert( const char *msg )
{
	( void ) msg;
	/*FSL:only needed for debugging
	iot_printf(msg);
	iot_printf("\n\r");
	*/
	vPortEnterCritical_Internal(  );
	for(;;)
	;
}

/*-----------------------------------------------------------------------------------*/
//  Creates and returns a new semaphore. The "count" argument specifies
//  the initial state of the semaphore.
KEEP_FUNC  NO_FLASH_FUNC  err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	vSemaphoreCreateBinary(*sem);

	if( *sem == NULL )
	{

#if SYS_STATS
      ++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */

		return ERR_MEM;	// TODO need assert
	}

	if(count == 0)	// Means it can't be taken
	{
		xSemaphoreTake(*sem,0);
	}
	else
	{
        xSemaphoreGive(*sem);
	}

#if SYS_STATS
	++lwip_stats.sys.sem.used;
 	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif /* SYS_STATS */

	return ERR_OK;
}

KEEP_FUNC NO_FLASH_FUNC_OS void sys_sem_signal(sys_sem_t *sem)
{
	xSemaphoreGive(*sem);
}

/*-----------------------------------------------------------------------------------*/
// Deallocates a semaphore
KEEP_FUNC NO_FLASH_FUNC void sys_sem_free(sys_sem_t *sem)
{
#if SYS_STATS
      --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */

	vQueueDelete(*sem);
}


/*-----------------------------------------------------------------------------------*/

KEEP_FUNC  NO_FLASH_FUNC_OS  u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    portTickType StartTime, EndTime, Elapsed;

	StartTime = xTaskGetTickCount();

	if(	timeout != 0)
	{
		if( xSemaphoreTake( *sem, timeout / portTICK_RATE_MS ) == pdTRUE )
		{
			EndTime = xTaskGetTickCount();

			Elapsed = (StartTime+timeout / portTICK_RATE_MS -EndTime) * portTICK_RATE_MS;

			return (Elapsed); // return time blocked TODO test
		}
		else
		{
			return SYS_ARCH_TIMEOUT;
		}
	}
	else // must block without a timeout
	{
		while( xSemaphoreTake( *sem, portMAX_DELAY ) != pdTRUE ){}
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

		return ( Elapsed )+1; // return time blocked

	}
}

KEEP_FUNC  NO_FLASH_FUNC int sys_sem_valid(sys_sem_t *sem)
{
   if(*sem != NULL)
     return 1;
   else
	 return 0;
}

KEEP_FUNC NO_FLASH_FUNC void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = NULL;
}



NO_FLASH_FUNC_OS err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{

	( void ) size;

	*mbox = xQueueCreate( size, sizeof( void * ) );

	#if SYS_STATS
	      ++lwip_stats.sys.mbox.used;
	      if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
	         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
		  }
	#endif /* SYS_STATS */

    if(*mbox != NULL)
    {
    	return ERR_OK;
    }
    else
    {
		return ERR_MEM;

	}

}


FLASH_FUNC void sys_mbox_free(sys_mbox_t *mbox)
{
	if( uxQueueMessagesWaiting( *mbox ) )
	{
		/* Line for breakpoint.  Should never break here! */
		portNOP();
#if SYS_STATS
	    lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

		// TODO notify the user of failure.
	}

	vQueueDelete( *mbox );

#if SYS_STATS
     --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */

}

NO_FLASH_FUNC_OS int sys_mbox_valid(sys_mbox_t *mbox)
{
   if(*mbox != NULL)
	 return 1;
   else
	 return 0;
}


NO_FLASH_FUNC_OS void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	*mbox = NULL;
}


NO_FLASH_FUNC_OS void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	while ( xQueueSendToBack(*mbox, &msg, portMAX_DELAY ) != pdTRUE ){}
}

NO_FLASH_FUNC_OS err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
   err_t result;

   if ( xQueueSend( *mbox, &msg, 0 ) == pdPASS )
   {
      result = ERR_OK;
   }
   else {
      // could not post, queue must be full
      result = ERR_MEM;

#if SYS_STATS
      lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

   }

   return result;
}


/*等待邮箱中的消息*/
NO_FLASH_FUNC_OS u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	void *dummyptr;
	portTickType StartTime, EndTime, Elapsed;

	StartTime = xTaskGetTickCount();
//	DEBUG(1, 1, "StartTime=%d\n",StartTime);
	if ( msg == NULL )
	{
		msg = &dummyptr;
	}

	if ( timeout != 0 )
	{
		if ( pdTRUE == xQueueReceive( *mbox, &(*msg), timeout / portTICK_RATE_MS ) )
		{
			EndTime = xTaskGetTickCount();
			Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

			return ( Elapsed );
		}
		else // timed out blocking for message
		{
			*msg = NULL;

			return SYS_ARCH_TIMEOUT;
		}
	}
	else // block forever for a message.
	{
		while( pdTRUE != xQueueReceive( *mbox, &(*msg), portMAX_DELAY ) ){} // time is arbitrary
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;

		return ( Elapsed ); // return time blocked TODO test
	}
}

NO_FLASH_FUNC_OS u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	if ( msg == NULL )
	{
		KASSERT(0);
		return SYS_MBOX_EMPTY;
	}

	if ( pdTRUE == xQueueReceive( *mbox, &(*msg), 0 ) )
	{
	  return ERR_OK;
	}
	else
	{
	  return SYS_MBOX_EMPTY;
	}
}

err_t sys_mutex_new(sys_mutex_t *mutex)
{
    err_t ret = ERR_MEM;
    mutex_init(mutex);

    if (mutex != NULL) {
        ret = ERR_OK;
    }
    return ret;
}

NO_FLASH_FUNC_OS void sys_mutex_lock(sys_mutex_t * mutex)
{
	 mutex_lock(mutex);
}

NO_FLASH_FUNC_OS void sys_mutex_unlock(sys_mutex_t * mutex)
{
	 mutex_unlock(mutex);
}

/*-----------------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	/* In order to fix different stacksize type in lwip and FreeRTOS */
	if ( stacksize > 0xffff )
	{
		LWIP_ASSERT("This malloc a stack size, which is bigger than 16bits\n",0);
	}

	hal_thread_t task_thread;
	task_thread.fn			= thread;
	task_thread.name 		= (char *)name;
	task_thread.ptos		= NULL;
	task_thread.stack_size	= stacksize;
	task_thread.arg 		= arg;
	task_thread.prio 		= prio;
	task_thread.task		= NULL;

	if(hal_create_thread(&task_thread) == HAL_SUCCESS)
		return task_thread.task;
	else
		return NULL;
}



#endif /* SYS_ARCH_C_ */
