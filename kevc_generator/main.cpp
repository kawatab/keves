// kevc_generator/main.cpp - generator of binary code for Keves
// Keves will be an R6RS Scheme implementation.
//
// Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
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


#include <iostream>
#include "code_rnrs-base.hpp"
#include "code_rnrs-exceptions.hpp"
#include "code_rnrs-unicode.hpp"
#include "keves_base.hpp"
#include "test_code.hpp"


int main() {
  KevesBase base;

  TestCode::Code02::write(&base, "test02.kevc");
  TestCode::Code02::read(&base, "test02.kevc");

  std::cout << std::endl;
  
  TestCode::Code03::write(&base, "test03.kevc");
  TestCode::Code03::read(&base, "test03.kevc");

  Code_RnrsExceptions::write(&base, "lib/rnrs/exceptions.kevc");
  Code_RnrsExceptions::read(&base, "lib/rnrs/exceptions.kevc");

  Code_RnrsUnicode::write(&base, "lib/rnrs/unicode.kevc");
  Code_RnrsUnicode::read(&base, "lib/rnrs/unicode.kevc");

  Code_RnrsBase::write(&base, "lib/rnrs/base.kevc");
  Code_RnrsBase::read(&base, "lib/rnrs/base.kevc");

  TestCode::KevesBaseCode::write(&base, "lib/keves/base.kevc");
  TestCode::KevesBaseCode::read(&base, "lib/keves/base.kevc");

  return 0;
}
