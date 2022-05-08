#ifndef _PROCESS_H_
#define _PROCESS_H_

/* Standard library */
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#define PROCESS_STACK_SIZE 4096
#define MAX_PROCESSES 16

typedef int TPid;

/**
 * @brief Represents a process' entrypoint function.
 *
 * @returns The process' exit code.
 */
typedef int (*TProcessEntryPoint)(int argc, const char* argv[]);

/**
 * @brief Represents a function that will handle a file descriptor read operation.
 *
 * @returns The amount of bytes read, or -1 if an error occurred.
 */
typedef ssize_t (*TFdReadHandler)(TPid pid, int fd, void* resource, char* buf, size_t count);

/**
 * @brief Represents a function that will handle a file descriptor write operation.
 *
 * @returns The amount of bytes written, or -1 if an error occurred.
 */
typedef ssize_t (*TFdWriteHandler)(TPid pid, int fd, void* resource, const char* buf, size_t count);

/**
 * @brief Represents a function that will handle a file descriptor close operation.
 *
 * @returns 0 if the operation succeeded.
 */
typedef int (*TFdCloseHandler)(TPid pid, int fd, void* resource);

/**
 * @brief Creates a new process from a given entry point and arguments list.
 *
 * @returns The newly created TProcess, or NULL if process creation failed.
 */
TPid prc_create(TProcessEntryPoint entryPoint, int argc, const char* argv[]);

/**
 * @brief Kills a process and frees associated resources. Using a process after
 * it has been killed results in undefined behaviour.
 *
 * @returns 0 if the operation was successful.
 */
int prc_kill(TPid pid);

/**
 * @brief Maps a resource onto a process' I/O table.
 *
 * @returns The file descriptor on which the resource was mapped for the process,
 * or -1 if an error occurred.
 */
int prc_mapFd(TPid pid, void* resource, TFdReadHandler readHandler, TFdWriteHandler writeHandler, TFdCloseHandler closeHandler);

/**
 * @brief Unmaps a file descriptor from a process' I/O table.
 *
 * @returns 0 if the operation succeeded.
 */
int prc_unmapFd(TPid pid, int fd);

/**
 * @brief Handles a process' read operation from a file descriptor in its I/O table.
 *
 * @returns The amount of bytes read, or -1 if an error occurred.
 */
ssize_t prc_handleReadFd(TPid pid, int fd, char* buf, size_t count);

/**
 * @brief Handles a process' write operation to a file descriptor in its I/O table.
 *
 * @returns The amount of bytes written, or -1 if an error occurred.
 */
ssize_t prc_handleWriteFd(TPid pid, int fd, const char* buf, size_t count);

#endif