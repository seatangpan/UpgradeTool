#include "widget.h"

#include <QApplication>

Widget *pw = nullptr;
QMutex mutex;

void MessageOutput(QtMsgType type, const QMessageLogContext & context, const QString &msg)
{
    Q_UNUSED(context);

    QDir dir("log");
    if (!dir.exists())
    {
        QDir dir;
        dir.mkdir("log");
    }

    //创建log文件
    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString logName = "log" + currentDate + ".txt";
    QString logFileName = "log/" + logName;

    //加锁
    mutex.lock();

    //信息分类
    QString strMsg("");

    switch (type)
    {
    case QtDebugMsg:
        strMsg = QString("[Communication]");
        break;
    case QtWarningMsg:
        strMsg = QString("[Info]");
        break;
    case QtCriticalMsg:
        strMsg = QString("[Critical]");
        break;
    case QtFatalMsg:
        strMsg = QString("[Error]");
        break;
    default:
        break;
    }

    //设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString strMessage = QString("%1 %2 :%3").arg(strDateTime).arg(strMsg).arg(msg);

    if(pw)
    {
        //输出日志到UI
        pw->appendMsgout(strMessage);
    }

    //写入文件
    QFile file(logFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.close();
        return;
    }
    QTextStream stream(&file);
    stream << strMessage << "\r\n";;
    file.flush();
    file.close();

    //解锁
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(MessageOutput);

    Widget w;
    pw = &w;
    w.show();
    return a.exec();
}
