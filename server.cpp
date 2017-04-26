#include "server.h"
#include <QDebug>
#include <QBluetoothUuid>
#include <QLowEnergyDescriptorData>
#include <QLowEnergyAdvertisingParameters>

//
//
//
//
//90de6eba-28e2-11e7-93ae-92361f002671
//90de6f8c-28e2-11e7-93ae-92361f002671
//90de7054-28e2-11e7-93ae-92361f002671
//90de711c-28e2-11e7-93ae-92361f002671
//90de757c-28e2-11e7-93ae-92361f002671

static const QBluetoothUuid serviceUUID(QString("{90de601e-28e2-11e7-93ae-92361f002671}"));
static const QBluetoothUuid sendChar(QString("{90de6492-28e2-11e7-93ae-92361f002671}"));
static const QBluetoothUuid writeChar(QString("{90de6b68-28e2-11e7-93ae-92361f002671}"));
static const QBluetoothUuid writeDescriptor(QString("{90de6ce4-28e2-11e7-93ae-92361f002671}"));
static const QBluetoothUuid test5(QString("{90de6dde-28e2-11e7-93ae-92361f002671}"));


Server::Server(QObject *parent)
    : QObject(parent)
    , m_serverName("")
    , m_leController(nullptr)
{

}

QString Server::serverName() const
{
    return m_serverName;
}

void Server::setServerName(QString serverName)
{
    if (m_serverName == serverName)
        return;
    m_serverName = serverName;
    emit serverNameChanged(serverName);
}

void Server::sendMessage(const QString &message)
{
    if(m_service == nullptr) {
        qDebug() << "SERVICE IS NOT AVIABLE";
        return;
    }
    QByteArray value;
    value.append(message); // Actual value.
    QLowEnergyCharacteristic characteristic
            = m_service->characteristic(writeChar);
    Q_ASSERT(characteristic.isValid());
    m_service->writeCharacteristic(characteristic, value);
}

void Server::startServer()
{
    qDebug() << "START SERVER " << serverName();
    setupAdvertisingData();
    setupServiceData();
    setupService();
}

void Server::setupAdvertisingData()
{
    m_advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    m_advertisingData.setIncludePowerLevel(true);
    m_advertisingData.setLocalName(serverName());
    //m_advertisingData.setServices(QList<QBluetoothUuid>() <<  serviceUUID);
}

void Server::setupServiceData()
{
    QLowEnergyCharacteristicData charData;

    charData.setUuid(writeChar);
    charData.setValue(QByteArray(2, 0));
    charData.setProperties(QLowEnergyCharacteristic::Notify);
    const QLowEnergyDescriptorData clientConfig(QBluetoothUuid::ClientCharacteristicConfiguration,
                                                QByteArray(2, 0));
    charData.addDescriptor(clientConfig);
    m_serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    m_serviceData.setUuid(test5);
    m_serviceData.addCharacteristic(charData);

 }

void Server::setupService()
{
    if(m_leController != nullptr) {
        m_leController->stopAdvertising();
        delete m_leController;
        m_leController = nullptr;
    }

    m_leController = QLowEnergyController::createPeripheral(this);
    m_service = m_leController->addService(m_serviceData, this);
    m_leController->startAdvertising(QLowEnergyAdvertisingParameters(), m_advertisingData,
                                   m_advertisingData);
    auto reconnect = [=]() {
        m_leController->startAdvertising(QLowEnergyAdvertisingParameters(), m_advertisingData,
                                       m_advertisingData);
    };
    QObject::connect(m_leController, &QLowEnergyController::disconnected, reconnect);

}

