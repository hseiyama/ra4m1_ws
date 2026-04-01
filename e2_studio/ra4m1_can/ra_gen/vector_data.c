/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
            [1] = can_error_isr, /* CAN0 ERROR (Error interrupt) */
            [2] = can_rx_isr, /* CAN0 MAILBOX RX (Reception complete interrupt) */
            [3] = can_tx_isr, /* CAN0 MAILBOX TX (Transmission complete interrupt) */
            [4] = can_rx_isr, /* CAN0 FIFO RX (Receive FIFO interrupt) */
            [5] = can_tx_isr, /* CAN0 FIFO TX (Transmit FIFO interrupt) */
            [6] = SCI9_RXI_Handler, /* SCI9 RXI (Receive data full) */
            [7] = SCI9_TXI_Handler, /* SCI9 TXI (Transmit data empty) */
            [8] = SCI9_ERI_Handler, /* SCI9 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP0), /* ICU IRQ0 (External pin interrupt 0) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_CAN0_ERROR,GROUP1), /* CAN0 ERROR (Error interrupt) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_CAN0_MAILBOX_RX,GROUP2), /* CAN0 MAILBOX RX (Reception complete interrupt) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_CAN0_MAILBOX_TX,GROUP3), /* CAN0 MAILBOX TX (Transmission complete interrupt) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_CAN0_FIFO_RX,GROUP4), /* CAN0 FIFO RX (Receive FIFO interrupt) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_CAN0_FIFO_TX,GROUP5), /* CAN0 FIFO TX (Transmit FIFO interrupt) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP6), /* SCI9 RXI (Receive data full) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP7), /* SCI9 TXI (Transmit data empty) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP0), /* SCI9 ERI (Receive error) */
        };
        #endif
        #endif
