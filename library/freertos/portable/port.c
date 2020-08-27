/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 * 
 * MOD: Georgi Angelov 2020
 * 
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM7 port.
 *
 * Components that can be compiled to either ARM or THUMB mode are
 * contained in this file.  The ISR routines, which can only be compiled
 * to ARM mode are contained in portISR.c.
 *----------------------------------------------------------*/

/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include <sys.h>

/* Processor constants. */

/* Constants required to setup the task context. */
#define portINITIAL_SPSR ((StackType_t)0x1f) /* System mode, ARM mode, interrupts enabled. */
#define portTHUMB_MODE_BIT ((StackType_t)0x20)
#define portINSTRUCTION_SIZE ((StackType_t)4)
#define portNO_CRITICAL_SECTION_NESTING ((StackType_t)0)

/*-----------------------------------------------------------*/

/* Setup the timer to generate the tick interrupts. */
static void prvSetupTimerInterrupt(void);

/* 
 * The scheduler can only be started from ARM mode, so 
 * vPortISRStartFirstSTask() is defined in portISR.c. 
 */
extern void vPortISRStartFirstTask(void);

/*-----------------------------------------------------------*/

/* 
 * Initialise the stack of a task to look exactly as if a call to 
 * portSAVE_CONTEXT had been called.
 *
 * See header file for description. 
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
	StackType_t *pxOriginalTOS;

	pxOriginalTOS = pxTopOfStack;

	/* To ensure asserts in tasks.c don't fail, although in this case the assert
	is not really required. */
	pxTopOfStack--;

	/* Setup the initial stack of the task.  The stack is set exactly as 
	expected by the portRESTORE_CONTEXT() macro. */

	/* First on the stack is the return address - which in this case is the
	start of the task.  The offset is added to make the return address appear
	as it would within an IRQ ISR. */
	*pxTopOfStack = (StackType_t)pxCode + portINSTRUCTION_SIZE;
	pxTopOfStack--;

	*pxTopOfStack = (StackType_t)0x00000000; /* R14 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)pxOriginalTOS; /* Stack used when task starts goes in R13. */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x12121212; /* R12 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x11111111; /* R11 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x10101010; /* R10 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x09090909; /* R9 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x08080808; /* R8 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x07070707; /* R7 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x06060606; /* R6 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x05050505; /* R5 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x04040404; /* R4 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x03030303; /* R3 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x02020202; /* R2 */
	pxTopOfStack--;
	*pxTopOfStack = (StackType_t)0x01010101; /* R1 */
	pxTopOfStack--;

	/* When the task starts is will expect to find the function parameter in
	R0. */
	*pxTopOfStack = (StackType_t)pvParameters; /* R0 */
	pxTopOfStack--;

	/* The last thing onto the stack is the status register, which is set for
	system mode, with interrupts enabled. */
	*pxTopOfStack = (StackType_t)portINITIAL_SPSR;

#ifdef THUMB_INTERWORK
	{
		/* We want the task to start in thumb mode. */
		*pxTopOfStack |= portTHUMB_MODE_BIT;
	}
#endif

	pxTopOfStack--;

	/* Some optimisation levels use the stack differently to others.  This 
	means the interrupt flags cannot always be stored on the stack and will
	instead be stored in a variable, which is then saved as part of the
	tasks context. */
	*pxTopOfStack = portNO_CRITICAL_SECTION_NESTING;

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler(void)
{
	/* Start the timer that generates the tick ISR.  Interrupts are disabled
	here already. */
	prvSetupTimerInterrupt();

	/* Start the first task. */
	vPortISRStartFirstTask();

	/* Should not get here! */
	return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler(void)
{
	/* It is unlikely that the ARM port will require this function as there is nothing to return to.  */
}
/*-----------------------------------------------------------*/

#include <sys.h>
static volatile int gpt_flag = 0;
void GPT1_Handler(void)
{
	gpt_flag = 1;
}

static void prvSetupTimerInterrupt(void)
{
	GPT_SetupTimer(GP_TIMER1, configTICK_RATE_HZ, true, GPT1_Handler, true);
}

extern void NVIC_C_IRQ_Handler(void);
void NVIC_IRQ_Handler(void) __attribute__((naked));
void NVIC_IRQ_Handler(void)
{
	portSAVE_CONTEXT();
	NVIC_C_IRQ_Handler();
	if (gpt_flag)
	{
		gpt_flag = 0;
		if (xTaskIncrementTick() != pdFALSE)
			vTaskSwitchContext();
	}
	portRESTORE_CONTEXT();
}

void SWI_Handler(void) __attribute__((naked));
void SWI_Handler(void)
{
	/* DISABLE ISR !? */
	__asm volatile ( "ADD LR, LR, #4" );
	portSAVE_CONTEXT();
	vTaskSwitchContext();
	/* ENABLE ISR !? */
	portRESTORE_CONTEXT();
}