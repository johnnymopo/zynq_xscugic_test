

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xparameters.h"
#include "xscugic.h"

#define INTC_DEVICE_ID      XPAR_SCUGIC_0_DEVICE_ID
#define INTC_DEVICE_INT_ID  XPAR_FABRIC_DUMMY_0_INTERRUPT_INTR

XScuGic InterruptController;
static XScuGic_Config *GicConfig;

static volatile int num = 0;

void DeviceDriverHandler(void *CallbackRef)
{
	num += 1;
	xil_printf("yo %d\r\n", num);
}

int main()
{
	int Status;

    init_platform();

    print("Hello World\n\r");

    GicConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    if (NULL == GicConfig) {
        print("XScuGic_LookupConfig failed\r\n");
        return 1;
    }

    Status = XScuGic_CfgInitialize(&InterruptController, GicConfig, GicConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS) {
        print("XScuGic_CfgInitialize failed\r\n");
        return 1;
    }

    Status = XScuGic_SelfTest(&InterruptController);
    if (Status != XST_SUCCESS) {
        print("XScuGic_SelfTest failed\r\n");
        return 1;
    }

    Status = XScuGic_Connect(&InterruptController, INTC_DEVICE_INT_ID, (Xil_ExceptionHandler)DeviceDriverHandler, 0);//(void*)&InterruptController);
    if (Status != XST_SUCCESS) {
        print("XScuGic_Connect failed \r\n");
        return 1;
    }

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &InterruptController);
    Xil_ExceptionEnable();

    XScuGic_SetPriorityTriggerType(&InterruptController, INTC_DEVICE_INT_ID, 0x0, 0x3);

    XScuGic_Enable(&InterruptController, INTC_DEVICE_INT_ID);

    while(1) {
    	usleep(10);
    }


    print("finished\r\n");

    cleanup_platform();
    return 0;
}
