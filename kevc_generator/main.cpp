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


#include "code_keves-base.hpp"
#include "code_keves-parse.hpp"
#include "code_rnrs-base.hpp"
#include "code_rnrs-exceptions.hpp"
#include "code_rnrs-lists.hpp"
#include "code_rnrs-mutable-strings.hpp"
#include "code_rnrs-unicode.hpp"
#include "kevc_generator.hpp"
#include "keves_common.hpp"
#include "test_code.hpp"


int main() {
  KevesCommon common;

  {
    KevcGenerator generator(&common, "test02.kevc", "main");
    TestCode::Code02::write(&generator);
  }

  {
    KevcGenerator generator(&common, "test03.kevc", "main");
    TestCode::Code03::write(&generator);
  }

  {
    KevcGenerator generator(&common, "lib/rnrs/exceptions.kevc",
			    "keves", "exceptions", 6);

    Code_RnrsExceptions::write(&generator);
  }


  {
    KevcGenerator generator(&common, "lib/rnrs/lists.kevc",
			    "rnrs", "lists", 6);

    Code_RnrsLists::write(&generator);
  }

  {
    KevcGenerator generator(&common, "lib/rnrs/mutable-strings.kevc",
			    "rnrs", "mutable-strings", 6);

    Code_RnrsMutableStrings::write(&generator);
  }

  {
    KevcGenerator generator(&common, "lib/rnrs/unicode.kevc",
			    "rnrs", "unicode", 6);

    Code_RnrsUnicode::write(&generator);
  }

  {
    KevcGenerator generator(&common, "lib/rnrs/base.kevc",
			    "rnrs", "base", 6);

    Code_RnrsBase::write(&generator);
  }

  {
    KevcGenerator generator(&common, "lib/keves/parse.kevc",
			    "keves", "parse", 0, 1);

    Code_KevesParse::write(&generator);
  }

  {
    KevcGenerator generator(&common, "lib/keves/base.kevc",
			    "keves", "base", 0, 1);

    Code_KevesBase::write(&generator);
  }

  KevcGenerator::testRead(&common, "test02.kevc");
  KevcGenerator::testRead(&common, "test03.kevc");
  KevcGenerator::testRead(&common, "lib/rnrs/exceptions.kevc");
  KevcGenerator::testRead(&common, "lib/rnrs/lists.kevc");
  KevcGenerator::testRead(&common, "lib/rnrs/mutable-strings.kevc");
  KevcGenerator::testRead(&common, "lib/rnrs/unicode.kevc");
  KevcGenerator::testRead(&common, "lib/rnrs/base.kevc");
  KevcGenerator::testRead(&common, "lib/keves/parse.kevc");
  KevcGenerator::testRead(&common, "lib/keves/base.kevc");

  return 0;
}
