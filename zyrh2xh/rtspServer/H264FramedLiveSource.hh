#ifndef _H264FRAMEDLIVESOURCE_HH
#define _H264FRAMEDLIVESOURCE_HH

#include <FramedSource.hh>
#include <iostream>
#include <stack>
#include "../devsdk/wmpclient.h"
#include "../devsdk/AnalyzeDataInterface.h"
#include <boost/asio/detail/mutex.hpp>

class SDKServerData
{
public:

	SDKServerData()
	{
		m_sSdkServerIp="";
		m_nSdkServerPort=0;
		m_spassword="";
		m_sUserName="";
		m_nServerLine=0;

		m_nDevLine=0;
		m_nnchannel=0;
		m_sDevId="";
	}

	SDKServerData(std::string sSdkServerIp,unsigned int nSdkServerPort,std::string sPassword,
						std::string sUserName,unsigned int nServerLine,unsigned int nDevLine,
						unsigned int nChannel,std::string sDevId)
	{
		m_sSdkServerIp=sSdkServerIp;
		m_nSdkServerPort=nSdkServerPort;
		m_spassword=sPassword;
		m_sUserName=sUserName;
		m_nServerLine=nServerLine;

		m_nDevLine=nDevLine;
		m_nnchannel=nChannel;
		m_sDevId=sDevId;
	};
public:	
	//sdk ��Ϣ
	std::string 	m_sSdkServerIp;
	unsigned int 	m_nSdkServerPort;
	std::string	 	m_spassword;
	std::string 	m_sUserName;
	unsigned int 	m_nServerLine;

	unsigned int 	m_nDevLine;
	unsigned int 	m_nnchannel;
	std::string 	m_sDevId;
	
};

#pragma pack(1)
	
union littel_endian_size
{
	unsigned short int	length;
	unsigned char		byte[2];
};

struct pack_start_code
{
	unsigned char start_code[3];
	unsigned char stream_id[1];
};

struct program_stream_pack_header
{
	pack_start_code PackStart;// 4
	unsigned char Buf[9];
	unsigned char stuffinglen;
};

struct program_stream_map
{
	pack_start_code PackStart;
	littel_endian_size PackLength;//we mast do exchange
	//program_stream_info_length
	//info
	//elementary_stream_map_length
	//elem
};

struct program_stream_e
{
	pack_start_code		PackStart;
	littel_endian_size	PackLength;//we mast do exchange
	char				PackInfo1[2];
	unsigned char		stuffing_length;
};

#pragma pack()

class H264FrameDeviceSource
{

public:

	H264FrameDeviceSource(SDKServerData nSdkServerData);
	bool GetVideoData(unsigned char *ptData,unsigned int &frameSize,unsigned int dataMaxSize);
	
	~H264FrameDeviceSource();
	
	//***********************zyrh begin******************************
	bool StartDev(char* sSdkServerIp,unsigned int nSdkServerPort,char* user,char* password,int nServerLine,char* sdevId,unsigned int nDevLine,unsigned int nchannel);
	bool StartDev();
	void StopDev();
	bool ReStartDev();
	bool InPutPsData(unsigned char* videoPsBuf,unsigned int  psBufsize,int nType );
	bool zyrh_AnalyzeDataGetPacketEx(char* h264OutBuf,unsigned int& nh264Size,unsigned int& TimeStamp,unsigned int& nDataType);
	//***********************zyrh end******************************	
	
public:
	int         m_stream_handle;
	//�豸�Ľ���ľ��
	int	 		m_nAnalyzeHandle;
	std::string m_strHead;
	char* 		m_buf;
	boost::asio::detail::mutex m_mutex;
	std::stack<std::string > m_h264Data;

private:
	WMP_HANDLE m_wmp_handle;
	SDKServerData m_SdkServerData;

};

class H264FramedLiveSource : public FramedSource
{
public:
	static H264FramedLiveSource* createNew(UsageEnvironment& env,H264FrameDeviceSource *ptH264FrameDeviceSource, unsigned preferredFrameSize = 0, unsigned playTimePerFrame = 0);
	void setDeviceSource(H264FrameDeviceSource *deviceSource)
	{
		m_ptH264FrameDeviceSource = deviceSource;
		m_ptH264FrameDeviceSource->StartDev();
	}

protected:
	H264FramedLiveSource(UsageEnvironment& env,H264FrameDeviceSource *ptH264FrameDeviceSource, unsigned preferredFrameSize, unsigned playTimePerFrame);
	~H264FramedLiveSource();
	

private:
	virtual void doGetNextFrame();
	
	int TransportData(unsigned char* to, unsigned maxSize);
	
	
protected:
	//int Framed_datasize;//��������Сָ��
	//unsigned char *Framed_databuf;//������ָ��
	//bool Framed_dosent;//���ͱ�ʾ

	//int readbufsize;//��¼�Ѷ�ȡ��������С
	//int bufsizel;//��¼��������С
	
public:
	H264FrameDeviceSource *m_ptH264FrameDeviceSource;

};

#endif
