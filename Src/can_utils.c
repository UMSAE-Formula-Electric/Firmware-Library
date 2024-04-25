/*
 * can_utils.c
 *
 *  Created on: Apr 23, 2024
 *      Author: tonyz
 */

#include <stdint.h>
#include "can_utils.h"
#include "logger.h"

void buildTxPacket(const uint8_t *data, int32_t length, uint32_t dest, uint8_t isRTR, uint8_t isExtended,
                   CAN_TxPacketTypeDef *TxPacket) {
    TxPacket->txPacketHeader.RTR = isRTR ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    TxPacket->txPacketHeader.IDE = isExtended ? CAN_ID_EXT : CAN_ID_STD;
    TxPacket->txPacketHeader.ExtId = isExtended ? dest : TxPacket->txPacketHeader.ExtId;
    TxPacket->txPacketHeader.StdId = isExtended ? TxPacket->txPacketHeader.StdId : dest;
    TxPacket->txPacketHeader.DLC = length;

    for(int i = 0; i < length; i++){
        TxPacket->txPacketData[i] = data[i];
    }
}

uint8_t addMsgToCanTxQueue(CAN_TxPacketTypeDef *TxPacket) {
    uint8_t sendSuccess = 0x0;

    if (osMessageQueuePut(canTxPacketQueueHandle, TxPacket, 0, 0) == osOK) {
        logMessage("Added message to the CAN Tx Queue.\r\n", false);
        return sendSuccess;
    }

    uint32_t currQueueSize = osMessageQueueGetCount(canTxPacketQueueHandle);
    uint32_t maxQueueCapacity = osMessageQueueGetCapacity(canTxPacketQueueHandle);
    if (currQueueSize == maxQueueCapacity) {  /* Queue is full */
        logMessage("Error adding message to transmit to the CAN Tx Queue because the queue is full.\r\n", false);
    }

    sendSuccess = 0x1;
    return sendSuccess;
}

/**
  * @brief  send a can message, delays until sent confirmed.
  * @param  hcan: where x can be 1 or 2 to select the CAN peripheral.
  * @param  data: bytes of data to send, max length 8.
  * @param	length: length of data to send. (Length is 1 indexed for some reason, 7 means 8), likely a bug
  * @param 	dest: destination ID ??? Austin sucks, don't understand CAN at all
  * @param	isRTR: is request for transmission, 1 for request, 0 for data
  * @param	isExtended: is the ID and extended address, 0 for standard, 1 for extended
  * @retval 0 on success, 1 if timeout, 2 hcan not init, 3 length too long
  */
uint8_t sendCan(CAN_HandleTypeDef *hcan, uint8_t const *data, int32_t length, uint32_t dest, uint8_t isRTR, uint8_t isExtended){
    CAN_TxPacketTypeDef TxPacket;
    uint8_t sendSuccess = 0x0;

    //check the length of the data
    if(length > 8){
        sendSuccess = 0x3;
        return sendSuccess;
    }

    //build the can packet
    buildTxPacket(data, length, dest, isRTR, isExtended, &TxPacket);

    //add the can message to the queue
    sendSuccess = addMsgToCanTxQueue(&TxPacket);

    return sendSuccess;
}