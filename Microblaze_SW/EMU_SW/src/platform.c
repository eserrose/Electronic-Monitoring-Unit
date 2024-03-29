/******************************************************************************
*
* Copyright (C) 2010 - 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include "xparameters.h"
#include "xil_cache.h"

#include "platform_config.h"

/*
 * Uncomment one of the following two lines, depending on the target,
 * if ps7/psu init source files are added in the source directory for
 * compiling example outside of SDK.
 */
/*#include "ps7_init.h"*/
/*#include "psu_init.h"*/

#ifdef STDOUT_IS_16550
 #include "xuartns550_l.h"

 #define UART_BAUD 9600
#endif


XGpio Gpio; 					/* The Instance of the GPIO Driver */
XUartLite UartLite;             /* The instance of the UartLite Device */
XIntc InterruptController;      /* The instance of the Interrupt Controller */
XSpi SpiInstance;			    /* The instance of the SPI device */

u8 ReceiveBuffer[EMU_BUFFER_SIZE];			//UART RX Buffer
volatile int TotalReceivedCount = 0; 		//RX Counter

void
enable_caches()
{
#ifdef __PPC__
    Xil_ICacheEnableRegion(CACHEABLE_REGION_MASK);
    Xil_DCacheEnableRegion(CACHEABLE_REGION_MASK);
#elif __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheEnable();
#endif
#endif
}

void
disable_caches()
{
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheDisable();
#endif
#endif
}

uint8_t
init_uart_lite()
{
	//Init
	if (XUartLite_Initialize(&UartLite, UARTLITE_DEVICE_ID) != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//Perform self-test
	if (XUartLite_SelfTest(&UartLite) != XST_SUCCESS) {
		return XST_FAILURE;
	}

	 //Connect the UartLite to the interrupt subsystem such that interrupts can occur.
	if ( SetupInterruptSystem(&UartLite) != XST_SUCCESS) {
		xil_printf("Interrupt Initialization Failed\r\n");
		return XST_FAILURE;
	}

	//Setup the handler for the UartLite RX
	XUartLite_SetRecvHandler(&UartLite, RecvHandler, &UartLite);

	// Enable interrupt
	XUartLite_EnableInterrupt(&UartLite);

	return XST_SUCCESS;
}

void
init_gpio()
{
	/* Initialize the GPIO driver */
	if (XGpio_Initialize(&Gpio, XPAR_GPIO_0_DEVICE_ID) != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
	}
}

uint8_t
init_spi()
{
	XSpi_Config *ConfigPtr;
	int Status;

	ConfigPtr = XSpi_LookupConfig(SPI_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XSpi_CfgInitialize(&SpiInstance, ConfigPtr,
				ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("SPI Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XSpi_SelfTest(&SpiInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("SPI Self-Test Failed\r\n");
		return XST_FAILURE;
	}

	/*
	 * The SPI device is a slave by default and the clock phase and polarity
	 * have to be set according to its master. In this instance, CPOL is set
	 * to active low and CPHA is set to 1.
	 */
	Status = XSpi_SetOptions(&SpiInstance, XSP_MASTER_OPTION |
							XSP_MANUAL_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the SPI driver so that the device is enabled.
	 */
	XSpi_Start(&SpiInstance);

	/*
	 * Disable Global interrupt to use polled mode operation.
	 */
	XSpi_IntrGlobalDisable(&SpiInstance);

	/*
	 * Select the slave device
	 */
	XSpi_SetSlaveSelect(&SpiInstance, 0x01);

	return XST_SUCCESS;
}

void
init_platform()
{
    /*
     * If you want to run this example outside of SDK,
     * uncomment one of the following two lines and also #include "ps7_init.h"
     * or #include "ps7_init.h" at the top, depending on the target.
     * Make sure that the ps7/psu_init.c and ps7/psu_init.h files are included
     * along with this example source files for compilation.
     */
    /* ps7_init();*/
    /* psu_init();*/
    enable_caches();
    init_gpio();
    init_uart_lite();
    init_spi();
    microblaze_enable_interrupts(); //this is the most important step that's not included in tutorials!

}

void
cleanup_platform()
{
    disable_caches();
}

/**
*
* This function setups the interrupt system such that interrupts can occur
* for the UartLite device. This function is application specific since the
* actual system may or may not have an interrupt controller. The UartLite
* could be directly connected to a processor without an interrupt controller.
* The user should modify this function to fit the application.
*
* @param    UartLitePtr contains a pointer to the instance of the UartLite
*           component which is going to be connected to the interrupt
*           controller.
*
* @return   XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note     None.
*
****************************************************************************/
int SetupInterruptSystem(XUartLite *UartLitePtr)
{

	int Status;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, UARTLITE_INT_IRQ_ID,
			   (XInterruptHandler)XUartLite_InterruptHandler,
			   (void *)UartLitePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts, specific real mode so that
	 * the UartLite can cause interrupts through the interrupt controller.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupt for the UartLite device.
	 */
	XIntc_Enable(&InterruptController, UARTLITE_INT_IRQ_ID);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 (Xil_ExceptionHandler)XIntc_InterruptHandler,
			 &InterruptController);

	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

void RecvHandler(void *CallBackRef, unsigned int EventData)
{
	u8 buf;
	XUartLite_Recv(&UartLite, &buf, 1);
	XIntc_Acknowledge(&InterruptController, UARTLITE_INT_IRQ_ID);
	ReceiveBuffer[TotalReceivedCount++] = buf;
}
