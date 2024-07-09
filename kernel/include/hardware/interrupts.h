#ifndef __MERCURY__HARDWARE__INTERRUPTS_H
#define __MERCURY__HARDWARE__INTERRUPTS_H

#include <hardware/pic.h>
#include <hardware/pci.h>
#include <core/types.h>
#include <memory/gdt.h>
#include <core/screen.h>
#include <hardware/port.h>

struct IDT
{
	uint16_t size;
	uint32_t address;
} __attribute__((packed));

struct IDTDescriptor
{
	uint16_t offset_low;
	uint16_t segment_selector;

	uint8_t reserved;
	uint8_t type_attribute;
	uint16_t offset_high;
} __attribute__((packed));

void install_idt();

void handle_irq_00();
void handle_irq_01();
void handle_irq_02();
void handle_irq_03();
void handle_irq_04();
void handle_irq_05();
void handle_irq_06();
void handle_irq_07();
void handle_irq_08();
void handle_irq_09();
void handle_irq_10();
void handle_irq_11();
void handle_irq_12();
void handle_irq_13();
void handle_irq_14();
void handle_irq_15();
void handle_irq_32();
void handle_irq_33();
void handle_irq_49();

typedef struct cpu_state
{
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
} __attribute__((packed)) cpu_state_t; 

typedef struct stack_state
{
	uint32_t int_no;
	uint32_t error_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
} __attribute__((packed)) stack_state_t;

typedef struct registers
{
	uint32_t ds;
	cpu_state_t cpu_registers;
	stack_state_t stack_content;
} registers_t;

typedef void (*isr_t)(uint32_t interrupt);

void register_interrupt_handler(uint8_t n, isr_t handler);
void interrupt_handler(/*struct cpu_state cpu, */uint32_t interrupt/*, struct stack_state stack*/);

#endif 
