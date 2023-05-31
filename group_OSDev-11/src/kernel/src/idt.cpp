#include "idt.h"
#include <stdint.h>
#include <cstddef>
#include "common.h"

void initialize_idt() asm("initialize_idt");

extern "C" {
    extern void idt_flush(uint32_t);
}

struct idt_entry idt[IDT_ENTRIES];            // IDT entry array
struct idt_pointer idt_pointer;               // IDT pointer structure

void initialize_idt() {
    // IDT limit
    idt_pointer.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;    // Set the IDT limit based on the size of an IDT entry multiplied by the number of entries
    
    // IDT base address
    idt_pointer.base = (uint32_t) &idt;         // Set the IDT base address to the start of the IDT array


    // Initialize IDT entries
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i].base_low = 0x0000;               // Set the low 16 bits of the base address of the IDT entry to 0x0000
        idt[i].base_high = 0x0000;              // Set the high 16 bits of the base address of the IDT entry to 0x0000
        idt[i].selector = 0x08;                  // Set the selector field of the IDT entry to 0x08
        idt[i].always0 = 0x00;                   // Set the 'always0' field of the IDT entry to 0x00
        idt[i].flags = 0x8E;                     // Set the flags field of the IDT entry to 0x8E

        int_handlers[i].handler = NULL;         // Set the interrupt handler for the corresponding IDT entry to NULL
    }

    init_interrupts();                          // Initialize interrupts
    initialize_interrupt_handlers();            // Initialize interrupt handlers
    irq_init();                                 // Initialize IRQs
    init_irq_handlers();                        // Initialize IRQ handlers

    // Load IDT
    idt_flush((uint32_t)&idt_pointer);           // Load the IDT by calling the idt_flush function with the address of the IDT pointer
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;           // Set the low 16 bits of the base address of the IDT entry
    idt[num].base_high = (base >> 16) & 0xFFFF;  // Set the high 16 bits of the base address of the IDT entry
    idt[num].selector = sel;                     // Set the selector field of the IDT entry
    idt[num].always0 = 0;                        // Set the 'always0' field of the IDT entry
    idt[num].flags = flags | 0x60;               // Set the flags field of the IDT entry
}


void init_interrupts()
{
    write_to_port(0x20, 0x11);
    write_to_port(0xA0, 0x11);
    write_to_port(0x21, 0x20);
    write_to_port(0xA1, 0x28);
    write_to_port(0x21, 0x04);
    write_to_port(0xA1, 0x02);
    write_to_port(0x21, 0x01);
    write_to_port(0xA1, 0x01);
    write_to_port(0x21, 0x0);
    write_to_port(0xA1, 0x0);

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
}