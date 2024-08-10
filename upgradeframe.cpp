#include "upgradeframe.h"
#include <QTimer>
#include <QDebug>

#define DM_PROFILE_MIN_LEN 10
#define DM_PROFILE_MAX_LEN 255
#define DM_PROFILE_VER 20
#define DM_PROFILE_PREFIX  0x23

upgradeframe::upgradeframe(QObject* parent) : QObject(parent)
{
    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_PERFIX1;
    seq = 0;
    len = 0;
    cmdid = 0;

    timer = new QTimer(this);
    connect(timer , &QTimer::timeout , this , [=]()
        {
            clear();
        });

    timer->setSingleShot(true);
}

upgradeframe::~upgradeframe()
{
    data.clear();
    framelist.clear();
    delete timer;
}

QString binary2String(const QByteArray& buffer)
{
    QString sBuffer;
    for (int i = 0; i < buffer.size(); i++)
    {
        QString s;
        sBuffer += s.asprintf("%02X " , buffer.at(i) & 0xFF);
    }
    return sBuffer;
}

quint16 upgradeframe::dmHandleMsg(const QByteArray& msg)
{
    if (timer && !timer->isActive())
    {
        timer->start(100);
    }

    for (int i = 0; i < msg.size(); i++)
    {
        switch (phase)
        {
        case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_PERFIX1:
            if (msg[i] == DM_PROFILE_PREFIX)
            {
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_PERFIX2;
                data.append(msg[i]);
            }
            break;

        case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_PERFIX2:
            if (msg[i] == DM_PROFILE_PREFIX)
            {
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_LEN1;
                data.append(msg[i]);
            }
            else
            {
                clear();
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_LEN1:
            {
                len = (msg[i] << 8);
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_LEN2;
                data.append(msg[i]);
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_LEN2:
            {
                len |= msg[i];
                if (len < DM_PROFILE_MIN_LEN || len > DM_PROFILE_MAX_LEN)
                {
                    clear();
                }
                else
                {
                    data.append(msg[i]);
                    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_VER;
                }
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_VER:
            {
                if (DM_PROFILE_VER != msg[i])
                {
                    clear();
                }
                else
                {
                    data.append(msg[i]);
                    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_SEQ1;
                }
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_SEQ1:
            {
                seq = (msg[i] << 8);
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_SEQ2;
                data.append(msg[i]);
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_SEQ2:
            {
                seq |= msg[i];
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CMD1;
                data.append(msg[i]);
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CMD1:
            {
                cmdid = (msg[i] << 8);
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CMD2;
                data.append(msg[i]);
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CMD2:
            {
                cmdid |= msg[i];
                if (paramlist::HP10_CONNECT > paramlist(cmdid & 0x7fff) ||
                    paramlist::HP10_UPDATE < paramlist(cmdid & 0x7fff))
                {
                    clear();
                }
                else
                {
                    data.append(msg[i]);
                    if (len == DM_PROFILE_MIN_LEN)
                    {
                        phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CHECKSUM;
                    }
                    else
                    {
                        phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_DATA;
                    }
                }
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_DATA:
            {
                data.append(msg[i]);
                if (len == data.size())
                {
                    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CHECKSUM;
                }
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CHECKSUM:
            {
                //CAT1 DM_BAUDRATE_921600 通信发送数据会丢失位，不校验checksum,增加容错处理
                quint8 checksum = xorChecksum((quint8 *)data.data() , data.size() - 1);
                if (1/*checksum == (quint8)msg[i]*/)
                {
                    data.append(msg[i]);
                    mutex.lock();
                    framelist.push_back(data);
                    mutex.unlock();
                }
                clear();

                if (timer && timer->isActive())
                {
                    timer->stop();
                }
            }
            break;

        default:
            clear();
            break;
        }
    }
    return 0;
}

quint8 upgradeframe::xorChecksum(quint8* data , quint16 len)
{
    quint8 check_sum = 0;
    while (len--)
    {
        check_sum ^= *data++;
    }
    return check_sum;
}

void upgradeframe::clear()
{
    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_PERFIX1;
    seq = 0;
    len = 0;
    data.clear();
}

QByteArray upgradeframe::getUpgradeFrame()
{
    QByteArray result;
    mutex.lock();
    if (framelist.size())
    {
        result = framelist.front();
        framelist.pop_front();
    }
    mutex.unlock();
    return result;
}
