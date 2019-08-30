
#ifndef __TBA_CONFIG_H__
#define __TBA_CONFIG_H__


#define MAJOR_VERSION        	1
#define MINOR_VERSION        	0
#define SUB_VERSION    			1



#define TBA_NET_BIG_ENDIAN

#define TBA_BIG_ENDIAN		1
#define TBA_LITTLE_ENDIAN	2

#if defined(TBA_NET_BIG_ENDIAN)
    #define TBA_NET_BYTE_ORDER		TBA_BIG_ENDIAN
#endif
#if defined(TBA_NET_LITTLE_ENDIAN)
    #define TBA_NET_BYTE_ORDER		TBA_LITTLE_ENDIAN
#endif







//#include "SL_DataType.h"


#endif

