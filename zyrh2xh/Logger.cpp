#include "stdafx.h"
#include "Logger.h"
#include <imagehlp.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <direct.h>
#include <io.h>
#include "LogManager.h"
#pragma comment(lib, "DbgHelp.lib")
Logger g_logInfo;
Logger g_logError("log","error");
//���캯��
Logger::Logger(const char * strLogPath,const char* pStrFileName)
{
	//��ʼ��
	m_pFileStream = NULL;
	char scurdir[1024];
	char sBuf[1024];
	char FilePath[MAX_PATH+1]= {0};
	char *p = NULL;
	GetModuleFileName(NULL, scurdir, sizeof(scurdir)); //��ȡ����ǰִ���ļ���
	p=strrchr(scurdir, '\\');                       
	*p='\0';
	//GetCurrentDirectory(1024,scurdir);   //�õ���ǰ����·�� 
	 
	sprintf(sBuf,"%s\\%s",scurdir,strLogPath);
	m_strLogPath = sBuf;
	m_strCurLogName = pStrFileName;
	CreateLogPath();
	GenerateLogName();
	CLogManager::GetInstance()->AddLog(this);
}


//��������
Logger::~Logger()
{
	//�ر��ļ���
	if(m_pFileStream)
		fclose(m_pFileStream);
}



void Logger::Write2Caching(const char * strInfo, ...)
{
	//�жϵ�ǰ��д��־����������ֻд����;�����Ϣ��������
	if(!strInfo)
		return;
	char pTemp[MAX_STR_LEN] = {0};
	strcpy(pTemp, GetCurrentTime());
	strcat(pTemp,INFOPREFIX);
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
	va_end(arg_ptr);
	printf(pTemp);
	printf("\r\n");
	m_vcStrList.pushBack(pTemp);
	arg_ptr = NULL;
}

//��ȡϵͳ��ǰʱ��
char * Logger::GetCurrentTime()
{
	time_t curTime;
	struct tm * pTimeInfo = NULL;
	time(&curTime);
	pTimeInfo = localtime(&curTime);
	char temp[MAX_STR_LEN] = {0};
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d",pTimeInfo->tm_year+1900,pTimeInfo->tm_mon+1,pTimeInfo->tm_mday, pTimeInfo->tm_hour, pTimeInfo->tm_min, pTimeInfo->tm_sec);
	char * pTemp = temp;
	return pTemp;	
}


//д�ļ�����
void Logger::DoWriteLog()
{
	if (!m_pFileStream)
	{
		return;
	}
	std::vector<std::string> vcStrlist;
	m_vcStrList.GetDataListAndClear(vcStrlist);
	for (int i = 0;i<vcStrlist.size();i++)
	{
		//д��־��Ϣ���ļ���
		fprintf(m_pFileStream, "%s\r\n", vcStrlist[i].c_str());
		m_nlogCount++;
		if (m_nlogCount>10000)
		{
			CreateLogPath();
			m_nlogCount = 0;
		}
	}
	fflush(m_pFileStream);
		
}

//������־�ļ�������
void Logger::GenerateLogName()
{
	time_t curTime;
	struct tm * pTimeInfo = NULL;
	time(&curTime);
	pTimeInfo = localtime(&curTime);
	char temp[1024] = {0};
	//��־�������磺2013-01-01.log
	sprintf(temp, "\\%s%d.%s",m_strCurLogName.c_str(),m_nFileIndex,"log");
	m_nFileIndex++;
	std::string filename = temp;
	if(m_strCurLogName != temp)
	{

		if(m_pFileStream)
			fclose(m_pFileStream);
		char tempfull[1024] = {0};
		strcat(tempfull, m_strLogPath.c_str());
		strcat(tempfull, filename.c_str());
		//��׷�ӵķ�ʽ���ļ���
		m_pFileStream = fopen(tempfull, "a+");
	}

}
#define ACCESS _access  
#define MKDIR(a) _mkdir((a)) 
int CreatMultiDir(char *pDir)
{
	int i = 0;
	int iRet;
	int iLen;
	if(NULL == pDir)
	{
		return 0;
	}

	char pszDir[1024];
	strcpy(pszDir,pDir);
	iLen = strlen(pszDir);

	// �����м�Ŀ¼
	for (i = 0;i < iLen;i ++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{ 
			pszDir[i] = '\0';

			//���������,����
			iRet = _access(pszDir,0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				} 
			}
			//֧��linux,������\����/
			pszDir[i] = '/';
		} 
	}

	iRet = MKDIR(pszDir);
	return iRet;
}
//������־�ļ���·��
void Logger::CreateLogPath()
{
	CreatMultiDir((char*)m_strLogPath.c_str());
	m_nFileIndex = 0;
	m_nlogCount = 0;
}