//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_PROCESSTHREAD_H
#define __ORCUS_COMMON_PROCESSTHREAD_H
//-------------------------------------------------------------------------------------------

#include <QThread>
#include <QTimer>
#include <QObject>
#include <QSharedPointer>

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------


class ProcessService : public QObject
{
	public:
		Q_OBJECT
		
	public:
		ProcessService(QObject *parent = 0);
		virtual ~ProcessService();
		
		virtual void process() = 0;
};

//-------------------------------------------------------------------------------------------

class ProcessThread : public QThread
{
	Q_OBJECT
	
	public:
		ProcessThread(QSharedPointer<ProcessService> pService,int period,QObject *parent = 0);
		virtual ~ProcessThread();	
		
	protected:
	
		QSharedPointer<ProcessService> m_pService;
		int m_period;
		QTimer *m_timer;
	
		virtual void run();
	
	private slots:
		void onTimer();
};

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
