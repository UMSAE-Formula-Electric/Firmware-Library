#ifndef LOGGER_H_
#define LOGGER_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

//	Configuration definitions
#define VCU_LOG_MSG_LEN 256
#define LOG_QUEUE_LENGTH 16
#define LOG_ENABLE_METADATA 0
#define LIBRARY_LOG_LEVEL LOG_DEBUG
#define LOGGING_TASK_ENABLED 1
static QueueHandle_t xLogQueue;
static osThreadId_t loggerTaskHandle;


#define LOG_NONE	0
#define LOG_ERROR	1
#define LOG_WARN	2
#define LOG_INFO	3
#define LOG_DEBUG	4

/* Metadata information to prepend to every log message. */
//TODO Messages were not happy about this being in the macros. Will have to address this in a future update.
#if LOG_ENABLE_METADATA
    #define LOG_METADATA_FORMAT  "[%s:%d][%s] "
    #define LOG_METADATA_ARGS    __FUNCTION__, __LINE__, pcTaskGetName(NULL)
#else
    #define LOG_METADATA_FORMAT  ""
    #define LOG_METADATA_ARGS
#endif

/**
 * @brief Common macro that maps all the logging interfaces,
 * (#LOGDEBUG, #LOGINFO, #LOGWARN, #LOGERROR) to the platform-specific logging
 * function.
 *
 * @note The default definition of this macro generates logging via a printf-like
 * vLoggingPrintf function.
 */
#ifndef SdkLog
    #define SdkLog( message )   vLoggerEnqueuePrintf(message)
#endif

/**
 * Disable definition of logging interface macros when generating doxygen output,
 * to avoid conflict with documentation of macros at the end of the file.
 */
/* Check that LIBRARY_LOG_LEVEL is defined and has a valid value. */
#if !defined( LIBRARY_LOG_LEVEL ) ||       \
    ( ( LIBRARY_LOG_LEVEL != LOG_NONE ) && \
    ( LIBRARY_LOG_LEVEL != LOG_ERROR ) &&  \
    ( LIBRARY_LOG_LEVEL != LOG_WARN ) &&   \
    ( LIBRARY_LOG_LEVEL != LOG_INFO ) &&   \
    ( LIBRARY_LOG_LEVEL != LOG_DEBUG ) )
    #error "Please define LIBRARY_LOG_LEVEL as either LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, or LOG_DEBUG."
#else
    #if LIBRARY_LOG_LEVEL == LOG_DEBUG
        /* All log level messages will logged. */
		#define LOGALWAYS(message, ...) vFormattedLog("ALWAYS", message, ##__VA_ARGS__)
		#define LOGERROR(message, ...) vFormattedLog("ERROR", message, ##__VA_ARGS__)
		#define LOGWARN(message, ...) vFormattedLog("WARN", message "\r\n", ##__VA_ARGS__)
		#define LOGINFO(message, ...) vFormattedLog("INFO", message, ##__VA_ARGS__)
		#define LOGDEBUG(message, ...) vFormattedLog("DEBUG", message, ##__VA_ARGS__)
    #elif LIBRARY_LOG_LEVEL == LOG_INFO
        /* Only INFO, WARNING, ERROR, and ALWAYS messages will be logged. */
		#define LOGALWAYS(message, ...) vFormattedLog("ALWAYS", message, ##__VA_ARGS__)
		#define LOGERROR(message, ...) vFormattedLog("ERROR", message, ##__VA_ARGS__)
		#define LOGWARN(message, ...) vFormattedLog("WARN", message "\r\n", ##__VA_ARGS__)
		#define LOGINFO(message, ...) vFormattedLog("INFO", message, ##__VA_ARGS__)
		#define LOGDEBUG( message )

    #elif LIBRARY_LOG_LEVEL == LOG_WARN
        /* Only WARNING, ERROR, and ALWAYS messages will be logged. */
		#define LOGALWAYS(message, ...) vFormattedLog("ALWAYS", message, ##__VA_ARGS__)
		#define LOGERROR(message, ...) vFormattedLog("ERROR", message, ##__VA_ARGS__)
		#define LOGWARN(message, ...) vFormattedLog("WARN", message "\r\n", ##__VA_ARGS__)
		#define LOGINFO( message )
        #define LOGDEBUG( message )

    #elif LIBRARY_LOG_LEVEL == LOG_ERROR
        /* Only ERROR and ALWAYS messages will be logged. */
		#define LOGALWAYS(message, ...) vFormattedLog("ALWAYS", message, ##__VA_ARGS__)
		#define LOGERROR(message, ...) vFormattedLog("ERROR", message, ##__VA_ARGS__)
		#define LOGWARN( message )
        #define LOGINFO( message )
        #define LOGDEBUG( message )

    #else /* if LIBRARY_LOG_LEVEL == LOG_NONE */

        #define LOGALWAYS( message )
        #define LOGERROR( message )
        #define LOGWARN( message )
        #define LOGINFO( message )
        #define LOGDEBUG( message )

    #endif /* if LIBRARY_LOG_LEVEL == LOG_NONE */
#endif /* if !defined( LIBRARY_LOG_LEVEL ) || ( ( LIBRARY_LOG_LEVEL != LOG_NONE ) && ( LIBRARY_LOG_LEVEL != LOG_ERROR ) && ( LIBRARY_LOG_LEVEL != LOG_WARN ) && ( LIBRARY_LOG_LEVEL != LOG_INFO ) && ( LIBRARY_LOG_LEVEL != LOG_DEBUG ) ) */

extern char SD_ERROR_STATE;

/*	Typedef'd enumerator for sensors
*		Final value, NUM_OF_SENSORS returns total number of sensors
*		--ALWAYS HAVE NUM_OF_SENSORS AS THE LAST ENUM VALUE--
*		--IF YOU ADD A SENSOR, DON'T FORGET TO ADD TO THE SENSORNAMES ARRAY--
*/
typedef enum {
	TR_SENS1, 					// 01
	TR_SENS2, 					// 02
	TR_REQUEST, 				// 03
	MC_ACUAL_SPEED_REG_LOG, 	// 04
	MC_ACUAL_SPEED_REG_LOG100,	// 05
	MC_N_CMD_LOG,				// 06
	MC_N_CMD_LOG100,			// 07
	MC_N_CMD_RAMP_LOG,			// 08
	MC_N_CMD_RAMP_LOG100,		// 09
	MC_N_ERROR_LOG,				// 10
	MC_N_ERROR_LOG100,			// 11
	MC_I_CMD_LOG,				// 12
	MC_I_CMD_RAMP_LOG,			// 13
	MC_I_ACTUAL_LOG,			// 14
	MC_V_D_LOG,					// 15
	MC_V_Q_LOG,					// 16
	MC_V_OUT_LOG,				// 17
	MC_T_MOTOR_LOG,				// 18
	MC_T_IGBT_LOG,				// 19
	MC_T_AIR_LOG,				// 20
	MC_DIG_TORQUE_LOG,			// 21
	MC_BUS_VOLTAGE_LOG,			// 22
	MC_BUS_VOLTAGE_LOG100,		// 23
	FR_WHEEL_SPEED,				// 24
	FL_WHEEL_SPEED,				// 25
	BRAKE_1,					// 26
	BRAKE_2,					// 27
	MC_RPM,                     // 28
	BNO055_ACCEL_X,             // 29
	BNO055_ACCEL_Y,             // 30
	BNO055_ACCEL_Z,             // 31
	BNO055_GYRO_X,              // 32
	BNO055_GYRO_Y,                // 33
	BNO055_GYRO_Z,                // 34
	VBATT,							//35
	T_IC,							//36
	NUM_OF_SENSORS				// 37 (36)
} SENSOR;

/* Typedef'd enumerator for indicators
 * 		Final value, NUM_OF_INDICATORS returns total number of indicators
 */
typedef enum {
	GENERAL_ERROR,				// 00
	GENERAL_WARNING,			// 01
	LOW_BATTERY,				// 02
	NO_ACB,						// 03
	THROTTLE_ERROR,				// 04
	SAFETY_LOOP,				// 05
	MESSAGE,					//06
	NUM_OF_INDICATORS			// 07
} INDICATOR;

typedef struct {
    uint8_t header;  // Level:bits[7-6], Task:bits[5-2], State:bits[1-0]
    char message[128];
} log_message_t;


// Debug message structure
typedef struct {
	uint8_t start;
    uint8_t header;         // Packed 2-4-2 bit fields
    char message[256];      // Null-terminated string
    uint8_t checksum;
    uint8_t end;
} DebugMessage_t;


//Sensor-Related
extern float data_sensors[NUM_OF_SENSORS];
extern char *data_ids_sd[NUM_OF_SENSORS+1];
extern char *data_ids_bt[NUM_OF_SENSORS+NUM_OF_INDICATORS];

/**
 * BT ERROR STATES:
 *
 * 0x00: No Error
 * 0x01: Failed to Create BT RTOS Task for Dumping
 * 0x02: Invalid Data Entry Type
 **/

extern char BT_ERROR_STATE;

/**
 * SD ERROR STATES:
 *
 * 0x00: No Error
 * 0x01: Mounting Error (No SD card inserted?)
 * 0x02: Failed to Create Folders
 * 0x03: Failed to Create/Open Files
 * 0x04: Failed to Seek to End of Files
 * 0x05: Failed to Dump Sensor Headers
 * 0x06: Failed to Create Diagnostics Logging Queue
 * 0x07: Failed to Create SD RTOS Task for Dumping
 * 0x08: Failed to Get Free Space Available
 * 0x09: Not Enough Free Space Available
 **/

bool logInitialize(void);
bool logTerminate(void);
// Forward declaration
void vUSARTLoggerTask(void *pvParameters);
/**
 * @brief Logs a formatted message with a specified log level to a queue.
 *
 * This function formats a log message with a variable number of arguments, prefixes it with a log level (e.g., "[INFO]"),
 * and sends it to a FreeRTOS queue for processing. If the queue is full, it falls back to sending an error message via USART.
 *
 * @param Log_Level The severity level of the log (e.g., "INFO", "ERROR", "DEBUG").
 * @param format A printf-style format string for the log message.
 * @param ... Variable arguments to be formatted into the log message.
 *
 * @note
 * - Requires `LOGGING_INITIALIZED` to be true; otherwise, the function does nothing.
 * - Uses `vsnprintf` to safely format the message with a fixed buffer size (`VCU_LOG_MSG_LEN`).
 * - Appends a timestamp (`xTaskGetTickCount()`) and a header byte (currently `0x00`) to the log message.
 * - If the queue (`xLogQueue`) is full, it transmits a "Queue full!" warning via USART.
 * - The message is truncated if it exceeds the buffer size.
 *
 * @warning
 * - Ensure `xLogQueue` is properly initialized before calling this function.
 *
 * @see xQueueSend(), vsnprintf(), HAL_USART_Transmit()
 */
void vFormattedLog(const char *Log_Level, const char *format, ...);


void logMessage(char *data, bool critical);
void enableVCULogging();
void nullTerminate(char *str);


// Initialize debug system
void debug_init(void);

// Send debug message
void debug_log(void);

#endif
