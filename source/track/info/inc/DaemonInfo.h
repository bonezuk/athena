//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_INFO_VORBISINFO_H
#define __ORCUS_TRACK_INFO_VORBISINFO_H
//-------------------------------------------------------------------------------------------

#include "track/info/inc/Info.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace info
{
//-------------------------------------------------------------------------------------------

class TRACK_INFO_EXPORT DaemonInfo : public Info
{
	public:
		DaemonInfo(tint id, void *client);
		virtual ~DaemonInfo();

		virtual tint id() const;
		
		virtual const QString& imageUrl() const;
		virtual QString& imageUrl();
		
		virtual void setBitrate(tint rate);
		virtual void setNoChannels(tint noChs);
		virtual void setFrequency(tint freq);
		
		virtual void *client();
		
	protected:
		tint m_id;
		QString m_imageUrl;
		void *m_client;
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace info
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
