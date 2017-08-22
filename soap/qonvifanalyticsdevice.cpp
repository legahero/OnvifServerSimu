#include "qonvifanalyticsdevice.h"

QOnvifAnalyticsdevice::QOnvifAnalyticsdevice()
{

}

QSoapMessage QOnvifAnalyticsdevice::OnGetAnalyticsEngines(QSoapMessage& req)
{
    QSoapMessage resp;

    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-envelope","SOAP-ENV");
    resp.envelope.AddNameSpace("http://www.w3.org/2003/05/soap-encoding","SOAP-ENC");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    resp.envelope.AddNameSpace("http://www.w3.org/2001/XMLSchema","xsd");
    resp.envelope.AddNameSpace("http://www.w3.org/2005/08/addressing","a");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/device/wsdl","d");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/schema","tt");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/media/wsdl","m");
    resp.envelope.AddNameSpace("http://www.onvif.org/ver10/analyticsdevice/wsdl","ad");

    QSoapElement ddElement;
    ddElement.tagName="ad:GetAnalyticsEnginesResponse";

    /*
    QSoapElement ConfigurationEle;
    ConfigurationEle.tagName="tt:Configuration";

    ddElement.AppendChild(ConfigurationEle);
    */

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}
