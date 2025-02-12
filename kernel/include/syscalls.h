#ifndef __QS__SYSCALLS_H
#define __QS__SYSCALLS_H

#include <hardware/interrupts.h>
#include <core/types.h>

typedef void (*syscall_t)(CPUState* cpu);

void syscall(CPUState* cpu);
void register_syscall_handler(uint32_t syscall, syscall_t handler);

#endif
