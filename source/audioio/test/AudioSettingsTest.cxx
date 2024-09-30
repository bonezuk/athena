#include "gtest/gtest.h"

#include "audioio/inc/AudioSettings.h"

using namespace omega::audioio;

//-------------------------------------------------------------------------------------------

void removeAudioSettings(const QString& devName)
{
	QString groupName = "audio" + devName;
	QSettings settings;
	settings.beginGroup(groupName);
	settings.remove();
	settings.endGroup();
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isCenterOnByDefault)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
	ASSERT_FALSE(pSettings.isNull())
	EXPECT_TRUE(pSettings->isCenter());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isCenterOffWhenSet)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
	ASSERT_FALSE(pSettings.isNull())
	EXPECT_TRUE(pSettings->isCenter());
	pSettings->setCenter(false);
	EXPECT_FALSE(pSettings->isCenter());
	EXPECT_TRUE(pSettings->isLFE());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isLFEOnByDefault)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
	ASSERT_FALSE(pSettings.isNull())
	EXPECT_TRUE(pSettings->isLFE());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isLFEOffWhenSet)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
	ASSERT_FALSE(pSettings.isNull())
	EXPECT_TRUE(pSettings->isLFE());
	pSettings->setLFE(false);
	EXPECT_FALSE(pSettings->isLFE());
	EXPECT_TRUE(pSettings->isCenter());
}

//-------------------------------------------------------------------------------------------
