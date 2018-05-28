message( "Parsing blackomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )

message( "Processing library blackomega" )

TARGET = blackomega

HEADRES  = inc/AntiAlias.h
HEADERS += inc/BSequence.h
HEADERS += inc/BandIndex.h
HEADERS += inc/BlackOmegaDLL.h
HEADERS += inc/Dequantize.h
HEADERS += inc/DequantizeStereo.h
HEADERS += inc/Hybrid.h
HEADERS += inc/MPCodec.h
HEADERS += inc/MPHeader.h
HEADERS += inc/Reorder.h
HEADERS += inc/ScaleFactor.h
HEADERS += inc/SideInfo.h
HEADERS += inc/Stereo.h
HEADERS += inc/Subband.h

SOURCES  = src/AntiAlias.cxx
SOURCES += src/BSequence.cxx
SOURCES += src/BandIndex.cxx
SOURCES += src/Dequantize.cxx
SOURCES += src/DequantizeStereo.cxx
SOURCES += src/Hybrid.cxx
SOURCES += src/MPCodec.cxx
SOURCES += src/MPHeader.cxx
SOURCES += src/Reorder.cxx
SOURCES += src/ScaleFactor.cxx
SOURCES += src/SideInfo.cxx
SOURCES += src/Stereo.cxx
SOURCES += src/Subband.cxx

message( "Done - GNUmakefile generated" )
