//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_QALBUMIMAGEPROVIDER_H
#define __PLAYERAPP_PLAYERCOMMON_QALBUMIMAGEPROVIDER_H
//-------------------------------------------------------------------------------------------

#include "track/model/inc/ImageRepositary.h"

#include <QQuickImageProvider>

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class QAlbumImageProvider : public QQuickImageProvider
{
	public:
		QAlbumImageProvider();
		virtual ~QAlbumImageProvider();
		
		virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
		
	private:
	
		virtual int imageIDFromString(const QString& id) const;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

