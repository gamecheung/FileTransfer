#include "clienttransform.h"
#include "ui_clienttransform.h"
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QtGlobal>

ClientTransform::ClientTransform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientTransform)
{
    ui->setupUi(this);
    m_client = new QTcpSocket(this);
    connect(ui->pushButton_Open,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(ui->pushButton_Send,SIGNAL(clicked()),this,SLOT(send()));
    connect(ui->pushButton_Connect,SIGNAL(clicked()),this,SLOT(connectToHost()));
    connect(m_client,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    connect(m_client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));


    ui->pushButton_Send->setEnabled(false);
    m_bytesToWrite = 0;
    m_bytesWritten = 0;
    m_totalBytes = 0;
    m_payloadSize = 64 * 1024;
}

ClientTransform::~ClientTransform()
{
    delete ui;
}

void ClientTransform::openFile()
{
    m_file = QFileDialog::getOpenFileName(this,tr("Choose transform file"),tr("."));

    if(!m_file.isEmpty())
    {
        ui->pushButton_Send->setEnabled(true);
    }

}

void ClientTransform::connectToHost()
{
    QString host = ui->lineEdit_Host->text();
    quint16  port = ui->lineEdit_Port->text().toInt();

    m_client->abort();
    m_client->connectToHost(host,port);
    updateStatus(tr("connect ok!"));

}

void ClientTransform::send()
{
    if(!m_file.isEmpty())
    {
        m_localFile = new  QFile(m_file);
        if(!m_localFile->open(QFile::ReadOnly))
            updateStatus(tr("Open file failed!"));
        m_totalBytes = m_localFile->size();
        ui->progressBar->setMaximum(m_totalBytes);
        QDataStream out(&m_outBlock,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_9);

        QString fileName = m_file.right(m_file.size() - m_file.lastIndexOf('/') - 1);
        out << (qint64)0 << (qint64)0 << fileName;

        m_totalBytes += m_outBlock.size();

        out.device()->seek(0);

        out << m_totalBytes << (qint64)(m_outBlock.size() - sizeof(qint64)*2);

//        m_bytesWritten = m_client->write(m_outBlock);

        m_bytesToWrite = m_totalBytes - m_client->write(m_outBlock);
        m_outBlock.resize(0);
    }
}

void ClientTransform::updateStatus(const QString &status)
{
    ui->label_Status->setText(status);
}

void ClientTransform::displayError(QAbstractSocket::SocketError e)
{
    e;
    updateStatus(m_client->errorString());
}

void ClientTransform::updateClientProgress(qint64 bytesWritten)
{
    m_bytesWritten += bytesWritten;
    ui->progressBar->setValue(m_bytesWritten);

    if(m_bytesToWrite > 0)
    {
        m_outBlock = m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
        QDataStream out(&m_outBlock,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_9);
        m_bytesToWrite -= m_client->write(m_outBlock);
        m_outBlock.resize(0);
    }else
    {
        m_localFile->close();
    }

    ui->progressBar->setMaximum(m_totalBytes);
    ui->progressBar->setValue(m_bytesWritten);

    if(m_bytesWritten == m_totalBytes)
    {
        updateStatus(tr("Transform file finished!"));
        m_localFile->close();
        m_client->close();
    }
}

bool ClientTransform::sendHead()
{
//    m_localFile = new  QFile(m_file);
//    m_totalBytes = m_localFile->size();
//    ui->progressBar->setMaximum(m_totalBytes);
//    QDataStream out(&m_outBlock,QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_9);

//    QString fileName = m_file.right(m_file.size() - m_file.lastIndexOf('/') - 1);
//    out << (qint64)0 << (qint64)0 << fileName;

//    m_totalBytes += m_outBlock.size();

//    out.device()->seek(0);

//    out << m_totalBytes << (qint64)(m_outBlock.size() - sizeof(qint64)*2);

//    m_bytesWritten = m_client->write(m_outBlock);
//    ui->progressBar->setValue(m_bytesWritten);

//    m_bytesToWrite = m_totalBytes - m_bytesWritten;
//    m_outBlock.resize(0);

//    return true;
}

bool ClientTransform::sendContent()
{
//    QFile file(m_file);
//    QDataStream out(&m_outBlock,QIODevice::WriteOnly);

//    out.setVersion(QDataStream::Qt_5_9);
//    m_outBlock.resize(file.size());

//    if(!file.open(QIODevice::ReadOnly))
//    {
//        updateStatus(tr("Open file failed!"));
//        return false;
//    }

//    out << file.readAll();
//    m_bytesWritten = m_client->write(m_outBlock);
}
