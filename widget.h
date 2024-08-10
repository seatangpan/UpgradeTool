#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "cserial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define TEMPPATH  QDir::toNativeSeparators(QDir::temp().absolutePath()) + "/hpm_upgarde/"
#define SETTINGSPATH QStandardPaths::writableLocation( QStandardPaths::GenericDataLocation )+ "/hpm_upgarde/"
#define SETTINGSFILE ((QFile::exists("hpm_settings.ini")) ? ("hpm_settings.ini") : ((SETTINGSPATH)  + "hpm_settings.ini"))

#define DM_UART_RECV_BLOCK_CNT (32)

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void appendMsgout(QString msg);

protected slots:

    void onSerialChanged();

    void onConnectResp(quint8 result);

    void onDisConnectResp(quint8 result);

    void onUpdateResp(quint16 result , QByteArray buffer);

    void onMsgRead(QByteArray buffer);

    void onSerialDisconnectedResp();

    void onUpdateSend();

signals:

    void signalOutputLog(QString);

    void sigalUpdateDataSend();

private slots:
    void on_btn_connect_clicked();

    void on_btn_choice_clicked();

    void on_btn_upgrade_clicked();

private:

    quint32 crc32(QByteArray buffer , quint32 init);

    void enableUI(quint8 enable);

private:
    Ui::Widget *ui;

    CSerial *serial;

    bool isConnected{false};

    QTimer *timerConnect{nullptr};

    QTimer* timerUpgrade{ nullptr };

    std::mutex mutex;

    std::mutex mutexSend;

    QList<QByteArray> listUpdateFrame;

    std::atomic<quint32> updateFrameCount{ 0 };

    std::atomic<quint32> retryCnt{ 0 };
};
#endif // WIDGET_H
