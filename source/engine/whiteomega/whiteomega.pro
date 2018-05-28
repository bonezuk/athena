message( "Parsing whiteomega.pro" )

include ( $(OMEGA_HOME)/common/common.pri )
include ( $(OMEGA_HOME)/engine/engine.pri )
include ( $(OMEGA_HOME)/engine/redomega/redomega.pri )

message( "Processing library whiteomega" )

TARGET = whiteomega

HEADERS  = inc/AACDecode.h
HEADERS += inc/AACRoot.h
HEADERS += inc/ALACAtomContainer.h
HEADERS += inc/Atom.h
HEADERS += inc/CPEDecode.h
HEADERS += inc/Common.h
HEADERS += inc/FillDecode.h
HEADERS += inc/GAConfig.h
HEADERS += inc/HCR.h
HEADERS += inc/ICS.h
HEADERS += inc/PNS.h
HEADERS += inc/Prediction.h
HEADERS += inc/PredictionLTP.h
HEADERS += inc/ProgramConfig.h
HEADERS += inc/TNS.h
HEADERS += inc/WCompare.h
HEADERS += inc/WSequence.h
HEADERS += inc/WhiteCodec.h
HEADERS += inc/WhiteOmegaDLL.h
HEADERS += inc/Window.h

SOURCES  = src/AACDecode.cxx
SOURCES += src/AACRoot.cxx
SOURCES += src/ALACAtomContainer.cxx
SOURCES += src/Atom.cxx
SOURCES += src/CPEDecode.cxx
SOURCES += src/Common.cxx
SOURCES += src/FillDecode.cxx
SOURCES += src/GAConfig.cxx
SOURCES += src/HCR.cxx
SOURCES += src/PNS.cxx
SOURCES += src/Prediction.cxx
SOURCES += src/PredictionLTP.cxx
SOURCES += src/ProgramConfig.cxx
SOURCES += src/TNS.cxx
SOURCES += src/WCompare.cxx
SOURCES += src/WSequence.cxx
SOURCES += src/WhiteCodec.cxx
SOURCES += src/Window.cxx

message( "Done - GNUmakefile generated" )
