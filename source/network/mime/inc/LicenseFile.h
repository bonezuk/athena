//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_LICENSEFILE_H
#define __ORCUS_NETWORK_LICENSEFILE_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "common/inc/BIOStream.h"
#include "common/inc/Memory.h"
#include "network/mime/inc/MimeForm.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

#define LICENSE_FILE_ID(a,b,c,d) (((static_cast<tuint32>(static_cast<tint32>(d)) << 24) & 0xff000000) | \
                                 ((static_cast<tuint32>(static_cast<tint32>(c)) << 16) & 0x00ff0000) | \
                                 ((static_cast<tuint32>(static_cast<tint32>(b)) <<  8) & 0x0000ff00) | \
                                 ((static_cast<tuint32>(static_cast<tint32>(a))      ) & 0x000000ff))

#define LICENSE_HEADER_ID LICENSE_FILE_ID('L','I','C','E')
#define LICENSE_BLOCK_ID  LICENSE_FILE_ID('L','B','L','K')

//-------------------------------------------------------------------------------------------

const tchar c_licenseServerHost[] = "bonezuk.homeip.net";
const tint c_licenseServerPort = 80;

//-------------------------------------------------------------------------------------------

class MIME_EXPORT LicenseFile
{
	public:
		LicenseFile();
		virtual ~LicenseFile();

		int size() const;
		const QByteArray& block(int idx) const;

		bool read(const QString& name);
		bool read(const QByteArray& lData);		
		
		bool write(const QString& name,const QVector<QByteArray>& licList);
		bool write(QByteArray& lData,const QVector<QByteArray>& licList);
		
	protected:
	
		QVector<QByteArray> m_licenseList;
};

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

