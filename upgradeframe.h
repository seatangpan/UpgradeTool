#ifndef UPGRADEFRAME_H
#define UPGRADEFRAME_H

#include <QObject>
#include <qstack.h>

enum class paramlist : quint16
{
    HP10_INVALID = 0x00,
    HP10_CONNECT = 0x01 ,    //��������
    HP10_DISCONNECT ,        //�Ͽ�����
    HP10_READ_VERSION ,      //��ȡ�ն˲���
    HP10_WRITE_VERSION ,     //�����ն˲���
    HP10_READ_INFO ,         //��ȡ��������
    HP10_WRIET_INFO ,        //���ó�������
    HP10_UPDATE ,            //�ն�����
    HP10_SELFCHECK ,         //���Խ����ѯ
    HP10_VOICE_TEST ,        //��������
    HP10_REBOOT ,            //�ն�����
    HP10_TESTMODE ,          //��������
    HP10_CALLTEST ,          //ͨ������
};

class upgradeframe
{
    enum class DM_UART_RECV_PHASE : quint8
    {
        DM_UART_RECV_PHASE_PERFIX1 = 0x00 ,
        DM_UART_RECV_PHASE_PERFIX2 ,
        DM_UART_RECV_PHASE_LEN1 ,
        DM_UART_RECV_PHASE_LEN2 ,
        DM_UART_RECV_PHASE_VER ,
        DM_UART_RECV_PHASE_SEQ1 ,
        DM_UART_RECV_PHASE_SEQ2 ,
        DM_UART_RECV_PHASE_CMD1 ,
        DM_UART_RECV_PHASE_CMD2 ,
        DM_UART_RECV_PHASE_DATA ,
        DM_UART_RECV_PHASE_CHECKSUM
    };

public:
    upgradeframe();
    virtual ~upgradeframe();

    quint16 dmHandleMsg(const QByteArray &msg);

    static quint8 xorChecksum(quint8* data , quint16 len);

    void clear();

    QByteArray getUpgradeFrame();

private:
    QByteArray data;
    DM_UART_RECV_PHASE phase;
    quint16 seq;
    quint16 len;
    quint32 cmdid;
    QStack< QByteArray> framelist;
};

#endif // UPGRADEFRAME_H
