
#pragma once

/*
	timers.h
	
	Written by Marion Sudvarg, msudvarg@wustl.edu, January, 2022
	
	A platform-specific timer header for the CAmkES TimeServer global component.
	Targets the Broadcom BCM2837 SoC, which is used in the:
		Later Raspberry Pi 2 Model B
		Raspberry Pi 3 Model B
		Raspberry Pi 3 Model B+
		Raspberry Pi Compute Module 3
		
	Allows the TimeServer to interface with existing seL4 drivers for the BCM2837 system timer,
	found in the seL4 util_libs repo, with functionality declared and defined in:
	libplatsupport/plat_include/bcm2837/platsupport/plat/system_timer.h
	libplatsupport/src/plat/bcm2837/system_timer.c
*/


#define HARDWARE_TIMER_COMPONENT \
        component SpecialPurposeTimer { \
                hardware; \
                dataport Buf(4096) mem; \
                emits SpecialPurposeTimerIRQ irq; \
        } \
        component SystemTimer { \
                hardware; \
                dataport Buf(4096) mem; \
                emits SystemTimerIRQ irq1; \
        }

#define HARDWARE_TIMER_INTERFACES \                                                   
        dataport Buf(4096) spt_mem; \                                     
        dataport Buf(4096) system_timer_mem; \                                     
        consumes SpecialPurposeTimerIRQ spt_irq; \
        consumes SystemTimerIRQ system_timer_irq1; \
    
#define HARDWARE_TIMER_ATTRIBUTES

#define HARDWARE_TIMER_COMPOSITION \                                                   
        component SpecialPurposeTimer spt; \
        component SystemTimer system_timer; \
        connection seL4HardwareMMIO spt_mem( \
                from spt_mem, to spt.mem); \
        connection seL4HardwareInterrupt spt_irq( \
                from spt.irq, to spt_irq); \
        connection seL4HardwareMMIO system_timer_mem( \
                from system_timer_mem, to system_timer.mem); \
        connection seL4HardwareInterrupt system_timer_irq1( \
                from system_timer.irq1, to system_timer_irq1); \

        
#define HARDWARE_TIMER_CONFIG \
        spt.mem_paddr = 0x3F00B000; \
        spt.mem_size = 0x1000; \
        spt.irq_irq_number = 0x20; \        
        system_timer.mem_paddr = 0x3F003000; \
        system_timer.mem_size = 0x1000; \
        system_timer.irq1_irq_number = 0x41; \
        
#define HARDWARE_TIMER_PLAT_INTERFACES
