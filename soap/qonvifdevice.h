#ifndef QONVIFDEVICE_H
#define QONVIFDEVICE_H
#include "qsoapmessage.h"

class QOnvifDevice
{
public:
    QOnvifDevice();

    QSoapMessage OnGetDNS(QSoapMessage& req);
    QSoapMessage OnGetNetworkInterfaces(QSoapMessage& req);
    QSoapMessage OnGetDeviceInformation(QSoapMessage& req);
    QSoapMessage OnGetCapabilities(QSoapMessage& req);
    QSoapMessage OnGetServices(QSoapMessage& req);
    QSoapMessage OnGetScopes(QSoapMessage& req);
public:
    QString IPAddress;
    QString MacAddress;
    //DeviceInfo
    QString Manufacturer;
    QString Model;
    QString FirmwareVersion;
    QString SerialNumber;
    QString HardwareId;

    //Analytics
    QString anaXAddr;
    QString anaRuleSupport;
    QString anaAnalyticsModuleSupport;

    //Events
    QString eveXAddr;
    QString eveWSSubscriptionPolicySupport;
    QString eveWSPullPointSupport;
    QString eveWSPausableSubscriptionManagerInterfaceSupport;

    //Imaging
    QString imgXAddr;

    //Media
    QString medXAddr;
    //QString medStreamingCapabilities;
    //PTZ
    QString ptzXAddr;

    //Extension
    QString ext_adXAddr;


};

#endif // QONVIFDEVICE_H
