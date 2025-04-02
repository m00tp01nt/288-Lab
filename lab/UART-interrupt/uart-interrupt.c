/*
*
*   uart-interrupt.c
*
*
*
*   @author
*   @date
*/

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.

#include <inc/tm4c123gh6pm.h>
#include <lab/UART-interrupt/uart-interrupt.h>
#include <stdint.h>

volatile char command_byte = 'l';
// These variables are declared as examples for your use in the interrupt handler.
//volatile char command_byte = 's';
volatile char forward_command_byte = 'w';
volatile char rotate_left_command_byte = 'a';
volatile char backward_command_byte = 's';
volatile char rotate_right_command_byte = 'd';
volatile char stop_moving_command_byte = 'q';
volatile char scan_command_byte = 'e';
volatile int command_flag = 0; // flag to tell the main program a special command was received

void uart_interrupt_init(void){

  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0b10;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0b10;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0b000010) == 0) {};         // page 406
  while ((SYSCTL_PRUART_R & 0b00000010) == 0) {};       // page 410

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x3;

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0x3;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations    // page 1351, pb0
  GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the desired locations          // page 1351, pb1

  //calculate baud rate                             // page 896
  uint16_t iBRD = (int)(16000000 / (16 * 115200)); //use equations
  uint16_t fBRD = (((16000000 / (16 * 115200)) - iBRD) * 64 + 0.5); //use equations

  iBRD &= 0x0000FFFF;
  fBRD &= 0x0000FFFF;

  //turn off UART1 while setting it up
  UART1_CTL_R &= ~0b1;                 // page 918

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R |= iBRD;
  //UART1_FBRD_R |= fBRD;
  UART1_FBRD_R |= 0b101100;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0b1100000;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R &= ~0xF;

  //////Enable interrupts

  //first clear RX interrupt flag (clear by writing 1 to ICR)
  UART1_ICR_R |= 0b00010000;

  //enable RX raw interrupts in interrupt mask register
  UART1_IM_R |= 0b10000;

  //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

  //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
  NVIC_EN0_R |= 0b1000000;

  //tell CPU to use ISR handler for UART1 (see interrupt.h file)
  //from system header file: #define INT_UART1 22
  IntRegister(INT_UART1, UART1_Handler);

  //globally allow CPU to service interrupts (see interrupt.h file)
  IntMasterEnable();

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R |= 0x0301;

}

void uart_sendChar(char data)
{
    // Wait to send
    while (UART1_FR_R & 0x20) {}

    // Send
    UART1_DR_R = data;
    //UART1_DR_R |= 0x00;
}

char uart_receive(void)
{
    char data = 0;

    while ((UART1_FR_R & UART_FR_RXFE)) {}

    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

char uart_receive_nonblocking(void)
{
    char data = 0;

    if (!(UART1_FR_R & 0b1000000)) {
        return 0;
    }

    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

void uart_sendStr(const char *data){

    int counter = 0;

    while (1) {
        if (data[counter] != '\0') {
            uart_sendChar(data[counter]);
        }
        else {
            return;
        }
        counter++;
    }
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char byte_received;
    //check if handler called due to RX event
    if (UART1_MIS_R & 0b10000)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0b00010000;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        byte_received = uart_receive_nonblocking();
        uart_sendChar(byte_received);

        //if byte received is a carriage return
        if (byte_received == '\r')
        {
            //send a newline character back to PuTTY
            uart_sendChar('\n');
        }
        else if (byte_received == 'g') {
            command_flag = 0;
        }
        else
        {
            //AS NEEDED
            //code to handle any other special characters
            //code to update global shared variables
            //DO NOT PUT TIME-CONSUMING CODE IN AN ISR

            if (byte_received == forward_command_byte)
            {
              command_flag = 1;
            }
            else if (byte_received == backward_command_byte)
            {
                command_flag = 2;
            }
            else if (byte_received == rotate_left_command_byte)
            {
                command_flag = 3;
            }
            else if (byte_received == rotate_right_command_byte)
            {
                command_flag = 4;
            }
            else if (byte_received == stop_moving_command_byte)
            {
                command_flag = 5;
            }
            else if (byte_received == scan_command_byte)
            {
                command_flag = 6;
            }
        }
    }
}
