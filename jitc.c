/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * jitc.c
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dlfcn.h>
#include "system.h"
#include "jitc.h"

/**
 * Needs:
 *   fork()
 *   execv()
 *   waitpid()
 *   WIFEXITED()
 *   WEXITSTATUS()
 *   dlopen()
 *   dlclose()
 *   dlsym()
 */

/* research the above Needed API and design accordingly */

struct jitc
{
    void *handle;
};

int jitc_compile(const char *input, const char *output)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
        return -1;
    }
    else if (pid > 0) /* parent process */
    {
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
        {
            return 0;
        }
        printf("Child exited with status %d\n", WEXITSTATUS(status));
        return -1;
    }
    else /* child process */
    {
        /*const char *args[] = {"gcc", "-shared", "-fPIC", "-o", output, input, NULL};*/
        char *args[13];
        args[0] = "gcc";
        args[1] = "-shared";
        args[2] = "-fPIC";
        args[3] = "-o";
        args[4] = (char *)output;
        args[5] = (char *)input;
        args[6] = "-lm";    /* link to math library */
        args[7] = "main.o"; /* link with main object file */
        args[8] = "lexer.o";
        args[9] = "parser.o";
        args[10] = "jitc.o";
        args[11] = "system.o";
        args[12] = NULL;

        printf("jitc_compile\n");

        if (execv("/usr/bin/gcc", args) == -1)
        {
            perror("execv");
            return -1;
        }
    }
    return 0; /* 0 means successful */
}

struct jitc *jitc_open(const char *pathname)
{
    char path[100] = "./";
    struct jitc *jit = malloc(sizeof(struct jitc));

    jit->handle = dlopen(strcat(path, pathname), RTLD_LAZY);

    if (!jit->handle)
    {
        printf("dlopen failed: %s \n", dlerror());
        return NULL; /* error */
    }

    return jit;
}

void jitc_close(struct jitc *jitc)
{
    dlclose(jitc->handle);
    free(jitc);
}

long jitc_lookup(struct jitc *jitc, const char *symbol)
{
    void *addr;
    if (!jitc || !jitc->handle)
    {
        return 0; /* error */
    }
    addr = (char *)dlsym(jitc->handle, symbol);
    return (long)addr;
}