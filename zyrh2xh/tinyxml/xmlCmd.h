#pragma  once
#include "xmlparser.h"
#include <WinSock2.h>
#define  REQUEST  "REQUEST"
#define  RESPONSE "RESPONSE"
#define  REGISTER  "REGISTER"  //�豸��ƽ̨CMSע��
#define  KEEPALIVE "KEEPALIVE" //�豸��CMS���ͱ���ģ�
#define  INVITESTREAM "INVITESTREAM" //CMS��PU����TCPȡ�������PU��������Ŀ��IP�Ͷ˿ڣ�Ȼ��������
#define  BYESTREAM    "BYESTREAM"

#define  MSG_START_VIDEO_REQ  "MSG_START_VIDEO_REQ" //��ʼʵʱ��Ƶ�ط�
#define  MSG_STOP_VIDEO_REQ   "MSG_STOP_VIDEO_REQ"
#define  MSG_START_VIDEO_RESP "MSG_START_VIDEO_RESP"
#define  MSG_STOP_VIDEO_RESP   "MSG_STOP_VIDEO_RESP"
#define  MSG_PTZ_REQ   "MSG_PTZ_REQ"
#define  MSG_PTZ_RESP   "MSG_PTZ_RESP"
#define  MSG_PU_HEART_RESP "MSG_PU_HEART_RESP"

//#define  MSG_PU_LOGOUT_REQ "MSG_PU_LOGOUT_REQ"
#define  MSG_PU_LOGOUT_RESP "MSG_PU_LOGOUT_RESP"

#define SYSTEM_NULL			0x0				// û��ϵͳ�㣬����Ƶ������Ƶ��	
#define SYSTEM_HIK          0x1				// �����ļ���
#define SYSTEM_MPEG2_PS     0x2				// PS��װ
#define SYSTEM_MPEG2_TS     0x3				// TS��װ
#define SYSTEM_RTP          0x4				// rtp��װ
#define SYSTEM_RTPHIK       0x401				// rtp��װ

// ��Ƶ��������
#define VIDEO_NULL          0x0 // û����Ƶ
#define VIDEO_H264          0x1 // ����H.264
#define VIDEO_MPEG4         0x3 // ��׼MPEG4
#define VIDEO_MJPEG			0x4
#define VIDEO_AVC264        0x0100
extern unsigned int g_nSequence;
 

struct HK_MEDIAINFO_				// 
{
	unsigned int    media_fourcc;			// "HKMI": 0x484B4D49 private Media Information
	unsigned short  media_version;			// �汾�ţ�ָ����Ϣ�ṹ�汾�ţ�ĿǰΪ0x0101,��1.01�汾��01�����汾�ţ�01���Ӱ汾�š�
	unsigned short  device_id;				// �豸ID�����ڸ���/����			

	unsigned short  system_format;          // ϵͳ��װ��
	unsigned short  video_format;           // ��Ƶ��������

	unsigned short  audio_format;           // ��Ƶ��������
	unsigned char   audio_channels;         // ͨ����  
	unsigned char   audio_bits_per_sample;  // ��λ��
	unsigned int    audio_samplesrate;      // ������ 
	unsigned int    audio_bitrate;          // ѹ����Ƶ����,��λ��bit

	unsigned int    reserved[4];            // ����
};
struct RtpVideoHead
{
	unsigned short HeaderLen;//��ͷ���ȣ���256
	unsigned char  ChannelNum;//��·������Ӧ�ļ��ͨ���ţ���1��ʼ
	unsigned char  StreamType;//���������ͣ�0����������1����������
	unsigned short StreamHeaderLen;//������������ͷ���ȣ�
	unsigned char  Reserved1;
	unsigned char  Reserved2;
	char  SessionID[4];	//  ��InviteStream����ر����е�sessionֵ
	char  DeviceID[128];//���豸puid
	HK_MEDIAINFO_ _HK_MEDIAINFO_;
	char  Filled[76];
	RtpVideoHead(std::string sSessionID,std::string sDeviceID)
	{
		memset(this,0,sizeof(RtpVideoHead));
		HeaderLen = htons(256);
		ChannelNum = 1;
		StreamType = 1;
		memcpy(SessionID,sSessionID.c_str(),sSessionID.length());
		strcpy(DeviceID,sDeviceID.c_str());
		StreamHeaderLen = htons(40);
		_HK_MEDIAINFO_.media_fourcc = 'HKMI';
		_HK_MEDIAINFO_.media_version = 0x0101;
		_HK_MEDIAINFO_.device_id = 1;
		_HK_MEDIAINFO_.system_format = SYSTEM_MPEG2_PS;
		_HK_MEDIAINFO_.video_format = VIDEO_H264;
		_HK_MEDIAINFO_.audio_format = 0x7110;
		_HK_MEDIAINFO_.audio_channels = 0x01;
		_HK_MEDIAINFO_.audio_bits_per_sample = 0x10;
		_HK_MEDIAINFO_.audio_samplesrate = 0x1F40;
		_HK_MEDIAINFO_.audio_bitrate = 0xFA00;
	}
};
//�豸��¼
struct MSG_PU_LOGIN_REQ:public xml_data
{
	std::string DOMAIN_ID;//��ID
	std::string PU_IP;//PU_IP
	std::string PU_ID;
	std::string CallID;
	std::string PU_NAME;
	MSG_PU_LOGIN_REQ(std::string _DOMAIN_ID,std::string _PU_IP,std::string _PU_ID,std::string _CallID,std::string _PU_NAME)
	{
		DOMAIN_ID = _DOMAIN_ID;
		PU_IP = _PU_IP;
		PU_ID = _PU_ID;
		CallID = _CallID;
		PU_NAME = _PU_NAME;
	}
	std::string GetCmdStr(int nSeq,int nLength = 0)
	{
		std::string str;
		unsigned int Port = 0;
		char buf[2048];
		sprintf(buf,"REGISTER sip:%s;MSG_TYPE=MSG_PU_LOGIN_REQ SIP/2.0\r\n",DOMAIN_ID.c_str());
		str += buf;

		sprintf(buf,"Via: SIP/2.0/TCP %s:%d;branch=z9hG4bk%d\r\n",PU_IP.c_str(),Port,GetTickCount());
		str += buf;

		sprintf(buf,"From: <sip:%s@%s>;tag=%d\r\n",PU_ID.c_str(),DOMAIN_ID.c_str(),GetTickCount());
		str += buf;

		sprintf(buf,"To: <sip:%s@%s>\r\n",PU_ID.c_str(),DOMAIN_ID.c_str());
		str += buf;

		sprintf(buf,"Call-ID: %s@%s\r\n",CallID.c_str(),PU_ID.c_str());
		str += buf;

		sprintf(buf,"CSeq:%d REGISTER\r\n",nSeq);
		str += buf;

		sprintf(buf,"Contact: <sip:%s:%d>\r\n",PU_IP.c_str(),Port);
		str += buf;

		sprintf(buf,"Max-Forwards: 70\r\n");
		str += buf;

		sprintf(buf,"Expires: 30\r\n");
		str += buf;

		sprintf(buf,"User-Agent: hik pu\r\n");
		str += buf;

		sprintf(buf,"Content-Type: text/xml\r\n");
		str += buf;

		sprintf(buf,"Content-Length: %d\r\n\r\n",nLength);
		str += buf;
		return str;
	}
	std::string GetCmdStrXml(int nSeq)
	{
		MapValue IE_PU_INFO;
		IE_PU_INFO["PUID"] = PU_ID;
		IE_PU_INFO["PUName"] = PU_NAME;
		IE_PU_INFO["DomainID"] = DOMAIN_ID;
		IE_PU_INFO["PassWord"] = "";
		IE_PU_INFO["FactoryCode"] = "11";
		IE_PU_INFO["FactoryName"] = "Hikvision";
		IE_PU_INFO["DeviceType"] = "";
		IE_PU_INFO["SWVersion"] = "V2.0 B100330";
		IE_PU_INFO["SerialNo"] = "DS6101HF-IP0020120523AACH408661026WCU";
		IE_PU_INFO["PUMPNO"] = "10000";
		IE_PU_INFO["VideoTransMode"] = "0";
		param["IE_PU_INFO"] = IE_PU_INFO;


		MapValue IE_HARDDISK;
		IE_HARDDISK["TotalCapacity"] = "29";
		IE_HARDDISK["SpareCapacity"] = "0";
		IE_HARDDISK["DiskNum"] = "1";
		IE_HARDDISK["HardDiskStatus"] = "OK";
		param["IE_HARDDISK"] = IE_HARDDISK;

		MapValue IE_VIDEO_CHANNEL_LIST;
        IE_VIDEO_CHANNEL_LIST["VideoChannelNum"] = "1";
		param["IE_VIDEO_CHANNEL_LIST"] = IE_VIDEO_CHANNEL_LIST;

		MapValue IE_WARNIN_CHANNEL_LIST;
		IE_WARNIN_CHANNEL_LIST["WarnInChannelNum"] = "1";
		param["IE_WARNIN_CHANNEL_LIST"] = IE_WARNIN_CHANNEL_LIST;

		MapValue IE_WARNOUT_CHANNEL_LIST;
		IE_WARNOUT_CHANNEL_LIST["WarnOutChannelNum"] = "1";
		param["IE_WARNOUT_CHANNEL_LIST"] = IE_WARNOUT_CHANNEL_LIST;


		MapValue IE_PU_CAPABILITY;
		IE_PU_CAPABILITY["SingelChannelMaxConnNum"] = "6";
		IE_PU_CAPABILITY["AllChannelMaxVideoConnNum"] = "24";
		IE_PU_CAPABILITY["OutMaxBW"] = "100";
		param["IE_PU_CAPABILITY"] = IE_PU_CAPABILITY;


		std::string strXml,strCmd;
		xmlparser::EncodeXml(this,strXml);
		strCmd = GetCmdStr(nSeq,strXml.length());
		strCmd += strXml;
		return strCmd;
	}
};
//�豸��¼
struct MSG_PU_LOGOUT_REQ:public xml_data
{
	std::string DOMAIN_ID;//��ID
	std::string PU_IP;//PU_IP
	std::string PU_ID;
	std::string CallID;
	std::string PU_NAME;
	MSG_PU_LOGOUT_REQ(std::string _DOMAIN_ID,std::string _PU_IP,std::string _PU_ID,std::string _CallID,std::string _PU_NAME)
	{
		DOMAIN_ID = _DOMAIN_ID;
		PU_IP = _PU_IP;
		PU_ID = _PU_ID;
		CallID = _CallID;
		PU_NAME = _PU_NAME;
		MessageType = "MSG_PU_LOGOUT_REQ";
	}
	std::string GetCmdStr(int nSeq,int nLength = 0)
	{
		std::string str;
		unsigned int Port = 0;
		char buf[2048];
		sprintf(buf,"REGISTER sip:%s;MSG_TYPE=MSG_PU_LOGOUT_REQ SIP/2.0\r\n",DOMAIN_ID.c_str());
		str += buf;

		sprintf(buf,"Via: SIP/2.0/TCP %s:%d;branch=z9hG4bk%d\r\n",PU_IP.c_str(),Port,GetTickCount());
		str += buf;

		sprintf(buf,"From: <sip:%s@%s>;tag=%d\r\n",PU_ID.c_str(),DOMAIN_ID.c_str(),GetTickCount());
		str += buf;

		sprintf(buf,"To: <sip:%s@%s>\r\n",PU_ID.c_str(),DOMAIN_ID.c_str());
		str += buf;

		sprintf(buf,"Call-ID: %s@%s\r\n",CallID.c_str(),PU_ID.c_str());
		str += buf;

		sprintf(buf,"CSeq:%d REGISTER\r\n",nSeq);
		str += buf;

		sprintf(buf,"Contact: <sip:%s:%d>\r\n",PU_IP.c_str(),Port);
		str += buf;

		sprintf(buf,"Max-Forwards: 70\r\n");
		str += buf;

		sprintf(buf,"Expires: 0\r\n");
		str += buf;

		sprintf(buf,"User-Agent: pu\r\n");
		str += buf;

		sprintf(buf,"Content-Type: text/xml\r\n");
		str += buf;

		sprintf(buf,"Content-Length: %d\r\n\r\n",nLength);
		str += buf;
		return str;
	}
	std::string GetCmdStrXml(int nSeq)
	{
		MapValue IE_PU_INFO;
		IE_PU_INFO["PUID"] = PU_ID;
		IE_PU_INFO["LogOutReason"] = "";
		param["IE_PU_INFO"] = IE_PU_INFO;


		std::string strXml,strCmd;
		xmlparser::EncodeXml(this,strXml);
		strCmd = GetCmdStr(nSeq,strXml.length());
		strCmd += strXml;
		return strCmd;
	}
};
//�豸����
struct MSG_PU_HEART:public xml_data
{
	std::string DOMAIN_ID;//��ID
	std::string PU_IP;//PU_IP
	std::string PU_ID;
	std::string CallID;
	std::string PU_NAME;
	MSG_PU_HEART(std::string _DOMAIN_ID,std::string _PU_IP,std::string _PU_ID,std::string _CallID,std::string _PU_NAME)
	{
		DOMAIN_ID = _DOMAIN_ID;
		PU_IP = _PU_IP;
		PU_ID = _PU_ID;
		CallID = _CallID;
		PU_NAME = _PU_NAME;
		MessageType = "MSG_PU_HEART";
	}
	std::string GetCmdStr(int nSeq,int nLength = 0)
	{
		std::string str;
		unsigned int Port = 0;
		char buf[2048];
		sprintf(buf,"OPTIONS sip:%s;MSG_TYPE=MSG_PU_HEART SIP/2.0\r\n",DOMAIN_ID.c_str());
		str += buf;

		sprintf(buf,"Via: SIP/2.0/TCP %s:%d;branch=z9hG4bk12%d23\r\n",PU_IP.c_str(),Port,GetTickCount());
		str += buf;

		sprintf(buf,"From: <sip:%s@%s>;tag=223%d\r\n",PU_ID.c_str(),DOMAIN_ID.c_str(),GetTickCount());
		str += buf;

		sprintf(buf,"To: <sip:%s@%s>\r\n",PU_ID.c_str(),DOMAIN_ID.c_str());
		str += buf;

		sprintf(buf,"Call-ID: %s@%s\r\n",CallID.c_str(),PU_ID.c_str());
		str += buf;

		sprintf(buf,"CSeq:%d OPTIONS\r\n",nSeq);
		str += buf;

		sprintf(buf,"Contact: <sip:%s@%s:%d>\r\n",PU_ID.c_str(),PU_IP.c_str(),Port);
		str += buf;

		sprintf(buf,"Max-Forwards: 70\r\n");
		str += buf;

		sprintf(buf,"Expires: 30\r\n");
		str += buf;

		sprintf(buf,"User-Agent: hik pu\r\n");
		str += buf;

		sprintf(buf,"Content-Type: text/xml\r\n");
		str += buf;

		sprintf(buf,"Content-Length: %d\r\n\r\n",nLength);
		str += buf;
		return str;
	}
	std::string GetCmdStrXml(int nSeq)
	{
		return "";
	}
};