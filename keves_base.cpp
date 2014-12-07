// keves/keves_base.cpp - base of Keves
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


#include "keves_base.hpp"
#include "keves_base-inl.hpp"

#include <iostream>
#include <QLibrary>
#include <QMutexLocker>
#include "keves_library.hpp"
#include "keves_vm.hpp"
#include "kev/bignum.hpp"
#include "kev/bignum-inl.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/frame.hpp"
#include "kev/frame-inl.hpp"
#include "kev/number.hpp"
#include "kev/number-inl.hpp"
#include "kev/pair.hpp"
#include "kev/pair-inl.hpp"
#include "kev/procedure.hpp"
#include "kev/procedure-inl.hpp"
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


KevesBase::KevesBase()
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
    ft_WriteObject_() {
  InitCMDTable();
  InitLibraryList();
  builtin_.Init(this);
  thread_pool_.setMaxThreadCount(4);

  SetFunctionTable<CodeKev>();
  SetFunctionTable<Bignum>();
  SetFunctionTable<RationalNumberKev>();
  SetFunctionTable<FlonumKev>();
  SetFunctionTable<ExactComplexNumberKev>();
  SetFunctionTable<InexactComplexNumberKev>();
  SetFunctionTable<StringCoreKev>();
  SetFunctionTable<StringKev>();
  SetFunctionTable<SymbolKev>();
  SetFunctionTable<VectorKev>();
  SetFunctionTable<WindKev>();
  SetFunctionTable<ReferenceKev>();
  SetFunctionTable<LambdaKev>();
  SetFunctionTable<ArgumentFrameKev>();
  SetFunctionTable<LocalVarFrameKev>();
  SetFunctionTable<FreeVarFrameKev>();
  SetFunctionTable<StackFrameKev>();
  SetFunctionTable<PairKev>();
}

KevesBase::~KevesBase() {
  thread_pool_.waitForDone();
  for (auto library : library_list_) delete library;
}

void KevesBase::RunThread() {
  KevesVM* vm(KevesVM::Make(this, default_result_field()));
  thread_pool_.start(vm);
}

void KevesBase::InitCMDTable() {
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
  SET_FUNCTION_TO_TABLE(TEST0);
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
  SET_FUNCTION_TO_TABLE(TEST_QEV_EQUAL);

  // for numbers
  SET_FUNCTION_TO_TABLE(TEST_NUMBER);
  SET_FUNCTION_TO_TABLE(TEST_NUMBER0);
  SET_FUNCTION_TO_TABLE(TEST_NUMBER0_R);

  SET_FUNCTION_TO_TABLE(TEST_NUMBER_EQUAL);

  // for char
  SET_FUNCTION_TO_TABLE(TEST_CHAR);
  SET_FUNCTION_TO_TABLE(TEST_CHAR0);
  SET_FUNCTION_TO_TABLE(TEST_CHAR_EQUAL);

  // for symbols
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL);
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL0);
  SET_FUNCTION_TO_TABLE(TEST_SYMBOL_EQUAL);

  // for strings
  SET_FUNCTION_TO_TABLE(STRING_CLONE);
  SET_FUNCTION_TO_TABLE(STRING_APPEND);
  SET_FUNCTION_TO_TABLE(TEST_STRING);
  SET_FUNCTION_TO_TABLE(TEST_STRING0);
  SET_FUNCTION_TO_TABLE(TEST_STRING_EQUAL);
  CHECK_SUM(TEST_STRING_EQUAL);

  // for pairs
  SET_FUNCTION_TO_TABLE(CONS);
  SET_FUNCTION_TO_TABLE(CONS_CONSTANT);
  SET_FUNCTION_TO_TABLE(CAR);
  SET_FUNCTION_TO_TABLE(CAR0);
  SET_FUNCTION_TO_TABLE(CDR);
  SET_FUNCTION_TO_TABLE(CDR0);
  SET_FUNCTION_TO_TABLE(TEST_PAIR);
  SET_FUNCTION_TO_TABLE(TEST_PAIR0);
  SET_FUNCTION_TO_TABLE(TEST_NULL);
  SET_FUNCTION_TO_TABLE(TEST_NULL0);

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

void KevesBase::InitLibraryList() {
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

const StringKev* KevesBase::GetMesgText(const QString& key) const {
  return builtin_.GetMesgText(key);
}

void KevesBase::AppendObjectList(QList<const Kev*>* list, const Kev* kev) {
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
      
void KevesBase::AddLibrary(KevesLibrary* library) {
  library_list_.append(library);
}

bool KevesBase::Match(const QStringList& list1, const QStringList& list2) {
  int size(list1.size());

  if (list2.size() != size) return false;

  for (int i(0); i < size; ++i)
    if (list1.at(i).compare(list2.at(i)) != 0) return false;

  return true;
}

KevesLibrary* KevesBase::GetLibrary(const QStringList& id) {
  Q_ASSERT(id.size() > 0);

  QMutexLocker locker(&mutex_);
  
  for (auto lib : library_list_)
    if (lib->Match(id)) return lib;

  for (auto pair : library_name_list_) {
    if (Match(id, pair.first)) {
      QString file_name("lib/keves-base/");
      file_name += pair.second;
      QLibrary lib_bin(file_name);
      typedef KevesLibrary*(*FuncMakeLib)(KevesBase*);
      auto make_lib = reinterpret_cast<FuncMakeLib>(lib_bin.resolve("Make"));
      
      if (!make_lib) {
	std::cerr << "file name: " << qPrintable(file_name)
		  << qPrintable(lib_bin.errorString());
	return nullptr;
      }

      KevesLibrary* library(make_lib(this));
      library->SetID(pair.first);
      AddLibrary(library);
      return library;
    }
  }
      
  int size(id.size());
  std::cerr << "Not found library: (";

  if (size > 0) {
    std::cerr << qPrintable(id.at(0));
    for (int i(1); i < size; ++i) std::cerr << ' ' << qPrintable(id.at(i));
  }

  std::cerr << ")\n";
  return nullptr;
}

void* KevesBase::Alloc(size_t alloc_size) {
  KevesBaseNode node(new char[((alloc_size + sizeof(quintptr) - 1) & ~(sizeof(quintptr) - 1)) + sizeof(KevesPrefix)]);
  shared_list_.Push(node);
  return node.ToPtr();
}

void KevesBase::PushToSharedList(MutableKev* kev) {
  kev->MarkPermanent();
  shared_list_.Push(kev);
}

uioword KevesBase::IndexAddress(const QList<const Kev*>& table,
				KevesValue value) {
  if (!value.IsPtr()) return value.toUIntPtr();
  
  int index(table.indexOf(value.ToPtr()));

  return index < 0 ?
    value.toUIntPtr() : (static_cast<quintptr>(index) << 2 | INDEX);
}

void KevesBase::PushValue(QStack<const Kev*>* pending, KevesValue value) {
  if (value.IsPtr()) pending->push(value.ToPtr());
}

void KevesBase::RevertObjects(const QList<const Kev*>& object_list, int start_pos) {
  for (int i(start_pos); i < object_list.size(); ++i) {
    MutableKevesValue kev(object_list.at(i));
    if (kev.IsPtr()) (*ft_RevertObject_[kev.type()])(object_list, kev);
  }   
}

void KevesBase::RevertValue(const QList<const Kev*>& object_list,
			    KevesValue* value) {
  if (IsIndex(*value)) *value = object_list.at(value->toUIntPtr() >> 2);
}

QString KevesBase::ToString(KevesValue value) const {
  QString str;

  if (value.IsPtr()) ToString_list(&str, value, 0);
  else ToString_element(&str, value);

  return str;
}

void KevesBase::ToString_list(QString* str, KevesValue value, int nest) const {
  if (value.IsPair()) {
    if (nest > 8) {
      str->append("(...)");
      return;
    }
    
    str->append("(");
    KevesValue temp(value);

    for (;;) {
      const PairKev* pair(temp);
      KevesValue car(pair->car());
      ToString_list(str, car, nest + 1);
      temp = pair->cdr();

      if (!temp.IsPair()) {
	if (temp != EMB_NULL) {
	  str->append(" . ");

	  if (temp.IsVector()) ToString_vector(str, temp, nest + 1);
	  else if (temp.IsCode()) ToString_code(str, temp, nest + 1);
	  else ToString_element(str, temp);
	}

	str->append(")");
	return;
      }

      str->append(" ");
    }
  } else if (value.IsVector()) {
    ToString_vector(str, value, nest);
  } else if (value.IsCode()) {
    ToString_code(str, value, nest);
  } else {
    ToString_element(str, value);
  }
}

void KevesBase::ToString_vector(QString* str, KevesValue value, int nest) const {
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

  ToString_list(str, *iter++, nest + 1);

  while (iter != end) {
    str->append(" ");
    ToString_list(str, *iter++, nest + 1);
  }

  str->append(")");
}

void KevesBase::ToString_code(QString* str, KevesValue value, int nest) const {
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

  ToString_list(str, *iter++, nest + 1);

  while (iter != end) {
    str->append("\n      ");
    ToString_list(str, *iter++, nest + 1);
  }

  str->append(")");
}

void KevesBase::ToString_element(QString* str, KevesValue value) const {
  if (value.IsBool()) {
    str->append(value == EMB_TRUE ? "#t" : "#f");
  } else if (value == EMB_NULL) {
    str->append("'()");
  } else if (value.IsInstruct()) {
    KevesInstruct inst(value);
    str->append(instruct_table()->GetLabel(inst));
  } else if (value.IsFixnum()) {
    KevesFixnum num(value);
    str->append(QString::number(num));
  } else if (value.IsChar()) {
    KevesChar chr(value);
    str->append("#\\").append(chr);
  } else if (value.IsPtr()) {
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
      str->append('"').append(string->ToQString()).append('"');
      break;
    }
      
    case SYMBOL: {
      const SymbolKev* symbol(value);
      str->append(symbol->ToQString());
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
      const LocalVarFrameKev* envp(value);
      str->append("<envp: ").append(QString::number(envp->size())).append('>');
      break;
    }
      
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
      
    case CPS: {
      const CPSKev* cps(value);
      if (cps->who().IsPtr() && cps->who().IsString()) {
	const StringKev* who(cps->who());
	str->append("<procedure: cps (")
	  .append(who->ToQString())
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
      
    case STACK_FRAME: {
      const StackFrameKev* stack(value);
      int cnt_clsr(0);
      int cnt_fp(0);

      for (const FreeVarFrameKev* clsr(stack->clsr()); clsr; clsr = clsr->next())
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
      str->append(QString::number(reinterpret_cast<quintptr>(value.ToPtr()), 16));
    }
  } else if (value == EMB_UNDEF) {
    str->append("<undef>");
  } else {
    str->append("Unknown");
  }
}
