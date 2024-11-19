#include "main.h"

#define PLD_SIZE 32
#define PLD_S 32

struct eeprom_type {
	uint8_t init;
	uint8_t pair;
	uint8_t main_address[6];
} xdata eData;

uint8_t xdata MAC[6];
uint16_t thresh_hold;
uint8_t tx_addr[5] = {0x45, 0x55, 0x67, 0x10, 0x21};
uint8_t dataT[PLD_S];
uint8_t dataR[PLD_S];
uint8_t ret=3;
uint8_t bump=0;
uint32_t count;
uint8_t set=0;
uint16_t tick;
uint8_t d;
void GPIO_INIT() {
	P04_PushPull_Mode;
	P03_PushPull_Mode;
	P10_PushPull_Mode;
	P00_PushPull_Mode;
	P01_Input_Mode;
	P11_Input_Mode;
	P12_Input_Mode;
	P13_PushPull_Mode;
	P14_Input_Mode;
	P17_Input_Mode;
}
void read_mac(void) {
	uint8_t uid[12];
	uint8_t i;
	Read_UID(&uid);
	for (i = 0; i < 6; i++) {
		MAC[i] = uid[i] ^ uid[i + 6];
	}
}

void USER_CONDITION(void)
{
	if(P11==0)
	{
		bump++;	
		while(!P11);
	}
	if(2<bump)
	{
		bump=0;
	}


	if(bump==0)
	{
		thresh_hold=P14;
		
		
		if(tick<50)
		{
			P13=1;
		}
		if(tick==50)
		{
			P13=0;
			if(thresh_hold ==0)
			{
				if(P17)
				{
					dataT[7]=0xae;	
				}
				else
				{
					dataT[7]=0x00;	
				}
			}

			if(thresh_hold ==1)
			{
				if(P17)
				{
					dataT[7]=0x00;	
				}
				else
				{
					dataT[7]=0xae;	
				}
			}
			ret=nrf24_transmit(dataT,32);	
		}
		
		if(100<=tick)	
		{
			tick=0;	
		}
		
		
	
	}
	else if(bump==1)
	{
		P13=0;
		dataT[7]=0xae;
		if(tick==50)
		{
			
			ret=nrf24_transmit(dataT,32);	
			
		}
		if(100<=tick)
		{
			tick=0;	
		}
	
	}
	else if(bump==2)
	{
		P13=1;
		dataT[7]=0x00;
		if(tick==50)
		{
			
			ret=nrf24_transmit(dataT,32);	
			
		}
		if(100<=tick)
		{
			tick=0;	
		}
	}
	while(!P12)
	{
		P13=0;
		dataT[7]=0x1e;
		ret=nrf24_transmit(dataT,32);	
		delay_ms(1000);
	}
}
void main() {
	MODIFY_HIRC_166();
	GPIO_INIT();
	read_mac();
	SPI_Initial();	
	csn_high();
	ce_high();
	delay_ms(5);
	ce_low();
	csn_low();
	nrf24_init();
	nrf24_stop_listen();
	nrf24_auto_ack_all(auto_ack);
	nrf24_en_ack_pld(disable);
	nrf24_dpl(disable);
	nrf24_set_crc(no_crc, _1byte);
	nrf24_tx_pwr(_0dbm);
	nrf24_data_rate(_250kbps);
	nrf24_set_channel(10);
	nrf24_set_addr_width(5);
	nrf24_set_rx_dpl(0, disable);
	nrf24_set_rx_dpl(1, disable);
	nrf24_set_rx_dpl(2, disable);
	nrf24_set_rx_dpl(3, disable);
	nrf24_set_rx_dpl(4, disable);
	nrf24_set_rx_dpl(5, disable);
	nrf24_pipe_pld_size(0, PLD_S);
	nrf24_auto_retr_delay(4);
	nrf24_auto_retr_limit(10);
	nrf24_open_tx_pipe(tx_addr);
	nrf24_open_rx_pipe(0, tx_addr);
	ce_high();
	dataT[0]=0xfe;
	for(d=0;d<6;d++)
	{
		dataT[d+1]=MAC[d];
	}
	while (1) 
	{
		 USER_CONDITION();
		 tick++;
		 delay_ms(1);
	}
}
