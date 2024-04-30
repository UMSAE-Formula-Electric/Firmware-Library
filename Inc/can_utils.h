/*
 * can_utils.h
 *
 *  Created on: Apr 23, 2024
 *      Author: tonyz
 */

#ifndef INC_CAN_UTILS_H_
#define INC_CAN_UTILS_H_

/* Defines */
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#define CAN_MC_QUEUE_LENGTH 		64
#define CAN_AMS_QUEUE_LENGTH 		64
#define CAN_ACB_VCU_QUEUE_LENGTH 	64
#define CAN_QUEUE_LENGTH 			64

#define CAN_QUEUE_ITEM_SIZE sizeof( CanRxMsg )

#define CANTXTIMEOUT 			10 //number of milleseconds to wait for the can packet to send
#define CANTXMBTIMEOUT			10 //number of retires to get a mailbox on send

#define CAN_NO_RTR 				0
#define CAN_RTR 				1

#define CAN_SUB_Q_DELAY_MS 		100

#define CAN_MC_ACTIVE_MESSAGES  0x0C1
#define CAN_MC_RESPONSE_MSG     0x0C2

#define CAN_ACU_CAN_ID			0x69
#define CAN_VCU_CAN_ID			0x88
#define CAN_SCU_CAN_ID			0x89
#define CAN_AMS_CAN_ID          0x70
#define CAN_VCU_LOG_ID			0x71 //ID for sending VCU data to ACB

/* HeartBeats */
#define CAN_ACU_TO_VCU_ID 0x200
#define CAN_ACU_TO_SCU_ID 0x201
#define CAN_VCU_TO_ACU_ID 0x202
#define CAN_VCU_TO_SCU_ID 0x203
#define CAN_SCU_TO_ACU_ID 0x204
#define CAN_SCU_TO_VCU_ID 0x205

#define CAN_BMS_BASE_ID         0x10

#define CAN_BMS_OVERALL_ID		CAN_BMS_BASE_ID + 0
#define CAN_BMS_VOLTAGE_ID		CAN_BMS_BASE_ID + 1
#define CAN_BMS_DIAGNOSTIC_ID	CAN_BMS_BASE_ID + 7
#define CAN_BMS_TEMP_ID			CAN_BMS_BASE_ID + 8

#define CAN_VCU_SET_ACB_STATE_ID	0x01

#define CAN_EXT 				1
#define CAN_NO_EXT 				0

extern osMessageQueueId_t canRxPacketQueueHandle;
extern osMessageQueueId_t canTxPacketQueueHandle;
extern CAN_HandleTypeDef hcan1;

typedef struct {
    CAN_RxHeaderTypeDef rxPacketHeader;
    uint8_t rxPacketData[8];
} CAN_RxPacketTypeDef;

typedef struct {
    CAN_TxHeaderTypeDef txPacketHeader;
    uint8_t txPacketData[8];
} CAN_TxPacketTypeDef;

/* End Defines */

/* Prototypes */
uint8_t sendCan(CAN_HandleTypeDef* hcan, uint8_t const * data, uint32_t length, uint32_t dest, uint32_t canRTR, uint8_t isExtended);
/* End Prototypes */
#endif /* INC_CAN_UTILS_H_ */
