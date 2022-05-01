#ifndef LIB_H
#define LIB_H

/* Standard library */
#include <stdint.h>
#include <stddef.h>

/**
* @brief fills the first n bytes of the memory area pointed to by s with the constant byte c
* 
* @param s memory area
* @param c constant byte 
* @param n bytes to fill
*
* @return a pointer to the memory area s
*/
void * memset(void * s, int32_t c, uint64_t n);

/**
* @brief copies n bytes from memory area source to memory area destination. The memory areas must not overlap
*
* @param src memory area source
* @param dest memory area destination
* @param n bytes to copy
*
* @return a pointer to dest
*/
void * memcpy(void * dest, const void * src, uint64_t n);

/**
* @brief reads 8042 status register
* @return unsigned int read from output register 60h 
*/
unsigned int sys_readKey();

/**
* @brief Returns he output from executing the cpuid instruction (a 12-character representation of the processor’s manufacturer)
*/
char * cpuVendor(char *result);

#endif  /* LIB_H */
