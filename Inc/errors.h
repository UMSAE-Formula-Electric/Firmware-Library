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

typedef enum LogLevel {
	LOG_Error = 0,
	LOG_Warning = 1,
	LOG_Info = 2
} LogLevel;

/*
typedef enum DataType {
	FLOAT = 0,
	INT32 = 1,
	UINT8 = 2,
	UINT32 = 4,
	BOOL = 8,
	NONE = 16,
} DataType;
*/

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
} DataType;

//uint8_t datatype_to_size(const enum DataType dt);

uint8_t DataTypeToSize(DataType type);

// All these tables must fit inside a CAN message using only 1 Byte
// So there can only be 256 of them from 0 - 255
typedef enum ErrorTable {
	ERR_BATTERY_VOLTAGE_LOWISH = 0,
	ERR_BATTERY_VOLTAGE_LOW = 1,
	ERR_BATTERY_VOLTAGE_LOWER = 2,
	ERR_BATTERY_VOLTAGE_LOWEST = 3,

	ERR_AMOUNT
} ERR_TABLE;

typedef enum WarningTable {
	WARN_BATTERY_VOLTAGE_LOWISH = 0,
	WARN_BATTERY_VOLTAGE_LOW = 1,
	WARN_BATTERY_VOLTAGE_LOWER = 2,
	WARN_BATTERY_VOLTAGE_LOWEST = 3,

	WARN_AMOUNT				// Tracks the Amount of values in the table
} WARN_TABLE;

typedef enum InfoTable {
	INFO_BATTERY_VOLTAGE = 0,

	INFO_AMOUNT
} INFO_TABLE;

// TO REMOVE
//
//// Logging functions
//
//#ifdef SCU
//bool log_can_msg(void *can_data);
//#endif

extern osMessageQueueId_t errorLogQueueHandle;

#endif /* INC_ERRORS_H_ */
