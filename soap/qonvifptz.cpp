#include "qonvifptz.h"

QOnvifPtz::QOnvifPtz()
{

}
/*
<ptzws:GetNodesResponse>
      <ptzws:PTZNode xsi:type="tt:PTZNode" token="">
        <tt:Name>PTZNode_Channel1</tt:Name>
        <tt:SupportedPTZSpaces xsi:type="tt:PTZSpaces">
          <tt:AbsolutePanTiltPositionSpace xsi:type="tt:Space2DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
            <tt:YRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:YRange>
          </tt:AbsolutePanTiltPositionSpace>
          <tt:AbsoluteZoomPositionSpace xsi:type="tt:Space1DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>0</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
          </tt:AbsoluteZoomPositionSpace>
          <tt:RelativePanTiltTranslationSpace xsi:type="tt:Space2DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
            <tt:YRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:YRange>
          </tt:RelativePanTiltTranslationSpace>
          <tt:RelativeZoomTranslationSpace xsi:type="tt:Space1DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
          </tt:RelativeZoomTranslationSpace>
          <tt:ContinuousPanTiltVelocitySpace xsi:type="tt:Space2DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
            <tt:YRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:YRange>
          </tt:ContinuousPanTiltVelocitySpace>
          <tt:ContinuousZoomVelocitySpace xsi:type="tt:Space1DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>-1</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
          </tt:ContinuousZoomVelocitySpace>
          <tt:PanTiltSpeedSpace xsi:type="tt:Space1DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>0</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
          </tt:PanTiltSpeedSpace>
          <tt:ZoomSpeedSpace xsi:type="tt:Space1DDescription">
            <tt:URI>http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace</tt:URI>
            <tt:XRange xsi:type="tt:FloatRange">
              <tt:Min>0</tt:Min>
              <tt:Max>1</tt:Max>
            </tt:XRange>
          </tt:ZoomSpeedSpace>
        </tt:SupportedPTZSpaces>
        <tt:MaximumNumberOfPresets>80</tt:MaximumNumberOfPresets>
        <tt:HomeSupported>true</tt:HomeSupported>
        <tt:Extension xsi:type="tt:PTZNodeExtension">
          <tt:SupportedPresetTour xsi:type="tt:PTZPresetTourSupported">
            <tt:MaximumNumberOfPresetTours>8</tt:MaximumNumberOfPresetTours>
            <tt:PTZPresetTourOperation>Start</tt:PTZPresetTourOperation>
            <tt:PTZPresetTourOperation>Stop</tt:PTZPresetTourOperation>
            <tt:PTZPresetTourOperation>Pause</tt:PTZPresetTourOperation>
          </tt:SupportedPresetTour>
        </tt:Extension>
      </ptzws:PTZNode>
    </ptzws:GetNodesResponse>
 * */
QSoapMessage QOnvifPtz::OnGetNodes(QSoapMessage& req)
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
    resp.envelope.AddNameSpace("http://www.onvif.org/ver20/ptz/wsdl","ptz");

    QSoapElement ddElement;
    ddElement.tagName="ptz:GetNodesResponse";


    QSoapElement PTZNodeEle;
    PTZNodeEle.tagName="ptz:PTZNode";
    PTZNodeEle.Attributes().insert("xsi:type","tt:PTZNode");
    PTZNodeEle.AppendChild("tt:Name","PTZNode_Channel1");

    QSoapElement SupportedPTZSpacesEle;
    SupportedPTZSpacesEle.tagName="tt:SupportedPTZSpaces";
    SupportedPTZSpacesEle.Attributes().insert("xsi:type","tt:PTZSpaces");

    QSoapElement AbsolutePanTiltPositionSpaceEle;
    AbsolutePanTiltPositionSpaceEle.tagName="tt:AbsolutePanTiltPositionSpace";
    AbsolutePanTiltPositionSpaceEle.Attributes().insert("xsi:type","tt:Space2DDescription");
    AbsolutePanTiltPositionSpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");

    QSoapElement XRangeEle;
    XRangeEle.tagName="tt:XRange";
    XRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    XRangeEle.AppendChild("tt:Min","-1");
    XRangeEle.AppendChild("tt:Max","1");
    AbsolutePanTiltPositionSpaceEle.AppendChild(XRangeEle);
    QSoapElement YRangeEle;
    YRangeEle.tagName="tt:YRange";
    YRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    YRangeEle.AppendChild("tt:Min","-1");
    YRangeEle.AppendChild("tt:Max","1");
    AbsolutePanTiltPositionSpaceEle.AppendChild(YRangeEle);

    SupportedPTZSpacesEle.AppendChild(AbsolutePanTiltPositionSpaceEle);

    QSoapElement AbsoluteZoomPositionSpaceEle;
    AbsoluteZoomPositionSpaceEle.tagName="tt:AbsoluteZoomPositionSpace";
    AbsoluteZoomPositionSpaceEle.Attributes().insert("xsi:type","tt:Space1DDescription");
    AbsoluteZoomPositionSpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");

    QSoapElement zXRangeEle;
    zXRangeEle.tagName="tt:XRange";
    zXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    zXRangeEle.AppendChild("tt:Min","0");
    zXRangeEle.AppendChild("tt:Max","1");
    AbsoluteZoomPositionSpaceEle.AppendChild(zXRangeEle);
    SupportedPTZSpacesEle.AppendChild(AbsoluteZoomPositionSpaceEle);

    QSoapElement RelativePanTiltTranslationSpaceEle;
    RelativePanTiltTranslationSpaceEle.tagName="tt:RelativePanTiltTranslationSpace";
    RelativePanTiltTranslationSpaceEle.Attributes().insert("xsi:type","tt:Space2DDescription");
    RelativePanTiltTranslationSpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");

    QSoapElement rXRangeEle;
    rXRangeEle.tagName="tt:XRange";
    rXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    rXRangeEle.AppendChild("tt:Min","-1");
    rXRangeEle.AppendChild("tt:Max","1");
    RelativePanTiltTranslationSpaceEle.AppendChild(rXRangeEle);
    QSoapElement rYRangeEle;
    rYRangeEle.tagName="tt:YRange";
    rYRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    rYRangeEle.AppendChild("tt:Min","-1");
    rYRangeEle.AppendChild("tt:Max","1");
    RelativePanTiltTranslationSpaceEle.AppendChild(rYRangeEle);

    SupportedPTZSpacesEle.AppendChild(RelativePanTiltTranslationSpaceEle);

    QSoapElement RelativeZoomTranslationSpaceEle;
    RelativeZoomTranslationSpaceEle.tagName="tt:RelativeZoomTranslationSpace";
    RelativeZoomTranslationSpaceEle.Attributes().insert("xsi:type","tt:Space1DDescription");
    RelativeZoomTranslationSpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");

    QSoapElement rzXRangeEle;
    rzXRangeEle.tagName="tt:XRange";
    rzXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    rzXRangeEle.AppendChild("tt:Min","-1");
    rzXRangeEle.AppendChild("tt:Max","1");
    RelativeZoomTranslationSpaceEle.AppendChild(rzXRangeEle);
    SupportedPTZSpacesEle.AppendChild(RelativeZoomTranslationSpaceEle);

    QSoapElement ContinuousPanTiltVelocitySpaceEle;
    ContinuousPanTiltVelocitySpaceEle.tagName="tt:ContinuousPanTiltVelocitySpace";
    ContinuousPanTiltVelocitySpaceEle.Attributes().insert("xsi:type","tt:Space2DDescription");
    ContinuousPanTiltVelocitySpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");

    QSoapElement cXRangeEle;
    cXRangeEle.tagName="tt:XRange";
    cXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    cXRangeEle.AppendChild("tt:Min","-1");
    cXRangeEle.AppendChild("tt:Max","1");
    ContinuousPanTiltVelocitySpaceEle.AppendChild(cXRangeEle);
    QSoapElement cYRangeEle;
    cYRangeEle.tagName="tt:YRange";
    cYRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    cYRangeEle.AppendChild("tt:Min","-1");
    cYRangeEle.AppendChild("tt:Max","1");
    ContinuousPanTiltVelocitySpaceEle.AppendChild(cYRangeEle);

    SupportedPTZSpacesEle.AppendChild(ContinuousPanTiltVelocitySpaceEle);

    QSoapElement ContinuousZoomVelocitySpaceEle;
    ContinuousZoomVelocitySpaceEle.tagName="tt:ContinuousZoomVelocitySpace";
    ContinuousZoomVelocitySpaceEle.Attributes().insert("xsi:type","tt:Space1DDescription");
    ContinuousZoomVelocitySpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");

    QSoapElement czXRangeEle;
    czXRangeEle.tagName="tt:XRange";
    czXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    czXRangeEle.AppendChild("tt:Min","-1");
    czXRangeEle.AppendChild("tt:Max","1");
    ContinuousZoomVelocitySpaceEle.AppendChild(czXRangeEle);
    SupportedPTZSpacesEle.AppendChild(ContinuousZoomVelocitySpaceEle);

    QSoapElement PanTiltSpeedSpaceEle;
    PanTiltSpeedSpaceEle.tagName="tt:PanTiltSpeedSpace";
    PanTiltSpeedSpaceEle.Attributes().insert("xsi:type","tt:Space1DDescription");
    PanTiltSpeedSpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");

    QSoapElement psXRangeEle;
    psXRangeEle.tagName="tt:XRange";
    psXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    psXRangeEle.AppendChild("tt:Min","-1");
    psXRangeEle.AppendChild("tt:Max","1");
    PanTiltSpeedSpaceEle.AppendChild(psXRangeEle);
    SupportedPTZSpacesEle.AppendChild(PanTiltSpeedSpaceEle);

    QSoapElement ZoomSpeedSpaceEle;
    ZoomSpeedSpaceEle.tagName="tt:ZoomSpeedSpace";
    ZoomSpeedSpaceEle.Attributes().insert("xsi:type","tt:Space1DDescription");
    ZoomSpeedSpaceEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");

    QSoapElement zsXRangeEle;
    zsXRangeEle.tagName="tt:XRange";
    zsXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
    zsXRangeEle.AppendChild("tt:Min","-1");
    zsXRangeEle.AppendChild("tt:Max","1");
    ZoomSpeedSpaceEle.AppendChild(zsXRangeEle);
    SupportedPTZSpacesEle.AppendChild(ZoomSpeedSpaceEle);

    PTZNodeEle.AppendChild(SupportedPTZSpacesEle);

    PTZNodeEle.AppendChild("MaximumNumberOfPresets","80");
    PTZNodeEle.AppendChild("HomeSupported","true");

    QSoapElement ExtensionEle;
    ExtensionEle.tagName="tt:Extension";
    QSoapElement SupportedPresetTourEle;
    SupportedPresetTourEle.tagName="tt:SupportedPresetTour";
    SupportedPresetTourEle.AppendChild("MaximumNumberOfPresetTours","8");
    SupportedPresetTourEle.AppendChild("PTZPresetTourOperation","Start");
    SupportedPresetTourEle.AppendChild("PTZPresetTourOperation","Stop");
    SupportedPresetTourEle.AppendChild("PTZPresetTourOperation","Pause");
    ExtensionEle.AppendChild(SupportedPresetTourEle);

    PTZNodeEle.AppendChild(ExtensionEle);


    ddElement.AppendChild(PTZNodeEle);
    resp.envelope.body.AppendChild(ddElement);

    return resp;
}
