#include "widget/inc/LicenseRegisterDialog.h"
#include "common/inc/ProductVersionInfo.h"

#include <QApplication>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace widget
{
//-------------------------------------------------------------------------------------------

BuildVersionDialog::BuildVersionDialog(QDialog *dialog,Qt::WindowFlags f) : QDialog(dialog,f)
{}

//-------------------------------------------------------------------------------------------

BuildVersionDialog::~BuildVersionDialog()
{}

//-------------------------------------------------------------------------------------------

QString BuildVersionDialog::getFormatedVersionText()
{
	common::ProductVersionInfoSPtr pVersion = common::ProductVersionInfo::instance();

	QStringList s;
	s << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
	s << "<html><head><meta name=\"qrichtext\" content=\"1\" />";
	s << "<style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:";
#if defined(ORCUS_WIN32)
	s << "8";
#else
	s << "12";
#endif
	s << "pt; font-weight:400; font-style:normal;\">";
	s << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
	s << "<span style=\" font-size:16pt; font-weight:600;\">";
	s << "Black Omega ";
    if(pVersion.data()!=0)
	{
		s << pVersion->version();
	}
	else
	{
		s << "2.1.1";
	}
	s << "</span>";
	s << "</p>";
	s << "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
    if(pVersion.data()!=0)
	{
		s << "Build #" + QString::number(pVersion->build());
	}
	s << "</p>";
	s << "</body>";
	s << "</html>";

	QString txt = s.join("");
	return txt;
}

//-------------------------------------------------------------------------------------------

QString BuildVersionDialog::getFormatedCopyrightText()
{
	QString copyright = "Copyright 1999 - 2012 Stuart A. MacLean. All rights reserved.";
	common::ProductVersionInfoSPtr pVersion = common::ProductVersionInfo::instance();
    if(pVersion.data()!=0)
	{
		copyright = pVersion->copyright();
	}
	return copyright;
}

//-------------------------------------------------------------------------------------------

LicenseRegisterDialog::LicenseRegisterDialog(QDialog *dialog,Qt::WindowFlags f) : BuildVersionDialog(dialog,f),
	m_licenseClient()
{
	m_licenseClient = network::LicenseClient::instance();
	ui.setupUi(this);
	
    QObject::connect(m_licenseClient.data(),SIGNAL(licensed()),this,SLOT(onLicense()));
    QObject::connect(m_licenseClient.data(),SIGNAL(notLicensed()),this,SLOT(onNotLicense()));
    QObject::connect(m_licenseClient.data(),SIGNAL(licenseError(const QString&)),this,SLOT(onLicenseError(const QString&)));
	
	QObject::connect(ui.m_tRegCodeAEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeTEditA(const QString&)));
	QObject::connect(ui.m_tRegCodeBEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeTEditB(const QString&)));
	QObject::connect(ui.m_tRegCodeCEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeTEditC(const QString&)));
	QObject::connect(ui.m_tRegCodeDEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeTEditD(const QString&)));
	QObject::connect(ui.m_tRegCodeEEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeTEditE(const QString&)));

	QObject::connect(ui.m_uRegCodeAEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeUEditA(const QString&)));
	QObject::connect(ui.m_uRegCodeBEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeUEditB(const QString&)));
	QObject::connect(ui.m_uRegCodeCEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeUEditC(const QString&)));
	QObject::connect(ui.m_uRegCodeDEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeUEditD(const QString&)));
	QObject::connect(ui.m_uRegCodeEEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onTextChangeUEditE(const QString&)));
	
	QObject::connect(ui.m_tRegPasteButton,SIGNAL(clicked(bool)),this,SLOT(onPasteRegistration()));
	QObject::connect(ui.m_uRegPasteButton,SIGNAL(clicked(bool)),this,SLOT(onPasteRegistration()));

	QObject::connect(ui.m_tRegisterButton,SIGNAL(clicked(bool)),this,SLOT(onRegister()));
	QObject::connect(ui.m_tCloseButton,SIGNAL(clicked(bool)),this,SLOT(onClose()));
	QObject::connect(ui.m_uRegisterButton,SIGNAL(clicked(bool)),this,SLOT(onRegister()));
	QObject::connect(ui.m_uCloseButton,SIGNAL(clicked(bool)),this,SLOT(onClose()));
	QObject::connect(ui.m_lDeactivateButton,SIGNAL(clicked(bool)),this,SLOT(onDeactivate()));
	QObject::connect(ui.m_lCloseButton,SIGNAL(clicked(bool)),this,SLOT(onClose()));

#if defined(OMEGA_WIN32)
	QObject::connect(ui.m_updateButton,SIGNAL(clicked(bool)),this,SLOT(onUpdatePress()));
#elif defined(OMEGA_MACOSX)
	ui.m_updateButton->setVisible(false);
#endif

	QApplication *app = dynamic_cast<QApplication *>(QCoreApplication::instance());
	if(app!=0)
	{
#if QT_VERSION >= 0x050000
        if(app->devicePixelRatio() >= 2)
		{
			QPixmap pixLogo(":/logo/Resources/omegaLogo48x48@2x.png");
            ui.m_logoImageLabel->setPixmap(pixLogo);
		}
		else
#endif
		{
			QPixmap pixLogo(":/logo/Resources/omegaLogo48x48.png");
            ui.m_logoImageLabel->setPixmap(pixLogo);
		}
	}
}

//-------------------------------------------------------------------------------------------

LicenseRegisterDialog::~LicenseRegisterDialog()
{}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::init()
{
	updateVersionText();
	updateCopyrightText();

	if(m_licenseClient->isValid())
	{
		ui.m_stackWidget->setVisible(true);
		if(m_licenseClient->isLicensed())
		{
			if(m_licenseClient->isTrial())
			{
				ui.m_stackWidget->setCurrentIndex(2);
				updateTrialText();
			}
			else
			{
				ui.m_stackWidget->setCurrentIndex(1);
				updateLicensedText();
			}
		}
		else
		{
			ui.m_stackWidget->setCurrentIndex(0);
			updateUnlicensedText();
		}
	}
	else
	{
		ui.m_stackWidget->setCurrentIndex(3);
	}
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::blockAllSignals(bool flag)
{
	blockAllSignals(this,flag);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::blockAllSignals(QWidget *widget,bool flag)
{
	QObjectList childList = widget->children();
	QObjectList::iterator ppI;
	
	for(ppI=childList.begin();ppI!=childList.end();ppI++)
	{
		QWidget *cW = dynamic_cast<QWidget *>(*ppI);
		if(cW!=0)
		{
			blockAllSignals(cW,flag);
		}
	}
	widget->blockSignals(flag);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::showEvent(QShowEvent *e)
{
	init();
	QDialog::showEvent(e);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onLicense()
{
	init();
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onNotLicense()
{
	init();
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onLicenseError(const QString& err)
{
	if(!err.trimmed().isEmpty())
	{
		QMessageBox::critical(this,"License",err);
	}
	init();
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::updateVersionText()
{
    ui.m_blackOmegaLabel->setText(getFormatedVersionText());
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::updateCopyrightText()
{
    ui.m_copyrightLabel->setText(getFormatedCopyrightText());
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::updateLicensedText()
{
	QStringList s;
	QString txt;
	
	s << printHeader();
	s << printParagraphStart();
	s << printSpanNormal();
	s << "This software has been licensed to ";
	s << printSpanEnd();
	s << printSpanBold();
	s << m_licenseClient->getCustomerName();
	s << printSpanEnd();
	s << printSpanNormal();
	s << ".";
	s << printSpanEnd();
	s << printParagraphBlank();
	s << printParagraphStartCenter();
	s << printSpanBold();
	s << m_licenseClient->getRegistrationCode();
	s << printSpanEnd();
	s << printParagraphEnd();
	s << printParagraphBlank();
	s << printParagraphStart();
	s << printSpanNormal();
	s << "Thank you for your support.";
	s << printSpanEnd();
	s << printParagraphEnd();
	s << printParagraphBlank();
	s << printParagraphStart();
	s << printSpanNormal();
	s << "If you wish, you can deactivate the license associated with this computer.";
	s << printSpanEnd();
	s << printParagraphEnd();
	s << printFooter();
	txt = s.join("");
	ui.m_licenseLabel->setText(txt);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::updateTrialText()
{
	int noDays;
	QStringList s;
	QString txt;
	common::TimeStamp endDate = m_licenseClient->endOfTrialDate();
	common::TimeStamp cDate = common::TimeStamp::now();
	
	blockAllSignals(true);
	
	if(cDate < endDate)
	{
		endDate -= cDate;
		noDays = (endDate.secondsTotal() / (24 * 60 * 60)) + 1;
	}
	else if(endDate==0)
	{
		noDays = 30;
	}
	else
	{
		noDays = 1;
	}	
	
	ui.m_trialProgress->setRange(0,30);
	ui.m_trialProgress->setValue(30 - noDays);
	ui.m_trialProgress->setTextVisible(false);

	s << printHeader();
	s << printParagraphStart();
	s << printSpanNormal();
	s << "You have ";
	s << printSpanEnd();
	s << printSpanBold();
	s << QString::number(noDays);
	s << printSpanEnd();
	s << printSpanNormal();
	s << " day";
	if(noDays>1)
	{
		s << "s";
	}
	s << " left on your trial.";
	s << printSpanEnd();
	s << printParagraphEnd();
	s << printParagraphBlank();
	s << printParagraphStart();
	s << printSpanNormal();
	s << "Please purchase a license from the ";
	s << printSpanEnd();
	s << printLink("Black Omega","http://www.blackomega.co.uk");
	s << printSpanNormal();
	s << " website or enter your license registration code to activate it on this computer.";
	txt = s.join("");
	ui.m_trialLabel->setText(txt);
	
	ui.m_tRegCodeAEdit->setText("");
	ui.m_tRegCodeBEdit->setText("");
	ui.m_tRegCodeCEdit->setText("");
	ui.m_tRegCodeDEdit->setText("");
	ui.m_tRegCodeEEdit->setText("");
	
	updateRegisterButton(true);
	blockAllSignals(false);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::updateUnlicensedText()
{
	blockAllSignals(true);
	
	ui.m_uRegCodeAEdit->setText("");
	ui.m_uRegCodeBEdit->setText("");
	ui.m_uRegCodeCEdit->setText("");
	ui.m_uRegCodeDEdit->setText("");
	ui.m_uRegCodeEEdit->setText("");
	
	updateRegisterButton(false);
	blockAllSignals(false);
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printHeader()
{
	QString s;
	s  = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">";
	s += "<html>";
	s += "<head>";
	s += "<meta name=\"qrichtext\" content=\"1\" />";
	s += "<style type=\"text/css\">p, li { white-space: pre-wrap; }</style>";
	s += "</head>";
	s += "<body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printFooter()
{
	QString s;
	s  = "</body>";
	s += "</html>";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printParagraphStart()
{
	QString s;
	s = "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printParagraphStartCenter()
{
	QString s;
	s = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printParagraphEnd()
{
	QString s;
	s = "</p>";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printParagraphBlank()
{
	QString s;
	s = "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Lucida Grande'; font-size:10pt;\"></p>";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printSpanNormal()
{
	QString s;
	s = "<span style=\" font-family:'Lucida Grande'; font-size:10pt;\">";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printSpanBold()
{
	QString s;
	s = "<span style=\" font-family:'Lucida Grande'; font-size:10pt; font-weight:600;\">";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printSpanEnd()
{
	QString s;
	s = "</span>";
	return s;
}

//-------------------------------------------------------------------------------------------

QString LicenseRegisterDialog::printLink(const QString& lText,const QString& lUrl)
{
	QString s;
	s  = "<a href=\"";
	s += lUrl;
	s += "\"><span style=\" font-family:'Lucida Grande'; font-size:10pt; text-decoration: underline; color:#0000ff;\">";
	s += lText;
	s += "</span></a>";
	return s;
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onRegEditChange(const QString& ts,int maxSize,QLineEdit *lineA,QLineEdit *lineB)
{
	if(ts.length()>maxSize)
	{
		QString s = ts.mid(maxSize).toUpper();
		QString t = ts.mid(0,maxSize).toUpper();
		lineA->blockSignals(true);
		lineA->setText(t);
		lineA->blockSignals(false);
		lineB->blockSignals(true);
		lineB->setText(s);
		lineB->blockSignals(false);
		lineB->setFocus(Qt::MouseFocusReason);
	}
	else
	{
		lineA->blockSignals(true);
		lineA->setText(ts.toUpper());
		lineA->blockSignals(false);
	}
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeTEditA(const QString& t)
{
	onRegEditChange(t,5,ui.m_tRegCodeAEdit,ui.m_tRegCodeBEdit);
	updateRegisterButton(true);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeTEditB(const QString& t)
{
	onRegEditChange(t,4,ui.m_tRegCodeBEdit,ui.m_tRegCodeCEdit);
	updateRegisterButton(true);
}
//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeTEditC(const QString& t)
{
	onRegEditChange(t,4,ui.m_tRegCodeCEdit,ui.m_tRegCodeDEdit);
	updateRegisterButton(true);
}
//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeTEditD(const QString& t)
{
	onRegEditChange(t,4,ui.m_tRegCodeDEdit,ui.m_tRegCodeEEdit);
	updateRegisterButton(true);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeTEditE(const QString& t)
{
	ui.m_tRegCodeEEdit->blockSignals(true);
	ui.m_tRegCodeEEdit->setText(t.toUpper().mid(0,5));
	ui.m_tRegCodeEEdit->blockSignals(false);
	updateRegisterButton(true);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeUEditA(const QString& t)
{
	onRegEditChange(t,5,ui.m_uRegCodeAEdit,ui.m_uRegCodeBEdit);
	updateRegisterButton(false);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeUEditB(const QString& t)
{
	onRegEditChange(t,4,ui.m_uRegCodeBEdit,ui.m_uRegCodeCEdit);
	updateRegisterButton(false);
}
//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeUEditC(const QString& t)
{
	onRegEditChange(t,4,ui.m_uRegCodeCEdit,ui.m_uRegCodeDEdit);
	updateRegisterButton(false);
}
//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeUEditD(const QString& t)
{
	onRegEditChange(t,4,ui.m_uRegCodeDEdit,ui.m_uRegCodeEEdit);
	updateRegisterButton(false);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onTextChangeUEditE(const QString& t)
{
	ui.m_uRegCodeEEdit->blockSignals(true);
	ui.m_uRegCodeEEdit->setText(t.toUpper().mid(0,5));
	ui.m_uRegCodeEEdit->blockSignals(false);
	updateRegisterButton(false);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::updateRegisterButton(bool fTrialPage)
{
	int lA=0,lB=0,lC=0,lD=0,lE=0;
	
	if(fTrialPage)
	{
		lA = ui.m_tRegCodeAEdit->text().length();
		lB = ui.m_tRegCodeBEdit->text().length();
		lC = ui.m_tRegCodeCEdit->text().length();
		lD = ui.m_tRegCodeDEdit->text().length();
		lE = ui.m_tRegCodeEEdit->text().length();
	}
	else
	{
		lA = ui.m_uRegCodeAEdit->text().length();
		lB = ui.m_uRegCodeBEdit->text().length();
		lC = ui.m_uRegCodeCEdit->text().length();
		lD = ui.m_uRegCodeDEdit->text().length();
		lE = ui.m_uRegCodeEEdit->text().length();		
	}
	if(lA==5 && lB==4 && lC==4 && lD==4 && lE==5)
	{
		ui.m_tRegisterButton->setEnabled(true);
		ui.m_uRegisterButton->setEnabled(true);
	}
	else
	{
		ui.m_tRegisterButton->setEnabled(false);
		ui.m_uRegisterButton->setEnabled(false);
	}
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onPasteRegistration()
{
	QClipboard *clipboard = QApplication::clipboard();
	QString rText = clipboard->text();
	if(!rText.isEmpty())
	{
		tint i;
		QString tA,tB,tC,tD,tE;
		
		rText = rText.toUpper();
		for(i=0;i<rText.length();i++)
		{
			if((rText.at(i)>=QChar('A') && rText.at(i)<=QChar('Z')) || (rText.at(i)>=QChar('0') && rText.at(i)<=QChar('9')))
			{
				tA += rText.at(i);
			}
		}
		rText = tA;
		
		if(rText.length()>5)
		{
			tA = rText.mid(0,5);
			rText = rText.mid(5);
			if(rText.length()>4)
			{
				tB = rText.mid(0,4);
				rText = rText.mid(4);
				if(rText.length()>4)
				{
					tC = rText.mid(0,4);
					rText = rText.mid(4);
					if(rText.length()>4)
					{
						tD = rText.mid(0,4);
						rText = rText.mid(4);
						if(rText.length()>5)
						{
							tE = rText.mid(0,5);
						}
						else
						{
							tE = rText;
						}
					}
				}
				else
				{
					tC = rText;
				}
			}
			else
			{
				tB = rText;
			}
		}
		else
		{
			tA = rText;
		}
		
		blockAllSignals(true);
		ui.m_tRegCodeAEdit->setText(tA);
		ui.m_uRegCodeAEdit->setText(tA);
		ui.m_tRegCodeBEdit->setText(tB);
		ui.m_uRegCodeBEdit->setText(tB);
		ui.m_tRegCodeCEdit->setText(tC);
		ui.m_uRegCodeCEdit->setText(tC);
		ui.m_tRegCodeDEdit->setText(tD);
		ui.m_uRegCodeDEdit->setText(tD);
		ui.m_tRegCodeEEdit->setText(tE);
		ui.m_uRegCodeEEdit->setText(tE);
		blockAllSignals(false);
		
		if(ui.m_stackWidget->currentIndex()==2)
		{
			updateRegisterButton(true);
		}
		else
		{
			updateRegisterButton(false);
		}
	}
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onRegister()
{
	QString tA,tB,tC,tD,tE,nRegCode;

    common::Log::g_Log.print("LicenseRegisterDialog::onRegister - A\n");

	if(ui.m_stackWidget->currentIndex()==2)
	{
		tA = ui.m_tRegCodeAEdit->text();
		tB = ui.m_tRegCodeBEdit->text();
		tC = ui.m_tRegCodeCEdit->text();
		tD = ui.m_tRegCodeDEdit->text();
		tE = ui.m_tRegCodeEEdit->text();
	}
	else
	{
		tA = ui.m_uRegCodeAEdit->text();
		tB = ui.m_uRegCodeBEdit->text();
		tC = ui.m_uRegCodeCEdit->text();
		tD = ui.m_uRegCodeDEdit->text();
		tE = ui.m_uRegCodeEEdit->text();
	}
	nRegCode = tA + "-" + tB + "-" + tC + "-" + tD + "-" + tE;
	
	QStringList cText;
	cText << printHeader();
	cText << printParagraphStartCenter();
	cText << printSpanNormal();
	cText << "Authenticating license...";
	cText << printSpanEnd();
	cText << printParagraphEnd();
	cText << printFooter();
	ui.m_messageLabel->setText(cText.join(""));
	ui.m_stackWidget->setCurrentIndex(3);

	m_licenseClient->setRegistrationCode(nRegCode);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onClose()
{
	done(0);
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onDeactivate()
{
	ui.m_stackWidget->setCurrentIndex(3);
	QStringList cText;
	cText << printHeader();
	cText << printParagraphStartCenter();
	cText << printSpanNormal();
	cText << "Deactivating license...";
	cText << printSpanEnd();
	cText << printParagraphEnd();
	cText << printFooter();
	ui.m_messageLabel->setText(cText.join(""));
	m_licenseClient->deactivate();
}

//-------------------------------------------------------------------------------------------

void LicenseRegisterDialog::onUpdatePress()
{
	emit onUpdate();
}

//-------------------------------------------------------------------------------------------

AboutDialog::AboutDialog(QDialog *dialog,Qt::WindowFlags f) : BuildVersionDialog(dialog,f)
{
	ui.setupUi(this);
	ui.m_blackOmegaLabel->setText(getFormatedVersionText());
	ui.m_copyrightLabel->setText(getFormatedCopyrightText());
	
	QApplication *app = dynamic_cast<QApplication *>(QCoreApplication::instance());
	if(app!=0)
	{
#if QT_VERSION >= 0x050000
		if(app->devicePixelRatio() >= 2)
		{
			QPixmap pixLogo(":/logo/Resources/omegaLogo48x48@2x.png");
            ui.m_logoImageLabel->setPixmap(pixLogo);
		}
		else
#endif
		{
			QPixmap pixLogo(":/logo/Resources/omegaLogo48x48.png");
            ui.m_logoImageLabel->setPixmap(pixLogo);
		}
	}
}

//-------------------------------------------------------------------------------------------

AboutDialog::~AboutDialog()
{}

//-------------------------------------------------------------------------------------------
} // namespace widget
} // namespace orcus
//-------------------------------------------------------------------------------------------
