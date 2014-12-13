// keves/keves_builtin_values.cpp - built-in values of Keves
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


#include "keves_builtin_values.hpp"

#include <iostream>
#include <QFile>
#include "keves_base.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"
#include "value/instruct.hpp"


#define DEFINE_MESG_KEY(mesg) \
  const char KevesBuiltinValues::mesg_ ## mesg[] = #mesg

DEFINE_MESG_KEY(Req0);
DEFINE_MESG_KEY(Req1Got0);
DEFINE_MESG_KEY(Req1GotMore);
DEFINE_MESG_KEY(Req1Or2Got0);
DEFINE_MESG_KEY(Req1Or2GotMore);
DEFINE_MESG_KEY(Req1OrMoreGot0);
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
DEFINE_MESG_KEY(ReqRealNumAs1st);
DEFINE_MESG_KEY(ReqRealNumAs2nd);
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
DEFINE_MESG_KEY(ReqListAsLast);
DEFINE_MESG_KEY(ReqProperList);
DEFINE_MESG_KEY(ReqVector);
DEFINE_MESG_KEY(ReqVectorAs1st);
DEFINE_MESG_KEY(ReqProc);
DEFINE_MESG_KEY(ReqProcAs1st);
DEFINE_MESG_KEY(ReqProcAs2nd);
DEFINE_MESG_KEY(ReqProcAs3rd);
DEFINE_MESG_KEY(1stObjNotProcOrSyn);
DEFINE_MESG_KEY(OutOfRange);
DEFINE_MESG_KEY(Div0);

#undef DEFINE_MESG_KEY


KevesBuiltinValues::KevesBuiltinValues()
  : mesg_text_(),
    sym_eval_(),
    builtin_code_(),
    code_APPLY_(),
    code_HALT_(),
    code_REMOVE_DYNAMIC_WIND_(),
    code_REVERT_DYNAMIC_WIND_and_APPLY_(),
    code_POP_APPLY_(),
    code_POP_RETURN_(),
    code_SET_DYNAMIC_WIND_(),
    code_APPLY_MULT_APPLY_(),
    amp_assert_("&assert"),
    amp_lexical_("&lexical"),
    amp_irritants_("&irritants"),
    amp_message_("&message"),
    amp_syntax_("&syntax"),
    amp_who_("&who") {}

void KevesBuiltinValues::Init(KevesBase* base) {
  InitMesgList("conf/mesg_text.csv", base);

  sym_eval_ = SymbolKev::Make(base, "eval");

  builtin_code_ = CodeKev::Make(base, 11);
  {
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
    code_SET_DYNAMIC_WIND_ = iter;
    *iter++ = KevesInstruct(CMD_SET_DYNAMIC_WIND);
    code_APPLY_MULT_APPLY_ = iter;
    *iter++ = KevesInstruct(CMD_APPLY_MULT);
    *iter++ = KevesInstruct(CMD_APPLY);
    Q_ASSERT(iter <= builtin_code_->end());
  }
}

void KevesBuiltinValues::InitMesgList(const QString& file_name, KevesBase* base) {
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

    mesg_text_.insert(key, StringKev::Make(base, value));
  }

  mesg_file.close();
}

const StringKev* KevesBuiltinValues::GetMesgText(const QString& key) const {
  return mesg_text_.value(key);
}
