message( "Parsing common.pro" )

include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/omega_nowidget_dll.pri )
include ( $(OMEGA_HOME)/build/$(TIGER_PLATFORM)/pri_files/libxml2.pri )

message( "Processing library common" )

TARGET = common

DEPENDPATH  += ../
INCLUDEPATH += ../

HEADERS  = inc/Allocation.h
HEADERS += inc/Array.h
HEADERS += inc/AsynchronousFileReader.h
HEADERS += inc/BIOBufferedStream.h
HEADERS += inc/BIOMemory.h
HEADERS += inc/BIOMemoryStream.h
HEADERS += inc/BIOStream.h
HEADERS += inc/BIOTimeCachedStream.h
HEADERS += inc/BOParse.h
HEADERS += inc/BOQueueTree.h
HEADERS += inc/BOTree.h
HEADERS += inc/BSemaphoreQueue.h
HEADERS += inc/BString.h
HEADERS += inc/BStringCollection.h
HEADERS += inc/CachedFileStream.h
HEADERS += inc/CRC32.h
HEADERS += inc/CommonDirectoriesForFiles.h
HEADERS += inc/CommonDLL.h
HEADERS += inc/CommonFunctions.h
HEADERS += inc/CommonTypes.h
HEADERS += inc/CryptoPrimes.h
HEADERS += inc/CryptoRSAEncrypt.h
HEADERS += inc/CryptoRandom.h
HEADERS += inc/DLong.h
HEADERS += inc/DebugOutput.h
HEADERS += inc/DiskOps.h
HEADERS += inc/DictionaryXMLMap.h
HEADERS += inc/Exception.h
HEADERS += inc/Factory.h
HEADERS += inc/HardwareIdentification.h
HEADERS += inc/JaroWinklerDistance.h
HEADERS += inc/LargeNumber.h
HEADERS += inc/Log.h
HEADERS += inc/MD5Digest.h
HEADERS += inc/Memory.h
HEADERS += inc/Mutex.h
HEADERS += inc/ProcessInfo.h
HEADERS += inc/ProductVersionInfo.h
HEADERS += inc/QBIOStreamDevice.h
HEADERS += inc/Random.h
HEADERS += inc/RegistryScript.h
HEADERS += inc/SHA1Digest.h
HEADERS += inc/SBService.h
HEADERS += inc/Semaphore.h
HEADERS += inc/ServiceEvent.h
HEADERS += inc/TTime.h
HEADERS += inc/TimeStamp.h
HEADERS += inc/UString.h

SOURCES  = src/Allocation.cxx
SOURCES += src/AsynchronousFileReader.cxx
SOURCES += src/BIOBufferedStream.cxx
SOURCES += src/BIOMemory.cxx
SOURCES += src/BIOMemoryStream.cxx
SOURCES += src/BIOStream.cxx
SOURCES += src/BIOTimeCachedStream.cxx
SOURCES += src/BOParse.cxx
SOURCES += src/BString.cxx
SOURCES += src/BStringCollection.cxx
SOURCES += src/CachedFileStream.cxx
SOURCES += src/CommonDirectoriesForFiles.cxx
SOURCES += src/CommonFunctions.cxx
SOURCES += src/CRC32.cxx
SOURCES += src/CryptoPrimes.cxx
SOURCES += src/CryptoRSAEncrypt.cxx
SOURCES += src/CryptoRandom.cxx
SOURCES += src/DLong.cxx
SOURCES += src/DebugOutput.cxx
SOURCES += src/DictionaryXMLMap.cxx
SOURCES += src/DiskOps.cxx
SOURCES += src/Exception.cxx
SOURCES += src/HardwareIdentification.cxx
SOURCES += src/JaroWinklerDistance.cxx
SOURCES += src/LargeNumber.cxx
SOURCES += src/Log.cxx
SOURCES += src/MD5Digest.cxx
SOURCES += src/Memory.cxx
SOURCES += src/Mutex.cxx
SOURCES += src/ProcessInfo.cxx
SOURCES += src/ProductVersionInfo.cxx
SOURCES += src/QBIOStreamDevice.cxx
SOURCES += src/Random.cxx
SOURCES += src/RegistryScript.cxx
SOURCES += src/SHA1Digest.cxx
SOURCES += src/SBService.cxx
SOURCES += src/Semaphore.cxx
SOURCES += src/ServiceEvent.cxx
SOURCES += src/TTime.cxx
SOURCES += src/TimeStamp.cxx
SOURCES += src/UString.cxx

mac {
	HEADERS += inc/Bundle.h
	OBJECTIVE_SOURCES += src/Bundle.mm
}

applestore {
mac {
	OBJECTIVE_SOURCES += src/SBServiceAppStore.mm
}
}

mac {
	LIBS += -framework Cocoa
	LIBS += -framework Foundation
}

message( "Done - GNUmakefile generated" )
