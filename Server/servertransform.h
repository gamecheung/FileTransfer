#ifndef SERVERTRANSFORM_H
#define SERVERTRANSFORM_H

#include <QDialog>
#include <QtNetwork>


class QFile;
namespace Ui {
class ServerTransform;
}

class ServerTransform : public QDialog
{
    Q_OBJECT

public:
    explicit ServerTransform(QWidget *parent = 0);
    ~ServerTransform();
private slots:
    void serverStatus(const QString& status);
    void on_pushButton_clicked();

    void receive();
    void acceptConnect();
private:
    Ui::ServerTransform *ui;
    QTcpServer          *m_server;
    QTcpSocket          *m_connectedSocket;
    qint64              m_totalBytes;
    qint64              m_bytesReceived;
    qint64              m_fileNameSize;

    QString             m_fileName;
    QFile               *m_localFile;

    QByteArray          m_inBlock;

};

#endif // SERVERTRANSFORM_H
