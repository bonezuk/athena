//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_QPLAYERAPPLICATION_H
#define __ORCUS_PLAYER_QPLAYERAPPLICATION_H
//-------------------------------------------------------------------------------------------

#include "remote/inc/KeyControlService.h"

#include <QApplication>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class QPlayerApplication : public QApplication
{
	public:
		Q_OBJECT

	public:
		QPlayerApplication(int& argc,char **argv);
		virtual ~QPlayerApplication();
		
		static QPlayerApplication *playerInstance();
		
	protected:
	
		static QPlayerApplication *m_instance;
	
		virtual bool eventFilter(QObject *obj,QEvent *evt);
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
