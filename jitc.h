/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * jitc.h
 */

#ifndef _JITC_H_
#define _JITC_H_

struct jitc; // cannot use before declaration(former declaration)

/**
 * Compile a C program into a dynamically loadable module.
 *
 * input: the file pathname of the C program
 * output: the file pathname of the dynamically loadable module
 *
 * return: 0 on success, otherwise error -> 0 is successful in OS
 *
 */

int jitc_compile(const char *input, const char *output); // involve gcc to compile the code

/**
 * Loads a dynamically loadable module into the calling process memory for execution.
 *
 * pathname: the file pathname of the dynamically loadable module
 *
 * return: an opaque handle or NULL on error
 */

struct jitc *jitc_open(const char *pathname);

/**
 * Unloads a previously loaded dynamically loadable module.
 *
 * jitc: an opaque handle previously obtained by calling jitc_open()
 *
 * Note: jitc may be NULL
 */

void jitc_close(struct jitc *jitc);

/**
 * Searches for a symbol in the dynamically loaded module associated with jitc.
 *
 * jitc: an opaque handle previously obtained by calling jitc_open()
 *
 * return: the memory address of the start of the symbol, or 0 on error
 */

long jitc_lookup(struct jitc *jitc, const char *symbol); // why long -> pointer is address is int -> long
// long may not long enough for 64 bit system pointer -> in project is safe
// solution: create a new type long enough
#endif /* _JITC_H_ */