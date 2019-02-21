#include "servertransform.h"
#include "ui_servertransform.h"
#include <QDebug>
#include <QFile>

ServerTransform::ServerTransform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerTransform)
{
    ui->setupUi(this);
    m_server = new QTcpServer(this);

    ui->progressBar->hide();
    m_bytesReceived = 0;
    m_totalBytes = 0;
    m_fileNameSize = 0;


    connect(m_server,SIGNAL(newConnection()),this,SLOT(acceptConnect()));
}

ServerTransform::~ServerTransform()
{
    delete ui;
}

void ServerTransform::serverStatus(const QString &status)
{
    ui->label_Status->setText(status);
}

void ServerTransform::on_pushButton_clicked()
{

    if(!m_server->listen(QHostAddress::AnyIPv4,6666))
    {
        serverStatus(tr("Server listen failed!"));
        close();
    }

//    m_server->close();

}

void ServerTransform::receive()
{
//    qDebug() << m_connectedSocket->bytesAvailable();
    QDataStream receive(m_connectedSocket);
    receive.setVersion(QDataStream::Qt_5_9);
    if(m_bytesReceived <= (qint64)(sizeof(qint64) * 2))
    {
        qDebug() << __FUNCTION__ << m_connectedSocket->bytesAvailable();
        if(m_connectedSocket->bytesAvailable() >= (qint64)(sizeof(qint64) * 2) && m_fileNameSize == 0)
        {
            receive >> m_totalBytes >> m_fileNameSize;
            m_bytesReceived += (qint64)(sizeof(qint64) * 2);
        }
        qDebug() << __FUNCTION__ << m_connectedSocket->bytesAvailable();
        if(m_connectedSocket->bytesAvailable() >= m_fileNameSize && m_fileNameSize > 0)
        {
           receive >> m_fileName;
           m_bytesReceived += m_fileNameSize;
           m_localFile = new QFile(m_fileName);
           if(!m_localFile->open(QIODevice::WriteOnly))
               serverStatus(tr("Open file %1 failed!").arg(m_fileName));
           else
               return;
        }

    }

    if(m_bytesReceived < m_totalBytes)
    {
        m_bytesReceived += m_connectedSocket->bytesAvailable();
        m_inBlock = m_connectedSocket->readAll();
        m_localFile->write(m_inBlock);
        m_inBlock.resize(0);
    }

    ui->progressBar->setMaximum(m_totalBytes);
    ui->progressBar->setValue(m_bytesReceived);

    if(m_bytesReceived == m_totalBytes)
    {
        m_connectedSocket->close();
        m_localFile->close();
        serverStatus(tr("Receive file ok!"));
    }
}

void ServerTransform::acceptConnect()
{
    m_connectedSocket = m_server->nextPendingConnection();
    connect(m_connectedSocket,SIGNAL(readyRead()),this,SLOT(receive()));
    connect(m_connectedSocket,SIGNAL(disconnected()),m_connectedSocket,SLOT(deleteLater()));
}
