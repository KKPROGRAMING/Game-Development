#ifndef __CONNECT_DATA_H__
#define __CONNECT_DATA_H__

// ��Ϸ��Э�����
// ������Ϣ
#define MSG_PUTSTEP     0x01
// ����������Ϣ
#define MSG_DRAW        0x02
// ͬ�������Ϣ
#define MSG_AGREE_DRAW	0x03
// �ܾ�������Ϣ
#define MSG_REFUSE_DRAW	0x04
// ������Ϣ
#define MSG_EXTERN		0x05

//the following definition is added newly
//������Ϣ
#define MSG_RETRACT     0x06
//ͬ�������Ϣ
#define MSG_AGREE_RETRACT   0x10
//�ܾ�������Ϣ
#define MSG_REFUSE_RETRACT  0x11

//������Ϣ
#define MSG_SURRENDER   0x07

//����������Ϣ
//#define MSG_CHANGE_BK   0x08
//������Ϣ��Ϣ
//#define MSG_SEND        0x09



typedef struct _tagMsgStruct {
    USHORT len;
	// ��ϢID
    BYTE msgType;
    // ������Ϣ
    int x;
    int y;
    int color;
    // ������Ϣ����
    BYTE byMsg[128];
} MSGSTRUCT;

#endif