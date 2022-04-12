#include "playerapp/webservice/inc/OmegaWebService.h"
#include "playerapp/webservice/inc/OmegaPLWebInterface.h"

#include <QFileInfo>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaWebServiceWS::OmegaWebServiceWS(const QString& rootDir) : OmegaWebServiceWS(rootDir)
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
	m_pWebInterface = QSharedPointer<OmegaPLWebInterface>(new OmegaPLWebInterface());
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
