/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef __VSC_CMN_OAPI_SERVER__H_
#define __VSC_CMN_OAPI_SERVER__H_
#include "utility.hpp"
#include "debug.hpp"
#include <QThread>
#include <QCoreApplication>
#include "factory.hpp"
#include "oapi/oapis.hpp"

#include "XSDK/XHash.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XSocket.h"
#include "XSDK/XSSLSocket.h"
#include "Webby/ClientSideRequest.h"
#include "Webby/ClientSideResponse.h"
#include "Webby/ServerSideRequest.h"
#include "Webby/ServerSideResponse.h"
#include "Webby/WebbyException.h"

using namespace std;
using namespace XSDK;
using namespace WEBBY;

/* each app can register a http REST path can move 
   the socket to itself, for streaming, it it for performance.
*/

using namespace UtilityLib;

class VE_LIBRARY_API CmnOAPIServerSession: public QThread
{
	Q_OBJECT
public:
	inline CmnOAPIServerSession(Factory &pFactory, VEventServer &pEvent, XRef<XSocket> pSocket);
	inline ~CmnOAPIServerSession();
	
public:
	inline void run();

private:
	XRef<XSocket> m_pSocket;
	Factory &m_pFactory;
	VEventServer &m_pEvent;
};

class VE_LIBRARY_API CmnOAPIServer:public QThread
{
	Q_OBJECT
public:
	inline CmnOAPIServer(Factory &pFactory, VEventServer &pEvent);
	inline ~CmnOAPIServer();
	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
public:
	inline void run();

private:
	fast_mutex m_Lock;
	u16 m_port;
	Factory &m_pFactory;
	VEventServer &m_pEvent;
};

class VE_LIBRARY_API CmnOAPISSLServer:public QThread
{
	Q_OBJECT
public:
	inline CmnOAPISSLServer(Factory &pFactory);
	inline ~CmnOAPISSLServer();
	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
public:
	inline void run();

private:
	fast_mutex m_Lock;
	u16 m_port;
};

typedef CmnOAPIServer* LPCmnOAPIServer;
typedef CmnOAPISSLServer* LPCmnOAPISSLServer;

class VE_LIBRARY_API OAPIServerWrapper
{
public:
	inline OAPIServerWrapper(Factory &pFactory, VEventServer &pEvent);
	inline ~OAPIServerWrapper();
	
public:
	void Lock()
	{
		m_Lock.lock();
	}
	void UnLock()
	{
		m_Lock.unlock();
	}
public:
	inline void start()
	{
		m_cmn.start();
		//m_ssl.start();
	}

private:
	fast_mutex m_Lock;
	CmnOAPIServer m_cmn;
	//CmnOAPISSLServer m_ssl;
};


#endif /* __VSC_CMN_OAPI_SERVER__H_ */

