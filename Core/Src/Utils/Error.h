#ifndef ERROR_H_
#define ERROR_H_

/***************************************************************************
 * Include
 **************************************************************************/
#include <stdint.h>

/***************************************************************************
 * Public Macros
 **************************************************************************/
#define STATUS_OK                         	( 0x00000000 )
#define STATUS_ERR_GENERIC                	( 0x00000001 )
#define STATUS_ERR_INVALID_ARGUMENT       	( 0x00000002 )
#define STATUS_ERR_NOT_ASSIGNED      		( 0x00000003 )
#define STATUS_ERR_NOT_SUPPORTED            ( 0x00000004 )
#define STATUS_ERR_RESOURCE_NOT_INITIALIZED	( 0x00000005 )
#define STATUS_ERR_RESOURCE_NOT_AVAILABLE	( 0x00000006 )

#define STATUS_ERR_BASE_UTILS            	( 0x00000010 )
#define STATUS_ERR_BASE_DRIVER           	( 0x00000100 )
#define STATUS_ERR_BASE_COMPONENT          	( 0x00001000 )
#define STATUS_ERR_BASE_TASK               	( 0x00010000 )

/*!
 * @brief  Assert function to check if a condition is valid
 */
#define ASSERT(a,s) do{\
        if(!(a)){\
            return s;\
        }\
}while(0)

/***************************************************************************
 * Public Types
 **************************************************************************/
typedef uint32_t Status;

/***************************************************************************
 * Public Variables
 **************************************************************************/

/***************************************************************************
 * Public Functions
 **************************************************************************/

#endif /* ERROR_H_ */
