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

static QueueHandle_t xLogQueue;

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

		xLogQueue = xQueueCreate(LOG_QUEUE_LENGTH, sizeof(log_message_t));
		if(xLogQueue == NULL){
			return false;
		}

		if(xTaskCreate(vUSARTLoggerTask, "USARTLogger", 512, NULL, tskIDLE_PRIORITY + 1, NULL)!= pdPASS){
			return false;
		}

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

void vLoggerEnqueuePrintf(const char *format, ...) {
    log_message_t msg;
    va_list args;
    va_start(args, format);
    vsnprintf(msg.message, VCU_LOG_MSG_LEN, format, args);
    va_end(args);

    msg.timestamp = xTaskGetTickCount(); // Or use your own timestamp
    msg.header = 0x00; // Optional: pack level info

    xQueueSend(xLogQueue, &msg, portMAX_DELAY);
}

void vFormattedLog(const char *level, const char *format, ...) {
    if (!LOGGING_INITIALIZED || xLogQueue == NULL) {
        return;
    }

    char buffer[VCU_LOG_MSG_LEN];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log_message_t msg;
    strncpy(msg.message, buffer, sizeof(msg.message));
    msg.message[sizeof(msg.message) - 1] = '\0';
    msg.timestamp = xTaskGetTickCount();
    msg.header = 0x00; // Optional: encode level/task/etc.

    xQueueSend(xLogQueue, &msg, portMAX_DELAY);
}


void vUSARTLoggerTask(void *pvParameters) {
    log_message_t msg;

    for (;;) {
        if (xQueueReceive(xLogQueue, &msg, portMAX_DELAY) == pdPASS) {
            // You can use HAL_UART_Transmit or any USART API here
            char buffer[300];
            snprintf(buffer, sizeof(buffer), "[%lu] %s\r\n", msg.timestamp, msg.message);
            HAL_USART_Transmit(&husart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        }
    }
}

