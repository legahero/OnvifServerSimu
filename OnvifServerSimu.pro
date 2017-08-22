#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T11:26:50
#
#-------------------------------------------------

QT       += core gui network sql xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OnvifServerSimu
TEMPLATE = app

INCLUDEPATH +=./lib/ffmpeg/include/
#INCLUDEPATH +=./Live555/
INCLUDEPATH +=./Live555/liveMedia/include/
INCLUDEPATH +=./Live555/UsageEnvironment/include/
INCLUDEPATH +=./Live555/BasicUsageEnvironment/include/
INCLUDEPATH +=./Live555/groupsock/include/


#LIBS += -lws2_32 libwsock32 libntdll libpowrprof
LIBS += -lpthread libwsock32 libws2_32 libntdll libpowrprof libwinmm

#this path need modify
LIBS += D:/git/OnvifServerSimu/lib/libx264.lib
LIBS += -LD:/git/OnvifServerSimu/lib/ffmpeg/lib/ -lavdevice -lavcodec -lavformat -lavutil -lswscale -lpostproc -lavfilter



SOURCES += main.cpp\
        mainwindow.cpp \
    httpbase/qhttprequest.cpp \
    httpbase/qhttpresponse.cpp \
    httpbase/qhttpserver.cpp \
    httpbase/http_parser.c \    
    httpbase/threadhandle.cpp \
    httpbase/qasyntcpserver.cpp \
    httpbase/qasynhttpsocket.cpp \
    httpbase/staticfilecontroller.cpp \
    httpbase/httphandler.cpp \
    httpbase/qasyntcpsocket.cpp \
    httpbase/qconnectpool.cpp \
    httpbase/qmultidbmanager.cpp \
    soap/qsoapenvelope.cpp \
    soap/qsoapmessage.cpp \
    soap/qsoapheader.cpp \
    soap/qsoapbody.cpp \
    soap/qsoapfault.cpp \
    soap/qsoapelement.cpp \
    soap/qsoapdiscoveryservice.cpp \    
    soap/qsoapserver.cpp \
    soap/qonvifdevice.cpp \
    soap/qonvifmedia.cpp \   
    media/qmediaserver.cpp \
    media/CamH264VideoStreamFramer.cpp \
    media/H264EndWrapper.cpp \
    media/H264LiveVideoServerMediaSubsession.cpp \
    Live555/BasicUsageEnvironment/BasicHashTable.cpp \
    Live555/BasicUsageEnvironment/BasicTaskScheduler.cpp \
    Live555/BasicUsageEnvironment/BasicTaskScheduler0.cpp \
    Live555/BasicUsageEnvironment/BasicUsageEnvironment.cpp \
    Live555/BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
    Live555/BasicUsageEnvironment/DelayQueue.cpp \
    Live555/groupsock/GroupEId.cpp \
    Live555/groupsock/Groupsock.cpp \
    Live555/groupsock/GroupsockHelper.cpp \
    Live555/groupsock/IOHandlers.cpp \
    Live555/groupsock/NetAddress.cpp \
    Live555/groupsock/NetInterface.cpp \
    Live555/groupsock/inet.c \
    Live555/liveMedia/AC3AudioFileServerMediaSubsession.cpp \
    Live555/liveMedia/AC3AudioRTPSink.cpp \
    Live555/liveMedia/AC3AudioRTPSource.cpp \
    Live555/liveMedia/AC3AudioStreamFramer.cpp \
    Live555/liveMedia/ADTSAudioFileServerMediaSubsession.cpp \
    Live555/liveMedia/ADTSAudioFileSource.cpp \
    Live555/liveMedia/AMRAudioFileServerMediaSubsession.cpp \
    Live555/liveMedia/AMRAudioFileSink.cpp \
    Live555/liveMedia/AMRAudioFileSource.cpp \
    Live555/liveMedia/AMRAudioRTPSink.cpp \
    Live555/liveMedia/AMRAudioRTPSource.cpp \
    Live555/liveMedia/AMRAudioSource.cpp \
    Live555/liveMedia/AudioInputDevice.cpp \
    Live555/liveMedia/AudioRTPSink.cpp \
    Live555/liveMedia/AVIFileSink.cpp \
    Live555/liveMedia/Base64.cpp \
    Live555/liveMedia/BasicUDPSink.cpp \
    Live555/liveMedia/BasicUDPSource.cpp \
    Live555/liveMedia/BitVector.cpp \
    Live555/liveMedia/ByteStreamFileSource.cpp \
    Live555/liveMedia/ByteStreamMemoryBufferSource.cpp \
    Live555/liveMedia/ByteStreamMultiFileSource.cpp \
    Live555/liveMedia/DeviceSource.cpp \
    Live555/liveMedia/DigestAuthentication.cpp \
    Live555/liveMedia/DVVideoFileServerMediaSubsession.cpp \
    Live555/liveMedia/DVVideoRTPSink.cpp \
    Live555/liveMedia/DVVideoRTPSource.cpp \
    Live555/liveMedia/DVVideoStreamFramer.cpp \
    Live555/liveMedia/EBMLNumber.cpp \
    Live555/liveMedia/FileServerMediaSubsession.cpp \
    Live555/liveMedia/FileSink.cpp \
    Live555/liveMedia/FramedFileSource.cpp \
    Live555/liveMedia/FramedFilter.cpp \
    Live555/liveMedia/FramedSource.cpp \
    Live555/liveMedia/GenericMediaServer.cpp \
    Live555/liveMedia/GSMAudioRTPSink.cpp \
    Live555/liveMedia/H261VideoRTPSource.cpp \
    Live555/liveMedia/H263plusVideoFileServerMediaSubsession.cpp \
    Live555/liveMedia/H263plusVideoRTPSink.cpp \
    Live555/liveMedia/H263plusVideoRTPSource.cpp \
    Live555/liveMedia/H263plusVideoStreamFramer.cpp \
    Live555/liveMedia/H263plusVideoStreamParser.cpp \
    Live555/liveMedia/H264or5VideoFileSink.cpp \
    Live555/liveMedia/H264or5VideoRTPSink.cpp \
    Live555/liveMedia/H264or5VideoStreamDiscreteFramer.cpp \
    Live555/liveMedia/H264or5VideoStreamFramer.cpp \
    Live555/liveMedia/H264VideoFileServerMediaSubsession.cpp \
    Live555/liveMedia/H264VideoFileSink.cpp \
    Live555/liveMedia/H264VideoRTPSink.cpp \
    Live555/liveMedia/H264VideoRTPSource.cpp \
    Live555/liveMedia/H264VideoStreamDiscreteFramer.cpp \
    Live555/liveMedia/H264VideoStreamFramer.cpp \
    Live555/liveMedia/H265VideoFileServerMediaSubsession.cpp \
    Live555/liveMedia/H265VideoFileSink.cpp \
    Live555/liveMedia/H265VideoRTPSink.cpp \
    Live555/liveMedia/H265VideoRTPSource.cpp \
    Live555/liveMedia/H265VideoStreamDiscreteFramer.cpp \
    Live555/liveMedia/H265VideoStreamFramer.cpp \
    Live555/liveMedia/InputFile.cpp \
    Live555/liveMedia/JPEGVideoRTPSink.cpp \
    Live555/liveMedia/JPEGVideoRTPSource.cpp \
    Live555/liveMedia/JPEGVideoSource.cpp \
    Live555/liveMedia/Locale.cpp \
    Live555/liveMedia/MatroskaDemuxedTrack.cpp \
    Live555/liveMedia/MatroskaFile.cpp \
    Live555/liveMedia/MatroskaFileParser.cpp \
    Live555/liveMedia/MatroskaFileServerDemux.cpp \
    Live555/liveMedia/MatroskaFileServerMediaSubsession.cpp \
    Live555/liveMedia/Media.cpp \
    Live555/liveMedia/MediaSession.cpp \
    Live555/liveMedia/MediaSink.cpp \
    Live555/liveMedia/MediaSource.cpp \
    Live555/liveMedia/MP3ADU.cpp \
    Live555/liveMedia/MP3ADUdescriptor.cpp \
    Live555/liveMedia/MP3ADUinterleaving.cpp \
    Live555/liveMedia/MP3ADURTPSink.cpp \
    Live555/liveMedia/MP3ADURTPSource.cpp \
    Live555/liveMedia/MP3ADUTranscoder.cpp \
    Live555/liveMedia/MP3AudioFileServerMediaSubsession.cpp \
    Live555/liveMedia/MP3AudioMatroskaFileServerMediaSubsession.cpp \
    Live555/liveMedia/MP3FileSource.cpp \
    Live555/liveMedia/MP3Internals.cpp \
    Live555/liveMedia/MP3InternalsHuffman.cpp \
    Live555/liveMedia/MP3InternalsHuffmanTable.cpp \
    Live555/liveMedia/MP3StreamState.cpp \
    Live555/liveMedia/MP3Transcoder.cpp \
    Live555/liveMedia/MPEG1or2AudioRTPSink.cpp \
    Live555/liveMedia/MPEG1or2AudioRTPSource.cpp \
    Live555/liveMedia/MPEG1or2AudioStreamFramer.cpp \
    Live555/liveMedia/MPEG1or2Demux.cpp \
    Live555/liveMedia/MPEG1or2DemuxedElementaryStream.cpp \
    Live555/liveMedia/MPEG1or2DemuxedServerMediaSubsession.cpp \
    Live555/liveMedia/MPEG1or2FileServerDemux.cpp \
    Live555/liveMedia/MPEG1or2VideoFileServerMediaSubsession.cpp \
    Live555/liveMedia/MPEG1or2VideoRTPSink.cpp \
    Live555/liveMedia/MPEG1or2VideoRTPSource.cpp \
    Live555/liveMedia/MPEG1or2VideoStreamDiscreteFramer.cpp \
    Live555/liveMedia/MPEG1or2VideoStreamFramer.cpp \
    Live555/liveMedia/MPEG2IndexFromTransportStream.cpp \
    Live555/liveMedia/MPEG2TransportFileServerMediaSubsession.cpp \
    Live555/liveMedia/MPEG2TransportStreamAccumulator.cpp \
    Live555/liveMedia/MPEG2TransportStreamFramer.cpp \
    Live555/liveMedia/MPEG2TransportStreamFromESSource.cpp \
    Live555/liveMedia/MPEG2TransportStreamFromPESSource.cpp \
    Live555/liveMedia/MPEG2TransportStreamIndexFile.cpp \
    Live555/liveMedia/MPEG2TransportStreamMultiplexor.cpp \
    Live555/liveMedia/MPEG2TransportStreamTrickModeFilter.cpp \
    Live555/liveMedia/MPEG2TransportUDPServerMediaSubsession.cpp \
    Live555/liveMedia/MPEG4ESVideoRTPSink.cpp \
    Live555/liveMedia/MPEG4ESVideoRTPSource.cpp \
    Live555/liveMedia/MPEG4GenericRTPSink.cpp \
    Live555/liveMedia/MPEG4GenericRTPSource.cpp \
    Live555/liveMedia/MPEG4LATMAudioRTPSink.cpp \
    Live555/liveMedia/MPEG4LATMAudioRTPSource.cpp \
    Live555/liveMedia/MPEG4VideoFileServerMediaSubsession.cpp \
    Live555/liveMedia/MPEG4VideoStreamDiscreteFramer.cpp \
    Live555/liveMedia/MPEG4VideoStreamFramer.cpp \
    Live555/liveMedia/MPEGVideoStreamFramer.cpp \
    Live555/liveMedia/MPEGVideoStreamParser.cpp \
    Live555/liveMedia/MultiFramedRTPSink.cpp \
    Live555/liveMedia/MultiFramedRTPSource.cpp \
    Live555/liveMedia/OggDemuxedTrack.cpp \
    Live555/liveMedia/OggFile.cpp \
    Live555/liveMedia/OggFileParser.cpp \
    Live555/liveMedia/OggFileServerDemux.cpp \
    Live555/liveMedia/OggFileServerMediaSubsession.cpp \
    Live555/liveMedia/OggFileSink.cpp \
    Live555/liveMedia/OnDemandServerMediaSubsession.cpp \
    Live555/liveMedia/ourMD5.cpp \
    Live555/liveMedia/OutputFile.cpp \
    Live555/liveMedia/PassiveServerMediaSubsession.cpp \
    Live555/liveMedia/ProxyServerMediaSession.cpp \
    Live555/liveMedia/QCELPAudioRTPSource.cpp \
    Live555/liveMedia/QuickTimeFileSink.cpp \
    Live555/liveMedia/QuickTimeGenericRTPSource.cpp \
    Live555/liveMedia/RTCP.cpp \
    Live555/liveMedia/RTPInterface.cpp \
    Live555/liveMedia/RTPSink.cpp \
    Live555/liveMedia/RTPSource.cpp \
    Live555/liveMedia/RTSPClient.cpp \
    Live555/liveMedia/RTSPCommon.cpp \
    Live555/liveMedia/RTSPRegisterSender.cpp \
    Live555/liveMedia/RTSPServer.cpp \
    Live555/liveMedia/RTSPServerRegister.cpp \
    Live555/liveMedia/RTSPServerSupportingHTTPStreaming.cpp \
    Live555/liveMedia/ServerMediaSession.cpp \
    Live555/liveMedia/SimpleRTPSink.cpp \
    Live555/liveMedia/SimpleRTPSource.cpp \
    Live555/liveMedia/SIPClient.cpp \
    Live555/liveMedia/StreamParser.cpp \
    Live555/liveMedia/StreamReplicator.cpp \
    Live555/liveMedia/T140TextRTPSink.cpp \
    Live555/liveMedia/TCPStreamSink.cpp \
    Live555/liveMedia/TextRTPSink.cpp \
    Live555/liveMedia/TheoraVideoRTPSink.cpp \
    Live555/liveMedia/TheoraVideoRTPSource.cpp \
    Live555/liveMedia/uLawAudioFilter.cpp \
    Live555/liveMedia/VideoRTPSink.cpp \
    Live555/liveMedia/VorbisAudioRTPSink.cpp \
    Live555/liveMedia/VorbisAudioRTPSource.cpp \
    Live555/liveMedia/VP8VideoRTPSink.cpp \
    Live555/liveMedia/VP8VideoRTPSource.cpp \
    Live555/liveMedia/VP9VideoRTPSink.cpp \
    Live555/liveMedia/VP9VideoRTPSource.cpp \
    Live555/liveMedia/WAVAudioFileServerMediaSubsession.cpp \
    Live555/liveMedia/WAVAudioFileSource.cpp \
    Live555/liveMedia/rtcp_from_spec.c \
    Live555/UsageEnvironment/HashTable.cpp \
    Live555/UsageEnvironment/strDup.cpp \
    Live555/UsageEnvironment/UsageEnvironment.cpp \
    soap/qonvifanalyticsdevice.cpp \
    soap/qonvifptz.cpp




HEADERS  += mainwindow.h \
    httpbase/http_parser.h \
    httpbase/qhttprequest.h \
    httpbase/qhttpresponse.h \
    httpbase/threadhandle.h \
    httpbase/qasyntcpserver.h \
    httpbase/qasynhttpsocket.h \
    httpbase/qhttpserver.h \
    httpbase/staticfilecontroller.h \
    httpbase/httphandler.h \
    httpbase/qasyntcpsocket.h \
    httpbase/qhttpserverfwd.h \
    httpbase/qconnectpool.h \
    httpbase/qmultidbmanager.h \
    soap/qsoapenvelope.h \
    soap/qsoapmessage.h \
    soap/qsoapheader.h \
    soap/qsoapbody.h \
    soap/qsoapfault.h \
    soap/qsoapelement.h \
    soap/qsoapdiscoveryservice.h \    
    soap/qsoapserver.h \
    soap/qonvifdevice.h \
    soap/qonvifmedia.h \
    media/qmediaserver.h \
    media/CamH264VideoStreamFramer.h \
    media/H264EndWrapper.h \
    media/H264LiveVideoServerMediaSubsession.h \
    Live555/liveMedia/EBMLNumber.hh \
    Live555/liveMedia/H263plusVideoStreamParser.hh \
    Live555/liveMedia/MatroskaDemuxedTrack.hh \
    Live555/liveMedia/MatroskaFileParser.hh \
    Live555/liveMedia/MatroskaFileServerMediaSubsession.hh \
    Live555/liveMedia/MP3ADUdescriptor.hh \
    Live555/liveMedia/MP3AudioMatroskaFileServerMediaSubsession.hh \
    Live555/liveMedia/MP3Internals.hh \
    Live555/liveMedia/MP3InternalsHuffman.hh \
    Live555/liveMedia/MP3StreamState.hh \
    Live555/liveMedia/MPEGVideoStreamParser.hh \
    Live555/liveMedia/OggDemuxedTrack.hh \
    Live555/liveMedia/OggFileParser.hh \
    Live555/liveMedia/OggFileServerMediaSubsession.hh \
    Live555/liveMedia/rtcp_from_spec.h \
    Live555/liveMedia/StreamParser.hh \
    soap/qonvifanalyticsdevice.h \
    soap/qonvifptz.h



FORMS    += mainwindow.ui

unix:include($$PWD/eventdispatcher_libev/eventdispatcher_libev.pri)


