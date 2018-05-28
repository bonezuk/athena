#include "dlna/inc/UPnPIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

ABSTRACT_FACTORY_CLASS_IMPL(UPnPIFFactory,UPnPIF)

//-------------------------------------------------------------------------------------------

QSharedPointer<UPnPIF> UPnPIF::m_instance;

//-------------------------------------------------------------------------------------------

UPnPIF::UPnPIF()
{}

//-------------------------------------------------------------------------------------------

UPnPIF::~UPnPIF()
{}

//-------------------------------------------------------------------------------------------

QSharedPointer<UPnPIF> UPnPIF::instance()
{
	return m_instance;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<UPnPIF> UPnPIF::instance(const char *factoryName)
{
    if(m_instance.data()==0)
	{
        QSharedPointer<UPnPIF> nInstance = UPnPIFFactory::createShared(factoryName);
		m_instance = nInstance;
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void UPnPIF::release()
{
	QSharedPointer<UPnPIF> eInstance;
	m_instance = eInstance;
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
