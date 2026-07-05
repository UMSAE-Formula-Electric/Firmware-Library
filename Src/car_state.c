#include "car_state.h"
#include "FreeRTOS.h"
#include "semphr.h"

static enum CAR_STATE current_state = IDLE;
static SemaphoreHandle_t carStateMutex = NULL;

void car_state_init(void){
	carStateMutex = xSemaphoreCreateMutex();
}

void set_car_state(enum CAR_STATE new_state){
	if(new_state < NUM_CAR_STATES){
		xSemaphoreTake(carStateMutex, portMAX_DELAY);
		current_state = new_state;
		xSemaphoreGive(carStateMutex);
	}
}

enum CAR_STATE get_car_state(){
	enum CAR_STATE state;
	xSemaphoreTake(carStateMutex, portMAX_DELAY);
	state = current_state;
	xSemaphoreGive(carStateMutex);
	return state;
}
