message( "Parsing engine.pro" )

include ( $(OMEGA_HOME)/common/common.pri )

message( "Processing library engine" )

TARGET = engine

HEADERS  = inc/AData.h
HEADERS += inc/BFileStream.h
HEADERS += inc/BTime.h
HEADERS += inc/BitStream.h
HEADERS += inc/Codec.h
HEADERS += inc/Compare.h
HEADERS += inc/EngineDLL.h
HEADERS += inc/File.h
HEADERS += inc/FormatType.h
HEADERS += inc/FormatTypeFromFloat.h
HEADERS += inc/InterleavedCodec.h
HEADERS += inc/IOFile.h
HEADERS += inc/KaiserFilter.h
HEADERS += inc/OrcusDCT.h
HEADERS += inc/OrcusDCT64.h
HEADERS += inc/RData.h
HEADERS += inc/Recycler.h
HEADERS += inc/Resample.h
HEADERS += inc/Sequence.h
HEADERS += inc/SequenceMemory.h
HEADERS += inc/WavChannelMask.h

SOURCES  = src/AData.cxx
SOURCES += src/BFileStream.cxx
SOURCES += src/BTime.cxx
SOURCES += src/BitStream.cxx
SOURCES += src/Codec.cxx
SOURCES += src/Compare.cxx
SOURCES += src/File.cxx
SOURCES += src/FormatType.cxx
SOURCES += src/FormatTypeFromFloat.cxx
SOURCES += src/IOFile.cxx
SOURCES += src/InterleavedCodec.cxx
SOURCES += src/KaiserFilter.cxx
SOURCES += src/OrcusDCT.cxx
SOURCES += src/OrcusDCT64.cxx
SOURCES += src/RData.cxx
SOURCES += src/Resample.cxx
SOURCES += src/Sequence.cxx
SOURCES += src/SequenceMemory.cxx
SOURCES += src/WavChannelMask.cxx

message( "Done - GNUmakefile generated" )
