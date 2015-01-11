// keves/kevc_generator/code_keves-base.cpp - a test code for kevc_generator
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

#include "kevc_generator.hpp"
#include "keves_common.hpp"
#include "kev/code.hpp"
#include "kev/environment.hpp"
#include "kev/frame.hpp"
#include "kev/pair.hpp"
#include "kev/procedure.hpp"
#include "kev/string.hpp"
#include "value/char.hpp"
#include "value/fixnum.hpp"


void Code_KevesBase::write(KevcGenerator* generator) {
  const LambdaKev* proc_expand(makeExpand(generator));
  const CodeKev* code_evaluate(makeEvaluate(generator, proc_expand));
  const EnvironmentKev* environment(makeEnvironment(generator));

  generator->exportBind("my-code", code_evaluate);
  generator->exportBind("environment", environment);

  generator->writeToFile();
}

const CodeKev* Code_KevesBase::makeEvaluate(KevcGenerator* generator,
					    const LambdaKev* proc_expand) {
  // import binds
  if (!generator->setImportLibrary("keves", "base-bin", 6)) return nullptr;
  KevesValue proc_display(generator->importBind("display"));
  KevesValue proc_newline(generator->importBind("newline"));
 
  if (!generator->setImportLibrary("rnrs", "base", 6)) return nullptr;
  KevesValue proc_append(generator->importBind("append"));
  KevesValue proc_list_to_vector(generator->importBind("list->vector"));
 
  if (!generator->setImportLibrary("keves", "parse", 0, 1)) return nullptr;
  KevesValue proc_keves_parser(generator->importBind("keves-parser"));


  CodeKev* code(generator->makeCode(37));
  {
    KevesIterator iter(code->begin());
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(29);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(19);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_list_to_vector;
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(14);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_append;
	  {
	    *iter++ = KevesInstruct(CMD_FRAME_R);
	    *iter++ = KevesFixnum(9);
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = proc_expand;
	    {
	      *iter++ = KevesInstruct(CMD_FRAME_R);
	      *iter++ = KevesFixnum(4);
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = proc_keves_parser;
	      *iter++ = KevesInstruct(CMD_PUSH);
	      *iter++ = KevesInstruct(CMD_APPLY);
	    }
	    *iter++ = KevesInstruct(CMD_APPLY);
	  }
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_LAST);
      *iter++ = KevesInstruct(CMD_POP);
      *iter++ = KevesInstruct(CMD_TERMINATE_EXPAND);
      *iter++ = KevesInstruct(CMD_BEGIN);
      *iter++ = KevesInstruct(CMD_RETURN0);
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    *iter++ = KevesInstruct(CMD_HALT); // jump02

    Q_ASSERT(iter <= code->end());
  }

  return code;
}

const LambdaKev* Code_KevesBase::makeExpand(KevcGenerator* generator) {
  // import binds
  if (!generator->setImportLibrary("keves", "base-bin", 6)) return nullptr;
  KevesValue proc_display(generator->importBind("display"));
  KevesValue proc_newline(generator->importBind("newline"));
 
  if (!generator->setImportLibrary("rnrs", "base", 6)) return nullptr;
  KevesValue proc_append(generator->importBind("append"));
  KevesValue proc_car(generator->importBind("car"));
  KevesValue proc_cadr(generator->importBind("cadr"));
  KevesValue proc_cons(generator->importBind("cons"));
  KevesValue proc_eqv_q(generator->importBind("eqv?"));
  KevesValue proc_eq_q(generator->importBind("eq?"));
  KevesValue proc_list(generator->importBind("list"));
  KevesValue proc_map(generator->importBind("map"));
  KevesValue proc_make_string(generator->importBind("make-string"));
  KevesValue proc_string_append(generator->importBind("string-append"));
 
  if (!generator->setImportLibrary("rnrs", "unicode", 6)) return nullptr;
  KevesValue proc_char_upcase(generator->importBind("char-upcase"));
  KevesValue proc_char_upper_case_q(generator->importBind("char-upper-case?"));
  KevesValue proc_char_downcase(generator->importBind("char-downcase"));
 
  if (!generator->setImportLibrary("rnrs", "lists", 6)) return nullptr;
  KevesValue proc_assq(generator->importBind("assq"));
  KevesValue proc_fold_left(generator->importBind("fold-left"));

 
  // values
  KevesValue list_HALT(generator->makePair(KevesInstruct(CMD_HALT), EMB_NULL));
  KevesValue list_APPLY_HALT(generator->makePair(KevesInstruct(CMD_APPLY), list_HALT));
  KevesValue list_PUSH_CONSTANT(generator->makePair(KevesInstruct(CMD_PUSH_CONSTANT), EMB_NULL));
  KevesValue list_display_PUSH_CONSTANT(generator->makePair(proc_display, list_PUSH_CONSTANT));
  KevesValue list_PUSH_CONSTANT_display_PUSH_CONSTANT(generator->makePair(KevesInstruct(CMD_PUSH_CONSTANT), list_display_PUSH_CONSTANT));
  KevesValue list_5_PUSH_CONSTANT_display_PUSH_CONSTANT(generator->makePair(KevesFixnum(5), list_PUSH_CONSTANT_display_PUSH_CONSTANT));
  KevesValue list_FRAME_R_5_PUSH_CONSTANT_display_PUSH_CONSTANT(generator->makePair(KevesInstruct(CMD_FRAME_R), list_5_PUSH_CONSTANT_display_PUSH_CONSTANT));
  KevesValue list_0_FRAME_R_5_PUSH_CONSTANT_display_PUSH_CONSTANT(generator->makePair(KevesFixnum(0), list_FRAME_R_5_PUSH_CONSTANT_display_PUSH_CONSTANT));

  CodeKev* code(generator->makeCode(17));
  {
    KevesIterator iter(code->begin());

    *iter++ = KevesInstruct(CMD_LAST);
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(13);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_append;
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = list_0_FRAME_R_5_PUSH_CONSTANT_display_PUSH_CONSTANT;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(6);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_cons;
	*iter++ = KevesInstruct(CMD_PUSH);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = list_APPLY_HALT;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    *iter++ = KevesInstruct(CMD_RETURN0);

    Q_ASSERT(iter <= code->end());
  }

  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  return generator->makeLambda(free_vars, code, 0);
}

const EnvironmentKev* Code_KevesBase::makeEnvironment(KevcGenerator* generator) {
  return generator->makeEnvironment(EMB_NULL);
}
