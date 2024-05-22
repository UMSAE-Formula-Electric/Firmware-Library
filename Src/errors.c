/*
 * errors.c
 *
 *  Created on: Apr 21, 2024
 *      Author: owenzonneveld
 */

#include "errors.h"
#include "string.h"

#define UNUSED_BYTE 0xFF

uint8_t dataTypeToSize(LogType type) {
	switch(type) {
		case BOOL:
		case CHAR:
		case INT8:
		case UINT8:
			return 1;
		case INT16:
		case UINT16:
			return 2;
		case INT32:
		case UINT32:
		case STRING4:
			return 4;
		default:
			return 0;
	}
}

/**
 * @brief Adds a message to the error log queue
 * This log queue implementation depends on the control unit.
 * VCU/ACU: The log queue is sent to the SCU via CAN
 * SCU: The log is sent directly to the SD Card/ Wireless Comms
 *
 * NOTE: NEVER USE WITHIN THE CAN TX/RX FREERTOS TASKS (CAUSES AN INFINITE LOOP)
 * @param logLevel The level of the log message
 * @param logCategory The category of the log message
 * @param logType The type of the log message
 * @param data The data to log
 *
 * @return True if the message was successfully added to queue, false otherwise
 * */
_Bool LogMessage(LogLevel logLevel, LogCategory logCategory, LogType logType, uint8_t *data){
    LogPacket logPacket;

    logPacket.level = logLevel;
    logPacket.category = logCategory;
    logPacket.type = logType;
    for (int i = 0; i < dataTypeToSize(logType); i++) {
        logPacket.data[i] = data[i];
    }
    logPacket.unused = UNUSED_BYTE;

    return osMessageQueuePut(errorLogQueueHandle, &logPacket, 0, 0) == osOK;
}

/**
 * @brief Logs an error message, check LogMessage() brief for more info
 */
_Bool LogError(LogCategory logCategory, LogType logType, uint8_t *data){
    return LogMessage(LOG_Error, logCategory, logType, data);
}

/**
 * @brief Logs a warning message, check LogMessage() brief for more info
 */
_Bool LogWarning(LogCategory logCategory, LogType logType, uint8_t *data){
    return LogMessage(LOG_Warning, logCategory, logType, data);
}

/**
 * @brief Logs an info message, check LogMessage() brief for more info
 */
_Bool LogInfo(LogCategory logCategory, LogType logType, uint8_t *data){
    return LogMessage(LOG_Info, logCategory, logType, data);
}

/**
 * @brief Logs a debug message, check LogMessage() brief for more info
 */
_Bool LogDebug(LogCategory logCategory, LogType logType, uint8_t *data){
    return LogMessage(LOG_Debug, logCategory, logType, data);
}