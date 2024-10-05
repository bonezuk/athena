#include "gtest/gtest.h"

#include "audioio/inc/AudioSettings.h"

using namespace omega::audioio;

//-------------------------------------------------------------------------------------------

void removeAudioSettings(const QString& devName)
{
	QString groupName = "audio" + devName;
	QSettings settings;
    settings.remove(groupName);
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isCenterOnByDefault)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
    ASSERT_FALSE(pSettings.isNull());
	EXPECT_TRUE(pSettings->isCenter());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isCenterOffWhenSet)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
    ASSERT_FALSE(pSettings.isNull());
	EXPECT_TRUE(pSettings->isCenter());
	pSettings->setCenter(false);
	EXPECT_FALSE(pSettings->isCenter());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isLFEOnByDefault)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
    ASSERT_FALSE(pSettings.isNull());
	EXPECT_TRUE(pSettings->isLFE());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isLFEOffWhenSet)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
    ASSERT_FALSE(pSettings.isNull());
	EXPECT_TRUE(pSettings->isLFE());
	pSettings->setLFE(false);
	EXPECT_FALSE(pSettings->isLFE());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isExclusiveOffByDefault)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
    ASSERT_FALSE(pSettings.isNull());
	EXPECT_FALSE(pSettings->isExclusive());
}

//-------------------------------------------------------------------------------------------

TEST(AudioSettings, isExclusiveOnWhenSet)
{
	QString devName = "test_audio_device";
	removeAudioSettings(devName);
	QSharedPointer<AudioSettings> pSettings = AudioSettings::instance(devName);
    ASSERT_FALSE(pSettings.isNull());
    EXPECT_FALSE(pSettings->isExclusive());
	pSettings->setExclusive(true);
	EXPECT_TRUE(pSettings->isExclusive());
}

//-------------------------------------------------------------------------------------------
