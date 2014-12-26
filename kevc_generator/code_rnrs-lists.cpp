// keves/kevc_generator/code_rnrs-lists.cpp - a code of (rnrs base)
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


#include "code_rnrs-lists.hpp"

#include "kevc_generator.hpp"
#include "keves_builtin_values.hpp"
#include "kev/code.hpp"
#include "kev/frame.hpp"
#include "kev/procedure.hpp"
#include "kev/symbol.hpp"
#include "value/fixnum.hpp"
#include "value/instruct.hpp"


void Code_RnrsLists::write(KevcGenerator* generator) {
  if (!generator->setImportLibrary("keves", "base-bin", 0, 1)) return;
  // for invoking makeAssertCondition()
  generator->importBind("&assert");
  generator->importBind("&message");
  generator->importBind("&irritants");
  generator->importBind("&who");
 
  if (!generator->setImportLibrary("rnrs", "base", 6)) return;
  KevesValue proc_apply(generator->importBind("apply"));
  KevesValue proc_car(generator->importBind("car"));
  KevesValue proc_cdr(generator->importBind("cdr"));
  KevesValue proc_cons(generator->importBind("cons"));
  KevesValue proc_map(generator->importBind("map"));
  KevesValue proc_pair_q(generator->importBind("pair?"));
  KevesValue proc_eq_q(generator->importBind("eq?"));

  if (!generator->setImportLibrary("rnrs", "lists-bin", 6)) return;
  KevesValue proc_cons_star(generator->importBind("cons*"));
 
  // export binds
  const char* binds[] = {
    "cons*"
  };

  for (auto bind : binds)
    generator->exportBind(bind, generator->importBind(bind));

  generator->exportBind("assq",
			makeCode_proc_assq(generator, proc_eq_q));

  generator->exportBind("fold-left",
			makeCode_proc_fold_left(generator,
						proc_apply,
						proc_car,
						proc_cdr,
						proc_cons,
						proc_map,
						proc_pair_q));
  
  generator->writeToFile();
}

KevesValue Code_RnrsLists::makeCode_proc_assq(KevcGenerator* generator,
					      KevesValue proc_eq_q) {
  SymbolKev* sym_assq(generator->makeSymbol("assq"));

  KevesValue err_no_arg(generator->
			makeAssertCondition(sym_assq,
					    KevesBuiltinValues::mesg_Req2Got0,
					    EMB_NULL));

  KevesValue err_an_arg(generator->
			makeAssertCondition(sym_assq,
					    KevesBuiltinValues::mesg_Req2Got1,
					    EMB_NULL));

  KevesValue err_req_pair(generator->
			  makeAssertCondition(sym_assq,
					      KevesBuiltinValues::mesg_ReqPair,
					      EMB_NULL));

  KevesValue err_too_many(generator->
			  makeAssertCondition(sym_assq,
					      KevesBuiltinValues::mesg_Req2GotMore,
					      EMB_NULL));
  
  CodeKev* code(generator->makeCode(59));
  {
    KevesIterator iter(code->begin());
    
    *iter++ = KevesInstruct(CMD_SWITCH_N_ARG_R);
    *iter++ = KevesFixnum(3);
    *iter++ = KevesFixnum(4); // no argument --->
    *iter++ = KevesFixnum(7); // an argument --->
    *iter++ = KevesFixnum(10); // valid number of arguments --->
    *iter++ = KevesFixnum(54); // too many arguments --->
    
    { // no argument
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- no argument
      *iter++ = err_no_arg;
      *iter++ = KevesInstruct(CMD_RAISE);
    }
    
    { // an argument
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- an argument
      *iter++ = err_an_arg;
      *iter++ = KevesInstruct(CMD_RAISE);
    }
    
    { // valid number of arguments
      *iter++ = KevesInstruct(CMD_ARGUMENT); // <--- valid number of arguments
      *iter++ = KevesFixnum(1);
      *iter++ = KevesInstruct(CMD_NEW_BOX);
      *iter++ = KevesFixnum(2);
      *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL);
      *iter++ = KevesFixnum(1);
      *iter++ = KevesInstruct(CMD_LAST);

      { // loop
	*iter++ = KevesInstruct(CMD_ASSIGN_LOCAL); // <--- loop
	*iter++ = KevesFixnum(0);
	*iter++ = KevesInstruct(CMD_TEST_PAIR_R);
	*iter++ = KevesFixnum(26); // break loop --->
	{
	  *iter++ = KevesInstruct(CMD_CAR); // <--- is list
	  *iter++ = KevesInstruct(CMD_TEST_PAIR_R);
	  *iter++ = KevesFixnum(20); // error not pair --->

	  *iter++ = KevesInstruct(CMD_CAR); // is pair
	  {
	    *iter++ = KevesInstruct(CMD_FRAME_R);
	    *iter++ = KevesFixnum(6);
	    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
	    *iter++ = proc_eq_q;
	    *iter++ = KevesInstruct(CMD_PUSH);
	    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
	    *iter++ = KevesFixnum(1);
	    *iter++ = KevesInstruct(CMD_APPLY);
	  }
	  *iter++ = KevesInstruct(CMD_TEST0_R);
	  *iter++ = KevesFixnum(4); // not_equal --->
	  { // return value (not false)
	    *iter++ = KevesInstruct(CMD_REFER_LOCAL); // equal
	    *iter++ = KevesFixnum(0);
	    *iter++ = KevesInstruct(CMD_CAR);
	    *iter++ = KevesInstruct(CMD_RETURN);
	  }
	    
	  *iter++ = KevesInstruct(CMD_REFER_LOCAL); // <--- not equal
	  *iter++ = KevesFixnum(0);
	  *iter++ = KevesInstruct(CMD_CDR);
	  *iter++ = KevesInstruct(CMD_JUMP_R);
	  *iter++ = KevesFixnum(-27); // loop --->
	}
	    
	{ // error not pair
	  *iter++ = KevesInstruct(CMD_CONSTANT); // <--- error not pair
	  *iter++ = err_req_pair;
	  *iter++ = KevesInstruct(CMD_RAISE);
	}
      }
	
      { //terminate
	*iter++ = KevesInstruct(CMD_TEST_NULL_R); // <--- break loop
	*iter++ = KevesFixnum(2); // error not null --->
	{
	  *iter++ = KevesInstruct(CMD_RETURN_CONSTANT); // <--- null
	  *iter++ = EMB_FALSE;
	}
	  
	*iter++ = KevesInstruct(CMD_CONSTANT); // <--- error not null
	*iter++ = err_req_pair;
	*iter++ = KevesInstruct(CMD_RAISE);
      }
    }
    
    { // too many arguments
      *iter++ = KevesInstruct(CMD_CONSTANT); // <--- too many arguments
      *iter++ = err_too_many;
      *iter++ = KevesInstruct(CMD_RAISE);
    }

    Q_ASSERT(iter <= code->end());
  }
  
  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  LambdaKev* lambda(generator->makeLambda(free_vars, code, 0));
  return lambda;
}

// fold-left
KevesValue Code_RnrsLists::makeCode_proc_fold_left(KevcGenerator* generator,
						   KevesValue proc_apply,
						   KevesValue proc_car,
						   KevesValue proc_cdr,
						   KevesValue proc_cons,
						   KevesValue proc_map,
						   KevesValue proc_pair_q) {
  CodeKev* code(generator->makeCode(74));
  LocalVarFrameKev* free_vars(generator->makeLocalVarFrame(0, nullptr));
  LambdaKev* proc_fold_left(generator->makeLambda(free_vars, code, 0));
  {
    KevesIterator iter(code->begin());

    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_FRAME_R);
    // *iter++ = KevesFixnum(91);
    // *iter++ = KevesInstruct(CMD_CLOSE_R);
    // *iter++ = KevesFixnum(86);
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_CALL_LAMBDA);
    // *iter++ = KevesFixnum(0);
    // *iter++ = KevesInstruct(CMD_BOX);
    // *iter++ = KevesFixnum(1);
    // *iter++ = KevesInstruct(CMD_CLOSE_R);
    // *iter++ = KevesFixnum(73);
    // *iter++ = KevesFixnum(1);
  
    *iter++ = KevesInstruct(CMD_CALL_LAMBDA_VLA);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_BOX);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(10);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_pair_q;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(5);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_car;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_LAST_POP_TEST_R);
    *iter++ = KevesFixnum(53);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(49);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_apply;
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT); // CMD_REFER_FREE0
    *iter++ = proc_fold_left; //  KevesFixnum(0)
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(42);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cons;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(35);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cons;
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(21);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_apply;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(14);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cons;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(7);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_map;
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_car;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_FRAME_R);
    *iter++ = KevesFixnum(7);
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_map;
    *iter++ = KevesInstruct(CMD_PUSH_CONSTANT);
    *iter++ = proc_cdr;
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(0);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_APPLY);
    *iter++ = KevesInstruct(CMD_JUMP_R);
    *iter++ = KevesFixnum(2);
    *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
    *iter++ = KevesFixnum(1);
    *iter++ = KevesInstruct(CMD_RETURN0);
  }

  // *iter++ = KevesInstruct(CMD_ASSIGN_LOCAL0);
  // *iter++ = KevesFixnum(0);
  // *iter++ = KevesInstruct(CMD_POP);
  // *iter++ = KevesInstruct(CMD_REFER_LOCAL0);
  // *iter++ = KevesFixnum(0);
  // *iter++ = KevesInstruct(CMD_RETURN0);
  // *iter++ = KevesInstruct(CMD_APPLY);
  // *iter++ = KevesInstruct(CMD_HALT);

  return proc_fold_left;
}
