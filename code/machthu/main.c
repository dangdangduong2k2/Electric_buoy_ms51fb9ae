#include "main.h"


#define PLD_SIZE 32
#define PLD_S 32
struct eeprom_type {
	uint8_t init;
	uint8_t pair;
	uint8_t main_address[6];
}eData;

uint8_t MAC[6];
uint16_t tick;
uint16_t count;
uint8_t bunmp_start;
uint8_t tx_addr[5] = {0x45, 0x55, 0x67, 0x10, 0x21};


uint8_t dataT[PLD_S];
uint8_t dataR[PLD_S];
uint8_t ret;
uint8_t i;
uint32_t but_count;
uint8_t set=0;
void GPIO_INIT() {
	P04_PushPull_Mode;
	P03_PushPull_Mode;
	P10_PushPull_Mode;
	P00_PushPull_Mode;
	P01_Input_Mode;
	P11_Input_Mode;
	P12_PushPull_Mode;
	P13_PushPull_Mode;
	P13=0;
}
void PAIR_FUNCTION(void)
{
	if(nrf24_data_available())
	{
		nrf24_receive(dataR, sizeof(dataR));
		if(dataR[0]==0xfe)
		{
			eData.main_address[0]=dataR[1];
			eData.main_address[1]=dataR[2];
			eData.main_address[2]=dataR[3];
			eData.main_address[3]=dataR[4];
			eData.main_address[4]=dataR[5];
			eData.main_address[5]=dataR[6];
			eData.pair=0xfe;
			AP_EEPROM_Commit();
			delay_ms(1000);
			P12=1;
			delay_ms(1000);
		}
	}
}
void USER_CONDITION(void)
{
	if(50==tick)
	{
		P12=1;
	}
	if(100<=tick)
	{
		if(nrf24_data_available())
		{
			nrf24_receive(dataR, sizeof(dataR));
			P12=0;
		}
		if(dataR[0]==0xfe
			&& dataR[1]==eData.main_address[0]
			&& dataR[2]==eData.main_address[1]
			&& dataR[3]==eData.main_address[2]
			&& dataR[4]==eData.main_address[3]
			&& dataR[5]==eData.main_address[4]
			&& dataR[6]==eData.main_address[5]
			&& dataR[7]==0x1e)
			{
					P12=0;
					P13=0;
					dataR[7]=0xFF;
					count=0;
			}
		if(dataR[0]==0xfe
			&& dataR[1]==eData.main_address[0]
			&& dataR[2]==eData.main_address[1]
			&& dataR[3]==eData.main_address[2]
			&& dataR[4]==eData.main_address[3]
			&& dataR[5]==eData.main_address[4]
			&& dataR[6]==eData.main_address[5]
			&& dataR[7]==0xae)
			{
					P12=0;
					P13=1;
					dataR[0]=0xFF;
					tick=0;
					count=0;
			}
		else
		{				
			count+=100;
			tick=0;
		}
	}
	if(count>=3000)
	{
		P13=0;
		count=0;
	}
}

void read_mac(void) {
	uint8_t uid[12];
	uint8_t i;
	Read_UID(&uid);
	for (i = 0; i < 6; i++) {
		MAC[i] = uid[i] ^ uid[i + 6];
	}
}

void main() {
	GPIO_INIT();
	MODIFY_HIRC_166();
	read_mac();
	SPI_Initial();
	
  csn_high();
  ce_high();

  delay_ms(5);

  ce_low();

  nrf24_init();

  nrf24_listen();

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
	
	AP_EEPROM_Init((uint8_t *)&eData, sizeof(eData));
	AP_EEPROM_Commit();

	while (1) 
	{
		nrf24_listen();
		while(!P11)
		{
			nrf24_stop_listen();
			P12=1;
			if(but_count>=5000)
			{
				P12=0;
				eData.pair=0xff;
				AP_EEPROM_Commit();
				delay_ms(1000);
				P12=1;
				delay_ms(1000);
				P12=0;
				delay_ms(1000);
				P12=1;
				delay_ms(1000);
				P12=0;
				delay_ms(1000);
				P12=1;
				delay_ms(1000);
				P12=0;
				but_count=0;
			}
			but_count++;
			delay_ms(1);
		}
		if(eData.pair==0xff)
		{
			P12=0;
			PAIR_FUNCTION();
		}
		USER_CONDITION();
		tick++;
		delay_ms(1);
		
	}
	
}
