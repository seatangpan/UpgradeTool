#ifndef CSERIAL_H
#define CSERIAL_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include "upgradeframe.h"

class CSerial : public QObject
{
    Q_OBJECT
public:

    enum class connectStatus : quint8
    {
        CONNECT_SUCCESS,
        CONNECT_BUSY,
        CONNECT_ALREADY,
        CONNECT_ERROR
    };

    explicit CSerial(QObject *parent = nullptr);
    virtual ~CSerial();

    //获取串口名称
    QList<QSerialPortInfo> getCommNames();

    //获取串口波特率列表
    QStringList getCommBps();

    //获取波特率的值
    qint32 getCommBps(const QString &sValue);

    bool isOpen() const {return opened;}

    quint16 getSeqence(){return seq;}

    //打开串口
    bool open(const QString &sPort, const qint32 &nBaudrate);

    bool open(const qint32& nBaudrate);

    //关闭串口
    void close();

    //连接请求
    bool connectReq();

    //断开连接
    bool disconnectReq();

    bool update(const QByteArray& buffer);

protected:

    QString binary2String(const QByteArray &buffer);

public slots:

    //串口断开
    void handleSerialError(QSerialPort::SerialPortError error);

    void readData();

    void readyWrite();

    bool writeData(paramlist para, const QByteArray &buffer);

signals:

    //串口断开
    void signalSerialDisconnected(void);

    //波特率错误
    void signalSerialCommError(void);

    //返回应答的报文
    void signalMsgRead(QByteArray buffer);

private:

    QStringList listBuadrate{"1200","2400","4800","9600","19200", "38400", "57600","115200" };

    QList<qint32> listBuadrateValue
    {
        QSerialPort::Baud1200, QSerialPort::Baud2400, QSerialPort::Baud4800,
        QSerialPort::Baud9600, QSerialPort::Baud115200, QSerialPort::invalid
    };

    bool opened{false};

    std::atomic_bool readyWriteen{ true };

    QSerialPort serialPort;

    quint16 seq { 0x0001 };

    QMutex mutex;

    upgradeframe frame;

    QString portName;

    qint32 baudrate;
};

#endif // CSERIAL_H
