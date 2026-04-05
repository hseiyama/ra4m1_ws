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

static volatile bool g_rx_flag  = false;
static volatile bool g_tx_flag  = false;
static volatile bool g_err_flag = false;
static volatile uint32_t g_rx_id;
static can_frame_t g_can_tx_frame;
static can_frame_t g_can_rx_frame;

/* Private function prototypes -----------------------------------------------*/
static void can_init(void);							/* CAN 初期化処理						*/
static void can_send(void);							/* CAN 送信処理							*/

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  CAN割り込みコールバック
  * @param  p_args: コールバック引数
  * @retval None
  */
void CAN_Callback(can_callback_args_t * p_args)
{
	switch (p_args->event) {
	case CAN_EVENT_RX_COMPLETE:				/* Receive complete event. */
		g_rx_flag = true;
		g_rx_id = p_args->frame.id;
		/* Read received frame */
		g_can_rx_frame = p_args->frame;
		break;
	case CAN_EVENT_TX_COMPLETE:				/* Transmit complete event. */
		g_tx_flag = true;
		break;
	case CAN_EVENT_ERR_BUS_OFF:				/* Bus error event. (bus off) */
	case CAN_EVENT_ERR_PASSIVE:				/* Bus error event. (error passive) */
	case CAN_EVENT_ERR_WARNING:				/* Bus error event. (error warning) */
	case CAN_EVENT_BUS_RECOVERY:			/* Bus error event. (bus recovery) */
	case CAN_EVENT_MAILBOX_MESSAGE_LOST:	/* Overwrite/overrun error */
		/* Set error flag */
		g_err_flag = true;
		break;
	default:
		break;
	}
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

	/* CAN 初期化処理 */
	can_init();

	/* タイマーを開始する */
	startTimer(&sts_Timer1s);

	/* プログラム開始メッセージを表示する */
	uartEchoStrln("");
	uartEchoStrln("Start CAN sample!!");
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
		/* CAN 送信処理 */
		can_send();
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
  * @brief  CAN 初期化処理
  * @param  None
  * @retval None
  */
static void can_init(void)
{
	/* Initialize the CAN module */
	R_CAN_Open(&g_can0_ctrl, &g_can0_cfg);
}

/**
  * @brief  CAN 送信処理
  * @param  None
  * @retval None
  */
static void can_send(void)
{
    g_can_tx_frame.id = 0x123;
    g_can_tx_frame.type = CAN_FRAME_TYPE_DATA;
    g_can_tx_frame.data_length_code = 8;
    /* Write some data to the transmit frame */
	g_can_tx_frame.data[0] = 0x11;
	g_can_tx_frame.data[1] = 0x12;
	g_can_tx_frame.data[2] = 0x13;
	g_can_tx_frame.data[3] = 0x14;
	g_can_tx_frame.data[4] = 0x15;
	g_can_tx_frame.data[5] = 0x16;
	g_can_tx_frame.data[6] = 0x17;
	g_can_tx_frame.data[7] = 0x18;
    /* Send data on the bus */
    g_tx_flag  = false;
    g_err_flag = false;
    R_CAN_Write(&g_can0_ctrl, CAN_MAILBOX_ID_0, &g_can_tx_frame);
}

