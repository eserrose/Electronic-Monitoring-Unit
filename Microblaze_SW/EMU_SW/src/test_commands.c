#ifdef TEST
void test_uart_tx(){
	  volatile int cnt = 0;
	  uint8_t test = 0;
	  uint8_t dur = 100;

	  while(dur--){
		XUartLite_Send(&UartLite, &test, 1);
		for(cnt = 0; cnt < 10; cnt++); //wait
		test++;
	  }
}

void test_gpio(){
	  volatile int cnt = 0;
	  uint8_t dur = 100;

	  while(dur--){
		XGpio_DiscreteWrite(&Gpio, 1, 0x01);
		for(cnt = 0; cnt < 10; cnt++);
		XGpio_DiscreteClear(&Gpio, 1, 0x01);
		for(cnt = 0; cnt < 10; cnt++);
	  }
}

void test_spi(){
	u8 data[5] = {2,3,4,5,6};
	XSpi_Transfer(&SpiInstance, data, NULL, 5);
}
#endif
