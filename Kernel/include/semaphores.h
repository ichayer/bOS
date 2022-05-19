#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <stddef.h>
#include <lib.h>
#include <memoryManager.h>
#include <resourceNamerADT.h>
#include <waitQueueADT.h>
#include <kernelTypes.h>
#include <scheduler.h>

#define MAX_SEMAPHORES 128
#define SEM_SUCCES 0
#define SEM_FAILED -1
#define SEM_NOTEXISTS -2

typedef int8_t TLock;

/**
 * @brief Initialize the semaphore system.
 * 
 * @returns 0 if the operation succeeded, SEM_FAILED otherwise.
 *
 */
int sem_init();


/**
 * @brief Creates a semaphore with the indicated name, or opens it if
 * it already existed. 
 * 
 * @return The semaphore, or SEM_FAILED (-1) if the operation failed.
 */
TSem sem_open(const char * name, uint8_t initialValue);


/**
 * @brief Closes a semaphore. A semaphore will be destroyed once all
 * the processes that where linked to it, close it. 
 * 
 * @return SEM_SUCCES (0) if the operation succeded or SEM_NOTEXISTS (-2) if the semaphore
 * did not exist. 
 */
int sem_close(TSem sem);


// will be deleted soon
int sem_printDebug();

/**
 * @brief Decrements or locks the semaphore pointed to by sem. If the semaphore's 
 * value is greater than zero, then the decrement proceeds, and the function returns, 
 * immediately.  If the semaphore currently has the value zero, then the call blocks
 * until the semaphore value rises above zero.
 * 
 * @return SEM_SUCCES (0) if the operation succeded or SEM_NOTEXISTS (-2) if the semaphore
 * did not exist. 
 */
int sem_wait(TSem sem);

/**
 * @brief Increments or unlocks the semaphore pointed to by sem. If the semaphore's 
 * value consequently becomes greater than zero, then another process or thread 
 * blocked in a sem_wait call will be woken up
 * 
 * @return SEM_SUCCES (0) if the operation succeded or SEM_NOTEXISTS (-2) if the semaphore
 * did not exist. 
 */
int sem_post(TSem sem);

/**
 * @brief Gets the information of up to maxSemaphores semaphores.
 * 
 * @returns the amount of semaphores read.
 */
int sem_listSemaphores(TSemaphoreInfo* array, int maxSemaphores);

#endif