/*
 * errors.h
 *
 *  Created on: Apr 21, 2024
 *      Author: owenzonneveld
 */

#ifndef INC_ERRORS_H_
#define INC_ERRORS_H_

#include "stdint.h"
#include "stdbool.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

typedef enum {
	LOG_Error = 0,
	LOG_Warning = 1,
	LOG_Info = 2,
    LOG_Debug = 3,
} LogLevel;

/* This should be updated to reflect all the categories */
typedef enum {
    LOG_HARDWARE,
    LOG_SOFTWARE,
} LogCategory;

typedef enum {
    BOOL,
    CHAR,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    STRING4,
    NONE = 255,
} LogType;

typedef struct {
    uint8_t level;
    uint8_t category;
    uint8_t type;
    uint8_t data[4];
    uint8_t unused;
} LogPacket;

uint8_t dataTypeToSize(LogType type);
_Bool LogMessage(LogLevel logLevel, LogCategory logCategory, LogType logType, uint8_t *data);
_Bool LogError(LogCategory logCategory, LogType logType, uint8_t *data);
_Bool LogWarning(LogCategory logCategory, LogType logType, uint8_t *data);
_Bool LogInfo(LogCategory logCategory, LogType logType, uint8_t *data);
_Bool LogDebug(LogCategory logCategory, LogType logType, uint8_t *data);

extern osMessageQueueId_t errorLogQueueHandle;

#endif /* INC_ERRORS_H_ */
