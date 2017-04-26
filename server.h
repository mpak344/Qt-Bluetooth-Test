#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QLowEnergyController>
#include <QLowEnergyService>
class Server : public QObject
{
    Q_PROPERTY(QString serverName READ serverName WRITE setServerName NOTIFY serverNameChanged)
    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    QString serverName() const;

signals:
    void serverNameChanged(QString serverName);

public slots:
    void setServerName(QString serverName);
    void sendMessage(const QString &message);

    /*****************************************/
    void startServer();
private:
    QString m_serverName;
    QLowEnergyAdvertisingData m_advertisingData;
    QLowEnergyServiceData m_serviceData;

    QLowEnergyController * m_leController;
    QLowEnergyService * m_service;


    void setupAdvertisingData();
    void setupServiceData();
    void setupService();
};

#endif // SERVER_H
