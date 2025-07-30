/*
 * can_utils.h
 *
 *  Created on: Apr 23, 2024
 *      Author: tonyz
 */

#ifndef INC_CAN_UTILS_H_
#define INC_CAN_UTILS_H_

/* Defines */
#include "main.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

#define CAN_MC_ACTIVE_MESSAGES  0x0C1
#define CAN_MC_RESPONSE_MSG     0x0C2 // unused but move to general CAN ID file for future use

// Keep these values for now and change them when you can do CAN bus testing
// TODO: consider general purpose board-to-board msg IDs ex. the prev: #define CAN_ACU_CAN_ID 0x69

/* HeartBeats */
#define CAN_VCU_SET_ACB_STATE_ID	0x001
#define CAN_ACU_TO_VCU_ID           0x002
#define CAN_VCU_TO_ACU_ID           0x003

/* CAN Message Flags */
#define CAN_NO_EXT 				0 // Define to send a CAN 2.0 message (11-bit standard ID)
#define CAN_EXT 				1 // Define to send a CAN 2.0B message (29-bit extended ID) 

#define CAN_NO_RTR 				0
#define CAN_RTR 				1 // not used? use CAN HAL version?

extern osMessageQueueId_t canRxPacketQueueHandle;
extern osMessageQueueId_t canTxPacketQueueHandle;
extern CAN_HandleTypeDef hcan1;
// TODO: Add hcan2 extern ref if we want to implement that on any board

typedef struct {
    CAN_RxHeaderTypeDef rxPacketHeader;
    uint8_t rxPacketData[8];
} CAN_RxPacketTypeDef;

typedef struct {
    CAN_TxHeaderTypeDef txPacketHeader;
    uint8_t txPacketData[8];
} CAN_TxPacketTypeDef;

enum STARTUP_STATUS_NOTIFY_MSG{
    CAN_ACB_TSA_ACK = 0,
    CAN_ACB_TSA_NACK,
    CAN_ACB_RTD_ACK,
    CAN_ACB_RTD_NACK,
    CAN_GO_IDLE_REQ,  //Request to go idle
    CAN_NO_SAFETY_LOOP_SET,  //Message to VCU to indicate that the safety loop is open at the VCU. Used when the car is idle
    CAN_NO_SAFETY_LOOP_CLEAR,//Message to VCU to indicate that the safety loop is closed at the VCU. Used when the car is idle
    CAN_AIR_WELD_SET,
    CAN_HEARTBEAT_REQUEST,
    CAN_HEARTBEAT_RESPONSE,
};

/* End Defines */

/* Prototypes */
uint8_t sendCan(CAN_HandleTypeDef* hcan, uint8_t const * data, uint32_t length, uint32_t dest, uint32_t canRTR, uint8_t isExtended);
/* End Prototypes */
#endif /* INC_CAN_UTILS_H_ */
