/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hal_data.h"
#include "lld.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* ON/OFF定義 */
#define OFF					(0)
#define ON					(1)

/* OK/NG定義 */
#define NG					(0)
#define OK					(1)

/* LOW/HIGH定義 */
#define LOW					(0)
#define HIGH				(1)

#define SYS_CYCLE_TIME		(5)		/* システムの周期時間[ms]		*/

/* IRQ番号の割り当て */
#define IRQ_SCI9_RXI		(1)		/* SCI9受信データフル割り込み			*/
#define IRQ_SCI9_TXI		(2)		/* SCI9送信データエンプティ割り込み		*/
#define IRQ_SCI9_ERI		(3)		/* SCI9受信エラー割り込み				*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* main_app.c */
extern void setup(void);									/* 初期化関数							*/
extern void loop(void);										/* 周期処理関数							*/
extern void Error_Handler(void);							/* エラー処理ハンドラ					*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
