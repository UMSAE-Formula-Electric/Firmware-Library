#include "logger.h"
#include "usart.h"
#include <string.h>

//Stores the current state of the logger initialization
bool LOGGING_INITIALIZED = false;

//Holds all the sensor values
float data_sensors[NUM_OF_SENSORS] = {0.0f/0.0f};

//Holds the VCUs message sent over CAN (Used by the ACB to decompile the VCUs message)
char VCU_msg[VCU_LOG_MSG_LEN];
int VCU_msgLen = 0;

static int VCU_loggingReady = 0;

/*
 * logInitialize()
 *
 * Initializes the SD card and readies the Bluetooth packet
 */
bool logInitialize() {
	//Ensures it isn't already initialized, and initializes the SD and BT
	if(!LOGGING_INITIALIZED) {
		//On success, set the bool flag and return true
		LOGGING_INITIALIZED = true;
		return true;
	}
	//Return false on any init failures
	return false;
}

/*
 * logTerminate()
 *
 * Closes the SD card/stops logging
 */
bool logTerminate() {
	//Ensures it has been initialized already, and terminates the SD and BT
	if(LOGGING_INITIALIZED) {
		//Reset the bool flag, return true
		LOGGING_INITIALIZED = false;
		return true;
	}
	//Return false on any term failures
	return false;
}

/**
 * Enables the VCU to start the logging process
 */
void enableVCULogging() {
	VCU_loggingReady = 1;
}

void nullTerminate(char *str) {
    size_t length = strlen(str);
    uint8_t isNullTerminated = str[length - 1] != '\0';
    if (isNullTerminated == 0) {
        str[length] = '\0';
    }
}


/*
 * logMessage(char *data, bool critical)
 *
 * Log a diagnostics message, by sending it to the ACB in 8 byte chunks
 *
 * data = Char array (String) that contains the message
 * critical = Boolean flag on if the message is critical, bypassing the log buffer
 */
void logMessage(char *data, bool critical) {
	if (VCU_loggingReady && LOGGING_INITIALIZED) {
		nullTerminate(data);
        HAL_USART_Transmit(&husart2, (uint8_t *)data, strlen(data), 10);
	}
}

