######################################################################
# Automatically generated by qmake (2.01a) Fri Oct 31 21:17:26 2014
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . kev lib value
INCLUDEPATH += . kev lib value
LIBS += -lgmpxx -lgmp
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += keves_base.hpp \
           keves_base-inl.hpp \
           keves_builtin_values.hpp \
#           keves_eval_window.hpp \
           keves_file_io.hpp \
           keves_gc.hpp \
#           keves_heap.hpp \
           keves_iterator.hpp \
           keves_library.hpp \
           keves_list.hpp \
           keves_stack.hpp \
           keves_template.hpp \
           keves_textual_port.hpp \
           keves_value.hpp \
           keves_vm.hpp \
           kev/bignum.hpp \
           kev/code.hpp \
           kev/condition.hpp \
           kev/environment.hpp \
           kev/generator.hpp \
           kev/jump.hpp \
           kev/frame.hpp \
#           kev/library_kev.hpp \
#           kev/macro.hpp \
           kev/number.hpp \
           kev/pair.hpp \
           kev/procedure.hpp \
           kev/record.hpp \
           kev/reference.hpp \
           kev/string.hpp \
           kev/symbol.hpp \
#           kev/template.hpp \
           kev/vector.hpp \
           kev/wind.hpp \
           kev/wrapped.hpp \
           lib/lib_keves_base.hpp \
#           lib/lib_keves_parse.hpp \
#           lib/lib_rnrs_base.hpp \
#           lib/lib_rnrs_eval.hpp \
#           lib/lib_rnrs_exceptions.hpp \
#           lib/lib_rnrs_lists.hpp \
#           lib/lib_rnrs_mutable_strings.hpp \
#           lib/lib_rnrs_syntax_case.hpp \
#           lib/lib_rnrs_unicode.hpp \
           value/char.hpp \
           value/fixnum.hpp \
           value/instruct.hpp
           

SOURCES += keves_base.cpp \
           keves_builtin_values.cpp \
           keves_file_io.cpp \
           keves_gc.cpp \
           keves_iterator.cpp \
           keves_library.cpp \
           keves_stack.cpp \
           keves_template.cpp \
           keves_textual_port.cpp \
           keves_vm.cpp \
           main.cpp \
           kev/bignum.cpp \
           kev/code.cpp \
           kev/condition.cpp \
           kev/environment.cpp \
           kev/generator.cpp \
           kev/jump.cpp \
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
           lib/lib_keves_base.cpp \
           value/char.cpp \
           value/instruct.cpp

CONFIG += debug
# CONFIG += qt release
