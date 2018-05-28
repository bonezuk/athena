#include "player/inc/SettingsBase.h"
#include "player/inc/Settings.h"
#if defined(OMEGA_WIN32)
#include <shlobj.h>
#endif
#include "player/inc/Player.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

SettingsBase::SettingsBase(QWidget *parent,Qt::WindowFlags f) : QWidget(parent,f)
{
    m_central = dynamic_cast<SettingsCentralWidget *>(parentWidget());
}

//-------------------------------------------------------------------------------------------

SettingsBase::~SettingsBase()
{}

//-------------------------------------------------------------------------------------------

Player *SettingsBase::getPlayerDialog()
{
    return m_central->getPlayer();
}

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
