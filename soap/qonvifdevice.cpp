#include "qonvifdevice.h"

QOnvifDevice::QOnvifDevice()
{    
    Manufacturer="legahero";
    Model="test";
    FirmwareVersion="1.0";
    SerialNumber="10001";
    HardwareId="1";

    anaXAddr;
    anaRuleSupport="false";
    anaAnalyticsModuleSupport="false";

    //Events
    //eveXAddr;
    eveWSSubscriptionPolicySupport="false";
    eveWSPullPointSupport="false";
    eveWSPausableSubscriptionManagerInterfaceSupport="false";

    //Imaging
    //imgXAddr;

    //Media
    //medXAddr;
    //medStreamingCapabilities;

    //PTZ
    //ptzXAddr;

    //Extension
    //ext_adXAddr;
}
/*
<tds:GetDNSResponse><tds:DNSInformation><tt:FromDHCP>false</tt:FromDHCP>
<tt:DNSManual><tt:Type>IPv4</tt:Type>
<tt:IPv4Address>8.8.8.8</tt:IPv4Address>
</tt:DNSManual>
</tds:DNSInformation>
</tds:GetDNSResponse>
 * */
QSoapMessage QOnvifDevice::OnGetDNS(QSoapMessage& req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");

    QSoapElement ddElement;
    ddElement.tagName="d:GetDNSResponse";

    QSoapElement DNSInformationEle;
    DNSInformationEle.tagName="d:DNSInformation";
    DNSInformationEle.AppendChild("tt:FromDHCP","false");

    QSoapElement DNSManualEle;
    DNSManualEle.tagName="tt:DNSManual";
    DNSManualEle.AppendChild("tt:Type","IPv4");
    DNSManualEle.AppendChild("tt:IPv4Address","8.8.8.8");
    ddElement.AppendChild(DNSManualEle);

    ddElement.AppendChild(DNSInformationEle);
    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

/*
请求：<GetNetworkInterfaces xmlns="http://www.onvif.org/ver10/device/wsdl"/>
应答：
<tds:GetNetworkInterfacesResponse><tds:NetworkInterfaces token="eth0"><tt:Enabled>true</tt:Enabled>
<tt:Info><tt:Name>eth0</tt:Name>
<tt:HwAddress>28:57:be:2d:7a:59</tt:HwAddress>
<tt:MTU>1500</tt:MTU>
</tt:Info>
<tt:Link><tt:AdminSettings><tt:AutoNegotiation>true</tt:AutoNegotiation>
<tt:Speed>100</tt:Speed>
<tt:Duplex>Full</tt:Duplex>
</tt:AdminSettings>
<tt:OperSettings><tt:AutoNegotiation>true</tt:AutoNegotiation>
<tt:Speed>100</tt:Speed>
<tt:Duplex>Full</tt:Duplex>
</tt:OperSettings>
<tt:InterfaceType>0</tt:InterfaceType>
</tt:Link>
<tt:IPv4><tt:Enabled>true</tt:Enabled>
<tt:Config><tt:Manual><tt:Address>10.7.36.222</tt:Address>
<tt:PrefixLength>24</tt:PrefixLength>
</tt:Manual>
<tt:DHCP>false</tt:DHCP>
</tt:Config>
</tt:IPv4>
<tt:IPv6><tt:Enabled>true</tt:Enabled>
<tt:Config><tt:AcceptRouterAdvert>false</tt:AcceptRouterAdvert>
<tt:DHCP>Off</tt:DHCP>
<tt:LinkLocal><tt:Address>fe80::2a57:beff:fe2d:7a59</tt:Address>
<tt:PrefixLength>64</tt:PrefixLength>
</tt:LinkLocal>
<tt:FromDHCP><tt:Address>fe80::2a57:beff:fe2d:7a59</tt:Address>
<tt:PrefixLength>64</tt:PrefixLength>
</tt:FromDHCP>
</tt:Config>
</tt:IPv6>
</tds:NetworkInterfaces>
<tds:NetworkInterfaces token="wlan0"><tt:Enabled>false</tt:Enabled>
<tt:Info><tt:Name>wlan0</tt:Name>
<tt:HwAddress>20:f4:1b:39:7d:0c</tt:HwAddress>
<tt:MTU>1500</tt:MTU>
</tt:Info>
<tt:IPv4><tt:Enabled>true</tt:Enabled>
<tt:Config><tt:FromDHCP><tt:Address>169.254.62.92</tt:Address>
<tt:PrefixLength>16</tt:PrefixLength>
</tt:FromDHCP>
<tt:DHCP>true</tt:DHCP>
</tt:Config>
</tt:IPv4>
</tds:NetworkInterfaces>
</tds:GetNetworkInterfacesResponse>
 * */
QSoapMessage QOnvifDevice::OnGetNetworkInterfaces(QSoapMessage& req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");

    QSoapElement ddElement;
    ddElement.tagName="d:GetNetworkInterfacesResponse";

    QSoapElement NetworkInterfacesEle;
    NetworkInterfacesEle.tagName="tt:NetworkInterfaces";
    NetworkInterfacesEle.Attributes().insert("token","eth0");
    NetworkInterfacesEle.AppendChild("tt:Enabled","true");

    QSoapElement InfoEle;
    InfoEle.tagName="tt:Info";
    InfoEle.AppendChild("tt:Name","eth0");
    InfoEle.AppendChild("tt:HwAddress",MacAddress);
    InfoEle.AppendChild("tt:MTU","1500");
    NetworkInterfacesEle.AppendChild(InfoEle);

    QSoapElement LinkEle;
    LinkEle.tagName="tt:Link";

    QSoapElement AdminSettingsEle;
    AdminSettingsEle.tagName="tt:AdminSettings";
    AdminSettingsEle.AppendChild("tt:AutoNegotiation","true");
    AdminSettingsEle.AppendChild("tt:Speed","100");
    AdminSettingsEle.AppendChild("tt:Duplex","Full");
    LinkEle.AppendChild(AdminSettingsEle);

    QSoapElement OperSettingsEle;
    OperSettingsEle.tagName="tt:OperSettings";
    OperSettingsEle.AppendChild("tt:AutoNegotiation","true");
    OperSettingsEle.AppendChild("tt:Speed","100");
    OperSettingsEle.AppendChild("tt:Duplex","Full");

    LinkEle.AppendChild(AdminSettingsEle);
    LinkEle.AppendChild("tt:InterfaceType","0");
    NetworkInterfacesEle.AppendChild(LinkEle);

    QSoapElement IPv4Ele;
    IPv4Ele.tagName="tt:IPv4";
    IPv4Ele.AppendChild("tt:Enabled","true");

    QSoapElement ConfigEle;
    ConfigEle.tagName="tt:Config";
    QSoapElement ManualEle;
    ManualEle.tagName="tt:Manual";
    ManualEle.AppendChild("tt:Address",IPAddress);
    ManualEle.AppendChild("tt:PrefixLength","24");
    ConfigEle.AppendChild("tt:DHCP","false");
    ConfigEle.AppendChild(ManualEle);

    IPv4Ele.AppendChild(ConfigEle);
    NetworkInterfacesEle.AppendChild(IPv4Ele);

    ddElement.AppendChild(NetworkInterfacesEle);
    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

/*
 * 请求：POST / HTTP/1.1\r\nHost: 192.168.1.51\r\nContent-Type: application/soap+xml; charset=utf-8\r\nContent-Length: 299\r\n\r\n
 * <?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\"><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">
 * <GetDeviceInformation xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/></s:Body></s:Envelope>
 * 应答：<SOAP-ENV:Body>
    <devicews:GetDeviceInformationResponse>
      <devicews:Manufacturer>This is sample onvif server</devicews:Manufacturer>
      <devicews:Model>2</devicews:Model>
      <devicews:FirmwareVersion>0.1</devicews:FirmwareVersion>
      <devicews:SerialNumber>100000</devicews:SerialNumber>
      <devicews:HardwareId>1</devicews:HardwareId>
    </devicews:GetDeviceInformationResponse>
  </SOAP-ENV:Body>
 * */
QSoapMessage QOnvifDevice::OnGetDeviceInformation(QSoapMessage &req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");

    QSoapElement ddElement;
    ddElement.tagName="d:GetDeviceInformationResponse";
    ddElement.AppendChild("d:Manufacturer",Manufacturer);
    ddElement.AppendChild("d:Model",Model);
    ddElement.AppendChild("d:FirmwareVersion",FirmwareVersion);
    ddElement.AppendChild("d:SerialNumber",SerialNumber);
    ddElement.AppendChild("d:HardwareId",HardwareId);
    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

/* 请求：<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">
 * <s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><GetCapabilities xmlns=\"http://www.onvif.org/ver10/device/wsdl\"><Category>All</Category></GetCapabilities></s:Body>
 * </s:Envelope>
 * 应答：<SOAP-ENV:Body><devicews:GetCapabilitiesResponse>
 * <devicews:Capabilities xsi:type="tt:Capabilities">
 * <tt:Analytics xsi:type="tt:AnalyticsCapabilities"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr><tt:RuleSupport>false</tt:RuleSupport><tt:AnalyticsModuleSupport>false</tt:AnalyticsModuleSupport></tt:Analytics>
 * <tt:Events xsi:type="tt:EventCapabilities"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr><tt:WSSubscriptionPolicySupport>false</tt:WSSubscriptionPolicySupport><tt:WSPullPointSupport>false</tt:WSPullPointSupport><tt:WSPausableSubscriptionManagerInterfaceSupport>false</tt:WSPausableSubscriptionManagerInterfaceSupport></tt:Events>
 * <tt:Imaging xsi:type="tt:ImagingCapabilities"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr></tt:Imaging>
 * <tt:Media xsi:type="tt:MediaCapabilities"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr>
 * <tt:StreamingCapabilities xsi:type="tt:RealTimeStreamingCapabilities"></tt:StreamingCapabilities></tt:Media>
 * <tt:PTZ xsi:type="tt:PTZCapabilities"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr></tt:PTZ><tt:Extension xsi:type="tt:CapabilitiesExtension">
 * <tt:AnalyticsDevice xsi:type="tt:AnalyticsDeviceCapabilities"><tt:XAddr>http://192.168.1.51:8004</tt:XAddr></tt:AnalyticsDevice></tt:Extension>
 * </devicews:Capabilities></devicews:GetCapabilitiesResponse></SOAP-ENV:Body>
 * */
QSoapMessage QOnvifDevice::OnGetCapabilities(QSoapMessage &req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");

    QSoapElement ddElement;
    ddElement.tagName="d:GetCapabilitiesResponse";

    QSoapElement CapabilitiesElement;
    CapabilitiesElement.tagName="d:Capabilities";

    QSoapElement AnalyticsEle;
    AnalyticsEle.tagName="tt:Analytics";
    AnalyticsEle.AppendChild("tt:XAddr",this->anaXAddr);
    AnalyticsEle.AppendChild("tt:RuleSupport",this->anaRuleSupport);
    AnalyticsEle.AppendChild("tt:AnalyticsModuleSupport",anaAnalyticsModuleSupport);
    CapabilitiesElement.AppendChild(AnalyticsEle);

    QSoapElement EventsEle;
    EventsEle.tagName="tt:Events";
    EventsEle.AppendChild("tt:XAddr",eveXAddr);
    EventsEle.AppendChild("tt:WSSubscriptionPolicySupport",eveWSSubscriptionPolicySupport);
    EventsEle.AppendChild("tt:WSPullPointSupport",eveWSPullPointSupport);
    EventsEle.AppendChild("tt:WSPausableSubscriptionManagerInterfaceSupport",eveWSPausableSubscriptionManagerInterfaceSupport);
    CapabilitiesElement.AppendChild(EventsEle);

    QSoapElement ImagingEle;
    ImagingEle.tagName="tt:Imaging";
    ImagingEle.AppendChild("tt:XAddr",imgXAddr);
    CapabilitiesElement.AppendChild(ImagingEle);

    QSoapElement MediaEle;
    MediaEle.tagName="tt:Media";
    MediaEle.AppendChild("tt:XAddr",medXAddr);
    MediaEle.AppendChild("tt:StreamingCapabilities","");
    CapabilitiesElement.AppendChild(MediaEle);

    QSoapElement ptzEle;
    ptzEle.tagName="tt:PTZ";
    ptzEle.AppendChild("tt:XAddr",ptzXAddr);
    CapabilitiesElement.AppendChild(ptzEle);

    QSoapElement ExtensionEle;
    ExtensionEle.tagName="tt:Extension";
    QSoapElement AnalyticsDeviceEle;
    AnalyticsDeviceEle.tagName="tt:AnalyticsDevice";
    AnalyticsDeviceEle.AppendChild("tt:XAddr",ext_adXAddr);
    ExtensionEle.AppendChild(AnalyticsDeviceEle);
    CapabilitiesElement.AppendChild(ExtensionEle);

    ddElement.AppendChild(CapabilitiesElement);
    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

QSoapMessage QOnvifDevice::OnGetServices(QSoapMessage& req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");

    QSoapElement ddElement;
    ddElement.tagName="d:GetServicesResponse";

    QSoapElement VersionEle;
    VersionEle.tagName="d:Version";
    VersionEle.AppendChild("tt:Major","2");
    VersionEle.AppendChild("tt:Minor","6");

    QSoapElement dServiceElement;
    dServiceElement.tagName="d:Service";
    dServiceElement.AppendChild("d:Namespace","http://www.onvif.org/ver10/device/wsdl");
    dServiceElement.AppendChild("d:XAddr",this->anaXAddr);
    dServiceElement.AppendChild(VersionEle);
    ddElement.AppendChild(dServiceElement);

    QSoapElement mServiceElement;
    mServiceElement.tagName="d:Service";
    mServiceElement.AppendChild("d:Namespace","http://www.onvif.org/ver10/media/wsdl");
    mServiceElement.AppendChild("d:XAddr",this->medXAddr);
    mServiceElement.AppendChild(VersionEle);
    ddElement.AppendChild(mServiceElement);

    QSoapElement eServiceElement;
    eServiceElement.tagName="d:Service";
    eServiceElement.AppendChild("d:Namespace","http://www.onvif.org/ver10/events/wsdl");
    eServiceElement.AppendChild("d:XAddr",this->eveXAddr);
    eServiceElement.AppendChild(VersionEle);
    ddElement.AppendChild(eServiceElement);

    QSoapElement pServiceElement;
    pServiceElement.tagName="d:Service";
    pServiceElement.AppendChild("d:Namespace","http://www.onvif.org/ver10/ptz/wsdl");
    pServiceElement.AppendChild("d:XAddr",this->ptzXAddr);
    pServiceElement.AppendChild(VersionEle);
    ddElement.AppendChild(pServiceElement);

    QSoapElement iServiceElement;
    iServiceElement.tagName="d:Service";
    iServiceElement.AppendChild("d:Namespace","http://www.onvif.org/ver10/imaging/wsdl");
    iServiceElement.AppendChild("d:XAddr",this->imgXAddr);
    iServiceElement.AppendChild(VersionEle);
    ddElement.AppendChild(iServiceElement);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}


QSoapMessage QOnvifDevice::OnGetScopes(QSoapMessage& req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");

    QSoapElement ddElement;
    ddElement.tagName="d:GetScopesResponse";

    QSoapElement sScopesElement;
    sScopesElement.tagName="d:Scopes";
    sScopesElement.AppendChild("tt:ScopeDef","Fixed");
    sScopesElement.AppendChild("tt:ScopeItem","onvif://www.onvif.org/Profile/Streaming");
    ddElement.AppendChild(sScopesElement);

    QSoapElement cScopesElement;
    cScopesElement.tagName="d:Scopes";
    cScopesElement.AppendChild("tt:ScopeDef","Fixed");
    cScopesElement.AppendChild("tt:ScopeItem","onvif://www.onvif.org/location/country/china");
    ddElement.AppendChild(cScopesElement);

    QSoapElement vScopesElement;
    vScopesElement.tagName="d:Scopes";
    vScopesElement.AppendChild("tt:ScopeDef","Fixed");
    vScopesElement.AppendChild("tt:ScopeItem","onvif://www.onvif.org/type/video_server");
    ddElement.AppendChild(vScopesElement);

    QSoapElement v2ScopesElement;
    v2ScopesElement.tagName="d:Scopes";
    v2ScopesElement.AppendChild("tt:ScopeDef","Fixed");
    v2ScopesElement.AppendChild("tt:ScopeItem","onvif://www.onvif.org/name/VideoServer");
    ddElement.AppendChild(v2ScopesElement);

    QSoapElement nScopesElement;
    nScopesElement.tagName="d:Scopes";
    nScopesElement.AppendChild("tt:ScopeDef","Fixed");
    nScopesElement.AppendChild("tt:ScopeItem","onvif://www.onvif.org/hardware/NVT");
    ddElement.AppendChild(nScopesElement);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}
