#include <stdint.h>

#define SRAM_START	0x20000000U
#define SRAM_SIZE	(128U * 1024U)	//128k
#define SRAM_END	(SRAM_START + SRAM_SIZE)

#define STACK_START	SRAM_END

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;

extern uint32_t _sbss;
extern uint32_t _ebss;

//prototype of main

int main(void) __attribute__((weak));
void __libc_init_array(void) __attribute__((weak));
void SystemInit(void) __attribute__((weak));

/* function prototypes of STM32F407x system exception and IRQ handlers */

void Reset_Handler(void);

void NMI_Handler(void) 						__attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) 				__attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)				__attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)					__attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)				__attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)						__attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)					__attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)					__attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)					__attribute__((weak, alias("Default_Handler")));
void WWDG_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void RTC_TAMP_LSECSS_IRQHandler(void)		__attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void USB_HP_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void USB_LP_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void FDCAN1_IT0_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void FDCAN1_IT1_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM15_IRQHandler(void)		__attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM16_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM17_IRQHandler(void)	__attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));	
void TIM3_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));	
void RTC_Alarm_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM8_UP_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));	
void TIM8_CC_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void ADC3_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void FMC_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));	
void LPTIM1_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM5_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));	
void UART4_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void UART5_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));	
void TIM7_DAC_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));	
void ADC4_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void ADC5_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void UCPD1_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));	
void COMP1_2_3_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void COMP4_5_6_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void COMP7_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));	
void HRTIM1_Master_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void HRTIM1_TIMA_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void HRTIM1_TIMB_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));	
void HRTIM1_TIMC_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void HRTIM1_TIMD_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void HRTIM1_TIME_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void HRTIM1_FLT_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void HRTIM1_TIMF_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));	
void CRS_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void TIM20_BRK_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM20_UP_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void TIM20_TRG_COM_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void TIM20_CC_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void I2C4_EV_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void I2C4_ER_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void SPI4_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void FDCAN2_IT0_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void FDCAN2_IT1_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));	
void FDCAN3_IT0_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void FDCAN3_IT1_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void RNG_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));	
void I2C3_ER_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void DMAMUX_OVR_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void QUADSPI_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel8_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));	
void DMA2_Channel6_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel7_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void DMA2_Channel8_IRQHandler(void)			__attribute__((weak, alias("Default_Handler")));
void CORDIC_IRQHandler(void)				__attribute__((weak, alias("Default_Handler")));	
void FMAC_IRQHandler(void)					__attribute__((weak, alias("Default_Handler")));
	
	

uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
	STACK_START,							//0x0000 0000
	(uint32_t)&Reset_Handler,				//0x0000 0004	Reset
	(uint32_t)&NMI_Handler,					//0x0000 0008
	(uint32_t)&HardFault_Handler,			//0x0000 000C
	(uint32_t)&MemManage_Handler,			//0x0000 0010
	(uint32_t)&BusFault_Handler,			//0x0000 0014
	(uint32_t)&UsageFault_Handler,			//0x0000 0018
	0,										//0x0000 001C	reserved
	0,										//0x0000 0020	reserved
	0,										//0x0000 0024	reserved
	0,										//0x0000 0028	reserved
	(uint32_t)&SVC_Handler,					//0x0000 002C
	(uint32_t)&DebugMon_Handler,			//0x0000 0030
	0,										//0x0000 0034	reserved
	(uint32_t)&PendSV_Handler,				//0x0000 0038
	(uint32_t)&SysTick_Handler,				//0x0000 003C
	(uint32_t)&WWDG_IRQHandler,				//0x0000 0040
	(uint32_t)&PVD_PVM_IRQHandler,			//0x0000 0044
	(uint32_t)&RTC_TAMP_LSECSS_IRQHandler,	//0x0000 0048
	(uint32_t)&RTC_WKUP_IRQHandler,			//0x0000 004C
	(uint32_t)&FLASH_IRQHandler,			//0x0000 0050
	(uint32_t)&RCC_IRQHandler,				//0x0000 0054
	(uint32_t)&EXTI0_IRQHandler,			//0x0000 0058
	(uint32_t)&EXTI1_IRQHandler,			//0x0000 005C
	(uint32_t)&EXTI2_IRQHandler,			//0x0000 0060
	(uint32_t)&EXTI3_IRQHandler,			//0x0000 0064
	(uint32_t)&EXTI4_IRQHandler,			//0x0000 0068
	(uint32_t)&DMA1_Channel1_IRQHandler,	//0x0000 006C	DMA1 channel 1 interrupt
	(uint32_t)&DMA1_Channel2_IRQHandler,	//0x0000 0070
	(uint32_t)&DMA1_Channel3_IRQHandler,	//0x0000 0074
	(uint32_t)&DMA1_Channel4_IRQHandler,	//0x0000 0078
	(uint32_t)&DMA1_Channel5_IRQHandler,	//0x0000 007C
	(uint32_t)&DMA1_Channel6_IRQHandler,	//0x0000 0080
	(uint32_t)&DMA1_Channel7_IRQHandler,	//0x0000 0084
	(uint32_t)&ADC1_2_IRQHandler,			//0x0000 0088	ADC1 and ADC2 global interrupt
	(uint32_t)&USB_HP_IRQHandler,			//0x0000 008C
	(uint32_t)&USB_LP_IRQHandler,			//0x0000 0090
	(uint32_t)&FDCAN1_IT0_IRQHandler,		//0x0000 0094
	(uint32_t)&FDCAN1_IT1_IRQHandler,		//0x0000 0098
	(uint32_t)&EXTI9_5_IRQHandler,			//0x0000 009C
	(uint32_t)&TIM1_BRK_TIM15_IRQHandler,	//0x0000 00A0	TIM1 Break/TIM15 global interrupts 
	(uint32_t)&TIM1_UP_TIM16_IRQHandler,	//0x0000 00A4
	(uint32_t)&TIM1_TRG_COM_TIM17_IRQHandler,	//0x0000 00A8
	(uint32_t)&TIM1_CC_IRQHandler,			//0x0000 00AC
	(uint32_t)&TIM2_IRQHandler,				//0x0000 00B0
	(uint32_t)&TIM3_IRQHandler,				//0x0000 00B4
	(uint32_t)&TIM4_IRQHandler,				//0x0000 00B8
	(uint32_t)&I2C1_EV_IRQHandler,			//0x0000 00BC	I2C1 event interrupt & EXTI line 23 interrupt
	(uint32_t)&I2C1_ER_IRQHandler,			//0x0000 00C0
	(uint32_t)&I2C2_EV_IRQHandler,			//0x0000 00C4
	(uint32_t)&I2C2_ER_IRQHandler,			//0x0000 00C8
	(uint32_t)&SPI1_IRQHandler,				//0x0000 00CC
	(uint32_t)&SPI2_IRQHandler,				//0x0000 00D0
	(uint32_t)&USART1_IRQHandler,			//0x0000 00D4
	(uint32_t)&USART2_IRQHandler,			//0x0000 00D8
	(uint32_t)&USART3_IRQHandler,			//0x0000 00DC
	(uint32_t)&EXTI15_10_IRQHandler,		//0x0000 00E0
	(uint32_t)&RTC_Alarm_IRQHandler,		//0x0000 00E4
	(uint32_t)&USBWakeUp_IRQHandler,		//0x0000 00E8
	(uint32_t)&TIM8_BRK_IRQHandler,			//0x0000 00EC
	(uint32_t)&TIM8_UP_IRQHandler,			//0x0000 00F0
	(uint32_t)&TIM8_TRG_COM_IRQHandler,		//0x0000 00F4
	(uint32_t)&TIM8_CC_IRQHandler,			//0x0000 00F8
	(uint32_t)&ADC3_IRQHandler,				//0x0000 00FC
	(uint32_t)&FMC_IRQHandler,				//0x0000 0100
	(uint32_t)&LPTIM1_IRQHandler,			//0x0000 0104
	(uint32_t)&TIM5_IRQHandler,				//0x0000 0108
	(uint32_t)&SPI3_IRQHandler,				//0x0000 010C
	(uint32_t)&UART4_IRQHandler,			//0x0000 0110
	(uint32_t)&UART5_IRQHandler,			//0x0000 0114
	(uint32_t)&TIM6_DAC_IRQHandler,			//0x0000 0118
	(uint32_t)&TIM7_DAC_IRQHandler,			//0x0000 011C
	(uint32_t)&DMA2_Channel1_IRQHandler,	//0x0000 0120
	(uint32_t)&DMA2_Channel2_IRQHandler,	//0x0000 0124
	(uint32_t)&DMA2_Channel3_IRQHandler,	//0x0000 0128
	(uint32_t)&DMA2_Channel4_IRQHandler,	//0x0000 012C
	(uint32_t)&DMA2_Channel5_IRQHandler,	//0x0000 0130
	(uint32_t)&ADC4_IRQHandler,				//0x0000 0134
	(uint32_t)&ADC5_IRQHandler,				//0x0000 0138
	(uint32_t)&UCPD1_IRQHandler,			//0x0000 013C
	(uint32_t)&COMP1_2_3_IRQHandler,		//0x0000 0140
	(uint32_t)&COMP4_5_6_IRQHandler,		//0x0000 0144
	(uint32_t)&COMP7_IRQHandler,			//0x0000 0148
	(uint32_t)&HRTIM1_Master_IRQHandler,	//0x0000 014C
	(uint32_t)&HRTIM1_TIMA_IRQHandler,		//0x0000 0150
	(uint32_t)&HRTIM1_TIMB_IRQHandler,		//0x0000 0154
	(uint32_t)&HRTIM1_TIMC_IRQHandler,		//0x0000 0158
	(uint32_t)&HRTIM1_TIMD_IRQHandler,		//0x0000 015C
	(uint32_t)&HRTIM1_TIME_IRQHandler,		//0x0000 0160
	(uint32_t)&HRTIM1_FLT_IRQHandler,		//0x0000 0164
	(uint32_t)&HRTIM1_TIMF_IRQHandler,		//0x0000 0168
	(uint32_t)&CRS_IRQHandler,				//0x0000 016C
	(uint32_t)&SAI1_IRQHandler,				//0x0000 0170
	(uint32_t)&TIM20_BRK_IRQHandler,		//0x0000 0174
	(uint32_t)&TIM20_UP_IRQHandler,			//0x0000 0178
	(uint32_t)&TIM20_TRG_COM_IRQHandler,	//0x0000 017C
	(uint32_t)&TIM20_CC_IRQHandler,			//0x0000 0180	TIM20 capture compare interrupt
	(uint32_t)&FPU_IRQHandler,				//0x0000 0184
	(uint32_t)&I2C4_EV_IRQHandler,			//0x0000 0188
	(uint32_t)&I2C4_ER_IRQHandler,			//0x0000 018C
	(uint32_t)&SPI4_IRQHandler,				//0x0000 0190
	0,										//0x0000 0194	AES global interrupt
	(uint32_t)&FDCAN2_IT0_IRQHandler,		//0x0000 0198	FDCAN2 Interrupt 0
	(uint32_t)&FDCAN2_IT1_IRQHandler,		//0x0000 019C
	(uint32_t)&FDCAN3_IT0_IRQHandler,		//0x0000 01A0
	(uint32_t)&FDCAN3_IT1_IRQHandler,		//0x0000 01A4
	(uint32_t)&RNG_IRQHandler,				//0x0000 01A8
	(uint32_t)&LPUART1_IRQHandler,			//0x0000 01AC
	(uint32_t)&I2C3_EV_IRQHandler,			//0x0000 01B0
	(uint32_t)&I2C3_ER_IRQHandler,			//0x0000 01B4
	(uint32_t)&DMAMUX_OVR_IRQHandler,		//0x0000 01B8
	(uint32_t)&QUADSPI_IRQHandler,			//0x0000 01BC
	(uint32_t)&DMA1_Channel8_IRQHandler,	//0x0000 01C0	 DMA1 channel 8 interrupt
	(uint32_t)&DMA2_Channel6_IRQHandler,	//0x0000 01C4	 DMA2 channel 6 interrup
	(uint32_t)&DMA2_Channel7_IRQHandler,	//0x0000 01C8
	(uint32_t)&DMA2_Channel8_IRQHandler,	//0x0000 01CC
	(uint32_t)&CORDIC_IRQHandler,			//0x0000 01D0
	(uint32_t)&FMAC_IRQHandler				//0x0000 01D4	
};

void Default_Handler(void)
{
	while(1);
}

void Reset_Handler(void)
{
	// copy .data section to SRAM
	uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
	
	uint8_t *pDst = (uint8_t*)&_sdata;  // sram
	uint8_t *pSrc = (uint8_t*)&_sidata; // flash
	
	for(uint32_t i =0 ; i < size ; i++)
	{
		*pDst++ = *pSrc++;
	}
	
	/* Initialize the .bss section to zero in SRAM */
	size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	pDst = (uint8_t*)&_sbss;
	for(uint32_t i =0 ; i < size ; i++)
	{
		*pDst++ = 0;
	}
	
	/* Setup the microcontroller system */
	SystemInit();
	
	/* Call init function of std library */
	__libc_init_array();
	
	/* Call main() */
	main();
}

