
#ifndef __TBA_RPC_MESSAGE_H__
#define __TBA_RPC_MESSAGE_H__

#include "tba_config.h"
#include "tba_byte_buffer.h"
#include "tba_thrift_binary_protocol.h"
#include "tba_event.h"



enum SL_RPC_MESSAGE_ENCODE_TYPE
{
    SL_RPC_MESSAGE_ENCODE_THRIFT = 0,                   //facebook thrift
    SL_RPC_MESSAGE_ENCODE_PROTOBUF = 1,                 //google protobuf
    SL_RPC_MESSAGE_ENCODE_MESSAGEPACK = 2,              //messagepack.org
};

//��Ϣ��ǵ�ȡֵ: һ���򼸸���λ���
enum SL_RPC_MESSAGE_FLAG
{
    SL_RPC_MESSAGE_FLAG_BROADCAST = 1,                  //�㲥(ͬ���ڱ��ع㲥)
    SL_RPC_MESSAGE_FLAG_LOCAL_BROADCAST = 1,            //���ع㲥
    SL_RPC_MESSAGE_FLAG_GLOBAL_BROADCAST = 2,           //ȫ�ֹ㲥
    SL_RPC_MESSAGE_FLAG_COMPRESS = 4,                   //ѹ��
    SL_RPC_MESSAGE_FLAG_CRYPTO = 8,                     //����
};

struct SL_Rpc_MessageHead       //16���ֽ�
{
    uint    length;             //����
    ushort  flag;               //��Ϣ���
    ushort  type;               //��Ϣ����
    uint    sequence;           //��Ϣ���к�
    uint    source;             //Դ(���ͷ�)
};
struct SL_Rpc_MessageHeadEx     //40���ֽ�
{
    uint    length;             //����
    ushort  flag;               //��Ϣ���
    ushort  type;               //��Ϣ����
    uint    sequence;           //��Ϣ���к�
    uint    source;             //Դ(���ͷ�)
    uint    destination;        //Ŀ��(���շ�)
    int     checksum;           //�����(��Ϣ��ļ����)
    int     attach_id1;         //����id1
    int     attach_id2;         //����id2
    int     attach_id3;         //����id3
    int     attach_id4;         //����id4
};
struct SL_Rpc_MessageHeadEx2    //64���ֽ�
{
    uint    length;             //����
    uint    flag;               //��Ϣ���
    uint    type;               //��Ϣ����
    int     checksum;           //�����(��Ϣ��ļ����)
    uint64  sequence;           //��Ϣ���к�
    uint64  source;             //Դ(���ͷ�)
    uint64  destination;        //Ŀ��(���շ�)
    int64   attach_id1;         //����id1
    int64   attach_id2;         //����id2
    int     attach_id3;         //����id3
    int     attach_id4;         //����id4
};
struct SL_Rpc_MessageHeadEx3    //40���ֽ�
{
    ushort  length;             //����
    ushort  flag;               //��Ϣ���
    ushort  type;               //��Ϣ����
    short   reserved;           //����
    uint64  source;             //Դ(���ͷ�)
    uint64  destination;        //Ŀ��(���շ�)
    uint    sequence;           //��Ϣ���к�
    int     attach_id1;         //����id1
    int64   attach_id2;         //����id2
};

class SL_Rpc_MessageParser
{
public:
    SL_Rpc_MessageParser()
    {
    }
    ~SL_Rpc_MessageParser()
    {
    }

    template<typename TMessage> 
    inline static int message_to_event(char *msg, 
        int len, 
        void *socket_handler,
        SL_Seda_RpcMessageProc rpc_proc, 
        SL_Seda_RpcMessageEvent *rpc_event)
    {
        TMessage *message = new TMessage;
        if (NULL == message)
        {
            return -1;
        }
        if (message->parse(msg, len))
        {
            rpc_event->socket_handler = socket_handler;
            rpc_event->rpc_proc = rpc_proc;
            rpc_event->rpc_message = message;
            return 0;
        }
        else
        {
            delete message;
        }
        return -2;
    };

    template<typename TMessageHead>
    inline static bool parse_head(char *buf, int len)
    {
        TMessageHead *head  = (TMessageHead *)buf;
        head->length        = len;
        head->flag          = ntohs(head->flag);
        head->type          = ntohs(head->type);
        head->sequence      = ntohl(head->sequence);
        head->source        = ntohl(head->source);
        return true;
    }

    template<typename TMessageHead>
    inline static bool serialize_head(TMessageHead *head)
    {
        head->length        = htonl(head->length);
        head->flag          = htons(head->flag);
        head->type          = htons(head->type);
        head->sequence      = htonl(head->sequence);
        head->source        = htonl(head->source);
        return true;
    }

    template<typename TMessageHeadEx>
    inline static bool parse_head_ex(char *buf, int len)
    {
        TMessageHeadEx *head = (TMessageHeadEx *)buf;
        head->length        = len;
        head->flag          = ntohs(head->flag);
        head->type          = ntohs(head->type);
        head->sequence      = ntohl(head->sequence);
        head->source        = ntohl(head->source);
        head->destination   = ntohl(head->destination);
        head->checksum      = ntohl(head->checksum);
        head->attach_id1    = ntohl(head->attach_id1);
        head->attach_id2    = ntohl(head->attach_id2);
        head->attach_id3    = ntohl(head->attach_id3);
        head->attach_id4    = ntohl(head->attach_id4);
        return true;
    }

    template<typename TMessageHeadEx>
    inline static bool serialize_head_ex(TMessageHeadEx *head)
    {
        head->length        = htonl(head->length);
        head->flag          = htons(head->flag);
        head->type          = htons(head->type);
        head->sequence      = htonl(head->sequence);
        head->source        = htonl(head->source);
        head->destination   = htonl(head->destination);
        head->checksum      = htonl(head->checksum);
        head->attach_id1    = htonl(head->attach_id1);
        head->attach_id2    = htonl(head->attach_id2);
        head->attach_id3    = htonl(head->attach_id3);
        head->attach_id4    = htonl(head->attach_id4);
        return true;
    }

    template<typename TMessageHeadEx2>
    inline static bool parse_head_ex2(char *buf, int len)
    {
        TMessageHeadEx2 *head = (TMessageHeadEx2 *)buf;
        head->length        = len;
        head->flag          = ntohl(head->flag);
        head->type          = ntohl(head->type);
        head->checksum      = ntohl(head->checksum);
        head->sequence      = util_ntohll(head->sequence);
        head->source        = util_ntohll(head->source);
        head->destination   = util_ntohll(head->destination);
        head->attach_id1    = util_ntohll(head->attach_id1);
        head->attach_id2    = util_ntohll(head->attach_id2);
        head->attach_id3    = ntohl(head->attach_id3);
        head->attach_id4    = ntohl(head->attach_id4);
        return true;
    }

    int64 util_ntohll(int64 net_int64)
    {
#if TBA_NET_BYTE_ORDER == TBA_BIG_ENDIAN
	return net_int64;
#else
#ifdef SOCKETLITE_OS_WINDOWS
    	return ( (((unsigned long long)ntohl(net_int64)) << 32) + ntohl(net_int64 >> 32) );
#else
    	return bswap_64(net_int64);
#endif
#endif
}

    template<typename TMessageHeadEx2>
    inline static bool serialize_head_ex2(TMessageHeadEx2 *head)
    {
        head->length        = htonl(head->length);
        head->flag          = htonl(head->flag);
        head->type          = htonl(head->type);
        head->checksum      = htonl(head->checksum);
        head->sequence      = util_htonll(head->sequence);
        head->source        = util_htonll(head->source);
        head->destination   = util_htonll(head->destination);
        head->attach_id1    = util_htonll(head->attach_id1);
        head->attach_id2    = util_htonll(head->attach_id2);
        head->attach_id3    = htonl(head->attach_id3);
        head->attach_id4    = htonl(head->attach_id4);
        return true;
    }

    template<typename TMessageHeadEx3>
    inline static bool parse_head_ex3(char *buf, int len)
    {
        TMessageHeadEx3 *head = (TMessageHeadEx3 *)buf;
        head->length        = len;
        head->flag          = ntohs(head->flag);
        head->type          = ntohs(head->type);
        head->reserved      = ntohs(head->reserved);
        head->source        = util_ntohll(head->source);
        head->destination   = util_ntohll(head->destination);
        head->sequence      = ntohl(head->sequence);
        head->attach_id1    = ntohl(head->attach_id1);
        head->attach_id2    = util_ntohll(head->attach_id2);
        return true;
    }

    template<typename TMessageHeadEx3>
    inline static bool serialize_head_ex3(TMessageHeadEx3 *head)
    {
        head->length        = htons(head->length);
        head->flag          = htons(head->flag);
        head->type          = htons(head->type);
        head->reserved      = htons(head->reserved);
        head->source        = util_htonll(head->source);
        head->destination   = util_htonll(head->destination);
        head->sequence      = htonl(head->sequence);
        head->attach_id1    = htonl(head->attach_id1);
        head->attach_id2    = util_htonll(head->attach_id2);
        return true;
    }

    template <typename TMessageBody>
    inline static bool parse_to_body(TMessageBody *body, char *buf, int len, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    {
        try
        {
        	tbs_thrift_binary_protocol<tba_byte_buffer> thrift_protocol;
            thrift_protocol.setBuffer(buf, len, NULL);
            body->read(&thrift_protocol);
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    template <typename TMessageBody, typename TByteBuffer>
    inline static bool serialize_to_buffer(TMessageBody *body, TByteBuffer *buf, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    {
        try
        {
        	tbs_thrift_binary_protocol<TByteBuffer> thrift_protocol;
            thrift_protocol.setBuffer(NULL, 0, buf);
            body->write(&thrift_protocol);
        }
        catch (...)
        {
            return false;
        }
        return true;
    }
};

template<typename TMessageHead, typename TMessageBody, int message_type>
class SL_Rpc_Message
{
public:
    inline SL_Rpc_Message()
    {
        memset(&head_, 0, sizeof(TMessageHead));
    }

    inline ~SL_Rpc_Message()
    {
    }

    inline bool parse(char *buf, int len, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    { 
        memcpy(&head_, buf, sizeof(TMessageHead));
        return SL_Rpc_MessageParser::parse_to_body<TMessageBody>(&body_, buf+sizeof(TMessageHead), len-sizeof(TMessageHead), encode_type);
    }

    inline bool serialize_head(char *buf, int len)
    {
        head_.length = len;
        if (0 == head_.type)
        {
            head_.type = MESSAGE_TYPE;
        }
        memcpy(buf, &head_, sizeof(TMessageHead));
        TMessageHead *head = (TMessageHead *)buf;
        return SL_Rpc_MessageParser::serialize_head<TMessageHead>(head);
    }

    template <typename TByteBuffer>
    inline bool serialize(TByteBuffer *buf, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    {
        buf->reserve(sizeof(TMessageHead));
        buf->data_begin(buf->data_begin() + sizeof(TMessageHead));
        if (SL_Rpc_MessageParser::serialize_to_buffer<TMessageBody, TByteBuffer>(&body_, buf, encode_type))
        {
            buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
            return serialize_head(buf->data(), buf->data_size());
        }
        buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
        return false;
    }

    inline bool serialize_head_ex(char *buf, int len)
    {
        head_.length = len;
        if (0 == head_.type)
        {
            head_.type = MESSAGE_TYPE;
        }
        memcpy(buf, &head_, sizeof(TMessageHead));
        TMessageHead *head = (TMessageHead *)buf;
        return SL_Rpc_MessageParser::serialize_head_ex<TMessageHead>(head);
    }

    template <typename TByteBuffer>
    inline bool serialize_ex(TByteBuffer *buf, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    {
        buf->reserve(sizeof(TMessageHead));
        buf->data_begin(buf->data_begin() + sizeof(TMessageHead));
        if (SL_Rpc_MessageParser::serialize_to_buffer<TMessageBody, TByteBuffer>(&body_, buf, encode_type))
        {
            buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
            return serialize_head_ex(buf->data(), buf->data_size());
        }
        buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
        return false;
    }

    inline bool serialize_head_ex2(char *buf, int len)
    {
        head_.length = len;
        if (0 == head_.type)
        {
            head_.type = MESSAGE_TYPE;
        }
        memcpy(buf, &head_, sizeof(TMessageHead));
        TMessageHead *head = (TMessageHead *)buf;
        return SL_Rpc_MessageParser::serialize_head_ex2<TMessageHead>(head);
    }

    template <typename TByteBuffer>
    inline bool serialize_ex2(TByteBuffer *buf, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    {
        buf->reserve(sizeof(TMessageHead));
        buf->data_begin(buf->data_begin() + sizeof(TMessageHead));
        if (SL_Rpc_MessageParser::serialize_to_buffer<TMessageBody, TByteBuffer>(&body_, buf, encode_type))
        {
            buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
            return serialize_head_ex2(buf->data(), buf->data_size());
        }
        buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
        return false;
    }

    inline bool serialize_head_ex3(char *buf, int len)
    {
        head_.length = len;
        if (0 == head_.type)
        {
            head_.type = MESSAGE_TYPE;
        }
        memcpy(buf, &head_, sizeof(TMessageHead));
        TMessageHead *head = (TMessageHead *)buf;
        return SL_Rpc_MessageParser::serialize_head_ex3<TMessageHead>(head);
    }

    template <typename TByteBuffer>
    inline bool serialize_ex3(TByteBuffer *buf, SL_RPC_MESSAGE_ENCODE_TYPE encode_type = SL_RPC_MESSAGE_ENCODE_THRIFT)
    {
        buf->reserve(sizeof(TMessageHead));
        buf->data_begin(buf->data_begin() + sizeof(TMessageHead));
        if (SL_Rpc_MessageParser::serialize_to_buffer<TMessageBody, TByteBuffer>(&body_, buf, encode_type))
        {
            buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
            return serialize_head_ex3(buf->data(), buf->data_size());
        }
        buf->data_begin(buf->data_begin() - sizeof(TMessageHead));
        return false;
    }

public:
    static const int MESSAGE_TYPE = message_type;

    TMessageHead    head_;
    TMessageBody    body_;
};

////rpc����Ϣ����, ��������SL_Rpc_Message
//class SL_Rpc_NULL_MessageBody
//{
//public:
//    inline SL_Rpc_NULL_MessageBody()
//    {
//    }
//    inline ~SL_Rpc_NULL_MessageBody()
//    {
//    }
//
////#ifdef SOCKETLITE_USE_THRIFT
////    inline uint32_t read(::apache::thrift::protocol::TProtocol *iprot)
////    {
////        return 0;
////    }
////
////    inline uint32_t write(::apache::thrift::protocol::TProtocol *oprot) const
////    {
////        return 0;
////    }
////#endif
//};

#endif

