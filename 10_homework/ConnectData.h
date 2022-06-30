#ifndef __CONNECT_DATA_H__
#define __CONNECT_DATA_H__

// 游戏的协议设计
// 落子消息
#define MSG_PUTSTEP     0x01
// 和棋请求消息
#define MSG_DRAW        0x02
// 同意和棋消息
#define MSG_AGREE_DRAW	0x03
// 拒绝和棋消息
#define MSG_REFUSE_DRAW	0x04
// 其他消息
#define MSG_EXTERN		0x05

//the following definition is added newly
//悔棋消息
#define MSG_RETRACT     0x06
//同意悔棋消息
#define MSG_AGREE_RETRACT   0x10
//拒绝悔棋消息
#define MSG_REFUSE_RETRACT  0x11

//认输消息
#define MSG_SURRENDER   0x07

//更换背景消息
//#define MSG_CHANGE_BK   0x08
//发送消息消息
//#define MSG_SEND        0x09



typedef struct _tagMsgStruct {
    USHORT len;
	// 消息ID
    BYTE msgType;
    // 落子信息
    int x;
    int y;
    int color;
    // 其他消息内容
    BYTE byMsg[128];
} MSGSTRUCT;

#endif