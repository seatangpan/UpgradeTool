#ifndef UPGRADEFRAME_H
#define UPGRADEFRAME_H

#include <QObject>
#include <qstack.h>

enum class paramlist : quint16
{
    HP10_INVALID = 0x00,
    HP10_CONNECT = 0x01 ,    //连接请求
    HP10_DISCONNECT ,        //断开请求
    HP10_READ_VERSION ,      //获取终端参数
    HP10_WRITE_VERSION ,     //设置终端参数
    HP10_READ_INFO ,         //获取车辆参数
    HP10_WRIET_INFO ,        //设置车辆参数
    HP10_UPDATE ,            //终端升级
    HP10_SELFCHECK ,         //测试结果查询
    HP10_VOICE_TEST ,        //声音测试
    HP10_REBOOT ,            //终端重启
    HP10_TESTMODE ,          //测试命令
    HP10_CALLTEST ,          //通话命令
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
