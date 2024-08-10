#ifndef UPGRADEFRAME_H
#define UPGRADEFRAME_H

#include <QObject>
#include <QString>
#include <mutex>

#define DM_UART_BLOCK_SIZE (32)

enum class paramlist : quint16
{
    HP10_INVALID = 0x00,
    HP10_CONNECT = 0x01 ,    //连接请求
    HP10_DISCONNECT ,        //断开请求
    HP10_READ_VERSION ,      //获取终端参数
    HP10_UPDATE ,            //终端升级
};

typedef enum
{
    DM_TRANSFORM_STATUS_OK ,
    DM_TRANSFORM_STATUS_CHECKSUM_ERR ,
    DM_TRANSFORM_STATUS_BLOCK_ERR ,
    DM_TRANSFORM_STATUS_CRC_ERR ,
    DM_TRANSFORM_STATUS_FILE_SIZE_ERR ,
    DM_TRANSFORM_STATUS_FILE_OPEN_ERR ,
    DM_TRANSFORM_STATUS_FILE_WRITE_ERR
}DM_TRANSFORM_STATUS;

typedef enum
{
    DM_TRANSFORM_START = 0x01 ,
    DM_TRANSFORM_DATA ,
    DM_TRANSFORM_FINISH
}DM_TRANSFORM_CMDID;

#define DM_BAUDRATE_115200 (115200)
#define DM_BAUDRATE_921600 (921600)

class upgradeframe : public QObject
{
    Q_OBJECT
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
    upgradeframe(QObject* parent);
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
    QList< QByteArray> framelist;
    QTimer* timer{ nullptr };

    std::mutex mutex;
};

#endif // UPGRADEFRAME_H
