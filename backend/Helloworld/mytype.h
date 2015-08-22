#ifndef MYTYPE_H
#define MYTYPE_H

#include <QObject>
#include <QTcpSocket>

enum{ST_READ_STARTED_OR_NOT, ST_xxx};

class MyType : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString helloWorld READ helloWorld WRITE setHelloWorld NOTIFY helloWorldChanged )
    Q_PROPERTY(QString PIN READ PIN WRITE setPIN NOTIFY PINChanged)

public:
    explicit MyType(QObject *parent = 0);
    ~MyType();

signals:
    void helloWorldChanged();
    void PINChanged();
    void connectedToImpressServer();
    void shouldSetPINinImpress(QString PIN);
    void paired();

public slots:
    void pair();

private slots:
    void emitConnectedState();
    void readMsg();


protected:
    QString helloWorld() { return m_message; }
    void setHelloWorld(QString msg) { m_message = msg; Q_EMIT helloWorldChanged(); }

    QString PIN() { return m_PIN; }
    void setPIN(QString p) { m_PIN = p; emit PINChanged(); }

    void sendMsg(QString msg);

    QString m_message;
    QString m_PIN;

    QTcpSocket *tcpSocket;
    QString respStr;
    quint16 blockSize;

    int status;
    bool
};

#endif // MYTYPE_H

