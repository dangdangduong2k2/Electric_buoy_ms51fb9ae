
#define SPI_MOSI P00
#define SPI_MISO P01
#define SPI_CLK P10
#define SPI_SS P04

/* Initialize SPI Master Mode */

void csn_high(void){
	//HAL_GPIO_WritePin(csn_gpio_port, csn_gpio_pin,GPIO_PIN_SET);
	P04=1;
}

void csn_low(void){
	//HAL_GPIO_WritePin(csn_gpio_port, csn_gpio_pin, GPIO_PIN_RESET);
	P04=0;
}

void ce_high(void){
	//HAL_GPIO_WritePin(ce_gpio_port, ce_gpio_pin, GPIO_PIN_SET);
	P03=1;
}

void ce_low(void){
	//HAL_GPIO_WritePin(ce_gpio_port, ce_gpio_pin, GPIO_PIN_RESET);
	P03=0;
}

#if soft

void SPI_Master_Init(void)
{
	SPI_SS = 0;
	SPI_CLK = 0;	/* CPOL = 0 */
}

/* Send a byte */
void Spi_Write_Byte(uint8_t senddata)
{
	int i;
	csn_low();
	for(i = 0; i < 8; i++)
	{
		/* LSB First */
		if ( (senddata & 0x80) == 0 )
		{
			SPI_MOSI = 0;
		}
		else  	/* (senddata & 0x80) != 0 */
		{
			SPI_MOSI = 1;
		}
		/* Send a bit by Falling Edge */

		SPI_CLK = 1;
		
		/* CPHA = 0 */
		SPI_CLK = 0;
		senddata <<= 1; 
	}
}

uint8_t Spi_Read_Byte(void)
{
	char rdata; /* 0x00 */
	int i;
	csn_low();
	for(i = 0; i < 8; i++)
	{
		rdata <<= 1;		// 0x00
		/* Receive a bit by Falling Edge */
		SPI_CLK = 1;			/* CPHA = 0 */
		
		if (SPI_MISO == 1)
		{
			rdata |= 0x01;
		}
		SPI_CLK = 0;
	}
	return rdata;
}

uint8_t SPI_TransmitReceive(uint8_t dat) {
    uint8_t receivedData = 0;
    int i;
		csn_low();
    for ( i= 0; i < 8; i++) {
				SPI_CLK = 1; // SCK high
        if (dat & 0x80) {
            SPI_MOSI = 1; // MOSI high
        } else {
            SPI_MOSI = 0; // MOSI low
        }

				
        
   
        receivedData <<= 1;
        if (SPI_MISO) { // N?u MISO high
            receivedData |= 0x01;
        }
        SPI_CLK = 0; // SCK low
        dat <<= 1; // 
    }
    
    return receivedData;
}
#else 

#define SPI_CLOCK 0

void SPI_Master_Init(void) {
#if SPI_CLOCK == 0
	clr_SPR1;
	clr_SPR0;
#elif SPI_CLOCK == 1
	clr_SPR1;
	set_SPR0;
#elif SPI_CLOCK == 2
	set_SPR1;
	clr_SPR0;
#elif SPI_CLOCK == 3
	set_SPR1;
	set_SPR0;
#endif

	/* /SS General purpose I/O ( No Mode Fault ) */
	set_DISMODF;
	
	set_SSOE;
	clr_SSOE;

	/* SPI in Master mode */
	set_MSTR;

	/* MSB first */
	clr_LSBFE;

	clr_CPOL;
	clr_CPHA;

	/* Enable SPI function */
	set_SPIEN;
}

void Spi_Write_Byte(uint8_t u8SpiWB)
{
    SPDR = u8SpiWB;
    while(!(SPSR&0x80));
    clr_SPIF;
}
/****************************************************************/
uint8_t Spi_Read_Byte(void)
{
    unsigned char u8SpiRB;
    SPDR = 0xff;
    while(!(SPSR&0x80));
    u8SpiRB = SPDR;
    clr_SPIF;
    return u8SpiRB;
}
uint8_t SPI_TransmitReceive(uint8_t u8Data) 
{
    SPDR = u8Data; 
    while(!(SPSR & 0x80)); 
    clr_SPIF; 
    return SPDR; 
}
#endif
