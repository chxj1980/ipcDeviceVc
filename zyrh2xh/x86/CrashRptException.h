/**
* @file CrashRptException.h
* @brief 
* @author ������
* @date 2013-8-23 10:51:19
* @version 
* <pre><b>copyright: </b></pre>
* <pre><b>email: </b>***@***</pre>
* <pre><b>company: ��������ӥ����Ƽ����޹�˾</b>http://www.4006043110.com/</pre>
* <pre><b>All rights reserved.</b></pre>
* <pre><b>modification:</b></pre>
* <pre>Write modifications here.</pre>
*/

#ifndef _CRASHRTP_EXCEPTION_H
#define _CRASHRTP_EXCEPTION_H

#pragma once

#include "CrashRpt.h"

class CCrashRptException
{
public:
	CCrashRptException(void);
	~CCrashRptException(void);

	bool InstallCrashRpt();
	void UnstallCrashRpt();
};

#endif
