//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_SETTINGSGENERAL_H
#define __ORCUS_PLAYER_SETTINGSGENERAL_H
//-------------------------------------------------------------------------------------------

#include "player/inc/SettingsBase.h"
#include "player/ui_SettingsGeneral.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class SettingsGeneral : public SettingsBase
{
	public:
		Q_OBJECT
		
	public:
		SettingsGeneral(QWidget *parent = 0,Qt::WindowFlags f = 0);
		virtual ~SettingsGeneral();
		
		virtual void onSelected(int index);
		
	protected:
	
		Ui::SettingsGeneral ui;
		
		virtual void setLoadMemoryCheck();
		
	protected slots:
	
		void onBufferMemory(bool checked);
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
