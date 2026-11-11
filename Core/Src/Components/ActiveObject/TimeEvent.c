/***************************************************************************
 * Include
 **************************************************************************/
#include "TimeEvent.h"

/***************************************************************************
 * Private Macros
 **************************************************************************/

/***************************************************************************
 * Private Types
 **************************************************************************/

/***************************************************************************
 * Private Variables
 **************************************************************************/

/***************************************************************************
 * Private Functions
 **************************************************************************/
/*!
 * @brief Callback for all time events
 */
static void TimeEvent_Callback( TimerHandle_t xTimer )
{
	TimeEvent *timeEvent = pvTimerGetTimerID( xTimer );

    ActiveObject_Post( timeEvent->ActiveObject, &timeEvent->Event );
}

/***************************************************************************
 * Public Functions
 **************************************************************************/
/*!
 * @brief Configure the time event
 *
 * @param[in] timeEvent    	Pointer to the time event structure
 * @param[in] event     	Event (signal) to be sent when the timer counter expires
 * @param[in] activeObject  Pointer to active object of this time event
 *
 * @return Status of the operation
 */
Status TimeEvent_Init( TimeEvent *timeEvent,
					   Event event,
					   ActiveObject *activeObject,
					   TimerType type )
{
	ASSERT( timeEvent != NULL, STATUS_ERR_INVALID_ARGUMENT );
	ASSERT( activeObject != NULL, STATUS_ERR_INVALID_ARGUMENT );

	timeEvent->ActiveObject = activeObject;
	timeEvent->Event        = event;
	timeEvent->Type         = type;
	timeEvent->OsHandle     = xTimerCreate( "TE", 1U, timeEvent->Type, timeEvent, TimeEvent_Callback );
	ASSERT( timeEvent->OsHandle != NULL, STATUS_ERR_RESOURCE_NOT_INITIALIZED );

	return STATUS_OK;
}

/*!
 * @brief Start time event using the period sent by function parameter
 *
 * @param[in] timeEvent    	Pointer to the time event structure
 * @param[in] milliseconds	Time event period in milliseconds
 *
 * @return Status of the operation
 */
Status TimeEvent_Start( TimeEvent *timeEvent, uint32_t milliseconds )
{
	ASSERT( timeEvent != NULL, STATUS_ERR_INVALID_ARGUMENT );

    TickType_t ticks = ( milliseconds / portTICK_PERIOD_MS );
    if( ticks == 0U )
    {
        ticks = 1U;
    }

    if( xTimerChangePeriod( timeEvent->OsHandle, ticks, 0 ) == pdPASS )
    {
        return STATUS_OK;
    }

    return STATUS_ERR_RESOURCE_NOT_AVAILABLE;
}

/*!
 * @brief Stop time event
 *
 * @param[in] timeEvent	Pointer to the time event structure
 *
 * @return Status of the operation
 */
Status TimeEvent_Stop( TimeEvent *timeEvent )
{
	ASSERT( timeEvent != NULL, STATUS_ERR_INVALID_ARGUMENT );

	if( xTimerStop( timeEvent->OsHandle, 0 ) == pdPASS )
	{
		return STATUS_OK;
	}

	return STATUS_ERR_RESOURCE_NOT_AVAILABLE;
}
