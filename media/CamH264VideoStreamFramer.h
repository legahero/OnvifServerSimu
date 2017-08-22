#include "H264VideoStreamFramer.hh"


//*********************************************************************
// 获取摄像头视频YUV数据后，编码成H264，并推送给DSS
class H264EncWrapper;
struct TNAL;

class CamH264VideoStreamFramer : public H264VideoStreamFramer
{
public:
	virtual ~CamH264VideoStreamFramer();
	CamH264VideoStreamFramer(UsageEnvironment& env,
		FramedSource* inputSource, H264EncWrapper* pH264Enc);

	static CamH264VideoStreamFramer* createNew(UsageEnvironment& env, FramedSource* inputSource);
	virtual Boolean currentNALUnitEndsAccessUnit();
	virtual void doGetNextFrame();

private:
	H264EncWrapper* m_pH264Enc;

	TNAL* m_pNalArray;
	int m_iCurNalNum;		//当前Frame一共有多少个NAL
	int m_iCurNal;		//当前使用的是第几个NAL
	unsigned int m_iCurFrame;
};
