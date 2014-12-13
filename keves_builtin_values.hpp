// keves/keves_builtin_values.hpp - built-in values of Keves
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


#pragma once

#include <QHash>
#include "keves_iterator.hpp"
#include "kev/record.hpp"


class CodeKev;
class KevesBase;
class KevesGC;
class QString;
class StringKev;
class SymbolKev;

class KevesBuiltinValues {
public:
  KevesBuiltinValues();
  KevesBuiltinValues(const KevesBuiltinValues&) = delete;
  KevesBuiltinValues(const KevesBuiltinValues&&) = delete;
  KevesBuiltinValues& operator=(const KevesBuiltinValues&) = delete;
  KevesBuiltinValues& operator=(const KevesBuiltinValues&&) = delete;
  ~KevesBuiltinValues() = default;

  void Init(KevesBase* base);

  // for messages
  static const char mesg_Req0[];
  static const char mesg_Req1Got0[];
  static const char mesg_Req1GotMore[];
  static const char mesg_Req1Or2Got0[];
  static const char mesg_Req1Or2GotMore[];
  static const char mesg_Req1OrMoreGot0[];
  static const char mesg_Req2Got0[];
  static const char mesg_Req2Got1[];
  static const char mesg_Req2GotMore[];
  static const char mesg_Req2OrMoreGot0[];
  static const char mesg_Req2OrMoreGot1[];
  static const char mesg_Req3Got0[];
  static const char mesg_Req3Got1[];
  static const char mesg_Req3Got2[];
  static const char mesg_Req3GotMore[];
  static const char mesg_ReqNotGet3Args[];
  static const char mesg_ReqMoreGotLess[];
  static const char mesg_ReqLessGotMore[];
  static const char mesg_ReqNum[];
  static const char mesg_ReqRealNum[];
  static const char mesg_ReqRealNumAs1st[];
  static const char mesg_ReqRealNumAs2nd[];
  static const char mesg_ReqIntNum[];
  static const char mesg_ReqIntNumAs1st[];
  static const char mesg_ReqIntNumAs2nd[];
  static const char mesg_ReqIntNumAs3rd[];
  static const char mesg_ReqChar[];
  static const char mesg_ReqCharAs1st[];
  static const char mesg_ReqCharAs2nd[];
  static const char mesg_ReqStr[];
  static const char mesg_ReqStrAs1st[];
  static const char mesg_ReqSym[];
  static const char mesg_ReqPair[];
  static const char mesg_ReqListAsLast[];
  static const char mesg_ReqProperList[];
  static const char mesg_ReqVector[];
  static const char mesg_ReqVectorAs1st[];
  static const char mesg_ReqProc[];
  static const char mesg_ReqProcAs1st[];
  static const char mesg_ReqProcAs2nd[];
  static const char mesg_ReqProcAs3rd[];
  static const char mesg_1stObjNotProcOrSyn[];
  static const char mesg_OutOfRange[];
  static const char mesg_Div0[];

  const StringKev* GetMesgText(const QString& key) const;

  SymbolKev* sym_eval() {
    return sym_eval_;
  }

  // for builtin code
  const_KevesIterator code_APPLY() const {
    return code_APPLY_;
  }
  
  const_KevesIterator code_HALT() const {
    return code_HALT_;
  }
  
  const_KevesIterator code_REMOVE_DYNAMIC_WIND() const {
    return code_REMOVE_DYNAMIC_WIND_;
  }
  
  const_KevesIterator code_REVERT_DYNAMIC_WIND_and_APPLY() const {
    return code_REVERT_DYNAMIC_WIND_and_APPLY_;
  }
  
  const_KevesIterator code_POP_APPLY() const {
    return code_POP_APPLY_;
  }
  
  const_KevesIterator code_POP_RETURN() const {
    return code_POP_RETURN_;
  }

  const_KevesIterator code_SET_DYNAMIC_WIND() const {
    return code_SET_DYNAMIC_WIND_;
  }

  const_KevesIterator code_APPLY_MULT_APPLY() const {
    return code_APPLY_MULT_APPLY_;
  }

  const RecordKev* amp_assert() const {
    return &amp_assert_;
  }
  
  const RecordKev* amp_lexical() const {
    return &amp_lexical_;
  }
  
  const RecordKev* amp_irritants() const {
    return &amp_irritants_;
  }
  
  const RecordKev* amp_message() const {
    return &amp_message_;
  }
  
  const RecordKev* amp_syntax() const {
    return &amp_syntax_;
  }
  
  const RecordKev* amp_who() const {
    return &amp_who_;
  }
  

private:
  void InitMesgList(const QString& file_name, KevesBase* base);

  QHash<QString, const StringKev*> mesg_text_;
  SymbolKev* sym_eval_;
  CodeKev* builtin_code_;
  const_KevesIterator code_APPLY_;
  const_KevesIterator code_HALT_;
  const_KevesIterator code_REMOVE_DYNAMIC_WIND_;
  const_KevesIterator code_REVERT_DYNAMIC_WIND_and_APPLY_;
  const_KevesIterator code_POP_APPLY_;
  const_KevesIterator code_POP_RETURN_;
  const_KevesIterator code_SET_DYNAMIC_WIND_;
  const_KevesIterator code_APPLY_MULT_APPLY_;

  RecordKev amp_assert_;
  RecordKev amp_lexical_;
  RecordKev amp_irritants_;
  RecordKev amp_message_;
  RecordKev amp_syntax_;
  RecordKev amp_who_;
};
