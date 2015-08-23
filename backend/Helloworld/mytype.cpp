#include "mytype.h"
#include <QFile>
#include <QByteArray>
#include <QUrl>
#include <QStandardPaths>
#include <QDir>

QString base64_encode(QString string);
QString base64_decode(QString string);

QString base64_encode(QString string){
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}

QString base64_decode(QString string){
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}




Impress::Impress(QObject *parent) :
    QObject(parent),
    m_PIN("1111"),
    totalPages(1),
    curPage(0),
    m_message("")
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost("127.0.0.1", 1599);
    connect(tcpSocket, SIGNAL(connected()),this, SLOT(emitConnectedState()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMsg()));

    qDebug() << "construct function done! ";
}

Impress::~Impress() {

}

void Impress::emitConnectedState()
{
    qDebug() << "connected to libre impress server";

    emit connectedToImpressServer();

    pair();
}

void Impress::readMsg()
{
    char buff[1024];
    QString line;

    while(tcpSocket->canReadLine())
    {
        qDebug() << "canReadLine, bytes: " << tcpSocket->bytesAvailable();

        tcpSocket->readLine(buff, 1024);

        line = QString::fromUtf8(buff);

        if(line == "\n") continue;

        qDebug() << respStr;

        if (line.indexOf("LO_SERVER_VALIDATING_PIN") >= 0)
        {
            emit shouldSetPINinImpress(m_PIN);
        }
        else if (line.indexOf("LO_SERVER_SERVER_PAIRED") >= 0)
        {
            emit paired();

            start();
        }
        else if (line.indexOf("LO_SERVER_INFO") >= 0)
        {
            status = ST_READ_STARTED_OR_NOT;
            tcpSocket->readLine(buff, 1024);
        }
        else if(line.indexOf("slideshow_started") >= 0)
        {
            tcpSocket->readLine(buff, 1024);
            line = QString::fromUtf8(buff);
            totalPages = line.toInt();

            tcpSocket->readLine(buff, 1024);
            line = QString::fromUtf8(buff);
            curPage = line.toInt();

            emit reportPageNumberGot(totalPages, curPage);

            if (status == ST_READ_STARTED_OR_NOT)
            {
                emit reportStartupStatus(true);
                status = ST_IDLE;
            }

        }
        else if (line.indexOf("slide_preview") >= 0)
        {
            tcpSocket->readLine(buff, 1024);
            line = QString::fromUtf8(buff);
            int pageNum = line.toInt();

            QFile file("preview_of_page_"+QString(pageNum)+".png");
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);

            QString png_base64;
            do
            {
                tcpSocket->readLine(buff, 1024);
                line = QString::fromUtf8(buff);
                png_base64 += line;
            }while(line.indexOf('\n')<0);

            out << base64_decode(png_base64);
            file.close();
        }
        else if (line.indexOf("slide_notes") >= 0)
        {
            tcpSocket->readLine(buff, 1024);
            line = QString::fromUtf8(buff);
            qDebug() << line;
            int pageNum = line.toInt();
            qDebug() << pageNum;

            QFile file(getFilePath("notes_of_page_"+QString(pageNum)+".txt"));
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);

            do
            {
                tcpSocket->readLine(buff, 1024);
                line = QString::fromUtf8(buff);
                out << line;
            }while(line.indexOf('\n')<0);
            file.close();
        }
    }
}

void Impress::sendMsg(QString msg)
{
    QByteArray array (msg.toStdString().c_str());
    tcpSocket->write(array);
}

void Impress::pair()
{
    sendMsg("LO_SERVER_CLIENT_PAIR\nubuntu Phone\n"+m_PIN+"\n\n");
}

void Impress::start()
{
    sendMsg("presentation_start\n\n");
}

void Impress::stop()
{
    sendMsg("presentation_stop\n\n");
}

void Impress::next_page()
{
    sendMsg("transition_next\n\n");
}

void Impress::prev_page()
{
    sendMsg("transition_previous\n\n");
}

void Impress::goto_page(int page)
{
    sendMsg("goto_slide\n"+QString(page)+"\n\n");
}

QString Impress::getFilePath(const QString filename) const
{
    QString writablePath = QStandardPaths::
            writableLocation(QStandardPaths::DataLocation);
    qDebug() << "writablePath: " << writablePath;

    QString absolutePath = QDir(writablePath).absolutePath();
    qDebug() << "absoluePath: " << absolutePath;

    // We need to make sure we have the path for storage
    QDir dir(absolutePath);
    if ( dir.mkdir(absolutePath) ) {
        qDebug() << "Successfully created the path!";
    }

    QString path = absolutePath + "/" + filename;

    qDebug() << "path: " << path;

    return path;
}
