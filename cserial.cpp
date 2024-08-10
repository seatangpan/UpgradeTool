#include "cserial.h"
#include <thread>

CSerial::CSerial(QObject *parent) : QObject(parent)
{
    frame = new upgradeframe(this);
}

CSerial::~CSerial()
{
    delete frame;
}

QList<QSerialPortInfo> CSerial::getCommNames()
{
    QList<QSerialPortInfo> list_avail_sp;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);

        //if (serial.open(QIODevice::ReadWrite))
        {
            list_avail_sp.append(info);
            serial.close();
        }
    }
    return list_avail_sp;
}

QStringList CSerial::getCommBps()
{
    return listBuadrate;
}

qint32 CSerial::getCommBps(const QString &sValue)
{
    foreach (const qint32 &var, listBuadrateValue)
    {
        if(var == sValue.toInt())
        {
            return var;
        }
    }
    return QSerialPort::invalid;
}

bool CSerial::open(const QString &sPort, const qint32 &nBaudrate)
{
    if(opened)
    {
        close();
    }

    serialName = sPort;
    baudrate = nBaudrate;

    serialPort.setPortName(serialName);
    serialPort.setBaudRate(baudrate);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    opened = serialPort.open(QIODevice::ReadWrite);
    if (!opened)
    {
        qDebug() << "Failed to open serial port:" << serialPort.errorString();
    }
    serialPort.setDataTerminalReady(true);
    QObject::connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
    QObject::connect(&serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),this, &CSerial::handleSerialError);
    qDebug() << QString("serial port:%1,Buadrate:%2, result:%3").arg(sPort).arg(nBaudrate).arg(opened?"OK":"NG");
    return opened;
}

bool CSerial::open(const qint32& nBaudrate)
{
    return open(serialName, nBaudrate);
}

bool CSerial::clear()
{
    serialPort.clear();
    return true;
}

void CSerial::setBaudRate(const qint32& nBaudrate)
{
    serialPort.setBaudRate(nBaudrate);
}

void CSerial::close()
{
    QObject::disconnect(&serialPort, 0, this, 0);//解除信号槽
    if(opened)
    {
        serialPort.clear();
        serialPort.close();
    }
    opened = false;
}

bool CSerial::connectReq()
{
    QByteArray empty;
    return writeData(paramlist::HP10_CONNECT, empty);
}

bool CSerial::disconnectReq()
{
    QByteArray empty;
    return writeData(paramlist::HP10_DISCONNECT , empty);
}

bool CSerial::update(const QByteArray& buffer)
{
    return writeData(paramlist::HP10_UPDATE , buffer);
}

QString CSerial::binary2String(const QByteArray &buffer)
{
    QString sBuffer;
    for (int i = 0; i < buffer.size(); i++)
    {
        QString s;
        sBuffer += s.asprintf("%02X ", buffer.at(i) & 0xFF);
    }
    return sBuffer;
}

void CSerial::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError || error == QSerialPort::PermissionError)
    {
        qDebug()<<"comm disconnected...";
        emit signalSerialDisconnected();
    }
}

void CSerial::readData()
{
    QByteArray frameData = serialPort.readAll();
    frame->dmHandleMsg(frameData);

    QByteArray result = frame->getUpgradeFrame();

    while (result.size())
    {
        //qDebug() << "recvData:" << binary2String(result);
        signalMsgRead(result);
        result = frame->getUpgradeFrame();
    }
}

bool CSerial::writeData(paramlist para, const QByteArray &buffer)
{
    const quint8 protocolVersion {0x14};
    const quint8 head[2] = { 0x23, 0x23};
    quint16 len = buffer.size();
    quint16 sendSize = len + 10;

    //23 23 00 0A 14 00 01 00 01 1E
    quint8 *transData = new quint8[sendSize];
    memcpy(transData, head, sizeof(head));
    transData[2] = (sendSize >> 8) & 0xFF;
    transData[3] = sendSize & 0xFF;
    transData[4] = protocolVersion;
    transData[5] = (seq >> 8) & 0xFF;
    transData[6] = seq & 0xFF;
    transData[7] = (quint16(para) >> 8) & 0xFF;
    transData[8] = quint16(para) & 0xFF;
    if(len > 0)
    {
        memcpy(transData + 9, buffer.data(), len );
    }

    transData[sendSize - 1] = upgradeframe::xorChecksum( transData + 2, sendSize - 3);
    seq++;

    QByteArray sendData((char *)transData, sendSize);
    //qDebug() << "SendData:" << binary2String(sendData);

    qint16 ret = 0;
    if(opened)
    {
        mutex.lock();
        ret = serialPort.write(sendData);
        if (ret <= 0)
        {
            emit signalSerialDisconnected();
        }
        mutex.unlock();
    }

    delete[] transData;
    return (ret > 0);
}
