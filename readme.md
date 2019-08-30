

## 基于thrift二进制协议的C++序列化/反序列化工具

### 准备

- yum install libevent libevent-devel
- 源码安装boost1.66

### 一键安装

- cd thrift-serialize/3rd/thrift/include
- cp thrift /usr/local/include -r
- cd ../lib
- cp * /usr/local/lib
- ldconfig
- cd ../../..
- make 
- ./sbin/serialize



### 采用框架及功能

- 实现了C++语言的RPC或者类似java的ORM
- 采用thrift二进制位协议


### 特性

- 利用thrift 的idl工具，可方便的生成多种跨平台协议。


### IDL工具使用

- 生成的文件就是协议。

#### C++命令
- ./thrift --gen cpp common.thrift 

#### JAVA命令
- ./thrift --gen java login_server.thrift


### 协议格式

#### head

- length（协议长度）和type（协议类型）是关键字段。


```
struct SL_Rpc_MessageHeadEx     //40个字节
{
    uint    length;             //长度
    ushort  flag;               //消息标记
    ushort  type;               //消息类型
    uint    sequence;           //消息序列号
    uint    source;             //源(发送方)
    uint    destination;        //目标(接收方)
    int     checksum;           //检验和(消息体的检验和)
    int     attach_id1;         //附加id1
    int     attach_id2;         //附加id2
    int     attach_id3;         //附加id3
    int     attach_id4;         //附加id4
};
```

#### body

- 在MessageMapping.h文件中构建RPC协议实体，SL_Rpc_Message是序列化和反序列化的基本单元。

```
typedef SL_Rpc_Message<SL_Rpc_MessageHeadEx, login_server::ClientPasswordLoginReq, login_server::MessageType::MT_CLIENT_PASSWORD_LOGIN_REQ> MsgClientPasswordLoginReq;
```

##### SL_Rpc_Message包括三个部分：

- SL_Rpc_MessageHeadEx 协议头
- login_server::ClientPasswordLoginReq 协议体，由thrift IDL 定义并生成
- login_server::MessageType::MT_CLIENT_PASSWORD_LOGIN_REQ 协议类型，由thrift IDL 定义并生成


### 演示样例

#### 序列化(encode)


```
	MsgClientLoginRes login_res;
	login_res.head_.source = 100;//协议头配置
	
	login_res.body_.error_code = 0;//协议体配置
	login_res.body_.error_text = "OK";//协议体配置
	SL_ByteBuffer buff(512);//transport
	login_res.serialize_ex<SL_ByteBuffer>(&buff);//序列化

	cout<<"buff len: "<<buff.data_size()<<endl;
	
	send(fd, buff.buffer(), buff.data_size());
```


#### 反序列化(decode)


```
void socket_event_cb(bufferevent *bev, void *arg) 
{
	  char msg_len_buff[4] = {0};  
	  size_t len = read(fd, msg_len_buff, 4);
	  
	  //前四个字节是协议长度
	  int offset = 0;
 	  int msg_len = ((msg_len_buff[offset++] & 0xff) << 24) |
			      		((msg_len_buff[offset++] & 0xff) << 16) |
			      		((msg_len_buff[offset++] & 0xff) <<  8) |
			      		(msg_len_buff[offset++] & 0xff);
						
	  printf("rpc msg len: %d\n", msg_len);
	  char *msg = (char *)malloc(sizeof(char)*msg_len);
	  memcpy(msg, head_len, 4);
	  
	  //获取协议的其余字节
	  len = read(bev, msg + 4, msg_len - 4);
	  printf("purl rpc msg len: %d\n", len); 
	  
	  //解析协议头信息
	  SL_Rpc_MessageParser::parse_head_ex<SL_Rpc_MessageHeadEx>((char*)msg, sizeof(SL_Rpc_MessageHeadEx));//header parser
	  SL_Rpc_MessageHeadEx *msg_head = (SL_Rpc_MessageHeadEx*)msg;
	  printf("msg_head->type: %d\n", msg_head->type);
	  
	  //定义RpcMessageEvent用于反序列化
	  int ret = -1;
	  SL_Seda_RpcMessageEvent rpc_event;
 	  switch (msg_head->type)
	  {
		case login_server::MessageType::MT_CLIENT_PASSWORD_LOGIN_REQ:
		    //CMainStageHandler::ClientLoginNotify静态绑定执行函数，在添加业务队列情形，用于解耦libevent和业务线程。
			ret = SL_Rpc_MessageParser::message_to_event<MsgClientPasswordLoginReq>((char*)msg, msg_len, NULL, bev, CMainStageHandler::ClientLoginNotify, &rpc_event, NULL);
			//rpc_event.rpc_proc只是用于演示，在生产环境此处要放到业务消息队列里面来执行，以免阻塞libevent 事件机制。
			rpc_event.rpc_proc(rpc_event.socket_handler_id, rpc_event.socket_handler, NULL, rpc_event.rpc_message, NULL);
			break;
		default:
			break;
	  }
	  
	  ...
}
```

```
int CMainStageHandler::ClientLoginNotify(int socket_handler_id, bufferevent *socket_handler, void *seda_stagehandler, void *rpc_message, SL_Socket_INET_Addr *remote_addr)
{
	CMainStageHandler *seda_handler = (CMainStageHandler*)seda_stagehandler;
	MsgClientPasswordLoginReq *notify = (MsgClientPasswordLoginReq*)rpc_message;

	cout<<"check_sum: "<<notify->body_.check_sum<<endl;
	
	//todo business
	...
}
```

### 参考代码

- .\src\event\event_common_api.cpp socket_event_cb
- .\src\biz\MainStageHandler.cpp CMainStageHandler::ClientLoginNotify


### 相关资源

- [java 版序列化工具](https://github.com/wanghuan578/libtba-java)(libtba-java)。



### 作者和贡献者信息


- spirit(57810140@qq.com)

