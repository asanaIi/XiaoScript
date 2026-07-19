#ifndef CORE_ENV_H
#define CORE_ENV_H

#define MAX_VARS 50
#define MAX_NAME_LEN 32
#define MAX_VAL_LEN 64

typedef struct {
    char type[16];
    char name[MAX_NAME_LEN];
    char value[MAX_VAL_LEN];
} SystemVariable;

typedef struct {
    SystemVariable vars[MAX_VARS];
    int var_count;
} HyperOSEnvironment;

#endif