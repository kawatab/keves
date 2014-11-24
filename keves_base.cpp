// Keves/keves_base.cpp - base of Keves
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

#include <QDataStream>
#include <QFile>
#include "bignum.hpp"
#include "keves_char.hpp"
#include "keves_instruct.hpp"
#include "keves_vm.hpp"
#include "number_kev.hpp"
#include "pair_kev.hpp"
#include "reference_kev.hpp"
#include "string_kev.hpp"
#include "symbol_kev.hpp"
#include "vector_kev.hpp"
#include "wind_kev.hpp"

#define DEFINE_MESG_KEY(mesg) const char KevesBase::mesg_ ## mesg[] = #mesg
DEFINE_MESG_KEY(Req0);
DEFINE_MESG_KEY(Req1Got0);
DEFINE_MESG_KEY(Req1GotMore);
DEFINE_MESG_KEY(Req2Got0);
DEFINE_MESG_KEY(Req2Got1);
DEFINE_MESG_KEY(Req2GotMore);
DEFINE_MESG_KEY(Req2OrMoreGot0);
DEFINE_MESG_KEY(Req2OrMoreGot1);
DEFINE_MESG_KEY(Req3Got0);
DEFINE_MESG_KEY(Req3Got1);
DEFINE_MESG_KEY(Req3Got2);
DEFINE_MESG_KEY(Req3GotMore);
DEFINE_MESG_KEY(ReqNotGet3Args);
DEFINE_MESG_KEY(ReqMoreGotLess);
DEFINE_MESG_KEY(ReqLessGotMore);
DEFINE_MESG_KEY(ReqNum);
DEFINE_MESG_KEY(ReqRealNum);
DEFINE_MESG_KEY(ReqIntNum);
DEFINE_MESG_KEY(ReqIntNumAs1st);
DEFINE_MESG_KEY(ReqIntNumAs2nd);
DEFINE_MESG_KEY(ReqIntNumAs3rd);
DEFINE_MESG_KEY(ReqChar);
DEFINE_MESG_KEY(ReqCharAs1st);
DEFINE_MESG_KEY(ReqCharAs2nd);
DEFINE_MESG_KEY(ReqStr);
DEFINE_MESG_KEY(ReqStrAs1st);
DEFINE_MESG_KEY(ReqSym);
DEFINE_MESG_KEY(ReqPair);
DEFINE_MESG_KEY(1stObjNotProcOrSyn);
#undef DEFINE_MESG_KEY

KevesBase::KevesBase()
  : gc_(10),
    cmd_table_(),
    mesg_text_(),
    sym_eval_(),
    builtin_code_(),
    code_APPLY_(),
    code_HALT_(),
    code_REMOVE_DYNAMIC_WIND_(),
    code_REVERT_DYNAMIC_WIND_and_APPLY_(),
    code_POP_APPLY_(),
    code_POP_RETURN_(),
    lib_keves_base_() {

  InitCMDTable();
  InitMesgList("mesg_text.csv");

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

  sym_eval_ = SymbolKev::Make(&gc_, "eval");

  constexpr int CODE_SIZE(8);
  builtin_code_ = CodeKev::make(&gc_, CODE_SIZE);
  KevesIterator iter(builtin_code_->begin());
  code_HALT_ = iter;
  *iter++ = KevesInstruct(CMD_HALT);
  code_POP_APPLY_ = iter;
  *iter++ = KevesInstruct(CMD_POP);
  code_APPLY_ = iter;
  *iter++ = KevesInstruct(CMD_APPLY);
  code_POP_RETURN_ = iter;
  *iter++ = KevesInstruct(CMD_POP);
  *iter++ = KevesInstruct(CMD_RETURN);
  code_REVERT_DYNAMIC_WIND_and_APPLY_ = iter;
  *iter++ = KevesInstruct(CMD_REVERT_DYNAMIC_WIND);
  *iter++ = KevesInstruct(CMD_APPLY);
  code_REMOVE_DYNAMIC_WIND_ = iter;
  *iter++ = KevesInstruct(CMD_REMOVE_DYNAMIC_WIND);
  Q_ASSERT(iter <= builtin_code_->begin() + CODE_SIZE);

  lib_keves_base_.Init(&gc_);
}

KevesBase::~KevesBase() {
  gc_.Reset();
}

KevesVM* KevesBase::MakeVM() {
  KevesVM* vm(new KevesVM(this, default_result_field()));
  gc_.Append(vm);
  return vm;
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

void KevesBase::InitMesgList(const QString& file_name) {
  QFile mesg_file(file_name);

  if (!mesg_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cerr << "Not found `mesg_text.csv'\n";
    return;
  }
  
  QString temp, key, value;
  int line_num(0);

  while (!mesg_file.atEnd()) {
    temp = mesg_file.readLine();
    ++line_num;
    int pos(temp.indexOf('\t'));
    int size(temp.length());

    if (size == 0) continue;

    if (pos < 0 || pos == size - 1) {
      std::cerr << "Error: There is no value in line: " << line_num
		<< ", mesg_text.csv.\n";
      continue;
    }

    key = temp.left(pos).trimmed();
    value = temp.mid(pos + 1).trimmed();

    if (key.isEmpty()) {
      std::cerr << "Error: There is no key in line: " << line_num
		<< ", mesg_text.csv.\n";
      continue;
    }

    if (value.isEmpty()) {
      std::cerr << "Error: There is no value in line: " << line_num
		<< ", mesg_text.csv.\n";
      continue;
    }

    mesg_text_.insert(key, StringKev::Make(&gc_, value));
  }

  mesg_file.close();
}

const StringKev* KevesBase::GetMesgText(const QString& key) const {
  return mesg_text_.value(key);
}

const QList<const Kev*> KevesBase::GetObjectList(const Kev* kev) {
  QList<const Kev*> list;
  QStack<const Kev*> pending;
  pending.push(kev);

  while (!pending.isEmpty()) {
    const Kev* temp(pending.pop());

    if (list.indexOf(temp) < 0) {
      (*ft_PushChildren_[temp->type()])(&pending, temp);
      list.append(temp);
    }
  }

  return list;
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

void KevesBase::RevertObjects(const QList<Kev*>& object_list) {
  for (int i(0); i < object_list.size(); ++i) {
    MutableKevesValue kev(object_list.at(i));
    (*ft_RevertObject_[kev.type()])(object_list, kev);
  }   
}

void KevesBase::RevertValue(const QList<Kev*>& object_list,
				KevesValue* value) {
  if (IsIndex(*value)) *value = object_list.at(value->toUIntPtr() >> 2);
}

QString KevesBase::ToString(KevesValue value) {
  QString str;

  if (value.IsPtr()) ToString_list(&str, value, 0);
  else ToString_element(&str, value);

  return str;
}

void KevesBase::ToString_list(QString* str, KevesValue value, int nest) {
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

void KevesBase::ToString_vector(QString* str, KevesValue value, int nest) {
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

void KevesBase::ToString_code(QString* str, KevesValue value, int nest) {
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

void KevesBase::ToString_element(QString* str, KevesValue value) {
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
