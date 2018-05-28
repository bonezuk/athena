//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_WIDGET_LICENSEREGISTERDIALOG_H
#define __ORCUS_WIDGET_LICENSEREGISTERDIALOG_H
//-------------------------------------------------------------------------------------------

#include <QDialog>
#include <QClipboard>
#include <QMessageBox>

#include "network/mime/inc/LicenseFile.h"
#include "network/mime/inc/LicenseClient.h"

#include "widget/ui_LicenseRegisterDialog.h"
#include "widget/ui_AboutDialog.h"
#include "widget/inc/WidgetDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace widget
{
//-------------------------------------------------------------------------------------------

class WIDGET_EXPORT BuildVersionDialog : public QDialog
{
	Q_OBJECT
	
	public:
		BuildVersionDialog(QDialog *dialog = 0,Qt::WindowFlags f = 0);
		virtual ~BuildVersionDialog();
	
	protected:
	
		virtual QString getFormatedVersionText();
		virtual QString getFormatedCopyrightText();
};

//-------------------------------------------------------------------------------------------

class WIDGET_EXPORT LicenseRegisterDialog : public BuildVersionDialog
{
	Q_OBJECT
	
	public:
		LicenseRegisterDialog(QDialog *dialog = 0,Qt::WindowFlags f = 0);
		virtual ~LicenseRegisterDialog();
		
	protected:
	
		Ui::LicenseRegisterDialog ui;
        QSharedPointer<network::LicenseClient> m_licenseClient;
		
		void init();
		
		virtual void showEvent(QShowEvent *e);
		
		void blockAllSignals(bool flag);
		void blockAllSignals(QWidget *widget,bool flag);
		
		void updateVersionText();
		void updateCopyrightText();

		void updateLicensedText();
		void updateTrialText();
		void updateUnlicensedText();
		
		QString printHeader();
		QString printFooter();
		QString printParagraphStart();
		QString printParagraphStartCenter();
		QString printParagraphEnd();
		QString printParagraphBlank();
		QString printSpanNormal();
		QString printSpanBold();
		QString printSpanEnd();
		QString printLink(const QString& lText,const QString& lUrl);
		
		void onRegEditChange(const QString& t,int maxSize,QLineEdit *lineA,QLineEdit *lineB);
		void updateRegisterButton(bool fTrialPage);

	signals:

		void onUpdate();
		
	protected slots:
		
		void onLicense();
		void onNotLicense();
		void onLicenseError(const QString& err);
		
		void onTextChangeTEditA(const QString& t);
		void onTextChangeTEditB(const QString& t);
		void onTextChangeTEditC(const QString& t);
		void onTextChangeTEditD(const QString& t);
		void onTextChangeTEditE(const QString& t);
		void onTextChangeUEditA(const QString& t);
		void onTextChangeUEditB(const QString& t);
		void onTextChangeUEditC(const QString& t);
		void onTextChangeUEditD(const QString& t);
		void onTextChangeUEditE(const QString& t);
		
		void onPasteRegistration();
		
		void onRegister();
		void onClose();
		void onDeactivate();

		void onUpdatePress();
};

//-------------------------------------------------------------------------------------------

class WIDGET_EXPORT AboutDialog : public BuildVersionDialog
{
	Q_OBJECT
	
	public:
		AboutDialog(QDialog *dialog = 0,Qt::WindowFlags f = 0);
		virtual ~AboutDialog();
		
	protected:
	
		Ui::AboutDialog ui;
};

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
