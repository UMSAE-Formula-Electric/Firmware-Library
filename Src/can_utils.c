/*
 * can_utils.c
 *
 *  Created on: Apr 23, 2024
 *      Author: tonyz
 */

#include <stdint.h>
#include "can_utils.h"
#include "logger.h"

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
    uint8_t sendSuccess = 0x0;

    //check the length of the data
    if(length > 8){
        sendSuccess = 0x3;
        return sendSuccess;
    }
    //check type of message to send
    if(isRTR){
        TxHeader.RTR = CAN_RTR_REMOTE;
    }
    else{
        TxHeader.RTR = CAN_RTR_DATA;
    }
    if(isExtended){
        TxHeader.IDE = CAN_ID_EXT;
        TxHeader.ExtId = dest;
    }
    else{
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.StdId = dest;
    }
    //copy data
    for(int i = 0; i < length; i++){
        TxData[i] = data[i];
    }
    TxHeader.DLC = length;

    //send the can message
    if (HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        logMessage("VCU couldn't send a message to the CAN Bus.\r\n", true);
    }
    else {
        logMessage("VCU sent a message to the CAN Bus.\r\n", true);
        sendSuccess = 0x1;
    }

    return sendSuccess;
}