#define LCD_SDA_PIN P14
#define LCD_SCL_PIN P13

#define PCF_ADDRES_WRITE 0x4E//(pcf8574A 7E) or 4e

#define LCD_BACKLIGHT    0x08
#define LCD_NO_BACKLIGHT 0x00

#define FUNCTION_SET    0X28
#define DISPLAY_CONTROL 0X0C
#define CLEAR_DISPLAY   0X01
#define ENTRY_MODE      0X06

#define LCD_RETURN_HOME       0x02
#define LCD_CURSOR_OFF        0x0C
#define LCD_UNDERLINE_ON      0x0E
#define LCD_BLINK_CURSOR_ON   0x0F
#define LCD_MOVE_CURSOR_LEFT  0x10
#define LCD_MOVE_CURSOR_RIGHT 0x14
#define LCD_TURN_ON           0x0C
#define LCD_TURN_OFF          0x08
#define LCD_SHIFT_LEFT        0x18
#define LCD_SHIFT_RIGHT       0x1E
#define LCD_LEFT_MODE         0x04
#define LCD_RIGHT_MODE        0x06

#define ADDR_LINE1 0X80
#define ADDR_LINE2 0XC0
#define ADDR_LINE3 0X94
#define ADDR_LINE4 0XD4

//void lcd_soft_i2c_start() {
//	LCD_SCL_PIN =1;
//	delay_us(1);
//	LCD_SCL_PIN =0;
//	delay_us(1);
//	LCD_SCL_PIN =1;
//	LCD_SDA_PIN =0;
//	delay_us(1);
//	LCD_SCL_PIN =0;
//}

//void lcd_soft_i2c_stop() {
//	// LCD_SDA_PIN =0;
//	// LCD_SCL_PIN =0;
//	LCD_SCL_PIN =1;
//	delay_us(1);
//	LCD_SDA_PIN =1;
//}

//void lcd_soft_i2c_write(uint8_t data_send) {
//	int i;
//	for (i = 0; i < 8; i++) {
//		// LCD_SCL_PIN =0;
//		output_bit(LCD_SDA_PIN, bit_test(data_send, 7 - i));
//		// delay_us(1);
//		LCD_SCL_PIN =1;
//		delay_us(1);
//		LCD_SCL_PIN =0;
//		delay_us(1);
//	}
//	LCD_SCL_PIN =1;
//	LCD_SDA_PIN =0;
//	delay_us(2);
//	LCD_SCL_PIN =0;
//}

//////////////////////////////////////////////
//void lcd_soft_i2c_write(uint8_t data_send) {
//    int i;
//    for (i = 0; i < 8; i++) {
//        // Set the SDA pin based on the data bit
//        if (data_send & 0x80) {
//						LCD_SDA_PIN = 1;// Set SDA to high
//        } else {
//            LCD_SDA_PIN = 0;  // Set SDA to low
//        }
//				data_send <<= 1;

//        
//    }
//		// Generate clock pulse on SCL
//        LCD_SCL_PIN = 1;  // Set SCL high
//				LCD_SDA_PIN = 0;
//        delay_us(2);                 // Delay 1us (depending on clock speed)
//        LCD_SCL_PIN = 0;  // Set SCL low
//                        // Delay 1us

//    
//}

void lcd_soft_i2c_start() {
	LCD_SCL_PIN = 1;
	LCD_SDA_PIN = 1;
	delay_us(10);
	LCD_SDA_PIN = 0;
	delay_us(10);
	LCD_SCL_PIN = 0;
}

void lcd_soft_i2c_stop() {
	// LCD_SCL_PIN = 0; 
	// LCD_SDA_PIN = 0;
	delay_us(10);
	LCD_SCL_PIN = 1;
	delay_us(10);
	LCD_SDA_PIN = 1;
}

void lcd_soft_i2c_write(uint8_t data_send) {
	int i;
	for ( i = 0; i < 8; i++) {
		delay_us(10);
		if (data_send & 0x80) {
			LCD_SDA_PIN = 1;
		}
		else LCD_SDA_PIN = 0;
		data_send <<= 1;

		LCD_SCL_PIN = 1;
		delay_us(10);
		LCD_SCL_PIN = 0;
	}

	delay_us(20);
	LCD_SCL_PIN = 1;
	delay_us(25);
	LCD_SCL_PIN = 0;
	LCD_SDA_PIN = 0;
}

void swap(uint8_t *da) {
    *da = (*da<<4)|(*da>>4);
}
void lcd_soft_i2c_command(char mdk) {
	uint8_t ms_bit_e, ls_bit_e, ms_data, ls_data;

	ms_data = mdk & 0xf0; // lay 4 bit cao
	ls_data = mdk & 0x0f; // lay 4 bit thap

	swap(&ls_data);

	ms_bit_e = ms_data | 0x04; // tao xung clock cap vao chan e
	ls_bit_e = ls_data | 0x04;

	lcd_soft_i2c_start();
	lcd_soft_i2c_write(PCF_ADDRES_WRITE);

	lcd_soft_i2c_write(ms_bit_e | LCD_BACKLIGHT); // luc nay se xuat car ms_data va 3 gia tri e=1,rw=0,rs=0
	lcd_soft_i2c_write(ms_data | LCD_BACKLIGHT);  // luc nay chi xuat ms_data, e=0,rw=0,rs=0

	lcd_soft_i2c_write(ls_bit_e | LCD_BACKLIGHT);
	lcd_soft_i2c_write(ls_data | LCD_BACKLIGHT);
	// lcd_soft_i2c_write(LCD_BACKLIGHT);
	lcd_soft_i2c_stop();
	if (mdk == CLEAR_DISPLAY || mdk == LCD_RETURN_HOME) delay_us(2000);
	else
		delay_us(100);
}



void lcd_soft_i2c_data(char mht) {
	uint8_t ms_bit_e, ls_bit_e, ms_data, ls_data;
	ms_data = mht & 0xf0;
	ls_data = mht & 0x0f;

	swap(&ls_data);
	ms_data++; // lu�n du cho rs o muc 1
	ls_data++;
	ms_bit_e = ms_data | 0x05;
	ls_bit_e = ls_data | 0x05;

	lcd_soft_i2c_start();
	lcd_soft_i2c_write(PCF_ADDRES_WRITE);

	lcd_soft_i2c_write(ms_bit_e | LCD_BACKLIGHT); // luc nay se xuat car ms_data va 3 gia tri e=1,rw=0,rs=1
	lcd_soft_i2c_write(ms_data | LCD_BACKLIGHT);

	lcd_soft_i2c_write(ls_bit_e | LCD_BACKLIGHT);
	lcd_soft_i2c_write(ls_data | LCD_BACKLIGHT);
	lcd_soft_i2c_stop();
	delay_us(100);
}


void lcd_soft_i2c_setup() {
	lcd_soft_i2c_start();
	lcd_soft_i2c_write(0x4E);
	lcd_soft_i2c_write(0x00);
	lcd_soft_i2c_stop();
	delay_us(10);  
	lcd_soft_i2c_command(3);
	delay_us(5000);  
	lcd_soft_i2c_command(3);
	delay_us(5000);  
	lcd_soft_i2c_command(3);
	delay_us(5000);  
	lcd_soft_i2c_command(LCD_RETURN_HOME);
	lcd_soft_i2c_command(FUNCTION_SET);
	lcd_soft_i2c_command(ENTRY_MODE);
	lcd_soft_i2c_command(LCD_TURN_ON);
	lcd_soft_i2c_command(CLEAR_DISPLAY);
}