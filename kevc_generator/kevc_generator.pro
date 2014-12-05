######################################################################
# Automatically generated by qmake (2.01a) Fri Nov 21 21:02:43 2014
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . ..
INCLUDEPATH += . ..
LIBS += -lgmpxx -lgmp
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += test_code.hpp \
           keves_base.hpp \
           keves_base-inl.hpp \
           keves_builtin_values.hpp \
           keves_file_io.hpp \
           keves_gc.hpp \
           keves_iterator.hpp \
           keves_library.hpp \
           keves_template.hpp \
           keves_textual_port.hpp \
           keves_value.hpp \
           keves_vm.hpp \
           kev/bignum.hpp \
           kev/code.hpp \
           kev/condition.hpp \
           kev/environment.hpp \
           kev/frame.hpp \
           kev/number.hpp \
           kev/pair.hpp \
           kev/procedure.hpp \
           kev/record.hpp \
           kev/reference.hpp \
           kev/string.hpp \
           kev/symbol.hpp \
           kev/vector.hpp \
           kev/wind.hpp \
           kev/wrapped.hpp \
           value/char.hpp \
           value/instruct.hpp


SOURCES += main.cpp \
           test_code.cpp \
           keves_base.cpp \
           keves_builtin_values.cpp \
           keves_file_io.cpp \
           keves_gc.cpp \
           keves_iterator.cpp \
           keves_library.cpp \
           keves_template.cpp \
           keves_textual_port.cpp \
           keves_value.cpp \
           keves_vm.cpp \
           kev/bignum.cpp \
           kev/code.cpp \
           kev/condition.cpp \
           kev/environment.cpp \
           kev/frame.cpp \
           kev/number.cpp \
           kev/pair.cpp \
           kev/procedure.cpp \
           kev/record.cpp \
           kev/reference.cpp \
           kev/string.cpp \
           kev/symbol.cpp \
           kev/vector.cpp \
           kev/wind.cpp \
           kev/wrapped.cpp \
           value/char.cpp \
           value/instruct.cpp

CONFIG += debug
# CONFIG += qt release
