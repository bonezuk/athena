#include "playerapp/webservice/inc/OmegaWebServiceWS.h"
#include "playerapp/webservice/inc/OmegaWebEventService.h"
#include "playerapp/webservice/inc/OmegaPLWebInterfaceWS.h"

#include <QFileInfo>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaWebServiceWS::OmegaWebServiceWS(const QString& rootDir) : OmegaWebService(rootDir)
{}

//-------------------------------------------------------------------------------------------

OmegaWebServiceWS::~OmegaWebServiceWS()
{}

//-------------------------------------------------------------------------------------------

void OmegaWebServiceWS::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "OmegaWebServiceWS::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool OmegaWebServiceWS::setupWebEvents()
{
	QSharedPointer<OmegaWebEventService> webEvents(new OmegaWebEventService(this));
	bool res = false;
	
	if(webEvents->start())
	{
		m_pWebEvents = webEvents.dynamicCast<WebEventRegisterInterface>();
		res = true;
	}
	else
	{
		printError("OmegaWebService", "Failed to start web events IPC service");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void OmegaWebServiceWS::setupPLWebInterface()
{
	QSharedPointer<OmegaPLWebInterfaceWS> pWebInterface(new OmegaPLWebInterfaceWS());
	m_pWebInterface = pWebInterface.dynamicCast<OmegaPLWebInterface>();
}

//-------------------------------------------------------------------------------------------

void OmegaWebServiceWS::stopEvents()
{
	if(!m_pWebEvents.isNull())
	{
		QSharedPointer<OmegaWebEventService> webEvents = m_pWebEvents.dynamicCast<OmegaWebEventService>();
		webEvents->stop();
		m_pWebEvents.clear();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
