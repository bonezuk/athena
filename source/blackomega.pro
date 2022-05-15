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

# --- engine ---

HEADERS += ./engine/inc/AData.h
HEADERS += ./engine/inc/BFileStream.h
HEADERS += ./engine/inc/BTime.h
HEADERS += ./engine/inc/BitStream.h
HEADERS += ./engine/inc/Codec.h
HEADERS += ./engine/inc/Compare.h
HEADERS += ./engine/inc/EngineDLL.h
HEADERS += ./engine/inc/File.h
HEADERS += ./engine/inc/FormatType.h
HEADERS += ./engine/inc/FormatTypeFromFloat.h
HEADERS += ./engine/inc/IOFile.h
HEADERS += ./engine/inc/InterleavedCodec.h
HEADERS += ./engine/inc/KaiserFilter.h
HEADERS += ./engine/inc/OmegaDCT.h
HEADERS += ./engine/inc/OmegaDCT64.h
HEADERS += ./engine/inc/RData.h
HEADERS += ./engine/inc/Recycler.h
HEADERS += ./engine/inc/Resample.h
HEADERS += ./engine/inc/Sequence.h
HEADERS += ./engine/inc/SequenceMemory.h
HEADERS += ./engine/inc/WavChannelMask.h

SOURCES += ./engine/src/AData.cxx
SOURCES += ./engine/src/BFileStream.cxx
SOURCES += ./engine/src/BTime.cxx
SOURCES += ./engine/src/BitStream.cxx
SOURCES += ./engine/src/Codec.cxx
SOURCES += ./engine/src/Compare.cxx
SOURCES += ./engine/src/File.cxx
SOURCES += ./engine/src/FormatType.cxx
SOURCES += ./engine/src/FormatTypeFromFloat.cxx
SOURCES += ./engine/src/IOFile.cxx
SOURCES += ./engine/src/InterleavedCodec.cxx
SOURCES += ./engine/src/KaiserFilter.cxx
SOURCES += ./engine/src/OmegaDCT.cxx
SOURCES += ./engine/src/OmegaDCT64.cxx
SOURCES += ./engine/src/RData.cxx
SOURCES += ./engine/src/Resample.cxx
SOURCES += ./engine/src/Sequence.cxx
SOURCES += ./engine/src/SequenceMemory.cxx
SOURCES += ./engine/src/WavChannelMask.cxx

# --- engine/blackomega ---

HEADERS += ./engine/blackomega/inc/AntiAlias.h
HEADERS += ./engine/blackomega/inc/BSequence.h
HEADERS += ./engine/blackomega/inc/BandIndex.h
HEADERS += ./engine/blackomega/inc/BlackOmegaDLL.h
HEADERS += ./engine/blackomega/inc/Dequantize.h
HEADERS += ./engine/blackomega/inc/DequantizeStereo.h
HEADERS += ./engine/blackomega/inc/Hybrid.h
HEADERS += ./engine/blackomega/inc/MPCodec.h
HEADERS += ./engine/blackomega/inc/MPHeader.h
HEADERS += ./engine/blackomega/inc/Reorder.h
HEADERS += ./engine/blackomega/inc/ScaleFactor.h
HEADERS += ./engine/blackomega/inc/SideInfo.h
HEADERS += ./engine/blackomega/inc/Stereo.h
HEADERS += ./engine/blackomega/inc/Subband.h

SOURCES += ./engine/blackomega/src/AntiAlias.cxx
SOURCES += ./engine/blackomega/src/BSequence.cxx
SOURCES += ./engine/blackomega/src/BandIndex.cxx
SOURCES += ./engine/blackomega/src/Dequantize.cxx
SOURCES += ./engine/blackomega/src/DequantizeStereo.cxx
SOURCES += ./engine/blackomega/src/Hybrid.cxx
SOURCES += ./engine/blackomega/src/MPCodec.cxx
SOURCES += ./engine/blackomega/src/MPHeader.cxx
SOURCES += ./engine/blackomega/src/Reorder.cxx
SOURCES += ./engine/blackomega/src/ScaleFactor.cxx
SOURCES += ./engine/blackomega/src/SideInfo.cxx
SOURCES += ./engine/blackomega/src/Stereo.cxx
SOURCES += ./engine/blackomega/src/Subband.cxx

# --- engine/blueomega ---

HEADERS += ./engine/blueomega/inc/BlueOmegaDLL.h
HEADERS += ./engine/blueomega/inc/WaveEngine.h
HEADERS += ./engine/blueomega/inc/WaveInformation.h

SOURCES += ./engine/blueomega/src/WaveEngine.cxx
SOURCES += ./engine/blueomega/src/WaveInformation.cxx

# --- engine/cyanomega ---

INCLUDEPATH += ./engine/cyanomega/musepack/inc

HEADERS += ./engine/cyanomega/musepack/inc/cuetools/cd.h
HEADERS += ./engine/cyanomega/musepack/inc/cuetools/cdtext.h
HEADERS += ./engine/cyanomega/musepack/inc/cuetools/cuefile.h

HEADERS += ./engine/cyanomega/musepack/src/cuefile/cue.h
HEADERS += ./engine/cyanomega/musepack/src/cuefile/cue_parse.h
HEADERS += ./engine/cyanomega/musepack/src/cuefile/cue_parse_prefix.h
HEADERS += ./engine/cyanomega/musepack/src/cuefile/time.h
HEADERS += ./engine/cyanomega/musepack/src/cuefile/toc.h
HEADERS += ./engine/cyanomega/musepack/src/cuefile/toc_parse.h
HEADERS += ./engine/cyanomega/musepack/src/cuefile/toc_parse_prefix.h

SOURCES += ./engine/cyanomega/musepack/src/cuefile/cd.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/cdtext.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/cue_parse.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/cue_print.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/cue_scan.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/cuefile.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/time.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/toc_parse.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/toc_print.c
SOURCES += ./engine/cyanomega/musepack/src/cuefile/toc_scan.c

HEADERS += ./engine/cyanomega/musepack/inc/replaygain/gain_analysis.h

SOURCES += ./engine/cyanomega/musepack/src/replaygain/gain_analysis.c

HEADERS += ./engine/cyanomega/musepack/inc/mpc/datatypes.h
HEADERS += ./engine/cyanomega/musepack/inc/mpc/minimax.h
HEADERS += ./engine/cyanomega/musepack/inc/mpc/mpc_types.h
HEADERS += ./engine/cyanomega/musepack/inc/mpc/mpcdec.h
HEADERS += ./engine/cyanomega/musepack/inc/mpc/mpcmath.h
HEADERS += ./engine/cyanomega/musepack/inc/mpc/reader.h
HEADERS += ./engine/cyanomega/musepack/inc/mpc/streaminfo.h

HEADERS += ./engine/cyanomega/musepack/src/libmpcdec/decoder.h
HEADERS += ./engine/cyanomega/musepack/src/libmpcdec/huffman.h
HEADERS += ./engine/cyanomega/musepack/src/libmpcdec/internal.h
HEADERS += ./engine/cyanomega/musepack/src/libmpcdec/mpc_bits_reader.h
HEADERS += ./engine/cyanomega/musepack/src/libmpcdec/mpcdec_math.h
HEADERS += ./engine/cyanomega/musepack/src/libmpcdec/requant.h

SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/huffman.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/mpc_bits_reader.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/mpc_decoder.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/mpc_demux.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/mpc_reader.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/requant.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/streaminfo.c
SOURCES += ./engine/cyanomega/musepack/src/libmpcdec/synth_filter.c

HEADERS += ./engine/cyanomega/inc/CyanCodec.h
HEADERS += ./engine/cyanomega/inc/CyanOmegaDLL.h
HEADERS += ./engine/cyanomega/inc/MusePackDecoder.h
HEADERS += ./engine/cyanomega/inc/MusePackDecoderImpl.h
HEADERS += ./engine/cyanomega/inc/MusePackIF.h

SOURCES += ./engine/cyanomega/src/CyanCodec.cxx
SOURCES += ./engine/cyanomega/src/MusePackDecoder.cxx
SOURCES += ./engine/cyanomega/src/MusePackDecoderImpl.cxx
SOURCES += ./engine/cyanomega/src/MusePackIF.cxx

# --- playerapp/playerios ---

HEADERS += ./playerapp/playerios/inc/PlayerIOSBaseModel.h

SOURCES += ./playerapp/playerios/src/PlayerIOSBaseModel.cxx
SOURCES += ./playerapp/playerios/src/PlayerIOSMain.cxx

RESOURCES = ./playerapp/playerios/playerios.qrc
