//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_SETTINGSITUNESMAC_H
#define __ORCUS_PLAYER_SETTINGSITUNESMAC_H
//-------------------------------------------------------------------------------------------

#include "player/inc/SettingsITunes.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class SettingsITunesMac : public SettingsITunes
{
	public:
		Q_OBJECT
		
	public:
		SettingsITunesMac(QWidget *parent = 0,Qt::WindowFlags f = 0);
		virtual ~SettingsITunesMac();
		
        void onFolderOpen(const QStringList& files);
        void onCancel();

	protected:
		
		void *m_loader;
		
		void *toUrl(const QString& fileName);
		
		virtual void processAddDirectory();
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

