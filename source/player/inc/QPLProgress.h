//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_QPLPROGRESS_H
#define __ORCUS_PLAYER_QPLPROGRESS_H
//-------------------------------------------------------------------------------------------

#include "player/inc/QPLItemBase.h"

#include <QTimer>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class QPLProgress : public QObject
{
	public:
		Q_OBJECT
		
	public:
		QPLProgress(QPlaylistWidget *parent);
		virtual ~QPLProgress();
		
		bool isActive();
		void activate(bool useReference = false);
		void deactivate(bool useReference = false);
				
		void paint(QPainter *painter);

		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
		
		void setProgress(tfloat32 v);
		tfloat32 getProgress();
		bool isCancelled();

	protected:
	
		QPlaylistWidget *m_parentWidget;
		
		QImage *m_leftImage;
		QImage *m_rightImage;
		QImage *m_rightMaskImage;
		QImage *m_rightHoverImage;
		QImage *m_rightClickImage;
		QList<QImage *> m_waitImageList;
		
		tfloat32 m_progressValue;
		int m_waitImageIndex;
		QTimer *m_waitTimer;
		int m_progressState;
		int m_progressReferenceCount;
		bool m_cancelFlag;
		
		bool isOnCancelButton(QMouseEvent *e);
		
		bool isRetina() const;
		void paintRetinaImage(QPainter *painter,QPoint pos,QImage *pImage) const;
		void paintRetinaImage(QPainter *painter,QPointF pos,QImage *pImage) const;
		
	protected slots:
	
		void onWaitTimer();
		
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
