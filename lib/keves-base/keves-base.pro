######################################################################
# Automatically generated by qmake (2.01a) Thu Dec 4 20:48:38 2014
######################################################################

TEMPLATE = lib
TARGET =
DEPENDPATH += . ../..
INCLUDEPATH += . ../..
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += keves-base.hpp \
           keves_builtin_values.hpp \
           keves_common.hpp \
           keves_gc.hpp \
           keves_library.hpp \
           keves_template.hpp \
           keves_textual_port.hpp \
           keves_vm.hpp \
           kev/code.hpp \
           kev/code-inl.hpp \
           kev/environment.hpp \
           kev/environment-inl.hpp \
           kev/frame.hpp \
           kev/procedure.hpp \
           kev/procedure-inl.hpp \
           kev/string.hpp \
           kev/string-inl.hpp \
           kev/symbol.hpp \
           kev/symbol-inl.hpp \
           kev/vector.hpp \
           kev/vector-inl.hpp

SOURCES += keves-base.cpp \
           keves_builtin_values.cpp \
           keves_common.cpp \
           keves_gc.cpp \
           keves_library.cpp \
           keves_template.cpp \
           keves_textual_port.cpp \
           keves_vm.cpp \
           kev/code.cpp \
           kev/environment.cpp \
           kev/frame.cpp \
           kev/procedure.cpp \
           kev/string.cpp \
           kev/symbol.cpp \
           kev/vector.cpp

CONFIG += debug
# CONFIG += qt release
