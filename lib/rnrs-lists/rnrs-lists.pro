######################################################################
# Automatically generated by qmake (2.01a) Thu Dec 4 20:48:38 2014
######################################################################

TEMPLATE = lib
TARGET =
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
# LIBS += -lgmpxx -lgmp
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += rnrs-lists.hpp \
           keves_base.hpp \
           keves_builtin_values.hpp \
           keves_library.hpp \
           keves_template.hpp \
           keves_textual_port.hpp \
           keves_vm.hpp \
           kev/code.hpp \
           kev/code-inl.hpp \
           kev/procedure.hpp \
           kev/procedure-inl.hpp \
           kev/string.hpp \
           kev/string-inl.hpp \
           kev/symbol.hpp \
           kev/symbol-inl.hpp \
           value/char.hpp

SOURCES += rnrs-lists.cpp \
           keves_base.cpp \
           keves_builtin_values.cpp \
           keves_library.cpp \
           keves_template.cpp \
           keves_textual_port.cpp \
           keves_vm.cpp \
           kev/code.cpp \
           kev/procedure.cpp \
           kev/string.cpp \
           kev/symbol.cpp \
           value/char.cpp

CONFIG += debug
# CONFIG += qt release
