//
// DeviceServer.cpp
//
// $Id: //poco/1.4/Net/samples/DeviceServer/src/DeviceServer.cpp#1 $
//
// This sample demonstrates the TCPServer and ServerSocket classes.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
#include "DeviceServer.h"
#include "./DeviceCommFuncs.h"
#include "../rtspServer/H264LiveVideoServerMediaSubssion.hh"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#define BUFSIZE 1024*1024*1

Poco::Mutex _mutex;

void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,char const* streamName)//��ʾRTSP������Ϣ
{
	char* url = rtspServer->rtspURL(sms);
	UsageEnvironment& env = rtspServer->envir();
	env <<streamName<< "\n";
	env << "Play this stream using the URL \"" << url << "\"\n";
	delete[] url;
}

DeviceServerConnection::DeviceServerConnection(const StreamSocket& s): 
	TCPServerConnection(s)
{

}
void DeviceServerConnection::run()
{
	int  cmdLength=0;
	char buffer[BUFFER_SIZE];
	int iRet =socket().receiveBytes(&cmdLength, sizeof(cmdLength));
	cmdLength=ntohl(cmdLength);
	NET_INFO(("receive size is %d cmdLength is %d\n",iRet,cmdLength));
	if(iRet==sizeof(cmdLength))
	{
		iRet = socket().receiveBytes(buffer+sizeof(cmdLength),cmdLength-4);
		memcpy(buffer,&cmdLength,sizeof(cmdLength));
		iRet = reloveOnePacket(socket(),buffer,cmdLength);
		NET_INFO(("reloveOnePacket is %d\n",iRet));
	}
	socket().shutdownSend();
}
	
DeviceServerConnectionFactory::DeviceServerConnectionFactory()
{

}

TCPServerConnection* DeviceServerConnectionFactory::createConnection(const StreamSocket& socket)
{
	return new DeviceServerConnection(socket);
}

DeviceServer::DeviceServer(int port,int rtspPort,std::string userName,std::string secret):_commServerActivity(this, &DeviceServer::runCommServerActivity),_rtspServerActivity(this, &DeviceServer::runRtspServerActivity)
{
	commServerPort=port;
	rtspServerPort=rtspPort;
	m_userName=userName;
	m_secret=secret;
}

DeviceServer::~DeviceServer()
{

}

void DeviceServer::setSdkServerData(SDKServerData sSDKserverData)
{
	m_sdkServerData=sSDKserverData;
}


void DeviceServer::start()
{
	_commServerActivity.start();
	_rtspServerActivity.start();
}

void DeviceServer::stop()
{
	_commServerActivity.stop();
	_commServerActivity.wait();
	_rtspServerActivity.stop();
	_rtspServerActivity.wait();
}

void DeviceServer::runCommServerActivity()
{
	std::cout << "commServer start." << std::endl;
	TCPServer srv(new DeviceServerConnectionFactory(),commServerPort);
	// start the TCPServer
	srv.start();
	while (!_commServerActivity.isStopped())
	{
		std::cout << "commServer running." << std::endl;
		//this->run();
		Thread::sleep(5000);
	}
	//srv.stop();
	std::cout << "commServer stopped." << std::endl;
}


int DeviceServer::startRtspServer(int rtspServerPort) 
{
	if(rtspServerPort<0)
		return -1;
	//���û���
	UsageEnvironment* env;
	OutPacketBuffer::maxSize = 1000000; // allow for some possibly large H.264 frames
	Boolean reuseFirstSource = False;//���Ϊ��true������������Ŀͻ��˸���һ���ͻ��˿���һ������Ƶ�������������ͻ��˽����ʱ�����²���
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	//����RTSP������
	UserAuthenticationDatabase* authDB = NULL;
	RTSPServer* rtspServer = RTSPServer::createNew(*env, rtspServerPort, authDB);
	if (rtspServer == NULL) {
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}
	char const* descriptionString= "Session streamed by \"testOnDemandRTSPServer\"";

#if 0
	//ģ��ʵʱ��������ر���
	int datasize;//����������
	unsigned char*  databuf;//������ָ��
	databuf = (unsigned char*)malloc(BUFSIZE);
	bool dosent;//rtsp���ͱ�־λ��Ϊtrue���ͣ������˳�
	Mutex::ScopedLock lock(_mutex);
	//���ļ��п���1M���ݵ��ڴ�����Ϊʵʱ���紫���ڴ�ģ�⣬���ʵʱ���紫��Ӧ����˫�߳̽ṹ���ǵ�����������߳���
	//����ʵʱ��������ݿ���Ӧ���Ƿ�����H264FramedLiveSource�ļ��У���������ֻ���������µĴ�ָ���ȥ����
	FILE *pf;
	fopen_s(&pf, "./test.264", "rb");
	fread(databuf, 1, BUFSIZE, pf);
	datasize = BUFSIZE;
	dosent = true;
	fclose(pf);
	_mutex.unlock();
#endif

 	H264FrameDeviceSource * nH264FrameDeviceSource = new H264FrameDeviceSource(m_sdkServerData);

	//����Ĳ��ֳ���ģ�����紫��Ĳ����������Ļ�����live555�ṩ��demoһ�������������޸�Ϊ���紫�����ʽ��Ϊ����дaddSubsession�ĵ�һ����������ļ�
	char const* streamName = "ch1/main/av_stream";
	ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName,descriptionString);
	sms->addSubsession(H264LiveVideoServerMediaSubssion::createNew(*env, reuseFirstSource,nH264FrameDeviceSource));//�޸�Ϊ�Լ�ʵ�ֵ�H264LiveVideoServerMediaSubssion
	rtspServer->addServerMediaSession(sms);
	announceStream(rtspServer, sms, streamName);//��ʾ�û�����������Ϣ
	while(1)
	{
		try
		{		
			env->taskScheduler().doEventLoop(); //ѭ���ȴ�����
		}
		catch(...)
		{
			printf("CException--\n");
		}
		Sleep(1000);
	}
	//free(databuf);//�ͷŵ��ڴ�
	return 0;
}

void DeviceServer::runRtspServerActivity()
{
	std::cout << "rtspServer start."<<rtspServerPort<< std::endl;
	while (!_rtspServerActivity.isStopped())
	{
		std::cout << "rtspServer running." << std::endl;
		startRtspServer(rtspServerPort);
		Thread::sleep(5000);
	}
	
	std::cout << "rtspServer stopped."<<rtspServerPort << std::endl;
}


#if 0
void DeviceServer::initialize(Application& self)
{
	loadConfiguration();
	ServerApplication::initialize(self);
}
	
void DeviceServer::uninitialize()
{
	ServerApplication::uninitialize();
}

int DeviceServer::main(const std::vector<std::string>& args)
{
	ServerSocket svs(commServerPort);
	TCPServer srv(new DeviceServerConnectionFactory(), svs);
	// start the TCPServer
	srv.start();
	// wait for CTRL-C or kill
	waitForTerminationRequest();
	// Stop the TCPServer
	srv.stop();
	return Application::EXIT_OK;
}
#endif


