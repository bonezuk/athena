//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_TEST_CODECTEST_H
#define __ORCUS_COMMON_TEST_CODECTEST_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <QCoreApplication>
#include <QtTest/QtTest>

#include "engine/inc/Codec.h"
#include "track/model/test/TrackDBTestEnviroment.h"
#include "dlna/inc/DiskIF.h"

using namespace orcus;
using namespace orcus::engine;
using namespace testing;

//-------------------------------------------------------------------------------------------

class CodecQtUnitTest : public QObject
{
	public:
		Q_OBJECT
	private slots:
		void bitsPerSecondAIFF();
#if !defined(OMEGA_LINUX)
		void bitsPerSecondAPEFast();
		void bitsPerSecondAPEHigh();
		void bitsPerSecondAPEInsane();
#endif
		void bitsPerSecondFlacCompression0();
		void bitsPerSecondFlacCompression5();
		void bitsPerSecondFlacCompression8();
		void bitsPerSecondFlacCompression8NoSeek();
		void bitsPerSecondAAC96Kbps();
		void bitsPerSecondAAC128Kbps();
		void bitsPerSecondAAC256Kbps();
		void bitsPerSecondALAC();
		void bitsPerSecondMP3At96Kbps();
		void bitsPerSecondMP3At128Kbps();
		void bitsPerSecondMP3At256Kbps();
		void bitsPerSecondMP3AtVBR();
		void bitsPerSecondMusePackAt129kbps();
		void bitsPerSecondMusePackAt291kbps();
		void bitsPerSecondVorisOggAt96kbps();
		void bitsPerSecondVorisOggAt128kbps();
		void bitsPerSecondVorisOggAt256kbps();
		void bitsPerSecondWAV();
		void bitsPerSecondWavPackLosslessAt96kbps();
		void bitsPerSecondWavPackLosslessAt256kbps();
		void bitsPerSecondWavPackLossyAt96kbps();
		void bitsPerSecondWavPackLossyAt256kbps();
		void bitsPerSecondALACCaf();
};

//-------------------------------------------------------------------------------------------

class CodecQtUnitApplication : public QCoreApplication
{
	public:
		Q_OBJECT
	public:
		CodecQtUnitApplication(int argc,char **argv);
		virtual ~CodecQtUnitApplication();
		bool testSucceeded() const;
		
	private:
		bool m_succeeded;
	private slots:
		void runTests();
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

