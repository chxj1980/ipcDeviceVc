

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


 __int64   InitEncoder();
//
/*
��in�� h264VidoeBuf:������һ֡h264��Ƶ����
��in�� nH264BufLength ��Ƶ���ݳ���
��in�� nTimeStamp��ʱ��� 
��out��OutHKBuf ���صĺ������Ļ�������������Ӧ�㹻��һ�㿼�ǹؼ�֡�Ļ�512kb
��out��nOutHKLen ���صĺ������ĳ���
��return�� bool true �ɹ� false ����ʧ��
*/
 bool  __stdcall ConvertH264ToHKPs(long nPort,char* h264VidoeBuf,unsigned int nH264BufLength,__int64 nTimeStamp,char* OutHKBuf,unsigned int& nOutHKLen );

 /*
 ��in�� audioBuf: ��Ƶ����
 ��in�� nAudioBufLength ��Ƶ���ݳ���
 ��out��outBufAudio���ص�������h264��Ƶ���ݳ��ȣ�������Ӧ�㹻��һ�㿼�ǹؼ�֡�Ļ�512kb
 ��out��nOutHKLen  ���ص�������h264��Ƶ���ݵĳ���
 ��return�� bool true �ɹ� false ����ʧ��
 */
bool  __stdcall ConvertAudioToHk(long nPort,char* audioBuf,unsigned int nAudioBufLength,__int64 nTimeStamp,char* outBufAudio,unsigned int& nOutHKLen );


 /*
 ��in�� h264VidoeBuf:������һ֡h264��Ƶ����
 ��in�� nH264BufLength ��Ƶ���ݳ���
 ��in�� nTimeStamp��ʱ��� 
 ��in��  char* sDevID:�豸ID
 ��out��OutHKBuf ���صĺ�����Rtp���Ļ�������������Ӧ�㹻��һ�㿼�ǹؼ�֡�Ļ�512kb
 ��out��nOutHKLen ���صĺ������ĳ���
 ��return�� bool true �ɹ� false ����ʧ��
 */
bool  __stdcall ConvertH264ToHKRtp(long nPort,char* h264VidoeBuf,unsigned int nH264BufLength,__int64 nTimeStamp,char* sDevID,char* OutHKBuf,unsigned int& nOutHKLen );
/*
��in�� h264Buf: ��������h264����
��in�� nH264BufLength ��Ƶ���ݳ���
��out��outBufH264 ���ص�������h264��Ƶ���ݳ��ȣ�������Ӧ�㹻��һ�㿼�ǹؼ�֡�Ļ�512kb
��out��nOutHKLen  ���ص�������h264��Ƶ���ݵĳ���
��return�� bool true �ɹ� false ����ʧ��
*/
 bool __stdcall H264_InputData(long nPort,char* h264Buf,unsigned int nH264BufLen,char* outBufH264,unsigned int& outLen);




  /*
  ����rtmp�ĵ�ַ
 ��in�� strRtmpUrl: rtmp�ĵ�ַ
 ��in�� nPort       rmtp��ͨ��
 ��return�� bool true �ɹ� false ����ʧ��
 */
int    openRtmpUrl(__int64 nPort,char* sdevId,char* password,__int64 nDevLine,__int64 nchannel, char* strRtmpUrl);

 bool  Ps_InputData(long nPort,char* psVideoData,unsigned int nPsDatalen);


 bool  Ps_AnalyzeDataGetPacketEx(long nPort,char* pH264VideoData,unsigned int& nH264Datalen,unsigned int& nTimeStamp,unsigned int& nType);


bool  SendRtmpVideoData(long nPort,char* videoData,unsigned int nvideolen,unsigned int nTimeStamp);


bool  SendRtmpAduioData(long nPort,char *pAudio, int i_size,unsigned char audioType, unsigned int NowTic,int nChannel,int nSoundRate);
bool  UnInitEncoder(__int64 nPort);
int  opentestchar(__int64 nPort, char* sdevIp);
#ifdef __cplusplus
}
#endif
//��������
/*h264 תHKPs��
1.long nPort = InitEncoder();
���ȡ������Ƶʱ������һ֡��ֱ�ӵ���
ConvertH264ToHK();
���ȡ������Ƶʱ����������һ֡��ֱ�ӵ���
if��H264_InputData��
{
   ConvertH264ToHK();
}
����ת��
UnInitEncoder(nPort);
*/


/*h264 תHK Rtp��
1.long nPort = InitEncoder();
���ȡ������Ƶʱ������һ֡��ֱ�ӵ���
ConvertH264ToHKRtp();
���ȡ������Ƶʱ����������һ֡��ֱ�ӵ���
if��H264_InputData��
{
ConvertH264ToHKRtp();
}
����ת��
UnInitEncoder(nPort);
*/


/*����Ps��ת�� תRtmp��
1.long nPort = InitEncoder();
//���ӵ�rtmp
openRtmpUrl��nPort,Rtmp�����ĵ�ַ��
UnInitEncoder(nPort);
*/
