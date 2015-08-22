#include "mytype.h"

MyType::MyType(QObject *parent) :
    QObject(parent),
    m_PIN("1111"),
    m_message("")
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost("127.0.0.1", 1599);
    connect(tcpSocket, SIGNAL(connected()),this, SLOT(emitConnectedState()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMsg()));

    qDebug() << "construct function done! ";
}

MyType::~MyType() {

}

void MyType::emitConnectedState()
{
    qDebug() << "connected to libre impress server";

    emit connectedToImpressServer();

    pair();
}

void MyType::readMsg()
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
        }
    }
}

void MyType::sendMsg(QString msg)
{
    QByteArray array (msg.toStdString().c_str());
    tcpSocket->write(array);
}

void MyType::pair()
{
    sendMsg("LO_SERVER_CLIENT_PAIR\nubuntu Phone\n"+m_PIN+"\n\n");
}

