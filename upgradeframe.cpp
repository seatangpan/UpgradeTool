#include "upgradeframe.h"

#define DM_PROFILE_MIN_LEN 10
#define DM_PROFILE_VER 20
#define DM_PROFILE_PREFIX  0x23

upgradeframe::upgradeframe()
{
    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_PERFIX1;
    seq = 0;
    len = 0;
    cmdid = 0;
}

upgradeframe::~upgradeframe()
{
    data.clear();
    framelist.clear();
}

quint16 upgradeframe::dmHandleMsg(const QByteArray& msg)
{
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
                if (len < DM_PROFILE_MIN_LEN)
                {
                    clear();
                    break;
                }
                data.append(msg[i]);
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_VER;
            }
            break;

            case DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_VER:
            {
                if (DM_PROFILE_VER != msg[i])
                {
                    clear();
                    break;
                }

                data.append(msg[i]);
                phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_SEQ1;
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
                    paramlist::HP10_CALLTEST < paramlist(cmdid & 0x7fff))
                {
                    clear();
                    break;
                }
                data.append(msg[i]);
                if (data.size() == DM_PROFILE_MIN_LEN)
                {
                    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_CHECKSUM;
                }
                else
                {
                    phase = DM_UART_RECV_PHASE::DM_UART_RECV_PHASE_DATA;
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
                quint8 checksum = xorChecksum((quint8 *)data.data() , data.size() - 1);
                if (checksum == (quint8)msg[i])
                {
                    data.append(msg[i]);
                    framelist.push(data);
                    clear();
                }
                else
                {
                    clear();
                }
            }
            break;

        default:
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
    if (framelist.size())
    {
        result = framelist.top();
        framelist.pop();
    }
    return result;
}
