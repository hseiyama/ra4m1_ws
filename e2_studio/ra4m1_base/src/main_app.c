/**
  ******************************************************************************
  * @file           : main_app.c
  * @brief          : MAINアプリケーション
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "drv.h"
#include "lib.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIME_1S				(1000)					/* 1秒判定時間[ms]			*/
#define UART_BUFF_SIZE		(64)					/* UARTバッファサイズ		*/

/* UART命令 */
#define UART_CMD_HELP		(0x08)					/* ヘルプ表示(^H)			*/
#define UART_CMD_RESET		(0x12)					/* リセット(^R)				*/
#define UART_CMD_SLEEP		(0x13)					/* スリープ(^S)				*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static Timer sts_Timer1s;							/* 1秒タイマー				*/
static uint8_t u8s_RcvData[UART_BUFF_SIZE];			/* UART受信データ			*/
static uint16_t u16s_RcvDataSize;					/* UART受信データサイズ		*/

/* Private function prototypes -----------------------------------------------*/
static void port_irq_init(void);					/* PORT_IRQ 初期化処理					*/

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  外部端子割り込みコールバック
  * @param  p_args: コールバック引数
  * @retval None
  */
void ICU_IRQ_Callback(external_irq_callback_args_t * p_args)
{
	(void)p_args;
	/* 文字を出力する */
	uartEchoStr("Irq0");
}

/**
  * @brief  初期化関数
  * @param  None
  * @retval None
  */
void setup(void)
{
	mem_set08(&u8s_RcvData[0], 0x00, UART_BUFF_SIZE);
	u16s_RcvDataSize = 0;

	/* PORT_IRQ 初期化処理 */
	port_irq_init();

	/* タイマーを開始する */
	startTimer(&sts_Timer1s);

	/* プログラム開始メッセージを表示する */
	uartEchoStrln("");
	uartEchoStrln("Start UART/GPIO sample!!");
}

/**
  * @brief  周期処理関数
  * @param  None
  * @retval None
  */
void loop(void)
{
	/* UART受信データを取得する */
	u16s_RcvDataSize = uartGetRxData(&u8s_RcvData[0], UART_BUFF_SIZE);
	/* UART受信データが存在する場合 */
	if (u16s_RcvDataSize > 0) {
		/* UART送信データを登録する */
		uartSetTxData(&u8s_RcvData[0], u16s_RcvDataSize);

		/* UART命令解析 */
		switch (u8s_RcvData[0]) {
		/* ヘルプ表示(^H) */
		case UART_CMD_HELP:
			/* UART命令表示 */
			uartEchoStrln("");
			uartEchoStrln("^H :Help");
			uartEchoStrln("^R :Reset");
			uartEchoStrln("^S :Sleep");
			break;
		/* リセット(^R) */
		case UART_CMD_RESET:
			/* リセット処理 */
			NVIC_SystemReset();
			break;
		/* スリープ(^S) */
		case UART_CMD_SLEEP:
			/* SysTickタイマー停止 */
			LL_SYSTICK_DisableIT();
			/* イベント待機 */
			__WFE();
			__WFE();
			/* SysTickタイマー開始 */
			LL_SYSTICK_EnableIT();
			/* 文字を出力する */
			uartEchoStr("<Wakeup!!>");
			break;
		}
	}

	/* 1秒判定時間が満了した場合 */
	if (checkTimer(&sts_Timer1s, TIME_1S)) {
		/* ユーザーLEDを反転出力する */
		R_PORT0->PODR_b.PODR12 = !R_PORT0->PODR_b.PODR12;
		/* 文字を出力する */
		uartEchoStr(".");

		/* タイマーを再開する */
		startTimer(&sts_Timer1s);
	}
}

/**
  * @brief  エラー処理ハンドラ
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
	__disable_irq();
	while (true) {
		/* D1 LED(P012)を反転出力する */
		R_PORT0->PODR_b.PODR12 = !R_PORT0->PODR_b.PODR12;
		/* 100ms待つ */
		LL_mDelay(100);
	}
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  PORT_IRQ 初期化処理
  * @param  None
  * @retval None
  */
static void port_irq_init(void)
{
	/* Configure the external interrupt. */
	R_ICU_ExternalIrqOpen(&g_external_irq0_ctrl, &g_external_irq0_cfg);

	/* Enable the external interrupt. */
	R_ICU_ExternalIrqEnable(&g_external_irq0_ctrl);
}

