
#ifndef __TBA_SEDA_EVENT_H__
#define __TBA_SEDA_EVENT_H__

#include "tba_types.h"
#include "tba_byte_buffer.h"

typedef int(* SL_Seda_RpcMessageProc)(void *socket_handler, void *seda_stagehandler, void *rpc_message);

enum
{
	MSG_TYPE_RPC  				= 0,
	MSG_TYPE_DATA_SYNC,
};

struct SL_Seda_Event
{
	int msg_type = MSG_TYPE_RPC;
};

struct SL_Seda_RpcMessageEvent : public SL_Seda_Event// : public SL_Seda_FixedSizeEventBase<SL_Seda_RpcMessageEvent, SL_Seda_EventType::RPC_MESSAGE>
{
    void *socket_handler;
	void *rpc_message;
    SL_Seda_RpcMessageProc rpc_proc;
};

#endif

