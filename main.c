#include "public.h"
#include "LCD.h"
#include "dht11.h"
#include "buzzer.h"
sbit LED1 = P2^3;
sbit LED2 = P2^4;

sbit Tupper = P3^3;
sbit Tlower = P3^4;
sbit Hupper = P3^5;
sbit Hlower = P3^6;
static u8 TupperThreshold=45,TlowerThreshold=0,HupperThreshold=85,HlowerThreshold=20;

void set_H_Threshold(u8 upper,lower){
	HupperThreshold = upper;
	HlowerThreshold = lower;
}
void set_T_Threshold(u8 upper,lower){
	TupperThreshold = upper;
	TlowerThreshold = lower;
}

void scan_button() {
    if (Tupper == 0 || Tlower == 0 || Hupper == 0 || Hlower == 0) {
        delay_ms(10); // 消抖处理
        if (Tupper == 0) {
            set_T_Threshold(TupperThreshold + 1, TlowerThreshold);
            while (Tupper == 0); // 等待按键释放
            return;
        } else if (Tlower == 0) {
            set_T_Threshold(TupperThreshold, TlowerThreshold - 1);
            while (Tlower == 0); // 等待按键释放
            return;
        } else if (Hupper == 0) {
            set_H_Threshold(HupperThreshold + 1, HlowerThreshold);
            while (Hupper == 0); // 等待按键释放
            return;
        } else if (Hlower == 0) {
            set_H_Threshold(HupperThreshold, HlowerThreshold - 1);
            while (Hlower == 0); // 等待按键释放
            return;
        }
    }
}

u8 judge_alert(u8 temp,humi){
	if((temp<TlowerThreshold || temp >TupperThreshold)){
		return 1;
	}
	if((humi<HlowerThreshold || humi >HupperThreshold)){
		return 2;
	}
	return 0;
}


void main() {
	u16 times = 60; // alert time setting (seconds)
	
	u8 state;
    u8 temp=0,humi=0;
	u8 i=0;
	u8 temp_buf[3],humi_buf[3],temp_upper_buf[3],temp_lower_buf[3],humi_lower_buf[3],humi_upper_buf[3];
	
	lcd_init();
	while(DHT11_Init())	//检测DHT11是否存在
	{
		display_string("Error");		
	}
	
	while(1)
	{	
		scan_button();
		
		goto_XY(0,0);
		//display_string("Temp: ");
	
		DHT11_Read_Data(&temp,&humi);
		
		temp_buf[0]=temp/10+0x30;	
		temp_buf[1]=temp%10+0x30;
		temp_buf[2]='\0';

		display_string(temp_buf);
		display_customized_char();
		display_string("C");
		goto_XY(0,1);
		//display_string("Humi: ");
		humi_buf[0]=humi/10+0x30;	
		humi_buf[1]=humi%10+0x30;
		humi_buf[2]='\0';
		display_string(humi_buf);
		display_string("%RH");

		// display the Tthreshold:
		
		goto_XY(8,0);
		temp_lower_buf[0]=TlowerThreshold/10+0x30;	
		temp_lower_buf[1]=TlowerThreshold%10+0x30;
		temp_lower_buf[2]='\0';
		display_string(temp_lower_buf);
		
		display_string("-");
		
		temp_upper_buf[0]=TupperThreshold/10+0x30;	
		temp_upper_buf[1]=TupperThreshold%10+0x30;
		temp_upper_buf[2]='\0';
		display_string(temp_upper_buf);
		
		// display the Hthreshold:
		goto_XY(8,1);
		humi_lower_buf[0]=HlowerThreshold/10+0x30;	
		humi_lower_buf[1]=HlowerThreshold%10+0x30;
		humi_lower_buf[2]='\0';
		display_string(humi_lower_buf);
		
		display_string("-");
		
		humi_upper_buf[0]=HupperThreshold/10+0x30;	
		humi_upper_buf[1]=HupperThreshold%10+0x30;
		humi_upper_buf[2]='\0';
		display_string(humi_upper_buf);
		
		DHT11_Read_Data(&temp,&humi);
		state = judge_alert(temp,humi);
		if(state){
			if(state==1) LED1 = 0;
			if(state==2) LED2 = 0;
			while(times--){
				DHT11_Read_Data(&temp,&humi);
				state = judge_alert(temp,humi);
				if(state){
					buzz_ms(2000);
				}else break;
			}
			LED1 = 1;LED2=1;
		}
	}
}

/*
void main()
{
	u16 i;
	u16 j;
	i=2000;
	j=0;
	while(1){
		for(;j<4;j++){
		 	while(i--){
			 	beep = !beep;
				delay_10us(30);

			}
			beep=0;
			i=2000;
			delay_10us(1000000/10);
		}
	}
}*/
