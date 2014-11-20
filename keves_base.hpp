/* Keves/keves_base_kev.hpp - base of Keves
 * Keves will be an R6RS Scheme implementation.
 *
 *  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include <QHash>
#include <QList>
#include "keves_gc.hpp"
#include "keves_vm.hpp"
#include "lib_keves_base.hpp"


class QString;

class KevesBase {
public:
  static const char mesg_ReqRealNum[];
  static const char mesg_1stObjNotProcOrSyn[];
  static const char mesg_ReqLessGotMore[];
  static const char mesg_ReqMoreGotLess[];

  KevesBase();
  KevesBase(const KevesBase&) = delete;
  KevesBase(const KevesBase&&) = delete;
  KevesBase& operator=(const KevesBase&) = delete;
  KevesBase& operator=(const KevesBase&&) = delete;
  ~KevesBase();

  KevesVM* MakeVM();
  
public:
  const StringKev* GetMesgText(const QString& key) const;
  void InitCMDTable();
  
  KevesGC* gc() {
    return &gc_;
  }

  LibKevesBase* lib_keves_base() {
    return &lib_keves_base_;
  }
  
  /*
  const KevesInstructTable* keves_instruct_table() {
    return &keves_instruct_table_;
  }
  */


private:
  void InitMesgList(const QString& file_name);

  // GC and tables
  KevesGC gc_;
  KevesInstructTable keves_instruct_table_;
  QHash<QString, const StringKev*> mesg_text_;

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
};
