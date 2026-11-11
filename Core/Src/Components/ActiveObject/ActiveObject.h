#ifndef ACTIVE_OBJECT_H_
#define ACTIVE_OBJECT_H_

/***************************************************************************
 * Include
 **************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Error.h"

/***************************************************************************
 * Public Macros
 **************************************************************************/

/***************************************************************************
 * Public Types
 **************************************************************************/
typedef enum
{
    SIG_INIT,   /* Dispatched to AO before entering event loop */
    SIG_USER    /* First signal available to the users */
} ReservedSignals;

typedef struct
{
    uint16_t Signal;
    union {
    	char c;
    	uint8_t u8;
        uint32_t u32;
        uint64_t u64;
        float f32;
    } Parameter;
    /* Event parameters should be added in subclasses of this struct */
} Event;

typedef struct ActiveObject ActiveObject;	/* Forward declaration */

typedef void ( *DispatchHandler )( ActiveObject *activeObject, Event event );

struct ActiveObject
{
    TaskHandle_t Task;
    QueueHandle_t Queue;
    DispatchHandler Dispatch;
};

/***************************************************************************
 * Public Variables
 **************************************************************************/

/***************************************************************************
 * Public Functions
 **************************************************************************/
Status ActiveObject_Init( ActiveObject *activeObject,
						  DispatchHandler dispatch,
						  uint16_t stackSize,
						  uint8_t priority,
						  uint16_t queueSize,
						  uint16_t maxEventSize );

Status ActiveObject_Post( ActiveObject *activeObject, Event *event );

Status ActiveObject_PostFromISR( ActiveObject *activeObject, Event *event );

#endif /* ACTIVE_OBJECT_H_ */
