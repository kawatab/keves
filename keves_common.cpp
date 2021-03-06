// keves/keves_common.cpp - common resources of Keves
// Keves will be an R6RS Scheme implementation.
//
//  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
//
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "keves_common.hpp"
#include "keves_common-inl.hpp"

#include <iostream>
#include <QLibrary>
#include <QMutexLocker>
#include "keves_library.hpp"
#include "keves_vm.hpp"
#include "kev/bignum.hpp"
#include "kev/bignum-inl.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/condition.hpp"
#include "kev/condition-inl.hpp"
#include "kev/environment.hpp"
#include "kev/environment-inl.hpp"
#include "kev/frame.hpp"
#include "kev/frame-inl.hpp"
#include "kev/number.hpp"
#include "kev/number-inl.hpp"
#include "kev/pair.hpp"
#include "kev/pair-inl.hpp"
#include "kev/procedure.hpp"
#include "kev/procedure-inl.hpp"
#include "kev/record.hpp"
#include "kev/reference.hpp"
#include "kev/reference-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"
#include "kev/vector.hpp"
#include "kev/vector-inl.hpp"
#include "kev/wind.hpp"
#include "kev/wind-inl.hpp"
#include "value/char.hpp"


KevesCommon::KevesCommon()
  : instruct_table_(),
    cmd_table_(),
    builtin_(),
    default_result_field_(),
    thread_pool_(),
    library_list_(),
    library_name_list_(),
    shared_list_(),
    ft_PushChildren_(),
    ft_RevertObject_(),
    ft_ReadObject_(),
    ft_WriteObject_(),
    mutex_(QMutex::Recursive) {
  initCMDTable();
  initLibraryList();
  builtin_.init(this);
  thread_pool_.setMaxThreadCount(4);

  setFunctionTable<CodeKev>();
  setFunctionTable<Bignum>();
  setFunctionTable<RationalNumberKev>();
  setFunctionTable<FlonumKev>();
  setFunctionTable<ExactComplexNumberKev>();
  setFunctionTable<InexactComplexNumberKev>();
  setFunctionTable<StringCoreKev>();
  setFunctionTable<StringKev>();
  setFunctionTable<SymbolKev>();
  setFunctionTable<VectorKev>();
  setFunctionTable<WindKev>();
  setFunctionTable<ReferenceKev>();
  setFunctionTable<RecordKev>();
  setFunctionTable<SimpleConditionKev>();
  setFunctionTable<CompoundConditionKev>();
  setFunctionTable<LambdaKev>();
  setFunctionTable<ArgumentFrameKev>();
  setFunctionTable<LocalVarFrameKev>();
  // setFunctionTable<FreeVarFrameKev>();
  setFunctionTable<StackFrameKev>();
  setFunctionTable<PairKev>();
  setFunctionTable<EnvironmentKev>();
}

KevesCommon::~KevesCommon() {
  thread_pool_.waitForDone();
  for (auto library : library_list_) delete library;
}

void KevesCommon::runThread(KevesValue arg) {
  KevesVM* vm(KevesVM::make(this, default_result_field()));
  vm->acc_ = arg;
  thread_pool_.start(vm);
}

void KevesCommon::initCMDTable() {
#define INIT_CHECK_SUM() int check_sum(0);
#define CHECK_SUM(x) Q_ASSERT(check_sum == (CMD_##x + 1) * CMD_##x / 2);

#define SET_FUNCTION_TO_TABLE(x)		\
  cmd_table_[CMD_##x] = &KevesVM::cmd_##x;	\
  check_sum += CMD_##x;
  
  INIT_CHECK_SUM();

  SET_FUNCTION_TO_TABLE(NOP);
  SET_FUNCTION_TO_TABLE(HALT);
  SET_FUNCTION_TO_TABLE(JUMP);
  SET_FUNCTION_TO_TABLE(JUMP_R);
  SET_FUNCTION_TO_TABLE(SKIP);
  SET_FUNCTION_TO_TABLE(TEST);
  SET_FUNCTION_TO_TABLE(TEST_R);
  SET_FUNCTION_TO_TABLE(TEST0_R);
  SET_FUNCTION_TO_TABLE(TEST_NOT_R);
  SET_FUNCTION_TO_TABLE(SWITCH_N_ARG);
  SET_FUNCTION_TO_TABLE(SWITCH_N_ARG_R);
  SET_FUNCTION_TO_TABLE(FRAME);
  SET_FUNCTION_TO_TABLE(FRAME_R);
  SET_FUNCTION_TO_TABLE(SFRAME);
  SET_FUNCTION_TO_TABLE(APPLY);
  SET_FUNCTION_TO_TABLE(APPLY_CONSTANT);
  SET_FUNCTION_TO_TABLE(APPLY_MULT);
  SET_FUNCTION_TO_TABLE(RETURN);
  SET_FUNCTION_TO_TABLE(RETURN_CONSTANT);
  SET_FUNCTION_TO_TABLE(RETURN0);
  SET_FUNCTION_TO_TABLE(RETURN_VOID);
  SET_FUNCTION_TO_TABLE(REFER_LOCAL);
  SET_FUNCTION_TO_TABLE(REFER_LOCAL0);
  SET_FUNCTION_TO_TABLE(REFER_FREE);
  SET_FUNCTION_TO_TABLE(REFER_FREE0);
  SET_FUNCTION_TO_TABLE(CONSTANT);
  SET_FUNCTION_TO_TABLE(ASSIGN_LOCAL);
  SET_FUNCTION_TO_TABLE(ASSIGN_LOCAL0);
  SET_FUNCTION_TO_TABLE(CALL_LAMBDA);
  SET_FUNCTION_TO_TABLE(CALL_LAMBDA_VLA);
  SET_FUNCTION_TO_TABLE(ASSIGN_FREE);
  SET_FUNCTION_TO_TABLE(ASSIGN_FREE0);
  SET_FUNCTION_TO_TABLE(ASSIGN_MULT);
  SET_FUNCTION_TO_TABLE(CLOSE_R);
  SET_FUNCTION_TO_TABLE(NEW_BOX);
  SET_FUNCTION_TO_TABLE(BOX);
  SET_FUNCTION_TO_TABLE(RAISE);
  SET_FUNCTION_TO_TABLE(RAISE_CONSTANT);
  SET_FUNCTION_TO_TABLE(CLEAR_VALS);
  SET_FUNCTION_TO_TABLE(LAST_POP_TEST);
  SET_FUNCTION_TO_TABLE(LAST_POP_TEST_R);
  SET_FUNCTION_TO_TABLE(DEBUG_CODE);
  CHECK_SUM(DEBUG_CODE);

  // argument
  SET_FUNCTION_TO_TABLE(LAST);
  SET_FUNCTION_TO_TABLE(ARGUMENT);
  SET_FUNCTION_TO_TABLE(PUSH);
  SET_FUNCTION_TO_TABLE(PUSH_CONSTANT);
  SET_FUNCTION_TO_TABLE(POP);
  CHECK_SUM(POP);

  // for Qev
  SET_FUNCTION_TO_TABLE(TEST_QEV_EQUAL_R);

  // for numbers
  SET_FUNCTION_TO_TABLE(TEST_NUMBER_R);
  SET_FUNCTION_TO_TABLE(TEST_NUMBER0);
  SET_FUNCTION_TO_TABLE(TEST_NUMBER0_R);

  SET_FUNCTION_TO_TABLE(TEST_NUMBER_EQUAL_R);

  // for char
  SET_FUNCTION_TO_TABLE(TEST_CHAR_R);
  SET_FUNCTION_TO_TABLE(TEST_CHAR0_R);
  SET_FUNCTION_TO_TABLE(TEST_CHAR_EQUAL_R);

  // for symbols
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL);
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL0);
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL_EQUAL_R);

  // for strings
  SET_FUNCTION_TO_TABLE(STRING_CLONE);
  SET_FUNCTION_TO_TABLE(STRING_APPEND);
  SET_FUNCTION_TO_TABLE(TEST_STRING_R);
  SET_FUNCTION_TO_TABLE(TEST_STRING0_R);
  SET_FUNCTION_TO_TABLE(TEST_STRING_EQUAL_R);
  CHECK_SUM(TEST_STRING_EQUAL_R);

  // for pairs
  SET_FUNCTION_TO_TABLE(CONS);
  SET_FUNCTION_TO_TABLE(CONS_CONSTANT);
  SET_FUNCTION_TO_TABLE(CAR);
  SET_FUNCTION_TO_TABLE(CAR0);
  SET_FUNCTION_TO_TABLE(CDR);
  SET_FUNCTION_TO_TABLE(CDR0);
  SET_FUNCTION_TO_TABLE(TEST_PAIR_R);
  SET_FUNCTION_TO_TABLE(TEST_PAIR0_R);
  SET_FUNCTION_TO_TABLE(TEST_NULL_R);
  SET_FUNCTION_TO_TABLE(TEST_NULL0_R);

  // for call/cc
  SET_FUNCTION_TO_TABLE(UNWIND_CONTINUATION);
  SET_FUNCTION_TO_TABLE(REVERT_DYNAMIC_WIND);

  // for dynamic-wind
  SET_FUNCTION_TO_TABLE(SET_DYNAMIC_WIND);
  SET_FUNCTION_TO_TABLE(REMOVE_DYNAMIC_WIND);

  // for vectors
  SET_FUNCTION_TO_TABLE(LIST_TO_VECTOR);
  CHECK_SUM(LIST_TO_VECTOR);

  // for conditions
  SET_FUNCTION_TO_TABLE(TEST_CONDITION);
  
  // for syntax
  SET_FUNCTION_TO_TABLE(SYNTAX);
  
  // expander
  SET_FUNCTION_TO_TABLE(BEGIN);
  SET_FUNCTION_TO_TABLE(TERMINATE_EXPAND);
  SET_FUNCTION_TO_TABLE(ADD_GLOBAL_VARIABLE);
  CHECK_SUM(ADD_GLOBAL_VARIABLE);

  // for library
  SET_FUNCTION_TO_TABLE(EXPORT_SYMBOL);
  SET_FUNCTION_TO_TABLE(TEST_ENVIRONMENT);
  CHECK_SUM(TEST_ENVIRONMENT);

#undef SET_FUNCTION_TO_TABLE
#undef CHECK_SUM
#undef INIT_CHECK_SUM
}

void KevesCommon::initLibraryList() {
  QFile library_list_file("conf/library.conf");

  if (!library_list_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cerr << "Not found `library.conf'\n";
    return;
  }
  
  QString temp, library_name, file_name;
  int line_num(0);

  while (!library_list_file.atEnd()) {
    temp = library_list_file.readLine();
    int pos(temp.indexOf('\t'));
    int size(temp.length());

    if (size == 0) continue;

    if (pos < 0 || pos == size - 1) {
      std::cerr << "Error: There is no value in line: " << line_num
		<< ", library.conf.\n";
      continue;
    }

    library_name = temp.left(pos).trimmed();
    file_name = temp.mid(pos + 1).trimmed();

    if (library_name.isEmpty()) {
      std::cerr << "Error: There is no library name in line: " << line_num
		<< ", library.conf.\n";
      continue;
    }

    if (file_name.isEmpty()) {
      std::cerr << "Error: There is no file name in line: " << line_num
		<< ", library.conf.\n";
      continue;
    }

    QPair<QStringList, QString> pair(library_name.split(' '), file_name);
    library_name_list_ << pair;
  }

  library_list_file.close();
}

const StringKev* KevesCommon::getMesgText(const QString& key) const {
  return builtin_.getMesgText(key);
}

void KevesCommon::appendObjectList(QList<const Kev*>* list, const Kev* kev) {
  QStack<const Kev*> pending;
  pending.push(kev);

  while (!pending.isEmpty()) {
    const Kev* temp(pending.pop());

    if (list->indexOf(temp) < 0) {
      (*ft_PushChildren_[temp->type()])(&pending, temp);
      list->append(temp);
    }
  }
}
      
void KevesCommon::addLibrary(KevesLibrary* library) {
  library_list_.append(library);
}

bool KevesCommon::match(const QStringList& list1, const QStringList& list2) {
  int size(list1.size());

  if (list2.size() != size) return false;

  for (int i(0); i < size; ++i)
    if (list1.at(i).compare(list2.at(i)) != 0) return false;

  return true;
}

KevesLibrary* KevesCommon::getLibrary(const QStringList& id,
				      const QList<ver_num_t>& ver_num) {
  Q_ASSERT(id.size() > 0);

  QMutexLocker locker(&mutex_);
  
  for (auto lib : library_list_)
    if (lib->match(id)) return lib;

  return loadLibrary(id, ver_num);
}

KevesLibrary* KevesCommon::loadLibrary(const QStringList& id,
				       const QList<ver_num_t>& ver_num) {
  for (auto pair : library_name_list_) {
    if (match(id, pair.first)) {
      QString file_name("lib/");
      file_name += pair.second;
      QLibrary lib_bin(file_name);
      typedef KevesLibrary*(*FuncMakeLib)(KevesCommon*);
      auto make_lib = reinterpret_cast<FuncMakeLib>(lib_bin.resolve("make"));
      
      if (!make_lib) {
	std::cerr << "file name: " << qPrintable(file_name)
		  << qPrintable(lib_bin.errorString())
		  << "\n";

	return nullptr;
      }

      KevesLibrary* library(make_lib(this));
      library->setID(pair.first, ver_num);
      addLibrary(library);
      return library;
    }
  }
      
  return loadCompiledLibrary(id, ver_num);
}

KevesLibrary* KevesCommon::loadCompiledLibrary(const QStringList& id,
					       const QList<ver_num_t>& ver_num) {
  QString file_name("lib");
  for (auto str : id) file_name += QString('/') + str;
  file_name += ".kevc";
  
  QFile file(file_name);

  if (!file.exists()) return nullptr;

  std::cout << "find library: " << qPrintable(file_name) << std::endl;
  KevesLibrary* library(KevesLibrary::readFromFile(file_name, this));
  addLibrary(library);
  return library;
}

void* KevesCommon::Alloc(size_t alloc_size) {
  KevesBaseNode node(new char[((alloc_size + sizeof(quintptr) - 1) & ~(sizeof(quintptr) - 1)) + sizeof(KevesPrefix)]);
  shared_list_.push(node);
  return node.toPtr();
}

void KevesCommon::pushToSharedList(MutableKev* kev) {
  kev->markPermanent();
  shared_list_.push(kev);
}

uioword KevesCommon::indexAddress(const QList<const Kev*>& table,
				  KevesValue value) {
  if (!value.isPtr()) return value.toUIntPtr();
  
  int index(table.indexOf(value.toPtr()));

  return index < 0 ?
    value.toUIntPtr() : (static_cast<quintptr>(index) << 2 | INDEX);
}

void KevesCommon::pushValue(QStack<const Kev*>* pending, KevesValue value) {
  if (value.isPtr()) pending->push(value.toPtr());
}

void KevesCommon::revertObjects(const QList<const Kev*>& object_list, int start_pos) {
  for (int i(start_pos); i < object_list.size(); ++i) {
    MutableKevesValue kev(object_list.at(i));
    if (kev.isPtr()) (*ft_RevertObject_[kev.type()])(object_list, kev);
  }   
}

void KevesCommon::revertValue(const QList<const Kev*>& object_list,
			      KevesValue* value) {
  if (isIndex(*value)) *value = object_list.at(value->toUIntPtr() >> 2);
}

QString KevesCommon::toString(KevesValue value) const {
  QString str;

  if (value.isPtr()) toString_list(&str, value, 0);
  else toString_element(&str, value);

  return str;
}

void KevesCommon::toString_list(QString* str, KevesValue value, int nest) const {
  if (value.isPair()) {
    if (nest > 8) {
      str->append("(...)");
      return;
    }
    
    str->append("(");
    KevesValue temp(value);

    for (;;) {
      const PairKev* pair(temp);
      KevesValue car(pair->car());
      toString_list(str, car, nest + 1);
      temp = pair->cdr();

      if (!temp.isPair()) {
	if (temp != EMB_NULL) {
	  str->append(" . ");

	  if (temp.isVector()) toString_vector(str, temp, nest + 1);
	  else if (temp.isCode()) toString_code(str, temp, nest + 1);
	  else toString_element(str, temp);
	}

	str->append(")");
	return;
      }

      str->append(" ");
    }
  } else if (value.isVector()) {
    toString_vector(str, value, nest);
  } else if (value.isCode()) {
    toString_code(str, value, nest);
  } else {
    toString_element(str, value);
  }
}

void KevesCommon::toString_vector(QString* str, KevesValue value, int nest) const {
  const VectorKev* vector(value);

  if (vector->size() == 0) {
    str->append("#()");
    return;
  }

  if (nest > 8) {
    str->append("#(...)");
    return;
  }
    
  const_KevesIterator iter(vector->begin());
  const_KevesIterator end(vector->end());

  str->append("#(");

  toString_list(str, *iter++, nest + 1);

  while (iter != end) {
    str->append(" ");
    toString_list(str, *iter++, nest + 1);
  }

  str->append(")");
}

void KevesCommon::toString_code(QString* str, KevesValue value, int nest) const {
  const CodeKev* code(value);

  if (code->size() == 0) {
    str->append("#code()");
    return;
  }

  if (nest > 8) {
    str->append("#code(...)");
    return;
  }
    
  const_KevesIterator iter(code->begin());
  const_KevesIterator end(code->end());

  str->append("#code(");

  toString_list(str, *iter++, nest + 1);

  while (iter != end) {
    str->append("\n      ");
    toString_list(str, *iter++, nest + 1);
  }

  str->append(")");
}

void KevesCommon::toString_element(QString* str, KevesValue value) const {
  if (value.isBool()) {
    str->append(value == EMB_TRUE ? "#t" : "#f");
  } else if (value == EMB_NULL) {
    str->append("'()");
  } else if (value.isInstruct()) {
    KevesInstruct inst(value);
    str->append(instruct_table()->getLabel(inst));
  } else if (value.isFixnum()) {
    KevesFixnum num(value);
    str->append(QString::number(num));
  } else if (value.isChar()) {
    KevesChar chr(value);
    str->append("#\\").append(chr);
  } else if (value.isPtr()) {
    switch (value.type()) {
    case BIGNUM: {
      const Bignum* bignum(value);
      str->append(bignum->toQString());
      break;
    }
      
    case RATIONAL_NUM: {
      const RationalNumberKev* rational(value);
      str->append(rational->toQString(false));
      break;
    }
      
    case FLONUM: {
      const FlonumKev* flonum(value);
      str->append(flonum->toQString(false, 32));
      break;
    }
      
    case EXCT_CPLX_NUM: {
      const ExactComplexNumberKev* complex(value);
      str->append(complex->toQString());
      break;
    }
      
    case INEX_CPLX_NUM: {
      const InexactComplexNumberKev* complex(value);
      str->append(complex->toQString(32));
      break;
    }
      
    case STRING: {
      const StringKev* string(value);
      str->append('"').append(string->toQString()).append('"');
      break;
    }
      
    case SYMBOL: {
      const SymbolKev* symbol(value);
      str->append(symbol->toQString());
      break;
    }
      
    case WIND:
      str->append("<wind>");
      break;
      
    case REFERENCE:
      str->append("<reference>");
      break;
      
    case ARG_FRAME: {
      const ArgumentFrameKev* argp(value);
      str->append("<argp: ").append(QString::number(argp->size())).append('>');
      break;
    }
      
    case LOCAL_FRAME: {
      /*
      const LocalVarFrameKev* envp(value);
      str->append("<envp: ").append(QString::number(envp->size())).append('>');
      break;
      */
      const LocalVarFrameKev* envp(value);
      int cnt_var(0);
      int cnt_frame(0);

      do {
	++cnt_frame;
	cnt_var += envp->size();
      } while ((envp = envp->next()));

      str->append("<envp: ")
	.append(QString::number(cnt_var))
	.append(", ")
	.append(QString::number(cnt_frame))
	.append('>');

      break;
    }
      
      /*
    case FREE_FRAME: {
      const FreeVarFrameKev* clsr(value);
      int cnt_var(0);
      int cnt_frame(0);

      do {
	++cnt_frame;
	cnt_var += clsr->size();
      } while ((clsr = clsr->next()));

      str->append("<clsr: ")
	.append(QString::number(cnt_var))
	.append(", ")
	.append(QString::number(cnt_frame))
	.append('>');

      break;
    }
      */
      
    case CPS: {
      const CPSKev* cps(value);
      if (cps->who().isPtr() && cps->who().isString()) {
	const StringKev* who(cps->who());
	str->append("<procedure: cps (")
	  .append(who->toQString())
	  .append(")>");
      } else {
	str->append("<procedure: cps>");
      }
      break;
    }
      
    case LAMBDA:
      str->append("<procedure: lambda>");
      break;
      
    case CONTINUATION:
      str->append("<procedure: continuation>");
      break;
      
    case ENVIRONMENT:
      str->append("<environment>");
      break;
      
    case STACK_FRAME: {
      const StackFrameKev* stack(value);
      int cnt_clsr(0);
      int cnt_fp(0);

      for (const LocalVarFrameKev* clsr(stack->clsr()); clsr; clsr = clsr->next())
	++cnt_clsr;
	
      for (const StackFrameKev* fp(stack->fp()); fp; fp = fp->fp())
	++cnt_fp;
	
      str->append("<fp: argn: ")
	.append(QString::number(stack->argn()))
	.append(", envn: ")
	.append(QString::number(stack->envn()))
	.append(", clsr-link: ")
	.append(QString::number(cnt_clsr))
	.append(", fp-link>")
	.append(QString::number(cnt_fp))
	.append(">");

      break;
    }
      
    case STACK_FRAME_B:
      str->append("<fp_b>");
      break;
      
    case STACK_FRAME_D:
      str->append("<fp_d>");
      break;
      
    case STACK_FRAME_E:
      str->append("<fp_e>");
      break;
      
    default:
      str->append(QString::number(reinterpret_cast<quintptr>(value.toPtr()), 16));
    }
  } else if (value == EMB_UNDEF) {
    str->append("<undef>");
  } else {
    str->append("Unknown");
  }
}

KevesValue KevesCommon::makeAssertCondition(KevesValue a,
					    KevesValue b,
					    KevesValue c) {
  VectorKev* values(VectorKev::make(this, 4));
  KevesIterator iter(values->begin());
  *iter++ = SimpleConditionKev::make(this, builtin()->amp_assert(), EMB_NULL);
  *iter++ = SimpleConditionKev::make(this, builtin()->amp_who(), a);
  *iter++ = SimpleConditionKev::make(this, builtin()->amp_message(), b);
  *iter++ = SimpleConditionKev::make(this, builtin()->amp_irritants(), c);
  return CompoundConditionKev::make(this, values);
}
