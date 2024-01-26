/*
 * sys_arch.h
 *
 *  Created on: 2019-8-10
 *      Author: NANXIAOFENG
 */

#ifndef SYS_ARCH_H_
#define SYS_ARCH_H_

#define SYS_MBOX_NULL (xQueueHandle)0
#define SYS_SEM_NULL  (xSemaphoreHandle)0

typedef struct hal_mutex {
	void *obj;
#ifdef SEMAPHORE_DEBUG
	unsigned char name[4];
#endif
} hal_mutex_t;

#define xSemaphoreHandle SemaphoreHandle_t
#define xQueueHandle QueueHandle_t
#define xTaskHandle TaskHandle_t

typedef xSemaphoreHandle     sys_sem_t;
typedef hal_mutex_t     sys_mutex_t;
typedef xQueueHandle         sys_mbox_t;
typedef xTaskHandle          sys_thread_t;

/* Message queue constants. */
#define archMESG_QUEUE_LENGTH	( 32 )
#define archPOST_BLOCK_TIME_MS	( ( unsigned long ) 10000 )


#endif /* SYS_ARCH_H_ */
