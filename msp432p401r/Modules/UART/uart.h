#ifndef __UART_H__
#define __UART_H__

UART_Params uartParams;
UART_Handle uartHandle;

void configUART();
void send_UART(char *message);
bool receive_UART();

#endif

