int notmain ( void )
{
    unsigned int ra;

    clock_init();

    PUT32(CLK_PERI_CTRL_RW,(1<<11)|(4<<5));

    PUT32(RESETS_RESET_CLR,(1<<5)); //IO_BANK0
    while(1)
    {
        if((GET32(RESETS_RESET_DONE_RW)&(1<<5))!=0) break;
    }
    PUT32(RESETS_RESET_CLR,(1<<22)); //UART0
    while(1)
    {
        if((GET32(RESETS_RESET_DONE_RW)&(1<<22))!=0) break;
    }

    //GPIO 0 UART0 TX function 2
    //GPIO 1 UART0 RX function 2

    //(12000000/(16/115200)) = 6.514
    //0.514*64 = 32.666
    PUT32(UART0_BASE_UARTIBRD_RW,6);
    PUT32(UART0_BASE_UARTFBRD_RW,33);
    //0 11 1 0 0 0 0
    //0111 0000
    PUT32(UART0_BASE_UARTLCR_H_RW,0x70);
    PUT32(UART0_BASE_UARTCR_RW,(1<<9)|(1<<8)|(1<<0));

    PUT32(IO_BANK0_GPIO0_CTRL_RW,2);  //UART
    PUT32(IO_BANK0_GPIO1_CTRL_RW,2);  //UART

    for(ra=0;ra<100;)
    {
        if((GET32(UART0_BASE_UARTFR_RW)&(1<<5))==0)
        {
            PUT32(UART0_BASE_UARTDR_RW,0x55);
            ra++;
        }
    }
    for(ra=0;ra<100;ra++)
    {
        hexstring(ra);
    }
    while(1)
    {
        hexstring(uart_recv());
    }

    return(0);
}
