ios {
	DEFINES += OMEGA_IOS
	QMAKE_CFLAGS   += -Wno-shorten-64-to-32
	QMAKE_CXXFLAGS += -Wno-shorten-64-to-32
}

android {
	DEFINES += OMEGA_ANDROID
}

TEMPLATE = app

QT += quick qml xml

TARGET = BlackOmegaIOS

# --- common ---

HEADERS  = ./common/inc/AbstractProgressInterface.h
HEADERS += ./common/inc/Allocation.h
HEADERS += ./common/inc/Array.h
HEADERS += ./common/inc/BIOBufferedStream.h
HEADERS += ./common/inc/BIOMemory.h
HEADERS += ./common/inc/BIOMemoryStream.h
HEADERS += ./common/inc/BIOStream.h
HEADERS += ./common/inc/BOParse.h
HEADERS += ./common/inc/BOQueueTree.h
HEADERS += ./common/inc/BOTree.h
HEADERS += ./common/inc/BSemaphoreQueue.h
HEADERS += ./common/inc/BString.h
HEADERS += ./common/inc/BStringCollection.h
HEADERS += ./common/inc/CRC32.h
HEADERS += ./common/inc/CommonDLL.h
HEADERS += ./common/inc/CommonDirectoriesForFiles.h
HEADERS += ./common/inc/CommonFunctions.h
HEADERS += ./common/inc/CommonTypes.h
HEADERS += ./common/inc/DLong.h
HEADERS += ./common/inc/DebugOutput.h
HEADERS += ./common/inc/DictionaryXMLMap.h
HEADERS += ./common/inc/DiskOps.h
HEADERS += ./common/inc/Exception.h
HEADERS += ./common/inc/Factory.h
HEADERS += ./common/inc/JaroWinklerDistance.h
HEADERS += ./common/inc/LargeNumber.h
HEADERS += ./common/inc/Log.h
HEADERS += ./common/inc/MD5Digest.h
HEADERS += ./common/inc/Memory.h
HEADERS += ./common/inc/Mutex.h
HEADERS += ./common/inc/ProcessThread.h
HEADERS += ./common/inc/ProductVersionInfo.h
HEADERS += ./common/inc/QBIOStreamDevice.h
HEADERS += ./common/inc/Random.h
HEADERS += ./common/inc/SBService.h
HEADERS += ./common/inc/SHA1Digest.h
HEADERS += ./common/inc/Semaphore.h
HEADERS += ./common/inc/ServiceEvent.h
HEADERS += ./common/inc/TTime.h
HEADERS += ./common/inc/TimeStamp.h
HEADERS += ./common/inc/UString.h

SOURCES  = ./common/src/AbstractProgressInterface.cxx
SOURCES += ./common/src/Allocation.cxx
SOURCES += ./common/src/BIOBufferedStream.cxx
SOURCES += ./common/src/BIOMemory.cxx
SOURCES += ./common/src/BIOMemoryStream.cxx
SOURCES += ./common/src/BIOStream.cxx
SOURCES += ./common/src/BOParse.cxx
SOURCES += ./common/src/BString.cxx
SOURCES += ./common/src/BStringCollection.cxx
SOURCES += ./common/src/CRC32.cxx
SOURCES += ./common/src/CommonDirectoriesForFiles.cxx
SOURCES += ./common/src/CommonFunctions.cxx
SOURCES += ./common/src/DLong.cxx
SOURCES += ./common/src/DebugOutput.cxx
SOURCES += ./common/src/DictionaryXMLMap.cxx
SOURCES += ./common/src/DiskOps.cxx
SOURCES += ./common/src/Exception.cxx
SOURCES += ./common/src/JaroWinklerDistance.cxx
SOURCES += ./common/src/LargeNumber.cxx
SOURCES += ./common/src/Log.cxx
SOURCES += ./common/src/MD5Digest.cxx
SOURCES += ./common/src/Memory.cxx
SOURCES += ./common/src/Mutex.cxx
SOURCES += ./common/src/ProcessThread.cxx
SOURCES += ./common/src/ProductVersionInfo.cxx
SOURCES += ./common/src/QBIOStreamDevice.cxx
SOURCES += ./common/src/Random.cxx
SOURCES += ./common/src/SBService.cxx
SOURCES += ./common/src/SHA1Digest.cxx
SOURCES += ./common/src/Semaphore.cxx
SOURCES += ./common/src/ServiceEvent.cxx
SOURCES += ./common/src/TTime.cxx
SOURCES += ./common/src/TimeStamp.cxx
SOURCES += ./common/src/UString.cxx

# --- playerapp/playerios ---

HEADERS += ./playerapp/playerios/inc/PlayerIOSBaseModel.h

SOURCES += ./playerapp/playerios/src/PlayerIOSBaseModel.cxx
SOURCES += ./playerapp/playerios/src/PlayerIOSMain.cxx

RESOURCES = ./playerapp/playerios/playerios.qrc
