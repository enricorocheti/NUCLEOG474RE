/***************************************************************************
 * Include
 **************************************************************************/
#include "ActiveObject.h"

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
 * @brief Task function used by all active objects
 */
static void ActiveObject_EventLoop( void *param )
{
    ActiveObject *activeObject = (ActiveObject *) param;
    Event event = { SIG_INIT };

    /* Initialize the active object by sending the INIT_SIG event */
    ( *activeObject->Dispatch )( activeObject, event );

    for (;;)
    {
        /* Block and wait for any events */
        xQueueReceive( activeObject->Queue, &event, portMAX_DELAY );

        /* Dispatch the event to the active object */
        ( *activeObject->Dispatch )( activeObject, event );
    }
}

/***************************************************************************
 * Public Functions
 **************************************************************************/
/*!
 * @brief Configure and initialize the active object
 *
 * @param[in] activeObject  Pointer to active object to be initialized
 * @param[in] dispatch      Function pointer to handle the active object events
 * @param[in] stackSize     Stack size of the internal task
 * @param[in] priority      Priority of the internal task
 * @param[in] queueSize     Event queue size
 * @param[in] maxEventSize  Maximum size (in bytes) of an event
 *
 * @return Status of the operation
 */
Status ActiveObject_Init( ActiveObject *activeObject,
						  DispatchHandler dispatch,
						  uint16_t stackSize,
						  uint8_t priority,
						  uint16_t queueSize,
						  uint16_t maxEventSize )
{
    ASSERT( activeObject != NULL, STATUS_ERR_INVALID_ARGUMENT );
    ASSERT( dispatch != 0, STATUS_ERR_INVALID_ARGUMENT );
    ASSERT( stackSize > 0, STATUS_ERR_INVALID_ARGUMENT );
    ASSERT( queueSize > 0, STATUS_ERR_INVALID_ARGUMENT );

    /* Function that will handle events */
    activeObject->Dispatch = dispatch;

    /* Internal queue */
    activeObject->Queue = xQueueCreate( queueSize, maxEventSize );
    if( activeObject->Queue == NULL )
	{
    	return STATUS_ERR_RESOURCE_NOT_INITIALIZED;
	}

    /* Internal task */
    if( xTaskCreate( &ActiveObject_EventLoop, "AO", stackSize, activeObject, priority, &activeObject->Task ) != pdPASS )
    {
        return STATUS_ERR_RESOURCE_NOT_INITIALIZED;
    }

    return STATUS_OK;
}

/*!
 * @brief Post event to active object internal queue, the event will be copied into the queue
 *
 * @param[in] activeObject  Pointer to active object
 * @param[in] event         Pointer to event
 *
 * @return Status of the operation
 */
Status ActiveObject_Post( ActiveObject *activeObject, Event *event )
{
    ASSERT( activeObject != NULL, STATUS_ERR_INVALID_ARGUMENT );
    ASSERT( event != NULL, STATUS_ERR_INVALID_ARGUMENT );

    if( xQueueSend( activeObject->Queue, (void *) event, 0U ) == pdPASS )
    {
        return STATUS_OK;
    }

    return STATUS_ERR_RESOURCE_NOT_AVAILABLE;
}

/*!
 * @brief Post event to active object internal queue from an ISR, this is needed due to an FreeRTOS limitation
 *
 * @param[in] activeObject  Pointer to active object
 * @param[in] event         Pointer to event
 *
 * @return Status of the operation
 */
Status ActiveObject_PostFromISR( ActiveObject *activeObject, Event *event )
{
    ASSERT( activeObject != NULL, STATUS_ERR_INVALID_ARGUMENT );
    ASSERT( event != NULL, STATUS_ERR_INVALID_ARGUMENT );

    if( xQueueSendFromISR( activeObject->Queue, (void *) event, NULL ) == pdPASS )
    {
        return STATUS_OK;
    }

    return STATUS_ERR_RESOURCE_NOT_AVAILABLE;
}
