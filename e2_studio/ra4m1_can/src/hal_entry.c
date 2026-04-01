/**
  ******************************************************************************
  * @file           : hal_entry.c
  * @brief          : HAL Entry
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "drv.h"
#include "lib.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t u32s_CycleTimeCounter;		/* 周期時間カウンター			*/

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void SysTick_Handler(void);

/**
  * @brief  SysTickタイマー割り込みハンドラ
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	u32s_CycleTimeCounter++;
}

/**
  * @brief  hal_entry
  * @param  None
  * @retval None
  */
void hal_entry(void)
{
	__enable_irq();

	u32s_CycleTimeCounter = 0;
	/* タイマー初期化処理 */
	taskTimerInit();
	/* UARTドライバー初期化処理 */
	taskUartDriverInit();
	/* 初期化関数 */
	setup();
	/* SysTickタイマー開始 */
	// MPUクロック=48MHz → 1tick=1ms
	SysTick_Config(48000000 / 1000);
	/* Infinite loop */
	while (true) {
		/* 周期時間カウンターがシステムの周期時間[ms]に達した場合 */
		if (u32s_CycleTimeCounter >= SYS_CYCLE_TIME) {
			/* Disable Interrupts */
			__disable_irq();
			u32s_CycleTimeCounter = 0;
			/* Enable Interrupts */
			__enable_irq();

			/* タイマー更新処理 */
			taskTimerUpdate();
			/* UARTドライバー入力処理 */
			taskUartDriverInput();
			/* 周期処理関数 */
			loop();
			/* UARTドライバー出力処理 */
			taskUartDriverOutput();
		}
	}
}

/* Private functions ---------------------------------------------------------*/

