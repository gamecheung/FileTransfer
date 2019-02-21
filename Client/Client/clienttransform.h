#ifndef CLIENTTRANSFORM_H
#define CLIENTTRANSFORM_H

#include <QDialog>
#include <QtNetwork>
#include <QByteArray>
#include <QFile>

namespace Ui {
class ClientTransform;
}

class ClientTransform : public QDialog
{
    Q_OBJECT

public:
    explicit ClientTransform(QWidget *parent = 0);
    ~ClientTransform();

private slots:
    void openFile();
    void connectToHost();
    void send();

    void updateStatus(const QString& status);
    void displayError(QAbstractSocket::SocketError e);

    void updateClientProgress(qint64 bytesWritten);
private:
    bool sendHead();
    bool sendContent();
private:
    Ui::ClientTransform *ui;
    QTcpSocket          *m_client;
    QString             m_file;

    qint64              m_totalBytes;
    qint64              m_bytesWritten;
    qint64              m_bytesToWrite;
    qint64              m_payloadSize;
    QByteArray          m_outBlock;

    QFile*              m_localFile;
};

#endif // CLIENTTRANSFORM_H
