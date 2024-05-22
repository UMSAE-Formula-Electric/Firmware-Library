/*
 * errors.c
 *
 *  Created on: Apr 21, 2024
 *      Author: owenzonneveld
 */

#include "errors.h"
#include "string.h"


uint8_t DataTypeToSize(DataType type) {
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

/*
uint8_t datatype_to_size(const enum DataType dt) {
	uint8_t size = 0;

	switch (dt) {
	case FLOAT:
	case INT32:
	case UINT32:
		size = 4;
		break;
	case UINT8:
	case BOOL:
		size = 1;
		break;
	case NONE:
		size = 0;
		break;
	}

	return size;
}
*/

// Initializes the queues
bool log_init() {




	return true;
}

//bool log_error(ERR_TABLE error_type, DataType data_type, uint8_t* data);
//bool log_warning(WARN_TABLE warning_type, DataType data_type, uint8_t* data);
//bool log_info(INFO_TABLE info_type, DataType data_type, uint8_t* data);

_Bool LogGenericMessage(uint8_t *data) {
	return osMessageQueuePut(errorLogQueueHandle, &data, 0, 0) == osOK;
}

uint8_t *BuildGenericMessage(LogLevel log_level, uint8_t message_type, DataType data_type, uint8_t *data) {
	// Send a CAN message
	//Send the chunk of the message over CAN
	uint8_t data_length = DataTypeToSize(data_type);
	uint8_t msg_length = 3 + data_length;
	uint8_t msg_data[8];


	msg_data[0] = (uint8_t)log_level;
	msg_data[1] = message_type;
	msg_data[2] = data_type;

	for (int i = 0; i < data_length; i++) {
		msg_data[i + 3] = data[i];
	}

	return data;
}

_Bool LogError(ERR_TABLE error_type, DataType data_type, uint8_t *data) { // TODO: pass important args to LogError
	uint8_t *message = BuildGenericMessage(LOG_Error, error_type, data_type, data);
	return LogGenericMessage(message);
}

_Bool LogWarn(WARN_TABLE warn_type, DataType data_type, uint8_t *data) { // TODO: pass important args to LogError
	uint8_t *message = BuildGenericMessage(LOG_Warning, warn_type, data_type, data);
	return LogGenericMessage(message);
}

_Bool LogInfo(INFO_TABLE info_type, DataType data_type, uint8_t *data) { // TODO: pass important args to LogError
	uint8_t *message = BuildGenericMessage(LOG_Info, info_type, data_type, data);
	return LogGenericMessage(message);
}
