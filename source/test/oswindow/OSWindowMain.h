//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_OSWINDOWTEST_OSWINDOWMAIN_H
#define __ORCUS_OSWINDOWTEST_OSWINDOWMAIN_H
//-------------------------------------------------------------------------------------------

#include <QWidget>

//-------------------------------------------------------------------------------------------

class QWinWidget : public QWidget
{
	public:
		Q_OBJECT
		
	public:
		QWinWidget(HWND hParentWnd,QObject *parent = 0,Qt::WindowFlags f = 0);
		virtual ~QWinWidget();
		
		void show();
		void center();
		void showCentered();
		
		HWND parentWindow() const;
		
	protected:
		
		void childEvent(QChildEvent *e);
		bool eventFilter(QObject *o,QEvent *e);
		bool focusNextPrevChild(bool next);
		void focusInEvent(QFocusEvent *e);
		bool nativeEvent(const QByteArray& eventType,void *message,long *result);
		
	private:
	
		void init();
		
		void saveFocus();
		void resetFocus();
		
		HWND m_hParent;
		HWND m_prevFocus;
		bool m_reEnableParent;
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
