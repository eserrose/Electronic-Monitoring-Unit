#ifndef __PLATFORM_CONFIG_H_
#define __PLATFORM_CONFIG_H_

#include "xgpio.h"
#include "xuartlite.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xspi.h"

#define UARTLITE_DEVICE_ID      XPAR_UARTLITE_0_DEVICE_ID
#define INTC_DEVICE_ID          XPAR_INTC_0_DEVICE_ID
#define UARTLITE_INT_IRQ_ID     XPAR_MICROBLAZE_0_AXI_INTC_AXI_UARTLITE_0_INTERRUPT_INTR
#define SPI_DEVICE_ID			XPAR_SPI_0_DEVICE_ID

#define EMU_BUFFER_SIZE         128

#define SET_ENABLE() 			XGpio_DiscreteWrite(&Gpio, 1, 0x01)

int SetupInterruptSystem(XUartLite *UartLitePtr);
void RecvHandler(void *CallBackRef, unsigned int EventData);

extern XGpio Gpio; 					   /* The Instance of the GPIO Driver */
extern XUartLite UartLite;             /* The instance of the UartLite Device */
extern XIntc InterruptController;      /* The instance of the Interrupt Controller */
extern XSpi SpiInstance;			   /* The instance of the SPI device */

#endif
