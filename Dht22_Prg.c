#include "StdTypes.h"
#include "DIO_Int.h"
#include "TIMER_Int.h"
#include "Dht22_Int.h"
volatile u8 flag=0,count,i,c,f,f2,f_minus;
u8 dht_arr[42],dht_arr_buffer[40];
//u16 arr_o[16];
volatile u16 t1,t2,temp,humidity,check_sum,ton,temp2;
void f_call_dht(void){

	if(flag==0){
		t1=ICR1;
		Timer1_InputCaptureEdge(FALLING);
		flag=1;
	}
	else if(flag==1){
		if(i==41){
			i=0;
		}
		t2=ICR1;
		dht_arr[i]=t2-t1;
		i++;
		Timer1_InputCaptureEdge(RISING);
		flag=0;
		
		
	}
	count++;
}
void Dht22_Init(void){
	Timer1_Init(TIMER1_NORMAL_MODE,TIMER1_SCALER_8);
	Timer1_ICU_SetCallBack(f_call_dht);
	Timer1_ICU_InterruptEnable();
	//Timer1_OVF_InterruptEnable();
	
	
}
void dht22_calc(void){
	if(f2==1){
		u8 m;
		for(m=0;m<16;m++){
		
			if(dht_arr_buffer[m]>=70){
				SET_BIT(humidity,0);
				humidity=humidity<<1;
				//arr_o[m]=1;
			}
			if(dht_arr_buffer[m]>=25&& dht_arr_buffer[m]<29){
				
				CLEAR_BIT(humidity,0);
				humidity=humidity<<1;
				//arr_o[m]=0;
			}
			
		}
		for(m=16;m<32;m++){
			if(dht_arr_buffer[m]>=70){
					if(dht_arr_buffer[0]>=70){
						f_minus=MINUS;
					}
					
				SET_BIT(temp,0);
				temp<<=1;
			}
			if(dht_arr_buffer[m]>=25&& dht_arr_buffer[m]<29){
				 if (dht_arr_buffer[0]>=25&& dht_arr_buffer[0]<29){
					 f_minus=POSITIVE;
				 }
				CLEAR_BIT(temp,0);
				temp<<=1;
			}
		}
		temp2=temp;
		f2=0;
	}
}
void Data_Pin_cfg(Data_Drection_t direction){
	switch(direction){
		case INPUT_DATA:
		DIO_PIN_Statues(PIND6,INPULL);
		break;
		case OUTPUT_DATA:
		DIO_PIN_Statues(PIND6,OUTPUT);
		break;

	}
	
}
void Dht22_Runnable(void){
	TCNT1=0;
	Data_Pin_cfg(INPUT_DATA);
	_delay_us(22);
	Timer1_InputCaptureEdge(RISING);
	flag=0;
	Timer1_ICU_InterruptEnable();

	if(count>42){
			f=1;
		Timer1_ICU_InterruptDisable();
		Data_Pin_cfg(OUTPUT_DATA);
		DIO_WritePin(PIND6,HIGH);
		_delay_us(2);
		DIO_WritePin(PIND6,LOW);
		count=0;
		c++;
		if(c==200){
			c=0;
		}
	}
if(f==1){
u8 m;
for(m=2;m<42;m++){
	dht_arr_buffer[m-2]=dht_arr[m];
}
f=0;
f2=1;
}
	dht22_calc();
}

u16 Dht22_Get_Temprature(u8* polarity){
	*polarity=f_minus;
	return temp>>2;
}
u16  Dht22_Get_Humidity(void){
	
	return humidity>>2;
}