// keves/kevc_generator/code_rnrs-base.cpp - a code of (rnrs base)
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


#include "code_rnrs-base.hpp"

#include "kevc_generator.hpp"
#include "keves_builtin_values.hpp"
#include "kev/code.hpp"
#include "kev/frame.hpp"
#include "kev/procedure.hpp"
#include "kev/symbol.hpp"
#include "value/fixnum.hpp"
#include "value/instruct.hpp"


void Code_RnrsBase::write(KevcGenerator* generator) {
  // import binds
  if (!generator->setImportLibrary("keves", "base-bin", 0, 1)) return;

  // for invoking makeAssertCondition()
  generator->importBind("&assert");
  generator->importBind("&message");
  generator->importBind("&irritants");
  generator->importBind("&who");
 
  if (!generator->setImportLibrary("rnrs", "exceptions", 6)) return;
  KevesValue proc_raise(generator->importBind("raise"));

  // import and export binds
  if (!generator->setImportLibrary("rnrs", "base-bin", 6)) return;

  // export binds
  const char* binds[] = {
    "abs",
    "+",
    // ADD_BIND("and");
    "angle",
    "apply",
    "assertion-violation",
    // ADD_BIND("begin");
    // ADD_BIND("begin0");
    "boolean?",
    "call/cc",
    "call-with-current-continuation",
    "call-with-values",
    "car",
    "cdr",
    "caar",
    "cadr",
    "cdar",
    "cddr",
    "caaar",
    "caadr",
    "cadar",
    "caddr",
    "cdaar",
    "cdadr",
    "cddar",
    "cdddr",
    "ceiling",
    "char=?",
    "char>?",
    "char>=?",
    "char<?",
    "char<=?",
    "char?",
    "char->integer",
    "complex?",
    "cons",
    "denominator",
    "/",
    "dynamic-wind",
    // ADD_BIND("eq?");
    "=",
    // ADD_BIND("eqv?");
    "exact?",
    "flacos",
    "flasin",
    "flatan",
    "flcos",
    "flexact",
    "flexp",
    "flexpt",
    "fllog",
    "floor",
    "flsin",
    "flsqrt",
    "fltan",
    ">",
    ">=",
    "even?",
    "finite?",
    "imag-part",
    "infinite?",
    "inexact",
    "inexact?",
    "integer?",
    "integer-valued?",
    "integer->char",
    "length",
    "<",
    "<=",
    "list",
    "magnitude",
    "make-polar",
    "make-rectangular",
    "make-string",
    "make-vector",
    "max",
    "min",
    "*",
    "nan?",
    "negative?",
    "not",
    "null?",
    "number?",
    "number->string",
    "numerator",
    "odd?",
    "pair?",
    "positive?",
    "procedure?",
    "rational?",
    "rational-valued?",
    "real-part",
    "real?",
    "real-valued?",
    "reverse",
    "round",
    "string",
    "string-copy",
    "string=?",
    "string>?",
    "string>=?",
    "string-length",
    "string<?",
    "string<=?",
    "string?",
    "string-ref",
    "string->list",
    "string->number",
    "string->symbol",
    "substring",
    "-",
    "symbol=?",
    "symbol?",
    "symbol->string",
    "truncate",
    "values",
    "vector",
    "vector-length",
    "vector?",
    "vector-ref",
    "vector-set!",
    "vector->list",
    "list->vector",
    "zero?"
  };

  for (auto bind : binds)
    generator->exportBind(bind, generator->importBind(bind));

  generator->exportBind("append",
			makeCode_proc_append(generator));

  generator->exportBind("string-append",
			makeCode_proc_string_append(generator));

  generator->exportBind("map",
			makeCode_proc_map(generator, proc_raise));

  generator->exportBind("eqv?", makeCode_proc_eqv_q(generator));
  generator->exportBind("eq?", makeCode_proc_eq_q(generator));
  
  generator->writeToFile();
}

KevesValue Code_RnrsBase::makeCode_proc_append(KevcGenerator* generator) {
  SymbolKev* sym_append(generator->makeSymbol("append"));

  KevesValue err_req_proper_list(generator->makeAssertCondition(sym_append,
								KevesBuiltinValues::mesg_ReqProperList,
								EMB_NULL));
      
  KevesValue err_req_pair(generator->makeAssertCondition(sym_append,
							 KevesBuiltinValues::mesg_ReqPair,
							 EMB_NULL));
  
  CodeKev* code(generator->makeCode(79));
  {
    KevesIterator iter(code->begin());
    *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesFixnum(3); // argn0 --->
    *iter++ = KevesFixnum(5); // argn1 --->
    *iter++ = KevesFixnum(12); // argn2 --->
    
    { // no arguments
      *iter++ = KevesInstruct(CMD_RETURN_CONSTANT); // <--- argn0
      *iter++ = EMB_NULL;
    }
    
    { // an argument
      *iter++ = KevesInstruct(CMD_TEST_PAIR0_R); // <--- argn1
      *iter++ = KevesFixnum(1); // argument is not pair --->
      { // argument is pair
	*iter++ = KevesInstruct(CMD_RETURN0);
      }
      
      { // argument is not pair
	*iter++ = KevesInstruct(CMD_TEST_NULL0_R); // <--- argument is not pair
	*iter++ = KevesFixnum(64); // error --->
	{ // argument is null
	  *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	  *iter++ = EMB_NULL;
	}
      }
    }
    
    { // more than 2 arguments
      *iter++ = KevesInstruct(CMD_TEST_PAIR0_R); // <--- argn2
      *iter++ = KevesFixnum(20); // skip1 --->
      { // 2nd is list
	*iter++ = KevesInstruct(CMD_NEW_BOX);
	*iter++ = KevesFixnum(2);
	{ // loop
	  *iter++ = KevesInstruct(CMD_LAST); // <--- loop
	  *iter++ = KevesInstruct(CMD_POP);
	  *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
	  *iter++ = KevesFixnum(0);
	  *iter++ = KevesInstruct(CMD_LAST);
	  *iter++ = KevesInstruct(CMD_POP);
	  *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
	  *iter++ = KevesFixnum(1);
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(2); // skip2 --->
	  {
	    *iter++ = KevesInstruct(CMD_JUMP_R);
	    *iter++ = KevesFixnum(14); // append 2 list --->
	  }
	  *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R); // <--- skip2
	  *iter++ = KevesFixnum(2);
	  *iter++ = KevesFixnum(3); // skip3 --->
	  *iter++ = KevesFixnum(3); // skip3 --->
	  *iter++ = KevesFixnum(-14); // loop --->
	}
      }
      
      { // terminate
	*iter++ = KevesInstruct(CMD_RETURN0); // <--- skip3
      }
      
      { // 2nd is not list
	*iter++ = KevesInstruct(CMD_TEST_NULL0_R); // <--- skip1
	*iter++ = KevesFixnum(38); // error
	{ // 2nd is null
	  *iter++ = KevesInstruct(CMD_POP);
	  *iter++ = KevesInstruct(CMD_JUMP_R);
	  *iter++ = KevesFixnum(-41); // restart --->
	}
      }
    }
    
    { // not list or null
      *iter++ = KevesInstruct(CMD_CONSTANT); // <---
      *iter++ = err_req_pair;
      *iter++ = KevesInstruct(CMD_RAISE);
    }

    *iter++ = KevesInstruct(CMD_REFER_LOCAL); // <--- append 2 lists
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_TEST_PAIR_R);
    *iter++ = KevesFixnum(23); // not pair --->
    { // local variable is pair
      { // expand list to frame
	*iter++ = KevesInstruct(CMD_PUSH); // loop1 <---
	*iter++ = KevesInstruct(CMD_CAR);
	*iter++ = KevesInstruct(CMD_PUSH);
	*iter++ = KevesInstruct(CMD_ARGUMENT);
	*iter++ = KevesFixnum(1);
	*iter++ = KevesInstruct(CMD_CDR);
	*iter++ = KevesInstruct(CMD_TEST_PAIR_R); 
	*iter++ = KevesFixnum(2); // break loop1 --->
	{
	  *iter++ = KevesInstruct(CMD_JUMP_R); 
	  *iter++ = KevesFixnum(-10); // loop1 --->
	}
      }
	  
      { // construct list
	*iter++ = KevesInstruct(CMD_TEST_NULL_R);  // <--- break loop1
	*iter++ = KevesFixnum(16); // error --->
	{ // proper list
	  *iter++ = KevesInstruct(CMD_PUSH);
	  *iter++ = KevesInstruct(CMD_REFER_LOCAL);
	  *iter++ = KevesFixnum(0);
	  { // loop
	    *iter++ = KevesInstruct(CMD_POP); // loop2 <---
	    *iter++ = KevesInstruct(CMD_CONS);
	    *iter++ = KevesInstruct(CMD_POP);
	    *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
	    *iter++ = KevesFixnum(1);
	    *iter++ = KevesFixnum(2); // break --->
	    *iter++ = KevesFixnum(-5); // loop2 --->
	  }

	  { // terminate
	    *iter++ = KevesInstruct(CMD_RETURN); // break <---
	  }
	}
      }
    }
      
    { // not pair
      *iter++ = KevesInstruct(CMD_TEST_NULL_R); // not pair <---
      *iter++ = KevesFixnum(3); // error --->
      { // empty list
	*iter++ = KevesInstruct(CMD_REFER_LOCAL);
	*iter++ = KevesFixnum(0);
	*iter++ = KevesInstruct(CMD_RETURN);
      }
    }
      
    { // not proper list pair
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- error
      *iter++ = err_req_proper_list;
      *iter++ = KevesInstruct(CMD_RAISE);
    }

    Q_ASSERT(iter <= code->end());
  }
  
  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  LambdaKev* lambda(generator->makeLambda(free_vars, code, 0));
  return lambda;
}

KevesValue Code_RnrsBase::makeCode_proc_string_append(KevcGenerator* generator) {
  SymbolKev* sym_string_append(generator->makeSymbol("string-append"));

  KevesValue err(generator->makeAssertCondition(sym_string_append,
						KevesBuiltinValues::mesg_ReqStr,
						EMB_NULL));
    
  KevesValue empty_str(generator->makeString(""));

  CodeKev* code(generator->makeCode(28));
  {
    KevesIterator iter(code->begin());
    *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesFixnum(3); // no argument --->
    *iter++ = KevesFixnum(5); // 1 argument --->
    *iter++ = KevesFixnum(10); // more than one argument --->

    { // no argument 
      *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
      *iter++ = empty_str;
    }

    { // an argument 
      *iter++ = KevesInstruct(CMD_LAST);
      *iter++ = KevesInstruct(CMD_TEST_STRING_R);
      *iter++ = KevesFixnum(15); // err --->
      {
	*iter++ = KevesInstruct(CMD_STRING_CLONE);
	*iter++ = KevesInstruct(CMD_RETURN);
      }
    }

    { // more than one argument 
      *iter++ = KevesInstruct(CMD_LAST);
      *iter++ = KevesInstruct(CMD_TEST_STRING_R);
      *iter++ = KevesFixnum(10); // err --->
      { // loop
	*iter++ = KevesInstruct(CMD_POP);
	*iter++ = KevesInstruct(CMD_TEST_STRING0_R);
	*iter++ = KevesFixnum(7); // err --->
	{ // argument is string
	  *iter++ = KevesInstruct(CMD_STRING_APPEND);
	  *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
	  *iter++ = KevesFixnum(2);
	  *iter++ = KevesFixnum(3); // break loop --->
	  *iter++ = KevesFixnum(3); // break loop --->
	  *iter++ = KevesFixnum(-6);
	}
      }

      { // terminate
	*iter++ = KevesInstruct(CMD_RETURN); // <--- break loop
      }
    }

    { // not string
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- err
      *iter++ = err;
      *iter++ = KevesInstruct(CMD_RAISE);
    }
    Q_ASSERT(iter <= code->end());
  }
  
  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  LambdaKev* lambda(generator->makeLambda(free_vars, code, 0));
  return lambda;
}

KevesValue Code_RnrsBase::makeCode_proc_map(KevcGenerator* generator,
					    KevesValue proc_raise) {
  SymbolKev* sym_map(generator->makeSymbol("map"));
  LambdaKev* proc_length_check;
  LambdaKev* proc_separate_first_value;
  LambdaKev* proc_transpose;
  LambdaKev* proc_map;

  KevesValue proc_equal(generator->findBind("="));
  KevesValue proc_apply(generator->findBind("apply"));
  KevesValue proc_car(generator->findBind("car"));
  KevesValue proc_caar(generator->findBind("caar"));
  KevesValue proc_call_with_values(generator->findBind("call-with-values"));
  KevesValue proc_cdr(generator->findBind("cdr"));
  KevesValue proc_cdar(generator->findBind("cdar"));
  KevesValue proc_cons(generator->findBind("cons"));
  KevesValue proc_length(generator->findBind("length"));
  KevesValue proc_list(generator->findBind("list"));
  KevesValue proc_not(generator->findBind("not"));
  KevesValue proc_null_q(generator->findBind("null?"));
  KevesValue proc_pair_q(generator->findBind("pair?"));
  KevesValue proc_procedure_q(generator->findBind("procedure?"));
  KevesValue proc_values(generator->findBind("values"));
  KevesValue proc_zero_q(generator->findBind("zero?"));

  KevesValue err51(generator->
		   makeAssertCondition(sym_map,
				       KevesBuiltinValues::mesg_Req1Got0,
				       EMB_NULL));
      

  KevesValue err52(generator->
		   makeAssertCondition(sym_map,
				       KevesBuiltinValues::mesg_Req1GotMore,
				       EMB_NULL));

  KevesValue err53(generator->
		   makeAssertCondition(sym_map,
				       KevesBuiltinValues::mesg_Req1OrMoreGot0,
				       EMB_NULL));

  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));

  CodeKev* code(generator->makeCode(500));
  KevesIterator iter(code->begin());

  proc_length_check
    = generator->makeLambda(free_vars, code, iter - code->begin());
  {
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CLOSE_R);
    // *iter++ = KevesFixnum(52);
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    // *iter++ = KevesFixnum(2);
    // *iter++ = KevesInstruct(CMD_BOX);
    // *iter++ = KevesFixnum(0);

    *iter++ = KevesInstruct(CMD_NEW_BOX);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_POP);
    *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_POP);
    *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
    *iter++ = KevesFixnum(1);

    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_null_q;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_TEST_NOT_R);
    *iter++ = KevesFixnum(38);
    *iter++ = KevesInstruct(CMD_POP);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(17);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_equal;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(10);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_length;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_car;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_TEST_R);
    *iter++ = KevesFixnum(15);
    *iter++ = KevesInstruct(CMD_POP);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(12);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_length_check;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cdr;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);

    // *iter++ = KevesInstruct(CMD_HALT);
    Q_ASSERT(iter <= code->end());
  }

  
  proc_separate_first_value
    = generator->makeLambda(free_vars, code, iter - code->begin());
  {
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CLOSE_R);
    // *iter++ = KevesFixnum(90);
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    // *iter++ = KevesFixnum(1);
    // *iter++ = KevesInstruct(CMD_BOX);
    // *iter++ = KevesFixnum(0);

    *iter++ = KevesInstruct(CMD_NEW_BOX);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_POP);
    *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
    *iter++ = KevesFixnum(0);

    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_null_q;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
    *iter++ = KevesFixnum(11);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(7);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_values;
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = EMB_NULL;
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = EMB_NULL;
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_JUMP_R);
    *iter++ = KevesFixnum(66);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(64);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_call_with_values;
    *iter++ = KevesInstruct(CMD_CLOSE_R);
    *iter++ = KevesFixnum(16);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_BOX);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(10);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_separate_first_value;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cdr;
    *iter++ = KevesInstruct(CMD_REFER_FREE0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);
    *iter++ = KevesInstruct(CMD_CLOSE_R);
    *iter++ = KevesFixnum(37);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_BOX);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(31);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_values;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(12);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cons;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_caar;
    *iter++ = KevesInstruct(CMD_REFER_FREE0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(12);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cons;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cdar;
    *iter++ = KevesInstruct(CMD_REFER_FREE0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);
    // *iter++ = KevesInstruct(CMD_HALT);
    
    Q_ASSERT(iter <= code->end());
  }

  proc_transpose = generator->makeLambda(free_vars, code, iter - code->begin());
  {
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CLOSE_R);
    // *iter++ = KevesFixnum(64);
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    // *iter++ = KevesFixnum(1);
    // *iter++ = KevesInstruct(CMD_BOX);
    // *iter++ = KevesFixnum(0);

    *iter++ = KevesInstruct(CMD_NEW_BOX);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_POP);
    *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
    *iter++ = KevesFixnum(0);

    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(10);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_null_q;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_car;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
    *iter++ = KevesFixnum(4);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = EMB_NULL;
    *iter++ = KevesInstruct(CMD_JUMP_R);
    *iter++ = KevesFixnum(42);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(40);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_call_with_values;
    *iter++ = KevesInstruct(CMD_CLOSE_R);
    *iter++ = KevesFixnum(11);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_BOX);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_separate_first_value;
    *iter++ = KevesInstruct(CMD_REFER_FREE0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);
    *iter++ = KevesInstruct(CMD_CLOSE_R);
    *iter++ = KevesFixnum(18);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_BOX);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(12);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cons;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_transpose;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_RETURN0);
    // *iter++ = KevesInstruct(CMD_HALT);

    Q_ASSERT(iter <= code->end());
  }

  proc_map = generator->makeLambda(free_vars, code, iter - code->begin());
  {
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CLOSE_R);
    // *iter++ = KevesFixnum(243);
    // *iter++ = KevesFixnum(0);

    *iter++ = KevesInstruct(CMD_CALL_LAMBDA_VLA);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_BOX);
    *iter++ = KevesFixnum(0);

    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(5);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_pair_q;
      *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
      *iter++ = KevesFixnum(0);
      *iter++ = KevesInstruct(CMD_APPLY);
    }
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_TEST_R);
    *iter++ = KevesFixnum(29);
    *iter++ = KevesInstruct(CMD_POP);
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(10);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_pair_q;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(5);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_cdr;
	*iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	*iter++ = KevesFixnum(0);
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }
    *iter++ = KevesInstruct(CMD_LAST);
    *iter++ = KevesInstruct(CMD_TEST_NOT_R);
    *iter++ = KevesFixnum(13);
    *iter++ = KevesInstruct(CMD_POP);
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(10);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_null_q;
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(5);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_cdr;
	*iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	*iter++ = KevesFixnum(0);
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }
    *iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
    *iter++ = KevesFixnum(191);
    {
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(187);
      {
	*iter++ = KevesInstruct(CMD_CLOSE_R);
	*iter++ = KevesFixnum(168);
	*iter++ = KevesFixnum(1);
	*iter++ = KevesInstruct(CMD_CALL_LAMBDA);
	*iter++ = KevesFixnum(2);
	*iter++ = KevesInstruct(CMD_BOX);
	*iter++ = KevesFixnum(0);
	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(10);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_not;
	  {
	    *iter++ = KevesInstruct(CMD_FRAME_R);
	    *iter++ = KevesFixnum(5);
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = proc_procedure_q;
	    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	    *iter++ = KevesFixnum(1);
	    *iter++ = KevesInstruct(CMD_APPLY);
	  }
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
	*iter++ = KevesFixnum(9);
	{ // not get procedure
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(5);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_raise;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = err51;
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_JUMP_R);
	*iter++ = KevesFixnum(141);

	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(27);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_not;
	  {
	    *iter++ = KevesInstruct(CMD_FRAME_R);
	    *iter++ = KevesFixnum(22);
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = proc_length_check;
	    {
	      *iter++ = KevesInstruct(CMD_FRAME_R);
	      *iter++ = KevesFixnum(10);
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = proc_length;
	      {
		*iter++ = KevesInstruct(CMD_FRAME_R);
		*iter++ = KevesFixnum(5);
		*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		*iter++ = proc_car;
		*iter++ = KevesInstruct(CMD_REFER_LOCAL0);
		*iter++ = KevesFixnum(0);
		*iter++ = KevesInstruct(CMD_APPLY);
	      }
	      *iter++ = KevesInstruct(CMD_APPLY);
	    }
	    {
	      *iter++ = KevesInstruct(CMD_FRAME_R);
	      *iter++ = KevesFixnum(5);
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = proc_cdr;
	      *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	      *iter++ = KevesFixnum(0);
	      *iter++ = KevesInstruct(CMD_APPLY);
	    }
	    *iter++ = KevesInstruct(CMD_APPLY);
	  }
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
	*iter++ = KevesFixnum(9);
	{ // lengths of lists are not same
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(5);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_raise;
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = err52;
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_JUMP_R);
	*iter++ = KevesFixnum(101);

	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(15);
	  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	  *iter++ = proc_zero_q;
	  {
	    *iter++ = KevesInstruct(CMD_FRAME_R);
	    *iter++ = KevesFixnum(10);
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = proc_length;
	    {
	      *iter++ = KevesInstruct(CMD_FRAME_R);
	      *iter++ = KevesFixnum(5);
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = proc_car;
	      *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	      *iter++ = KevesFixnum(0);
	      *iter++ = KevesInstruct(CMD_APPLY);
	    }
	    *iter++ = KevesInstruct(CMD_APPLY);
	  }
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
	*iter++ = KevesFixnum(4);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = EMB_NULL;
	*iter++ = KevesInstruct(CMD_JUMP_R);
	*iter++ = KevesFixnum(78);

	{
	  *iter++ = KevesInstruct(CMD_FRAME_R);
	  *iter++ = KevesFixnum(76);
	  {
	    *iter++ = KevesInstruct(CMD_CLOSE_R);
	    *iter++ = KevesFixnum(71);
	    *iter++ = KevesFixnum(2);
	    *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
	    *iter++ = KevesFixnum(0);
	    *iter++ = KevesInstruct(CMD_BOX);
	    *iter++ = KevesFixnum(1);
	    {
	      *iter++ = KevesInstruct(CMD_CLOSE_R);
	      *iter++ = KevesFixnum(48);
	      *iter++ = KevesFixnum(1);
	      *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
	      *iter++ = KevesFixnum(1);
	      *iter++ = KevesInstruct(CMD_BOX);
	      *iter++ = KevesFixnum(0);
	      {
		*iter++ = KevesInstruct(CMD_FRAME_R);
		*iter++ = KevesFixnum(5);
		*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		*iter++ = proc_null_q;
		*iter++ = KevesInstruct(CMD_REFER_LOCAL0);
		*iter++ = KevesFixnum(0);
		*iter++ = KevesInstruct(CMD_APPLY);
	      }
	      *iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
	      *iter++ = KevesFixnum(4);
	      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	      *iter++ = EMB_NULL;
	      *iter++ = KevesInstruct(CMD_JUMP_R);
	      *iter++ = KevesFixnum(31);
	      {
		*iter++ = KevesInstruct(CMD_FRAME_R);
		*iter++ = KevesFixnum(29);
		*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		*iter++ = proc_cons;
		{
		  *iter++ = KevesInstruct(CMD_FRAME_R);
		  *iter++ = KevesFixnum(12);
		  *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		  *iter++ = proc_apply;
		  *iter++ = KevesInstruct(CMD_REFER_FREE0);
		  *iter++ = KevesFixnum(1);
		  {
		    *iter++ = KevesInstruct(CMD_FRAME_R);
		    *iter++ = KevesFixnum(5);
		    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		    *iter++ = proc_car;
		    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
		    *iter++ = KevesFixnum(0);
		    *iter++ = KevesInstruct(CMD_APPLY);
		  }
		  *iter++ = KevesInstruct(CMD_APPLY);
		}
		{
		  *iter++ = KevesInstruct(CMD_FRAME_R);
		  *iter++ = KevesFixnum(10);
		  *iter++ = KevesInstruct(CMD_REFER_FREE0);
		  *iter++ = KevesFixnum(0);
		  {
		    *iter++ = KevesInstruct(CMD_FRAME_R);
		    *iter++ = KevesFixnum(5);
		    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		    *iter++ = proc_cdr;
		    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
		    *iter++ = KevesFixnum(0);
		    *iter++ = KevesInstruct(CMD_APPLY);
		  }
		  *iter++ = KevesInstruct(CMD_APPLY);
		}
		*iter++ = KevesInstruct(CMD_APPLY);
	      }
	      *iter++ = KevesInstruct(CMD_RETURN0);
	    }
	    *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL0);
	    *iter++ = KevesFixnum(0);
	    *iter++ = KevesInstruct(CMD_POP);
	    {
	      *iter++ = KevesInstruct(CMD_FRAME_R);
	      *iter++ = KevesFixnum(10);
	      *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	      *iter++ = KevesFixnum(0);
	      {
		*iter++ = KevesInstruct(CMD_FRAME_R);
		*iter++ = KevesFixnum(5);
		*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
		*iter++ = proc_transpose;
		*iter++ = KevesInstruct(CMD_REFER_FREE0);
		*iter++ = KevesFixnum(1);
		*iter++ = KevesInstruct(CMD_APPLY);
	      }
	      *iter++ = KevesInstruct(CMD_APPLY);
	    }
	    *iter++ = KevesInstruct(CMD_RETURN0);

	  }
	  *iter++ = KevesInstruct(CMD_APPLY);
	}
	*iter++ = KevesInstruct(CMD_RETURN0);
      }

      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(5);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_car;
	*iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	*iter++ = KevesFixnum(0);
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      {
	*iter++ = KevesInstruct(CMD_FRAME_R);
	*iter++ = KevesFixnum(5);
	*iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	*iter++ = proc_cdr;
	*iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	*iter++ = KevesFixnum(0);
	*iter++ = KevesInstruct(CMD_APPLY);
      }
      *iter++ = KevesInstruct(CMD_APPLY);
    }
    *iter++ = KevesInstruct(CMD_JUMP_R);
    *iter++ = KevesFixnum(7);

    { // error of improper lists
      *iter++ = KevesInstruct(CMD_FRAME_R);
      *iter++ = KevesFixnum(5);
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = proc_raise;
      *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
      *iter++ = err53;
      *iter++ = KevesInstruct(CMD_APPLY);
    }
    *iter++ = KevesInstruct(CMD_RETURN0);

    *iter++ = KevesInstruct(CMD_RETURN0);
    // *iter++ = KevesInstruct(CMD_HALT);

    Q_ASSERT(iter <= code->end());
  }

  return proc_map;
}

KevesValue Code_RnrsBase::makeCode_proc_eqv_q(KevcGenerator* generator) {
  SymbolKev* sym_eqv_q(generator->makeSymbol("eqv?"));
		       
  KevesValue err_no_arg(generator->
			makeAssertCondition(sym_eqv_q,
					    KevesBuiltinValues::mesg_Req2Got0,
					    EMB_NULL));
      
  KevesValue err_one_arg(generator->
			 makeAssertCondition(sym_eqv_q,
					     KevesBuiltinValues::mesg_Req2Got1,
					     EMB_NULL));
      
  KevesValue err_too_many_arg(generator->
			      makeAssertCondition(sym_eqv_q,
						  KevesBuiltinValues::mesg_Req2GotMore,
						  EMB_NULL));

  CodeKev* code(generator->makeCode(49));
  {
    KevesIterator iter(code->begin());
    
    *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
    *iter++ = KevesFixnum(3);
    *iter++ = KevesFixnum(4);
    *iter++ = KevesFixnum(7);
    *iter++ = KevesFixnum(10);
    *iter++ = KevesFixnum(44);;

    { // no argument
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- no arg
      *iter++ = err_no_arg;
      *iter++ = KevesInstruct(CMD_RAISE);
    }

    { // one argument
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- one arg
      *iter++ = err_one_arg;
      *iter++ = KevesInstruct(CMD_RAISE);
    }

    { // valid number of arguments
      *iter++ = KevesInstruct(CMD_LAST);
      *iter++ = KevesInstruct(CMD_POP);
      *iter++ = KevesInstruct(CMD_TEST_SYMBOL_EQUAL_R);
      *iter++ = KevesFixnum(27); // for not equal --->
      *iter++ = KevesFixnum(0); // for not symbol --->
      { // same values
	*iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	*iter++ = EMB_TRUE;
      }

      { // one is not a symbol
	*iter++ = KevesInstruct(CMD_TEST_STRING_EQUAL_R);
	*iter++ = KevesFixnum(22); // for not equal --->
	*iter++ = KevesFixnum(2); // for not string --->
	{ // same values
	  *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	  *iter++ = EMB_TRUE;
	}
      }
    
      { // one is not a symbol or a string
	*iter++ = KevesInstruct(CMD_TEST_CHAR_R);
	*iter++ = KevesFixnum(6); // for not char --->
	{ // one is a character
	  *iter++ = KevesInstruct(CMD_TEST_CHAR0_R);
	  *iter++ = KevesFixnum(16); // for not equal --->
	  { // both two is a character
	    *iter++ = KevesInstruct(CMD_TEST_CHAR_EQUAL_R);
	    *iter++ = KevesFixnum(14); // for not equal --->
	    { // same values
	      *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	      *iter++ = EMB_TRUE;
	    }
	  }
	}
      }
    
      { // one is not a symbol, a string or a charcter
	*iter++ = KevesInstruct(CMD_TEST_NUMBER_R); // not char <---
	*iter++ = KevesFixnum(6); // for not number --->
	{ // one is a number
	  *iter++ = KevesInstruct(CMD_TEST_NUMBER0_R);
	  *iter++ = KevesFixnum(8); // for not equal --->
	  { // both two is a number
	    *iter++ = KevesInstruct(CMD_TEST_NUMBER_EQUAL_R);
	    *iter++ = KevesFixnum(6); // for not equal --->
	    { // same values
	      *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	      *iter++ = EMB_TRUE;
	    }
	  }
	}
      }
    
      { // one is not a symbol, a string, a character or a number
	*iter++ = KevesInstruct(CMD_TEST_QEV_EQUAL_R); // not number <---
	*iter++ = KevesFixnum(2); // for not equal --->
	{ // same values
	  *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	  *iter++ = EMB_TRUE;
	}
      }

      { // different values
	*iter++ = KevesInstruct(CMD_RETURN_CONSTANT); // not equal <---
	*iter++ = EMB_FALSE;
      }
    }

    { // too many arguments
      *iter++ = KevesInstruct(CMD_CONSTANT);
      *iter++ = err_too_many_arg;
      *iter++ = KevesInstruct(CMD_RAISE);
    }

    Q_ASSERT(iter <= code->end());
  }
  
  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  LambdaKev* lambda(generator->makeLambda(free_vars, code, 0));
  return lambda;
}

// eq?
KevesValue Code_RnrsBase::makeCode_proc_eq_q(KevcGenerator* generator) {
  SymbolKev* sym_eq_q(generator->makeSymbol("eq?"));
		       
  KevesValue err_no_arg(generator->
			makeAssertCondition(sym_eq_q,
					    KevesBuiltinValues::mesg_Req2Got0,
					    EMB_NULL));
      
  KevesValue err_one_arg(generator->
			 makeAssertCondition(sym_eq_q,
					     KevesBuiltinValues::mesg_Req2Got1,
					     EMB_NULL));
      
  KevesValue err_too_many_arg(generator->
			      makeAssertCondition(sym_eq_q,
						  KevesBuiltinValues::mesg_Req2GotMore,
						  EMB_NULL));

  CodeKev* code(generator->makeCode(30));
  {
    KevesIterator iter(code->begin());
    
    *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
    *iter++ = KevesFixnum(3);
    *iter++ = KevesFixnum(4);
    *iter++ = KevesFixnum(6);
    *iter++ = KevesFixnum(8);
    *iter++ = KevesFixnum(26);
    
    { // no argument
      *iter++ = KevesInstruct(CMD_RAISE_CONSTANT); // <--- no arg
      *iter++ = err_no_arg;
    }
    
    { // one argument
      *iter++ = KevesInstruct(CMD_RAISE_CONSTANT); // <--- one arg
      *iter++ = err_one_arg;
    }
    
    { // valid number of arguments
      *iter++ = KevesInstruct(CMD_LAST);
      *iter++ = KevesInstruct(CMD_POP);
      *iter++ = KevesInstruct(CMD_TEST_SYMBOL_EQUAL_R);
      *iter++ = KevesFixnum(11); // for not equal --->
      *iter++ = KevesFixnum(2); // for not symbol --->
      { // same values
	*iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	*iter++ = EMB_TRUE;
      }
      
      { // one is not a symbol
	*iter++ = KevesInstruct(CMD_TEST_STRING_EQUAL_R);
	*iter++ = KevesFixnum(6); // for not equal --->
	*iter++ = KevesFixnum(2); // for not string --->
	{ // same values
	  *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	  *iter++ = EMB_TRUE;
	}
      }
      
      { // one is not a symbol or a string
	*iter++ = KevesInstruct(CMD_TEST_QEV_EQUAL_R);
	*iter++ = KevesFixnum(2); // for not equal --->
	{ // same values
	  *iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	  *iter++ = EMB_TRUE;
	}
      }
      
      { // different values
	*iter++ = KevesInstruct(CMD_RETURN_CONSTANT);
	*iter++ = EMB_FALSE;
      }
    }
    
    { // too many arguments
      *iter++ = KevesInstruct(CMD_RAISE_CONSTANT);
      *iter++ = err_too_many_arg;
    }

    Q_ASSERT(iter <= code->end());
  }
  
  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  LambdaKev* lambda(generator->makeLambda(free_vars, code, 0));
  return lambda;
}
