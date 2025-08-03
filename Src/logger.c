#include "logger.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
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
 * Initializes logging queue
 *
 */
bool logInitialize() {
	//Ensures it isn't already initialized, and initializes the SD and BT
	if(!LOGGING_INITIALIZED) {
		//On success, set the bool flag and return true

		xLogQueue = xQueueCreate(LOG_QUEUE_LENGTH, sizeof(log_message_t));
		if(xLogQueue == NULL){
			HAL_USART_Transmit(&husart2, (uint8_t *) "Logger queue failed to initialize\r\n", strlen("Logger queue failed to initialize\r\n"), 10);
			return false;
		}

        osThreadAttr_t loggerTaskAttr = {
            .name = "USARTLogger",
            .stack_size = 512,  // Adjust as needed
            .priority = osPriorityNormal,  // Maps to tskIDLE_PRIORITY + 1
        };

        osThreadId_t threadId = osThreadNew(vUSARTLoggerTask, NULL, &loggerTaskAttr);
        if (threadId == NULL) {
            vQueueDelete(xLogQueue);
            return false;
        }

		LOGGING_INITIALIZED = true;

		HAL_USART_Transmit(&husart2, (uint8_t *) "Logger queue initialized.\r\n", strlen("Logger queue initialized.\r\n"), 10);


		return true;
	}


	//Return false on any init failures
	return false;
}

/*
 * logTerminate()
 *
 * Closes the SD card/stops logging and cleans up resources
 */
bool logTerminate() {
    // Only proceed if logging is initialized
    if(!LOGGING_INITIALIZED) {
        return false;
    }

    bool success = true;

    // Delete the logger task
    osThreadId_t loggerTaskHandle = osThreadGetId("USARTLogger");
    if(loggerTaskHandle != NULL) {
        osThreadTerminate(loggerTaskHandle);
    } else {
        success = false;
    }

    // Delete the queue
    if(xLogQueue != NULL) {
        // First empty the queue if needed
        while(uxQueueMessagesWaiting(xLogQueue) > 0) {
            log_message_t dummy;
            xQueueReceive(xLogQueue, &dummy, 0);
        }
        vQueueDelete(xLogQueue);
        xLogQueue = NULL;
    } else {
        success = false;
    }

    // Reset the initialization flag only if everything succeeded
    if(success) {
        LOGGING_INITIALIZED = false;
        HAL_USART_Transmit(&husart2, (uint8_t *) "Logger terminated successfully.\r\n",
                          strlen("Logger terminated successfully.\r\n"), 10);
    } else {
        HAL_USART_Transmit(&husart2, (uint8_t *) "Logger termination had errors.\r\n",
                          strlen("Logger termination had errors.\r\n"), 10);
    }

    return success;
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


void vFormattedLog(const char *Log_Level, const char *format, ...) {
    if (LOGGING_INITIALIZED) {
        char buffer[VCU_LOG_MSG_LEN];

        // All the additional arguments used in the log will be handled by this block of code.
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        // Put the log level in front of the message
        log_message_t msg;
        snprintf(msg.message, sizeof(msg.message), "[%s] %s", Log_Level, buffer);
        msg.message[sizeof(msg.message) - 1] = '\0';
        msg.timestamp = xTaskGetTickCount();
        msg.header = 0x00; //TODO: This will be used in a future External application that will read the messages to an external computer.

        if (xQueueSend(xLogQueue, &msg, pdMS_TO_TICKS(10)) != pdPASS) {
            HAL_USART_Transmit(&husart2, (uint8_t *)"Queue full!\r\n", strlen("Queue full!\r\n"), 100);
        }
    }
}


void vUSARTLoggerTask(void *pvParameters) {
    log_message_t msg;
    HAL_USART_Transmit(&husart2, (uint8_t *)"Usart Task started\r\n", strlen("Usart Task started\r\n"), HAL_MAX_DELAY);
    for (;;) {
        if (xQueueReceive(xLogQueue, &msg, pdMS_TO_TICKS(10)) == pdPASS) {
            // You can use HAL_UART_Transmit or any USART API here
            char buffer[300];
            snprintf(buffer, sizeof(buffer), "[%lu] %s\r\n", msg.timestamp, msg.message);
            HAL_USART_Transmit(&husart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        }
    }
}

