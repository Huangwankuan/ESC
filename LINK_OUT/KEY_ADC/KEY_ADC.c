#include "KEY_ADC.h"
void KEY_ADC_Initial(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܻ���ʧ��APB2����ʱ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����������50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ������GPIOx�Ĵ���
}
/*u16 KEY_Calibration_X(void)
{
	u16 KEY_X_RES;
	KEY_X_RES=ADC_Get_Average_Value(5,1);
	return KEY_X_RES;
} 
u16 KEY_Calibration_Y(void)
{
	u16 KEY_Y_RES;
	KEY_Y_RES=ADC_Get_Average_Value(5,2);
	return KEY_Y_RES;
} 
*/
