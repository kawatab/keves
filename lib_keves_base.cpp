/* Keves/lib_keves_base.cpp - library of Keves base
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


#include "lib_keves_base.hpp"

#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>

#include "code_kev.hpp"
#include "condition_kev.hpp"
#include "jump_kev.hpp"
#include "number_kev.hpp"
#include "procedure_kev.hpp"
#include "keves_char.hpp"
#include "keves_vm.hpp"
#include "record_kev.hpp"
#include "symbol_kev.hpp"

LibKevesBase::LibKevesBase()
  : test_code_(),
    table_(),
    environment_base_(table_) {}

KevesLibrary* LibKevesBase::Init(KevesGC* gc) {
  std::cout << "LibKevesBase::Init()" << std::endl;

  test_code_ = CodeKev::make(gc, 3);
  KevesIterator iter(test_code_->begin());
  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
  *iter++ = EMB_NULL;
  *iter++ = KevesInstruct(CMD_HALT);

  Q_ASSERT(test_code_->type() == CODE);
  Q_ASSERT(test_code_->begin()->IsInstruct());
  
  // std::cout << "LibKevesBase: used: "
  // << (code_itr - builtin_code_->begin()) << std::endl;
  std::cout << "LibKevesBase: total: "
	    << SIZE_OF_BUILTIN_CODE / sizeof(KevesValue) << std::endl;

  return this;
}

KevesValue LibKevesBase::name() {
  return &pair_for_name1_;
}

EnvironmentKev* LibKevesBase::environment_base() {
  return &environment_base_;
}

const KevesValue* LibKevesBase::table() const {
  return table_;
}
