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
#include "kev/pair.hpp"
#include "kev/string.hpp"
#include "value/char.hpp"
#include "value/fixnum.hpp"


void Code_KevesBase::write(KevcGenerator* generator) {
  // import binds
  if (!generator->setImportLibrary("keves", "base-bin", 6)) return;
  KevesValue proc_display(generator->importBind("display"));
  KevesValue proc_newline(generator->importBind("newline"));
 
  if (!generator->setImportLibrary("rnrs", "base", 6)) return;
  KevesValue proc_append(generator->importBind("append"));
  KevesValue proc_car(generator->importBind("car"));
  KevesValue proc_cadr(generator->importBind("cadr"));
  KevesValue proc_cons(generator->importBind("cons"));
  KevesValue proc_eqv_q(generator->importBind("eqv?"));
  KevesValue proc_eq_q(generator->importBind("eq?"));
  KevesValue proc_list(generator->importBind("list"));
  KevesValue proc_list_to_vector(generator->importBind("list->vector"));
  KevesValue proc_map(generator->importBind("map"));
  KevesValue proc_make_string(generator->importBind("make-string"));
  KevesValue proc_string_append(generator->importBind("string-append"));
 
  if (!generator->setImportLibrary("rnrs", "unicode", 6)) return;
  KevesValue proc_char_upcase(generator->importBind("char-upcase"));
  KevesValue proc_char_upper_case_q(generator->importBind("char-upper-case?"));
  KevesValue proc_char_downcase(generator->importBind("char-downcase"));
 
  if (!generator->setImportLibrary("rnrs", "lists", 6)) return;
  KevesValue proc_assq(generator->importBind("assq"));
  KevesValue proc_fold_left(generator->importBind("fold-left"));

  if (!generator->setImportLibrary("keves", "parse", 0, 1)) return;
  KevesValue proc_keves_parser(generator->importBind("keves-parser"));

  // values
  KevesValue list01(generator->makePair(KevesChar('B'), EMB_NULL));
  KevesValue list02(generator->makePair(KevesChar('a'), list01));
  KevesValue str1(generator->makeString("ABC"));
  KevesValue str2(generator->makeString("def"));
  KevesValue str3(generator->makeString("xyz"));
  KevesValue list03(generator->makePair(str1, KevesChar('A')));
  KevesValue list04(generator->makePair(str2, KevesChar('d')));
  KevesValue list05(generator->makePair(str3, KevesChar('x')));
  KevesValue list06(generator->makePair(list05, EMB_NULL));
  KevesValue list07(generator->makePair(list04, list06));
  KevesValue list08(generator->makePair(list03, list07));
  KevesValue list09(generator->makePair(KevesInstruct(CMD_HALT), EMB_NULL));
  KevesValue list10(generator->makePair(KevesInstruct(CMD_APPLY), list09));
  KevesValue list11(generator->makePair(KevesInstruct(CMD_PUSH_CONSTANT), EMB_NULL));
  KevesValue list12(generator->makePair(proc_display, list11));
  KevesValue list13(generator->makePair(KevesInstruct(CMD_PUSH_CONSTANT), list12));
  KevesValue list14(generator->makePair(KevesFixnum(5), list13));
  KevesValue list15(generator->makePair(KevesInstruct(CMD_FRAME_R), list14));
  KevesValue list16(generator->makePair(KevesFixnum(0), list15));
  KevesValue str4(generator->makeString("(define (fib n) \
  (if (< n 2) \
      1 \
    (+ (fib (- n 2)) (fib (- n 1))))) \
(list `(abc) (quote (123 \"abc\")))"));

  CodeKev* code(generator->makeCode(226));
  {
    KevesIterator iter(code->begin());
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(15); // jump01
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(10); // jump03
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_char_downcase;
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(5); // jump04
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_cadr;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list02;
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_APPLY); // jump04
      }
      *iter++ = KevesInstruct(CMD_APPLY); // jump03
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R); // jump05
      *iter++ = KevesFixnum(3); // jump02
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R); // jump01
      *iter++ = KevesFixnum(17); // jump05
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(12); // jump06
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_append;
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(7); // jump07
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_append;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list02;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list02;
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_APPLY); // jump07
      }
      *iter++ = KevesInstruct(CMD_APPLY); // jump06
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R); // jump05
      *iter++ = KevesFixnum(3); // jump02
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(14);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(9);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_string_append;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str2;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str3;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_eqv_q;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	// *iter++ = proc_eq_q;
	*iter++ = proc_cons;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str1;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str2;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_map;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_char_upper_case_q;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = list02;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_assq;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = str3;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = list08;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(14);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(9);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_fold_left;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_cons;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = EMB_NULL;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = list08;
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(12);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(7);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_make_string;
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = KevesFixnum(5);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = KevesChar('X');
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(3);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_newline;
      *iter++ = KevesInstruct(CMD_APPLY);
    }

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(34);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_display;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(24);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_list_to_vector;
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(19);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_append;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = list16;
	  {
	    *iter++ = KevesInstruct(CMD_FRAME_R);
	    *iter++ = KevesFixnum(12);
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = proc_cons;
	    {
	      *iter++ = KevesInstruct(CMD_FRAME_R);
	      *iter++ = KevesFixnum(5);
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = proc_keves_parser;
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = str4;
	      *iter++ = KevesInstruct(CMD_APPLY);
	    }
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = list10;
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

  // export binds
  generator->exportBind("my-code", code);

  generator->writeToFile();
}
