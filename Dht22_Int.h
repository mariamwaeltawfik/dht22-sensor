/*
* Dht22_Int.h
*
* Created: 3/21/2024 3:17:31 AM
*  Author: NOTEBOOK
*/


#ifndef DHT22_INT_H_
#define DHT22_INT_H_

typedef enum{
	INPUT_DATA,
	OUTPUT_DATA
}Data_Drection_t;

typedef enum{
	MINUS=0,
	POSITIVE
}Temp_polar_t;

void f_call_dht(void);
void Dht22_Init(void);
void Data_Pin_cfg(Data_Drection_t direction);
void Dht22_Runnable(void);
u16 Dht22_Get_Temprature(u8* polarity);
u16  Dht22_Get_Humidity(void);
void dht22_ov_call(void);
void dht22_calc(void);
#endif /* DHT22_INT_H_ */