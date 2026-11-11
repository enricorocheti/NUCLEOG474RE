#ifndef TIME_EVENT_H_
#define TIME_EVENT_H_

/***************************************************************************
 * Include
 **************************************************************************/
#include "FreeRTOS.h"
#include "timers.h"

#include "Error.h"
#include "ActiveObject.h"

/***************************************************************************
 * Public Macros
 **************************************************************************/

/***************************************************************************
 * Public Types
 **************************************************************************/
typedef enum
{
	OneShotTimer  = pdFALSE,
	PeriodicTimer = pdTRUE
} TimerType;

typedef struct
{
    Event Event;
    ActiveObject *ActiveObject;
    TimerType Type;
    TimerHandle_t OsHandle;
} TimeEvent;

/***************************************************************************
 * Public Variables
 **************************************************************************/

/***************************************************************************
 * Public Functions
 **************************************************************************/
Status TimeEvent_Init( TimeEvent *timeEvent,
					   Event event,
					   ActiveObject *activeObject,
					   TimerType type );

Status TimeEvent_Start( TimeEvent *timeEvent, uint32_t milliseconds );

Status TimeEvent_Stop( TimeEvent *timeEvent );

#endif /* TIME_EVENT_H_ */
