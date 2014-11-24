// Keves/keves_base_kev.hpp - base of Keves
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
#include <QList>
#include <QStack>
#include "keves_gc.hpp"
#include "keves_instruct.hpp"
#include "keves_textual_port.hpp"
#include "lib_keves_base.hpp"


class KevesVM;
class QString;

class KevesBase {
public:
  KevesBase();
  KevesBase(const KevesBase&) = delete;
  KevesBase(const KevesBase&&) = delete;
  KevesBase& operator=(const KevesBase&) = delete;
  KevesBase& operator=(const KevesBase&&) = delete;
  ~KevesBase();

  KevesVM* MakeVM();
  
  // for Message Text
public:
  static const char mesg_Req0[];
  static const char mesg_Req1Got0[];
  static const char mesg_Req1GotMore[];
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
  static const char mesg_1stObjNotProcOrSyn[];

  const StringKev* GetMesgText(const QString& key) const;

private:
  void InitMesgList(const QString& file_name);


  // for indexing of address
public:
  const QList<const Kev*> GetObjectList(const Kev* kev);

  static uioword IndexAddress(const QList<const Kev*>& object_list,
			      KevesValue value);

  template<class KEV>
  static void ReadArray(QDataStream& in, KEV* kev);

  template<class KEV>
  static void RevertArray(const QList<Kev*>& object_list, KEV* kev);
  
  void RevertObjects(const QList<Kev*>& object_list);

  template<class KEV>
  static void RevertValue(const QList<Kev*>& object_list, KEV** kev);
  
  static void RevertValue(const QList<Kev*>& object_list, KevesValue* value);

  template<class KEV>
  static void PushArray(QStack<const Kev*>* pending, KEV* kev);

  template<class KEV>
  static void PushValue(QStack<const Kev*>* pending, KEV* kev);
  
  static void PushValue(QStack<const Kev*>* pending, KevesValue value);

  template<class KEV>
  static void WriteArray(const QList<const Kev*>& list, QDataStream& out, KEV* kev);

  Kev* (*ft_ReadObject(uioword value))(QDataStream&, KevesGC*) {
    return ft_ReadObject_[value];
  }
  
  void (*ft_WriteObject(kev_type type))(const QList<const Kev*>&, QDataStream&, KevesValue) {
    return ft_WriteObject_[type];
  }
  
private:
  static bool IsIndex(Kev* kev) {
    return IsIndex(KevesValue(kev));
  }

  static bool IsIndex(KevesValue value) {
    return (value.toUIntPtr() & ALIGN) == INDEX; // 10
  }
  
  template<class KEV>
  void SetFunctionTable();
  
  
  // for output port
public:
  KevesTextualOutputPort* default_result_field() {
    return &default_result_field_;
  }
  
  // for command table
  void InitCMDTable();
  enum {
    ALIGN = 0x3,
    INDEX = 0x2
  };
  
  vm_func* cmd_table() {
    return cmd_table_;
  }
  
  KevesGC* gc() {
    return &gc_;
  }

  LibKevesBase* lib_keves_base() {
    return &lib_keves_base_;
  }
  
  const KevesInstructTable* instruct_table() {
    return &instruct_table_;
  }

  // General functions
public:
  QString ToString(KevesValue);
  void ToString_code(QString*, KevesValue, int);
  void ToString_element(QString*, KevesValue);
  void ToString_list(QString*, KevesValue, int);
  void ToString_vector(QString*, KevesValue, int);


private:
  // GC and tables
  KevesGC gc_;
  KevesInstructTable instruct_table_;
  vm_func cmd_table_[END_OF_LIST];
  QHash<QString, const StringKev*> mesg_text_;
  KevesTextualOutputPort default_result_field_;

public:
  // variables
  SymbolKev* sym_eval_;
  CodeKev* builtin_code_;
  const_KevesIterator code_APPLY_;
  const_KevesIterator code_HALT_;
  const_KevesIterator code_REMOVE_DYNAMIC_WIND_;
  const_KevesIterator code_REVERT_DYNAMIC_WIND_and_APPLY_;
  const_KevesIterator code_POP_APPLY_;
  const_KevesIterator code_POP_RETURN_;

private:
  // libraries
  LibKevesBase lib_keves_base_;

  void (*ft_PushChildren_[0177])(QStack<const Kev*>*, KevesValue);
  void (*ft_RevertObject_[0177])(const QList<Kev*>&, MutableKevesValue);
  Kev* (*ft_ReadObject_[0177])(QDataStream&, KevesGC*);
  void (*ft_WriteObject_[0177])(const QList<const Kev*>&, QDataStream&, KevesValue);
};
