/*
 * errors.c
 *
 *  Created on: Apr 21, 2024
 *      Author: owenzonneveld
 */

#include "errors.h"
#include "string.h"




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

// Initializes the queues
bool log_init() {




	return true;
}

#ifdef SCU

//TODO! Add the other log from can message...
bool log_can_msg(void *can_data) {
	if (can_data == NULL) { return false; }

	LogLevel log_level = can_data[0];

	uint8_t log_type = can_data[1]; // Should be guaranteed to be a valid entry from the table index
	DataType data_type = can_data[2];

	void *can_bytes = &can_data[3];

	switch (log_level) {
	case LogLevel.Error:
		return log_error(log_type, data_type, can_bytes);
		break;
	case LogLevel.Warning:
		return log_warning(log_type, data_type, can_bytes);
		break;
	case LogLevel.Info:
		return log_info(log_type, data_type, can_bytes);
		break;
	}
}

bool log_error(ERR_TABLE error_type, DataType data_type, void* data) {
	LogLevel log_level = LogLevel.Error;	// Error

	return true;
}

bool log_warning(WARN_TABLE warning_type, DataType data_type, void* data) {
	return false;
}

bool log_info(INFO_TABLE info_type, DataType data_type, void* data) {
	return false;
}
#endif


#if defined VCU || defined ACU

bool log_error(ERR_TABLE error_type, DataType data_type, void* data) {
	LogLevel log_level = LogLevel.Error;	// Error

	// Send a CAN message
	//Send the chunk of the message over CAN
	uint8_t data_length = datatype_to_size(data_type);
	uint8_t msg_length = 3 + data_length;
	uint8_t msg_data[8];

	msg_data[0] = (uint8_t)log_level;
	msg_data[1] = error_type;
	msg_data[2] = data_type;

	for (int i = 0; i < data_length; i++) {
		msg_data[i + 3] = data[i];
	}

	return sendCan(CAN1, msg_data, msg_length, CAN_LOG_ERROR_ID, CAN_NO_EXT, CAN_NO_EXT) == 0;
}

bool log_warning(WARN_TABLE warning_type, DataType data_type, void* data) {
	LogLevel log_level = LogLevel.Warning;	// Warning

	// Send a CAN message
	//Send the chunk of the message over CAN
	uint8_t data_length = datatype_to_size(data_type);
	uint8_t msg_length = 3 + data_length;
	uint8_t msg_data[8];

	msg_data[0] = (uint8_t)log_level;
	msg_data[1] = warning_type;
	msg_data[2] = data_type;

	for (int i = 0; i < data_length; i++) {
		msg_data[i + 3] = data[i];
	}

	return sendCan(CAN1, msg_data, msg_length, CAN_LOG_WARNING_ID, CAN_NO_EXT, CAN_NO_EXT) == 0;
}

bool log_info(INFO_TABLE info_type, DataType data_type, void* data) {
	LogLevel log_level = LogLevel.Info;	// Info

	// Send a CAN message
	//Send the chunk of the message over CAN
	uint8_t data_length = datatype_to_size(data_type);
	uint8_t msg_length = 3 + data_length;
	uint8_t msg_data[8];

	msg_data[0] = (uint8_t)log_level;
	msg_data[1] = info_type;
	msg_data[2] = data_type;

	for (int i = 0; i < data_length; i++) {
		msg_data[i + 3] = data[i];
	}

	return sendCan(CAN1, msg_data, msg_length, CAN_LOG_INFO_ID, CAN_NO_EXT, CAN_NO_EXT) == 0;
}

#endif
