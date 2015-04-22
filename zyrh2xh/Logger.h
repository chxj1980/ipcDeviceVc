#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdio.h>
#include "Thread.h"
#include "MyVector.h"
//��־�������ʾ��Ϣ
static const char * INFOPREFIX      = " Info: ";

static const int MAX_STR_LEN = 2048;
//��־����ö��
typedef enum EnumLogLevel
{
	LogLevelAll = 0,    //������Ϣ��д��־
	LogLevelMid = 1,        //д���󡢾�����Ϣ
	LogLevelNormal = 2,     //ֻд������Ϣ
	LogLevelStop = 3       //��д��־
};

class Logger
{
public:
	//���캯��
	Logger(const char * strLogPath = "log",const char* pStrFileName = "File");
	//��������
	virtual ~Logger();
public:	
	//дһ����Ϣ
	void  Write2Caching(const char * strInfo, ...);


	//д�ļ�����
	void DoWriteLog();
private:

	
	//��ȡ��ǰϵͳʱ��
	char * GetCurrentTime();
	//������־�ļ�����
	void GenerateLogName();
	//������־·��
	void CreateLogPath();
private:
	//д��־�ļ���
	FILE * m_pFileStream;
	//��־��·��
	std::string m_strLogPath;
	//��־������
	std::string m_strCurLogName;

	//��־�ļ���������
	std::size_t   m_nFileIndex;
	//��־������
	std::size_t   m_nlogCount;

	CMyVector<std::string> m_vcStrList;

};
extern Logger g_logInfo;
extern Logger g_logError;
#endif