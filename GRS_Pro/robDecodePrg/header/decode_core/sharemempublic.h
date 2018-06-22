#ifndef ROBOTDECODEPUBLIC_H
#define ROBOTDECODEPUBLIC_H

#if defined(__cplusplus)
extern "C"
{
#endif


int createUserSharemem(char *memName, int size);

int openUserSharemem(char *memName, int size);

int closeUserSharemem(int id);

void* mmapUserSharemem(int id, int size, int initFlag);

#if defined(__cplusplus)
}
#endif

#endif
