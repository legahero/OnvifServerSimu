#include "qonvifmedia.h"

QOnvifMedia::QOnvifMedia()
{

}

QSoapMessage QOnvifMedia::OnGetProfiles(QSoapMessage& req)
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

    QSoapElement mElement;
    mElement.tagName="m:GetProfilesResponse";

    for(int i=1;i<3;i++)
    {
        QString sNo=QString::number(i);
        QSoapElement ProfilesEle;
        ProfilesEle.tagName="m:Profiles";
        //ProfilesEle.Attributes().insert("xsi:type","tt:Profile");
        ProfilesEle.Attributes().insert("token","profile"+sNo);
        ProfilesEle.Attributes().insert("fixed","true");
        ProfilesEle.AppendChild("tt:Name","Profile"+sNo);

        QSoapElement VideoSourceConfigurationEle;
        VideoSourceConfigurationEle.tagName="tt:VideoSourceConfiguration";
        //VideoSourceConfigurationEle.Attributes().insert("xsi:type","tt:VideoSourceConfiguration");
        VideoSourceConfigurationEle.Attributes().insert("token","video_source_config"+sNo);
        VideoSourceConfigurationEle.AppendChild("tt:Name","video_source_config"+sNo);
        VideoSourceConfigurationEle.AppendChild("tt:UseCount","1");
        VideoSourceConfigurationEle.AppendChild("tt:SourceToken","1");
        QSoapElement BoundsEle;
        BoundsEle.tagName="tt:Bounds";
        BoundsEle.Attributes().insert("height","720");
        BoundsEle.Attributes().insert("width","1024");
        BoundsEle.Attributes().insert("y","1");
        BoundsEle.Attributes().insert("x","1");
        BoundsEle.Attributes().insert("xsi:type","tt:IntRectangle");
        VideoSourceConfigurationEle.AppendChild(BoundsEle);
        ProfilesEle.AppendChild(VideoSourceConfigurationEle);

        QSoapElement VideoEncoderConfigurationEle;
        VideoEncoderConfigurationEle.tagName="tt:VideoEncoderConfiguration";
        //VideoEncoderConfigurationEle.Attributes().insert("xsi:type","tt:VideoEncoderConfiguration");
        VideoEncoderConfigurationEle.Attributes().insert("token","video_encoder_config"+sNo);
        VideoEncoderConfigurationEle.AppendChild("tt:Name","video_encoder_config"+sNo);
        VideoEncoderConfigurationEle.AppendChild("tt:UseCount","1");
        VideoEncoderConfigurationEle.AppendChild("tt:Encoding","H264");
        QSoapElement ResolutionEle;
        ResolutionEle.tagName="tt:Resolution";
        ResolutionEle.Attributes().insert("xsi:type","tt:VideoResolution");
        ResolutionEle.AppendChild("tt:Width","1024");
        ResolutionEle.AppendChild("tt:Height","720");
        VideoEncoderConfigurationEle.AppendChild(ResolutionEle);
        VideoEncoderConfigurationEle.AppendChild("tt:Quality","7");
        QSoapElement RateControlEle;
        RateControlEle.tagName="tt:RateControl";
        RateControlEle.Attributes().insert("xsi:type","tt:VideoRateControl");
        RateControlEle.AppendChild("tt:FrameRateLimit","30");
        RateControlEle.AppendChild("tt:EncodingInterval","0");
        RateControlEle.AppendChild("tt:BitrateLimit","2048");
        VideoEncoderConfigurationEle.AppendChild(RateControlEle);
        QSoapElement H264Ele;
        H264Ele.tagName="tt:H264";
        H264Ele.Attributes().insert("xsi:type","tt:H264Configuration");
        H264Ele.AppendChild("tt:GovLength","30");
        H264Ele.AppendChild("tt:H264Profile","Baseline");
        VideoEncoderConfigurationEle.AppendChild(H264Ele);

        QSoapElement MulticastEle;
        MulticastEle.tagName="tt:Multicast";
        MulticastEle.Attributes().insert("xsi:type","tt:MulticastConfiguration");
        QSoapElement AddressEle;
        AddressEle.tagName="tt:Address";
        AddressEle.Attributes().insert("xsi:type","tt:IPAddress");
        AddressEle.AppendChild("tt:Type","IPv4");
        AddressEle.AppendChild("tt:IPv4Address","0.0.0.0");
        MulticastEle.AppendChild(AddressEle);
        MulticastEle.AppendChild("tt:Port","0");
        MulticastEle.AppendChild("tt:TTL","3");
        MulticastEle.AppendChild("tt:AutoStart","false");
        VideoEncoderConfigurationEle.AppendChild(MulticastEle);

        VideoEncoderConfigurationEle.AppendChild("tt:SessionTimeout","PT0H0M1S");
        ProfilesEle.AppendChild(VideoEncoderConfigurationEle);

        QSoapElement PTZConfigurationEle;
        PTZConfigurationEle.tagName="tt:PTZConfiguration";
        //PTZConfigurationEle.Attributes().insert("xsi:type","tt:PTZConfiguration");
        PTZConfigurationEle.Attributes().insert("token","ptz"+sNo);
        PTZConfigurationEle.AppendChild("tt:Name","PTZConfig_Channel"+sNo);
        PTZConfigurationEle.AppendChild("tt:UseCount","2");
        PTZConfigurationEle.AppendChild("tt:NodeToken","");
        PTZConfigurationEle.AppendChild("tt:DefaultAbsolutePantTiltPositionSpace","http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
        PTZConfigurationEle.AppendChild("tt:DefaultAbsoluteZoomPositionSpace","http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
        PTZConfigurationEle.AppendChild("tt:DefaultRelativePanTiltTranslationSpace","http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
        PTZConfigurationEle.AppendChild("tt:DefaultRelativeZoomTranslationSpace","http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
        PTZConfigurationEle.AppendChild("tt:DefaultContinuousPanTiltVelocitySpace","http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
        PTZConfigurationEle.AppendChild("tt:DefaultContinuousZoomVelocitySpace","http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");
        QSoapElement DefaultPTZSpeedEle;
        DefaultPTZSpeedEle.tagName="tt:DefaultPTZSpeed";
        DefaultPTZSpeedEle.Attributes().insert("xsi:type","tt:PTZSpeed");
        QSoapElement PanTiltEle;
        PanTiltEle.tagName="tt:PanTilt";
        PanTiltEle.Attributes().insert("space","http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
        PanTiltEle.Attributes().insert("y","0.800000012");
        PanTiltEle.Attributes().insert("x","0.800000012");
        PanTiltEle.Attributes().insert("xsi:type","tt:Vector2D");
        DefaultPTZSpeedEle.AppendChild(PanTiltEle);
        QSoapElement ZoomEle;
        ZoomEle.tagName="tt:Zoom";
        ZoomEle.Attributes().insert("space","http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
        ZoomEle.Attributes().insert("x","0");
        ZoomEle.Attributes().insert("xsi:type","tt:Vector1D");
        DefaultPTZSpeedEle.AppendChild(ZoomEle);
        PTZConfigurationEle.AppendChild(DefaultPTZSpeedEle);
        QSoapElement PanTiltLimitsEle;
        PanTiltLimitsEle.tagName="tt:PanTiltLimits";
        PanTiltLimitsEle.Attributes().insert("xsi:type","tt:PanTiltLimits");
        QSoapElement RangeEle;
        RangeEle.tagName="tt:Range";
        RangeEle.Attributes().insert("xsi:type","tt:Space2DDescription");
        RangeEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
        QSoapElement XRangeEle;
        XRangeEle.tagName="tt:XRange";
        XRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
        XRangeEle.AppendChild("tt:Min","-1");
        XRangeEle.AppendChild("tt:Max","1");
        RangeEle.AppendChild(XRangeEle);
        QSoapElement YRangeEle;
        YRangeEle.tagName="tt:YRange";
        YRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
        YRangeEle.AppendChild("tt:Min","-1");
        YRangeEle.AppendChild("tt:Max","1");
        RangeEle.AppendChild(YRangeEle);
        PanTiltLimitsEle.AppendChild(RangeEle);
        PTZConfigurationEle.AppendChild(PanTiltLimitsEle);

        QSoapElement ZoomLimitsEle;
        ZoomLimitsEle.tagName="tt:ZoomLimits";
        ZoomLimitsEle.Attributes().insert("xsi:type","tt:ZoomLimits");
        QSoapElement zRangeEle;
        zRangeEle.tagName="tt:Range";
        zRangeEle.Attributes().insert("xsi:type","tt:Space1DDescription");
        zRangeEle.AppendChild("tt:URI","http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
        QSoapElement zXRangeEle;
        zXRangeEle.tagName="tt:XRange";
        zXRangeEle.Attributes().insert("xsi:type","tt:FloatRange");
        zXRangeEle.AppendChild("tt:Min","0");
        zXRangeEle.AppendChild("tt:Max","1");
        zRangeEle.AppendChild(zXRangeEle);
        ZoomLimitsEle.AppendChild(zRangeEle);
        PTZConfigurationEle.AppendChild(ZoomLimitsEle);

        ProfilesEle.AppendChild(PTZConfigurationEle);

        mElement.AppendChild(ProfilesEle);
    }
    resp.envelope.body.AppendChild(mElement);

    return resp;
}

QSoapMessage QOnvifMedia::packGetStreamUri(QString Stream,QString Protocol,QString ProfileToken)
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

    QSoapElement ddElement;
    ddElement.tagName="m:GetStreamUri";

    QSoapElement StreamSetup;
    StreamSetup.tagName="m:StreamSetup";

    StreamSetup.AppendChild("tt:Stream",Stream);//"RTP-Unicast"
    QSoapElement TransportEle;
    TransportEle.tagName="tt:Transport";
    TransportEle.AppendChild("tt:Protocol",Protocol);//"UDP"
    StreamSetup.AppendChild(TransportEle);

    StreamSetup.AppendChild("tt:ProfileToken",ProfileToken);

    ddElement.AppendChild(StreamSetup);

    resp.envelope.body.AppendChild(ddElement);

    return resp;


}

QSoapMessage QOnvifMedia::OnGetStreamUri(QSoapMessage& req)
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

    QSoapElement ddElement;
    ddElement.tagName="m:GetStreamUriResponse";

    QSoapElement MediaUriElement;
    MediaUriElement.tagName="m:MediaUri";
    MediaUriElement.Attributes().insert("xsi:type","tt:MediaUri");
    MediaUriElement.AppendChild("tt:Uri",StreamUri);
    MediaUriElement.AppendChild("tt:InvalidAfterConnect","true");
    MediaUriElement.AppendChild("tt:InvalidAfterReboot","true");
    MediaUriElement.AppendChild("tt:Timeout","PT0H0M10S");
    ddElement.AppendChild(MediaUriElement);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

/*
 * <m:GetSnapshotUriResponse>
       <m:MediaUri>
        <tt:Uri>http://192.168.31.144:81/snapshot.cgi</tt:Uri>
        <tt:InvalidAfterConnect>false</tt:InvalidAfterConnect>
        <tt:InvalidAfterReboot>false</tt:InvalidAfterReboot>
        <tt:Timeout>PT5S</tt:Timeout>
      </m:MediaUri>
    </m:GetSnapshotUriResponse>
 * */
QSoapMessage QOnvifMedia::OnGetSnapshotUri(QSoapMessage& req)
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

    QSoapElement ddElement;
    ddElement.tagName="m:GetSnapshotUriResponse";

    QSoapElement MediaUriElement;
    MediaUriElement.tagName="m:MediaUri";
    MediaUriElement.Attributes().insert("xsi:type","tt:MediaUri");
    MediaUriElement.AppendChild("tt:Uri",SnapshotUri);
    MediaUriElement.AppendChild("tt:InvalidAfterConnect","false");
    MediaUriElement.AppendChild("tt:InvalidAfterReboot","false");
    MediaUriElement.AppendChild("tt:Timeout","PT0H0M10S");//10秒，PT5S：5秒
    ddElement.AppendChild(MediaUriElement);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

QSoapMessage QOnvifMedia::OnGetVideoEncoderConfigurations(QSoapMessage& req)
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

    QSoapElement ddElement;
    ddElement.tagName="m:GetVideoEncoderConfigurationsResponse";

    QSoapElement OptionsElement;
    OptionsElement.tagName="m:Options";
    //OptionsElement.Attributes().insert("xsi:type","tt:MediaUri");
    //OptionsElement.AppendChild("tt:Uri",Uri1);
    //OptionsElement.AppendChild("tt:InvalidAfterConnect","true");
    //OptionsElement.AppendChild("tt:InvalidAfterReboot","true");
    //OptionsElement.AppendChild("tt:Timeout","PT0H0M10S");
    ddElement.AppendChild(OptionsElement);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}

/*请求：<s:Body xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
    <GetVideoEncoderConfigurationOptions xmlns="http://www.onvif.org/ver10/media/wsdl">
      <ConfigurationToken>video_encoder_config</ConfigurationToken>
      <ProfileToken>profile1</ProfileToken>
    </GetVideoEncoderConfigurationOptions>
  </s:Body>
  应答：
 * */
QSoapMessage QOnvifMedia::OnGetVideoEncoderConfigurationOptions(QSoapMessage& req)
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


    QSoapElement ddElement;
    ddElement.tagName="m:GetVideoEncoderConfigurationOptionsResponse";

    QSoapElement VideoEncoderConfigurationEle;
    VideoEncoderConfigurationEle.tagName="tt:VideoEncoderConfiguration";
    VideoEncoderConfigurationEle.Attributes().insert("token","video_encoder_config");
    VideoEncoderConfigurationEle.AppendChild("tt:Name","video_encoder_config");
    VideoEncoderConfigurationEle.AppendChild("tt:UseCount","1");
    VideoEncoderConfigurationEle.AppendChild("tt:Encoding","H264");
    QSoapElement ResolutionEle;
    ResolutionEle.tagName="tt:Resolution";
    ResolutionEle.Attributes().insert("xsi:type","tt:VideoResolution");
    ResolutionEle.AppendChild("tt:width","1024");
    ResolutionEle.AppendChild("tt:Height","720");
    VideoEncoderConfigurationEle.AppendChild(ResolutionEle);
    VideoEncoderConfigurationEle.AppendChild("tt:Quality","7");
    QSoapElement RateControlEle;
    RateControlEle.tagName="tt:RateControl";
    RateControlEle.Attributes().insert("xsi:type","tt:VideoRateControl");
    RateControlEle.AppendChild("tt:FrameRateLimit","30");
    RateControlEle.AppendChild("tt:EncodingInterval","0");
    RateControlEle.AppendChild("tt:BitrateLimit","2048");
    VideoEncoderConfigurationEle.AppendChild(RateControlEle);
    QSoapElement H264Ele;
    H264Ele.tagName="tt:H264";
    H264Ele.Attributes().insert("xsi:type","tt:H264Configuration");
    H264Ele.AppendChild("tt:GovLength","30");
    H264Ele.AppendChild("tt:H264Profile","Baseline");
    VideoEncoderConfigurationEle.AppendChild(H264Ele);

    ddElement.AppendChild(VideoEncoderConfigurationEle);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}
/*请求：<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:trt="http://www.onvif.org/ver10/media/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <soap:Body>
    <trt:GetVideoSources />
  </soap:Body>
</soap:Envelope>
应答：<SOAP-ENV:Body>
    <mediaws:GetVideoSourcesResponse>
      <mediaws:VideoSources xsi:type="tt:VideoSource" token="">
        <tt:Framerate>25</tt:Framerate>
        <tt:Resolution xsi:type="tt:VideoResolution">
          <tt:Width>1080</tt:Width>
          <tt:Height>720</tt:Height>
        </tt:Resolution>
      </mediaws:VideoSources>
    </mediaws:GetVideoSourcesResponse>
  </SOAP-ENV:Body>
 * */
//最重要的是token的填充，必须要和下面profile中的sourcetoken相同，需要匹配到这个视频源
QSoapMessage QOnvifMedia::OnGetVideoSources(QSoapMessage& req)
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


    QSoapElement ddElement;
    ddElement.tagName="m:GetVideoSourcesResponse";

    QSoapElement VideoSourcesEle;
    VideoSourcesEle.tagName="m:VideoSources";
    VideoSourcesEle.Attributes().insert("token","");
    VideoSourcesEle.Attributes().insert("xsi:type","tt:VideoSource");
    VideoSourcesEle.AppendChild("tt:Framerate","25");

    QSoapElement ResolutionEle;
    ResolutionEle.tagName="tt:Resolution";
    ResolutionEle.Attributes().insert("xsi:type","tt:VideoResolution");
    ResolutionEle.AppendChild("tt:width","1024");
    ResolutionEle.AppendChild("tt:Height","720");
    VideoSourcesEle.AppendChild(ResolutionEle);

    ddElement.AppendChild(VideoSourcesEle);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}
/*
AudioSources - optional, unbounded; [AudioSource]
List of existing Audio Sources
token - required; [ReferenceToken]
Unique identifier referencing the physical entity.
Channels [int]
number of available audio channels. (1: mono, 2: stereo)
 * */
QSoapMessage QOnvifMedia::OnGetAudioSources(QSoapMessage& req)
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


    QSoapElement ddElement;
    ddElement.tagName="m:GetAudioSourcesResponse";

    QSoapElement AudioSourcesEle;
    AudioSourcesEle.tagName="m:AudioSources";
    AudioSourcesEle.Attributes().insert("token","Audio1");
    AudioSourcesEle.Attributes().insert("xsi:type","tt:AudioSource");
    AudioSourcesEle.AppendChild("tt:Channels","1");//1: mono, 2: stereo
    ddElement.AppendChild(AudioSourcesEle);

    resp.envelope.body.AppendChild(ddElement);

    return resp;
}
