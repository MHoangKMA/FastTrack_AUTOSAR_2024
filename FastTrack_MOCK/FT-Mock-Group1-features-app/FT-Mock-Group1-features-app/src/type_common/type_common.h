#ifndef __TYPE_COMMON_H__
#define __TYPE_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "S32K144.h"

/* ========================================= TYPEDEF ============================================= */
/* <NODE APP> */


typedef enum Node_Type_t
{
    NODE_TYPE_SPEEED      = 0u,
    NODE_TYPE_TEMPERATURE = 1u,
    //NODE_TYPE_FORWARDER   = 2u      if need
} Node_Type_t;

typedef struct Node_Config_Data_Struct_t
{
    //uint8_t nodeID;        /* Node ID field */
	Node_Type_t nodeType;        /* Node type field */
    uint8_t threshold;       /* ADC Threshold */
} Node_Config_Data_Struct_type;



#ifdef __cplusplus
}
#endif

#endif /* __TYPE_COMMON_H__ */
