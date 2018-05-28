//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_QSPEAKERBUTTON_H
#define __ORCUS_PLAYER_QSPEAKERBUTTON_H
//-------------------------------------------------------------------------------------------

#include "player/inc/QPlaylistWidget.h"
#include "audioio/inc/AOBase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class QSpeakerButton : public QWidget
{
	public:
		Q_OBJECT
		
	public:
		QSpeakerButton(audioio::ChannelType t,QWidget *parent = 0);
		virtual ~QSpeakerButton();
		
	protected:
	
		audioio::ChannelType m_type;
		QImage *m_speakerEnabledImage;
		QImage *m_speakerDisabledImage;
		QImage *m_playNormalImage;
		QImage *m_playHoverImage;
		QImage *m_playClickImage;
		QImage *m_playMaskImage;
		int m_state;
	
		void setup();
		QImage *loadPlayButton(const QString& imgName);
		
		virtual void paintEvent(QPaintEvent *event);
		QImage *transPlayButton(float amount);
		QImage *currentPlayImage();

		virtual void mouseMoveEvent(QMouseEvent *e);
		virtual void mousePressEvent(QMouseEvent *e);
		virtual void mouseReleaseEvent(QMouseEvent *e);
		virtual void leaveEvent(QEvent *e);
		
		bool isRetina() const;
		void paintRetinaImage(QPainter *painter,QPoint pos,QImage *pImage) const;
		void paintRetinaImage(QPainter *painter,QPointF pos,QImage *pImage) const;
		
	signals:
	
		void clicked();
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
