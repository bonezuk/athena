message( "Parsing player.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_app.pri )
include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/blackomega/blackomega.pri )
include ( $(OMEGA_HOME)/engine/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/engine/blueomega/blueomega.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )
include ( $(OMEGA_HOME)/engine/whiteomega/whiteomega.pri )
include ( $(OMEGA_HOME)/engine/greenomega/greenomega.pri )
include ( $(OMEGA_HOME)/engine/toneomega/toneomega.pri )
include ( $(OMEGA_HOME)/engine/violetomega/violetomega.pri )
include ( $(OMEGA_HOME)/engine/cyanomega/cyanomega.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/musepack.pri )
!linux-build {
	include ( $(OMEGA_HOME)/engine/monkeyomega/monkeyomega.pri )
}
include ( $(OMEGA_HOME)/engine/wavpackomega/wavpackomega.pri )
include ( $(OMEGA_HOME)/audioio/audioio.pri )
include ( $(OMEGA_HOME)/network/network.pri )
include ( $(OMEGA_HOME)/network/rtp/rtp.pri )
include ( $(OMEGA_HOME)/network/rtp/silveromega/silveromega.pri )
include ( $(OMEGA_HOME)/network/http/http.pri )
include ( $(OMEGA_HOME)/network/mime/mime.pri )
include ( $(OMEGA_HOME)/track/info/info.pri )
include ( $(OMEGA_HOME)/track/db/db.pri )
include ( $(OMEGA_HOME)/dlna/dlna.pri )
include ( $(OMEGA_HOME)/remote/remote.pri )
include ( $(OMEGA_HOME)/widget/widget.pri )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libupnp.pri )

message( "Processing application player" )

QT_TIGER_PLATFORM = $$(TIGER_PLATFORM)
MAC_STORE_PLATFORM = $$(TIGER_PLATFORM_BUILD)

TEMPLATE = app
bundle {
	DEFINES += BUNDLE_PLUGINS
	CONFIG += app_bundle
}

TARGET = Omega

contains( QT_TIGER_PLATFORM, "Mac32" ) {
	ICON = omega.icns
	applestore {
		message( "App Store Info.plist" )
		QMAKE_INFO_PLIST = ./appstore/Info.plist
	}
	else {
    	include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/sparkle.pri )
		message( "Normal Info.plist" )
		QMAKE_INFO_PLIST = Info.plist
	}
}

FORMS   = inc/Player.ui
FORMS  += inc/SettingsAudio.ui
FORMS  += inc/SettingsFile.ui
FORMS  += inc/SettingsGeneral.ui
FORMS  += inc/SettingsITunes.ui
FORMS  += inc/SettingsKeyboard.ui
FORMS  += inc/NoASIODriverWidget.ui
FORMS  += inc/ProgressMessageDialog.ui

UI_DIR  = $(OMEGA_HOME)/player

RESOURCES = ./player.qrc
bundle {
	mac {
		RESOURCES += platform/mac/platform.qrc
	}
}

HEADERS  = inc/AutoUpdater.h
HEADERS += inc/AutoUpdaterMac.h
HEADERS += inc/AutoUpdaterWin.h
HEADERS += inc/CLIPipe.h
HEADERS += inc/CocoaInitializer.h
HEADERS += inc/ITunesConfig.h
HEADERS += inc/NoASIODriverDialog.h
HEADERS += inc/Player.h
HEADERS += inc/PlayerController.h
HEADERS += inc/PlaylistAbstractIO.h
HEADERS += inc/PlaylistM3UIO.h
HEADERS += inc/PlaylistPLSIO.h
HEADERS += inc/PlaylistXSPFIO.h
HEADERS += inc/ProgressMessageDialog.h
HEADERS += inc/QPLItemATrack.h
HEADERS += inc/QPLItemAlbum.h
HEADERS += inc/QPLItemBase.h
HEADERS += inc/QPLItemSingle.h
HEADERS += inc/QPLProgress.h
HEADERS += inc/QPlayControls.h
HEADERS += inc/QPlayerApplication.h
HEADERS += inc/QPlaylistWidget.h
HEADERS += inc/QSpeakerButton.h
HEADERS += inc/RegisterFileTypes.h
HEADERS += inc/Settings.h
HEADERS += inc/SettingsBase.h
HEADERS += inc/SettingsAudio.h
HEADERS += inc/SettingsFile.h
HEADERS += inc/SettingsGeneral.h
HEADERS += inc/SettingsITunes.h
HEADERS += inc/SettingsKeyboard.h
HEADERS += inc/SpeakerInfo.h

SOURCES  = src/AutoUpdater.cxx
SOURCES += src/AutoUpdaterWin.cxx
SOURCES += src/CLIPipe.cxx
SOURCES += src/ITunesConfig.cxx
SOURCES += src/Main.cxx
SOURCES += src/NoASIODriverDialog.cxx
SOURCES += src/Player.cxx
SOURCES += src/PlayerController.cxx
SOURCES += src/PlaylistAbstractIO.cxx
SOURCES += src/PlaylistM3UIO.cxx
SOURCES += src/PlaylistPLSIO.cxx
SOURCES += src/PlaylistXSPFIO.cxx
SOURCES += src/ProgressMessageDialog.cxx
SOURCES += src/QPlayControls.cxx
SOURCES += src/QPlayerApplication.cxx
SOURCES += src/QPlaylistWidget.cxx
SOURCES += src/QPLItemAlbum.cxx
SOURCES += src/QPLItemATrack.cxx
SOURCES += src/QPLItemBase.cxx
SOURCES += src/QPLItemSingle.cxx
SOURCES += src/QPLProgress.cxx
SOURCES += src/QSpeakerButton.cxx
SOURCES += src/RegisterFileTypes.cxx
SOURCES += src/Settings.cxx
SOURCES += src/SettingsBase.cxx
SOURCES += src/SettingsAudio.cxx
SOURCES += src/SettingsFile.cxx
SOURCES += src/SettingsGeneral.cxx
SOURCES += src/SettingsITunes.cxx
SOURCES += src/SettingsKeyboard.cxx
SOURCES += src/SpeakerInfo.cxx

mac {
	HEADERS += inc/SettingsITunesMac.h

	OBJECTIVE_SOURCES += src/AutoUpdaterMac.mm
	OBJECTIVE_SOURCES += src/ITunesConfigLocationMac.mm
	OBJECTIVE_SOURCES += src/CocoaInitializer.mm
	OBJECTIVE_SOURCES += src/SettingsITunesMac.mm
	
	LIBS += -framework Cocoa
	LIBS += -framework AppKit
	LIBS += -framework IOKit
}

message ( "Done - GNUmakefile generated" )
