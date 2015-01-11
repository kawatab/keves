// keves/keves_vm.cpp - VM for Keves
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


#include "keves_vm.hpp"

#include <iostream>
#include "keves_builtin_values.hpp"
#include "keves_common.hpp"
#include "keves_library.hpp"
#include "keves_stack.hpp"
#include "keves_textual_port.hpp"
#include "kev/bignum.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/condition.hpp"
#include "kev/condition-inl.hpp"
#include "kev/frame.hpp"
#include "kev/frame-inl.hpp"
#include "kev/jump.hpp"
#include "kev/jump-inl.hpp"
#include "kev/number.hpp"
#include "kev/number-inl.hpp"
#include "kev/pair.hpp"
#include "kev/pair-inl.hpp"
#include "kev/procedure.hpp"
#include "kev/record.hpp"
#include "kev/reference.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/vector.hpp"
#include "kev/vector-inl.hpp"
#include "kev/wind.hpp"
#include "kev/wrapped.hpp"
#include "value/char.hpp"
#include "value/fixnum.hpp"
#include "value/instruct.hpp"


KevesVM* KevesVM::make(KevesCommon* common) {
  return make(common, common->default_result_field());
}

KevesVM* KevesVM::make(KevesCommon* common, KevesTextualOutputPort* result_field) {
  KevesVM* vm(new KevesVM());
  vm->common_ = common;
  vm->cmd_table_ = common->cmd_table();
  vm->gc_.init(vm, common->shared_list());
  vm->result_field_ = result_field;
  return vm;
}

void KevesVM::run() {
  execute();
}
  
void KevesVM::checkStack(size_t* size, vm_func func, const_KevesIterator pc) {
  if (static_cast<void*>(size)
      < static_cast<void*>(static_cast<char*>(stack_safety_limit_) + *size)) {
    return executeGC(func, pc);
  }
}

void KevesVM::checkStack(const void* obj, vm_func func, const_KevesIterator pc) {
  if (obj < stack_safety_limit_)
    return executeGC(func, pc);
}

// for number
void KevesVM::makeRectangular(KevesVM* vm, const_KevesIterator pc) {
  KevesValue real(vm->acc_);
  KevesValue imag(vm->gr1_);

  if (real.isRealNumber() && imag.isRealNumber()) {
    if (real.isFixnum()) {
      KevesFixnum fixnum(real);
      RationalNumberKevFromInt real_part(fixnum);

      if (imag.isFixnum()) {
	KevesFixnum fixnum(imag);

	if (fixnum.isZero()) {
	  vm->acc_ = real;
	  return returnValueSafe(vm, pc);
	}
	
	RationalNumberKevFromInt imag_part(fixnum);
	ExactComplexNumberKev complex(real_part, imag_part);
	vm->acc_ = &complex;
	return returnValueSafe(vm, pc);
      }

      if (imag.is<RationalNumberKev>()) {
	const RationalNumberKev* imag_part(imag);

	if (imag_part->isZero()) {
	  vm->acc_ = real;
	  return returnValueSafe(vm, pc);
	}
	
	ExactComplexNumberKev complex(real_part, *imag_part);
	vm->acc_ = &complex;
	return returnValueSafe(vm, pc);
      }
      
      {
	FlonumKev real_part(fixnum);
	const FlonumKev* imag_part(imag);
	InexactComplexNumberKev complex(real_part, *imag_part);
	vm->acc_ = &complex;
	return returnValueSafe(vm, pc);
      }
    }

    if (real.is<RationalNumberKev>()) {
      const RationalNumberKev* real_part(real);
      
      if (imag.isFixnum()) {
	KevesFixnum fixnum(imag);
	
	if (fixnum.isZero()) {
	  vm->acc_ = real;
	  return returnValueSafe(vm, pc);
	}
	
	RationalNumberKevFromInt imag_part(fixnum);
	ExactComplexNumberKev complex(*real_part, imag_part);
	vm->acc_ = &complex;
	return returnValueSafe(vm, pc);
      }

      if (imag.is<RationalNumberKev>()) {
	const RationalNumberKev* imag_part(imag);

	if (imag_part->isZero()) {
	  vm->acc_ = real;
	  return returnValueSafe(vm, pc);
	}
	
	ExactComplexNumberKev complex(*real_part, *imag_part);
	vm->acc_ = &complex;
	return returnValueSafe(vm, pc);
      }
      
      FlonumKev flonum(real_part->toInexact());
      const FlonumKev* imag_part(imag);
      InexactComplexNumberKev complex(flonum, *imag_part);
      vm->acc_ = &complex;
      return returnValueSafe(vm, pc);
    }
    
    const FlonumKev* real_part(real);
    
    if (imag.isFixnum()) {
      KevesFixnum fixnum(imag);
      FlonumKev imag_part(fixnum);
      InexactComplexNumberKev complex(*real_part, imag_part);
      vm->acc_ = &complex;
      return returnValueSafe(vm, pc);
    }
    
    if (imag.is<RationalNumberKev>()) {
      const RationalNumberKev* rational(imag);
      FlonumKev imag_part(rational->toInexact());
      InexactComplexNumberKev complex(*real_part, imag_part);
      vm->acc_ = &complex;
      return returnValueSafe(vm, pc);
    }
    
    const FlonumKev* imag_part(imag);
    InexactComplexNumberKev complex(*real_part, *imag_part);
    vm->acc_ = &complex;
    return returnValueSafe(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common_->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
  vm->gr2_ = EMB_NULL;
  return raiseAssertCondition(vm, pc);
}

// for list
int KevesVM::getListLength(KevesValue temp) {
  int count(0);
  
  while (temp.isPair()) {
    const PairKev* pair(temp);
    temp = pair->cdr();
    ++count;
  }

  return temp == EMB_NULL ? count : -1;
}


// for strings
StringKev* KevesVM::makeString(int size) {
  StringKev* string(StringKev::make(&this->gc_, size));
  return string;
}

const StringKev* KevesVM::makeString(int size, QChar chr) {
  StringKev* string(makeString(size));
  string->fill(chr);
  return string;
}

// for vectors
VectorKev* KevesVM::makeVector(int size) {
  VectorKev* vector(VectorKev::make(&this->gc_, size));
  return vector;
}

const VectorKev* KevesVM::makeVector(int size, KevesValue obj) {
  VectorKev* vector(makeVector(size));
  vector->fill(obj);
  return vector;
}


// instructions
void KevesVM::cmd_NOP(KevesVM* vm, const_KevesIterator pc) {

#ifndef QT_NO_DEBUG
  int dummy;
  vm->checkStack(&dummy, &cmd_NOP, pc);
#endif

  return (vm->cmd_table_[KevesInstruct(*pc)])(vm, pc + 1);
}

void KevesVM::cmd_JUMP(KevesVM* vm, const_KevesIterator pc) {
  const JumpKev* jump(*pc);
  return cmd_NOP(vm, jump->destination());
}
  
void KevesVM::cmd_JUMP_R(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum offset(*pc);
  return cmd_NOP(vm, pc + offset + 1);
}
  
void KevesVM::cmd_SKIP(KevesVM* vm, const_KevesIterator pc) {
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::pushGr1ToArgumentSafe(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

  vm->checkStack(&argn, &pushGr1ToArgumentSafe, pc);

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->gr1_);
  return cmd_NOP(vm, pc);
}

void KevesVM::pushAccToArgumentSafe(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

  vm->checkStack(&argn, &pushAccToArgumentSafe, pc);

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->acc_);
  return cmd_NOP(vm, pc);
}

void KevesVM::pushGr1ToArgument(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

#ifndef QT_NO_DEBUG
  vm->checkStack(&argn, &pushGr1ToArgument, pc);
#endif

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->gr1_);
  return cmd_NOP(vm, pc);
}

void KevesVM::pushAccToArgument(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

#ifndef QT_NO_DEBUG
  vm->checkStack(&argn, &pushAccToArgument, pc);
#endif

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->acc_);
  return cmd_NOP(vm, pc);
}

void KevesVM::returnValue(KevesVM* vm, const_KevesIterator) {
  return pushAccToArgument(vm, vm->registers_.unwind());
  // return pushAccToArgumentSafe(vm, registers->unwind());
}

void KevesVM::returnValueSafe(KevesVM* vm, const_KevesIterator) {
  return pushAccToArgumentSafe(vm, vm->registers_.unwind());
}

void KevesVM::returnTwoValues(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &returnTwoValues, pc);
  
  arg_frame.assign(0, KevesFixnum(3));
  arg_frame.assign(1, vm->acc_);
  arg_frame.assign(2, vm->gr1_);
  vm->keves_vals_ = &arg_frame;
  vm->gr1_ = vm->acc_;
  return pushGr1ToArgument(vm, vm->registers_.unwind());
}

void KevesVM::continueRationalNumberKev(KevesVM* vm,
					const_KevesIterator pc,
					const RationalNumberKev* real,
					vm_func for_fixnum,
					vm_func for_real) {
  KevesValue fixnum(real->toKevesFixnum());
  vm->gr1_ = vm->registers_.lastArgument();

  if (fixnum.isFixnum()) {
    vm->acc_ = KevesFixnum(fixnum);
    return for_fixnum(vm, pc);
  }
  
  vm->acc_ = real;
  return for_real(vm, pc);
}

void KevesVM::continueExactComplexNumberKev(KevesVM* vm,
					    const_KevesIterator pc,
					    const ExactComplexNumberKev* complex,
					    vm_func for_fixnum,
					    vm_func for_real,
					    vm_func for_complex) {
  StackFrameKev* registers(&vm->registers_);

  if (complex->imag().isZero()) {
    RationalNumberKev real(complex->real());
    return continueRationalNumberKev(vm, pc, &real, for_fixnum, for_real);
  }
  
  registers->popArgument();
  vm->gr1_ = registers->lastArgument();
  vm->acc_ = complex;
  return for_complex(vm, pc);
}

void KevesVM::returnExactComplexNumberKev(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* complex(vm->acc_);

  if (complex->imag().isZero()) {
    RationalNumberKev real(complex->real());
    vm->acc_ = real.optimize();
    return returnValueSafe(vm, pc);
  }
  
  vm->acc_ = complex;
  return returnValueSafe(vm, pc);
}

void KevesVM::executeGC(vm_func current_func, const_KevesIterator pc) {
  std::cout << "start GC" << std::endl;
  current_function_ = current_func;
  return gc_.execute(pc);
}

int KevesVM::execute() {
  pthread_attr_t attr;
  pthread_getattr_np(pthread_self(), &attr);
  
  size_t stack_size(0);
  pthread_attr_getstack(&attr, &stack_lower_limit_, &stack_size);
  stack_higher_limit_ = static_cast<char*>(stack_lower_limit_) + stack_size;
  stack_safety_limit_ = static_cast<char*>(stack_lower_limit_) + 1024 * 1024;
  stack_higher_limit_ = reinterpret_cast<char*>(&attr);
  std::cout << "higher limit: " << stack_higher_limit_ << std::endl;

  return execute_helper();
}

int KevesVM::execute_helper() {
  // This is a bottom frame.
  registers_.set_fp(&registers_);

  registers_.changeToBottomFrame();
  QStringList library_name;
  library_name << "keves" << "base";
  QList<ver_num_t> ver_num;
  KevesLibrary* lib_keves_base(common_->getLibrary(library_name, ver_num));

  if (!lib_keves_base) {
    KevesLibrary::errorOfMissingLibrary(library_name, ver_num);
    return 1;
  }
  
  KevesValue code(lib_keves_base->findBind("my-code"));

  if (code == EMB_UNDEF) {
    std::cerr << "bind: my-code is not found!!!\n";
    return 1;
  } else if (!code.isCode()) {
    std::cerr << "my-code is not code!!!\n";
    return 1;
  }
  
  const_KevesIterator pc(code.toPtr<CodeKev>()->begin());
  Q_ASSERT(pc->isInstruct());

  KevesValue environment(lib_keves_base->findBind("environment"));

  if (environment == EMB_UNDEF) {
    std::cerr << "bind: environment is not found!!!\n";
    return 1;
  } else if (!environment.is<EnvironmentKev>()) {
    std::cerr << "environment is not environment!!!\n";
    return 1;
  }
  
  prev_global_vars_.copyFrom(*environment.toPtr<EnvironmentKev>());
  curt_global_vars_.copyFrom(prev_global_vars_);

  // prepare environment frame
  LocalVarFrameKevWithArray<02> env_frame;
  registers_.setEnvFrame(&env_frame, 2);
  registers_.assignLastLocalVar(EMB_NULL, 1);
  registers_.assignLastLocalVar(&prev_global_vars_, 0);

  // prepare stack frame
  ArgumentFrameKevWithArray<04> arg_frame;
  registers_.setArgFrame(&arg_frame);
  LocalVarFrameKev clsr;
  registers_.setClosure(&clsr);
  keves_vals_ = nullptr;
  gr1_ = gr2_ = gr3_ = EMB_UNDEF; // acc_ keeps value
  
  current_function_ = cmd_table_[KevesInstruct(*pc++)];
  current_pc_ = pc;
  
  std::cout << "&arg_frame: " << &arg_frame << std::endl;
  std::cout << "stack_higher_limit_: " << stack_higher_limit_ << std::endl;
  
  Q_ASSERT(&registers_ < stack_lower_limit_ || &registers_ >= stack_higher_limit_);
  
  switch(setjmp(jmp_exit_)) {
  case -2:
    std::cerr << "KevesVM: fatal error\n";
    return 1;

  case -1:
    std::cout << "gc time: " << gc_.getElapsedTime() << std::endl;
    return 0;

  default:
    (*current_function_)(this, current_pc_);
    return -1; // normally, not arrive here
  }
}

void KevesVM::cmd_HALT(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue temp(registers->lastArgument());

  if (registers->sfp()) {
    KevesValue ref(registers->fp()->lastLocalVar());

    if (ref.is<EnvironmentKev>()) {
      vm->curt_global_vars_.copyFrom(vm->prev_global_vars_);
      vm->prev_global_vars_.copyFrom(*ref.toPtr<EnvironmentKev>());
      vm->acc_ = temp;
      vm->gr1_ = vm->gr2_ = vm->gr3_ = EMB_UNDEF;
      return returnValue(vm, pc);
    }
    
    std::cerr << "KevesVM::cmd_HALT(): Not handle global values properly\n";
    longjmp(vm->jmp_exit_, -2);
  }

  longjmp(vm->jmp_exit_, -1);
}

void KevesVM::cmd_TEST(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_ != EMB_FALSE ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_R(KevesVM* vm, const_KevesIterator pc) {
  return vm->acc_ != EMB_FALSE ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_LAST_POP_TEST(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue temp(registers->lastArgument());
  registers->popArgument();
  vm->acc_ = temp;
  return (temp != EMB_FALSE ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_LAST_POP_TEST_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue temp(registers->lastArgument());
  registers->popArgument();
  vm->acc_ = temp;

  return temp != EMB_FALSE ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST0_R(KevesVM* vm, const_KevesIterator pc) {
  return vm->registers_.lastArgument() != EMB_FALSE ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_NOT_R(KevesVM* vm, const_KevesIterator pc) {
  return vm->acc_ == EMB_FALSE ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_SWITCH_N_ARG(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum  default_number(*pc);
  Q_ASSERT(default_number > 0);
  int argn(vm->registers_.argn());
  int case_num(argn > default_number ? default_number + 1 : argn);
  const JumpKev* jump(*(pc + case_num));

  return cmd_NOP(vm, jump->destination());
}

void KevesVM::cmd_SWITCH_N_ARG_R(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum default_number(*pc);
  Q_ASSERT(default_number > 0);
  int argn(vm->registers_.argn());
  int case_num(argn > default_number ? default_number + 1 : argn);
  KevesFixnum offset(*(pc + case_num));

  return cmd_NOP(vm, pc + offset + 1);
}

void KevesVM::cmd_FRAME(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &cmd_FRAME, pc);

  StackFrameKev stack_frame;
  const JumpKev* jump(*pc);
  vm->registers_.wind(jump->destination(), &stack_frame, &arg_frame);

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_FRAME_R(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &cmd_FRAME_R, pc);

  StackFrameKev stack_frame;
  KevesFixnum offset(*pc);
  vm->registers_.wind(pc + offset + 1, &stack_frame, &arg_frame);

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_SFRAME(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &cmd_SFRAME, pc);

  StackFrameKev stack_frame;
  const JumpKev* jump(*pc);

  vm->registers_.windWithExceptionHandler(jump->destination(),
				      &stack_frame,
				      &arg_frame);

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_APPLY(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = vm->registers_.argument(0);
  return applyProcedure(vm, pc);
}

void KevesVM::cmd_APPLY_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = *pc;
  return applyProcedure(vm, pc);
}

void KevesVM::applyProcedure(KevesVM* vm, const_KevesIterator pc) {
  KevesValue proc(vm->gr1_);

  if (proc.isPtr()) {
    vm->keves_vals_ = nullptr;
    kev_type type(proc.type());

    if (type == CPS) {
      const CPSKev* cps(proc);
      vm->gr2_ = cps->who();
      return (*cps->func())(vm, pc);
    }

    if (type == LAMBDA) {
      const LambdaKev* lambda(proc);
      vm->acc_ = proc;
      const CodeKev* code(lambda->code());
      vm->current_code_ = code;
      return cmd_NOP(vm, lambda->proc());
    }

    if (type == CONTINUATION)
      return cmd_UNWIND_CONTINUATION(vm, pc);
  }

  return raiseAssertFirstObjNotProc(vm, pc);
}

void KevesVM::raiseAssertFirstObjNotProc(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->common_->builtin()->sym_eval();
  vm->gr2_ = vm->gr1_;
  vm->gr1_ = vm->common_->getMesgText(KevesBuiltinValues::mesg_1stObjNotProcOrSyn);
  return raiseAssertCondition(vm, pc);
}

void KevesVM::cmd_CALL_LAMBDA(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum num_arg(*pc);

  Q_ASSERT(num_arg >= 0);
  
  int argn(registers->argn());

  if (argn == num_arg + 1) {
    registers->makeEnvFrame(&vm->gc_, num_arg);
    registers->setArgumentsToLocalVars();
    return cmd_CALL_LAMBDA_helper(vm, pc);
  }

  return (argn > num_arg + 1 ?
	  raiseAssertLambdaReqLess : raiseAssertLambdaReqMore)(vm, pc);
}

void KevesVM::cmd_CALL_LAMBDA_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  const LambdaKev* lambda(vm->acc_);
  registers->clearArgFrame();

  if (lambda->free_vars()) {
    registers->setClosure(vm->gc_.toMutable(lambda->free_vars()));
  }

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_CALL_LAMBDA_VLA(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum num_arg(*pc);
  int num_arg_in_list(registers->argn() - num_arg - 1);
  
  if (num_arg_in_list < 0)
    return raiseAssertLambdaReqMore(vm, pc);
  
  registers->makeEnvFrame(&vm->gc_, num_arg + 1);
  
  for (int idx(0); idx < num_arg; ++idx) {
    registers->assignLocalVar(idx, registers->argument(idx + 1));
  }
  
  vm->gr1_ = EMB_NULL;
  vm->gr2_ = KevesFixnum(num_arg_in_list);

  return cmd_CALL_LAMBDA_VLA_helper(vm, pc);
}

void KevesVM::cmd_CALL_LAMBDA_VLA_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum num_arg_in_list(vm->gr2_);
  KevesFixnum num_arg(*pc);

  if (num_arg_in_list > 0) {
    KevesValue temp(registers->argument(num_arg_in_list + num_arg));
    PairKev pair(temp, vm->gr1_);

    vm->checkStack(&pair, &cmd_CALL_LAMBDA_VLA_helper, pc);

    vm->gr1_ = &pair;
    vm->gr2_ = KevesFixnum(num_arg_in_list - 1);
    return cmd_CALL_LAMBDA_VLA_helper(vm, pc);
  }
  
  registers->assignLocalVar(num_arg, vm->gr1_);
  return cmd_CALL_LAMBDA_helper(vm, pc);
}

void KevesVM::raiseAssertLambdaReqLess(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common_->getMesgText(KevesBuiltinValues::mesg_ReqLessGotMore);
  vm->gr2_ = EMB_NULL;
  return raiseAssertCondition(vm, pc);
}

void KevesVM::raiseAssertLambdaReqMore(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common_->getMesgText(KevesBuiltinValues::mesg_ReqMoreGotLess);
  vm->gr2_ = EMB_NULL;
  return raiseAssertCondition(vm, pc);
}

void KevesVM::cmd_APPLY_MULT(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (vm->keves_vals_) {
    KevesValue first_obj(registers->argument(0));
    registers->setArgFrame(vm->keves_vals_, KevesFixnum(vm->keves_vals_->at(0)));
    registers->replaceFirstArgument(first_obj);
  }

  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_RETURN(KevesVM* vm, const_KevesIterator) {
  return pushAccToArgumentSafe(vm, vm->registers_.unwind());
}

void KevesVM::cmd_RETURN_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = *pc;
  return pushAccToArgumentSafe(vm, vm->registers_.unwind());
}

void KevesVM::cmd_RETURN0(KevesVM* vm, const_KevesIterator) {
  StackFrameKev* registers(&vm->registers_);
  vm->acc_ = registers->lastArgument();
  return pushAccToArgumentSafe(vm, registers->unwind());
}

void KevesVM::cmd_RETURN_VOID(KevesVM* vm, const_KevesIterator) {
  return cmd_NOP(vm, vm->registers_.unwind());
}

void KevesVM::cmd_REFER_LOCAL(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum idx(*pc);
  vm->acc_ = vm->registers_.lastLocalVar(idx);
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_REFER_LOCAL0(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum idx(*pc);
  vm->gr1_ = vm->registers_.lastLocalVar(idx);
  return pushGr1ToArgument(vm, pc + 1);
}

void KevesVM::cmd_REFER_FREE(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum idx(*pc);
  vm->acc_ = vm->registers_.freeVar(idx);
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_REFER_FREE0(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum idx(*pc);
  vm->gr1_ = vm->registers_.freeVar(idx);
  return pushGr1ToArgument(vm, pc + 1);
}

void KevesVM::cmd_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = *pc;
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_ASSIGN_LOCAL(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  registers->assignLastLocalVar(vm->acc_, KevesFixnum(*pc));
  vm->keves_vals_ = nullptr;
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_ASSIGN_LOCAL0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  registers->assignLastLocalVar(registers->lastArgument(), KevesFixnum(*pc));
  vm->keves_vals_ = nullptr;
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_ASSIGN_FREE(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  registers->assignFreeVar(KevesFixnum(*pc), vm->acc_);
  vm->keves_vals_ = nullptr;
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_ASSIGN_FREE0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  registers->assignFreeVar(KevesFixnum(*pc), registers->lastArgument());
  vm->keves_vals_ = nullptr;
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_ASSIGN_MULT(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum idx(vm->keves_vals_->at(0));
  KevesValue list_obj(*pc);
  KevesValue temp;

  while (list_obj.isPair()) {
    const PairKev* pair(list_obj);
    temp = pair->car();
    list_obj = pair->cdr();
    registers->assignLastLocalVar(vm->keves_vals_->at(--idx), KevesFixnum(temp));
  }

  vm->keves_vals_ = nullptr;
  registers->replaceLastArgument(EMB_UNDEF);
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_CLOSE_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum num_local_var(*(pc + 1));
  LocalVarFrameKev* closure(registers->close());
  const_KevesIterator body(pc + 2);
  LambdaKev lambda(closure, vm->gc_.toMutable(vm->current_code_), body);
  
  vm->keves_vals_ = nullptr;
  KevesFixnum offset(*pc);
  vm->gr1_ = &lambda;
  return pushGr1ToArgumentSafe(vm, pc + offset + 3);
}

void KevesVM::cmd_NEW_BOX(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum frame_size(*pc);
  vm->registers_.makeEnvFrame(&vm->gc_, frame_size);
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_BOX(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum q_num(*pc); // Don't increase pc here.
  int size(q_num);

  if (size > 0) {
    StackFrameKev* registers(&vm->registers_);
    /*
    int actual_frame_size(registers->envFrameSize());
    int frame_size(size + registers->envn());
    
    if (frame_size > actual_frame_size) {
      if (actual_frame_size == 0) actual_frame_size = 4;

      do {
	actual_frame_size *= 2;
      } while (frame_size > actual_frame_size);

      LocalVarFrameKev* env_frame(LocalVarFrameKev::make(&vm->gc_, actual_frame_size));

      registers->extendEnvFrame(env_frame);
    }

    auto ctor = [registers](void* ptr) {
      registers->pushLocal(new(ptr) ReferenceKev());
    };

  // vm->gc_.makeArray(StackFrameKev::initializeEnvFrame(registers), size);
    vm->gc_.makeArray(ctor, sizeof(ReferenceKev), size);
    */
    registers->extendEnvFrame(&vm->gc_, size);
    // for (int i(0); i < size; ++i) registers->pushLocal(EMB_UNDEF);
  }

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_RAISE(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  *registers = *registers->sfp();

  if (StackFrameKev::isBottom(registers)) {
    pc = vm->common_->builtin()->code_HALT();
    std::cout << "cmd_RAISE: raise!!!!!!!!!!!!!" << std::endl; 
    vm->gr1_ = vm->acc_;
    return pushGr1ToArgumentSafe(vm, pc);
  }
  
  std::cout << "cmd_RAISE: catch exception!!!!!!!!!!!!!" << std::endl; 
  pc = registers->pc();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_RAISE_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue condition(*pc);
  *registers = *registers->sfp();

  if (StackFrameKev::isBottom(registers)) {
    pc = vm->common_->builtin()->code_HALT();
    std::cout << "cmd_RAISE_CONSTANT: raise!!!!!!!!!!!!!" << std::endl; 
    vm->gr1_ = condition;
    return pushGr1ToArgumentSafe(vm, pc);
  }
  
  std::cout << "cmd_RAISE_CONSTANT: catch exception!!!!!!!!!!!!!" << std::endl; 
  pc = registers->pc();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_CLEAR_VALS(KevesVM* vm, const_KevesIterator pc) {
  vm->keves_vals_ = nullptr;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_DEBUG_CODE(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  {
    QString temp1;
    QString temp2;
    // try {
    temp1 = vm->common_->toString(vm->acc_);
      /*
    } catch (int i) {
      std::cerr << "KevesVM::cmd_DEBUG_CODE(): Allocation is failed!\n";
    }
    try {
      */
    if (registers->argn() > 0) temp2 = vm->common_->toString(registers->lastArgument());
      /*
    } catch (int i) {
      std::cerr << "KevesVM::cmd_DEBUG_CODE(): Allocation is failed!\n";
    }
      */
    if (pc->isInstruct()) {
      std::cout << "debug: "
		<< qPrintable(vm->common_->instruct_table()->getLabel(KevesInstruct(*pc)))
		<< std::endl;
      std::cout << "acc: " << qPrintable(temp1) << std::endl;
      std::cout << "last argument: " << qPrintable(temp2) << std::endl;
    } else {
      std::cout << "debug: " << pc->toPtr() << std::endl;
    }
  }

  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_LAST(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  vm->acc_ = registers->lastArgument();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_ARGUMENT(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum index(*pc);
  vm->acc_ = registers->lastArgument(index);
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_PUSH(KevesVM* vm, const_KevesIterator pc) {
  return pushAccToArgumentSafe(vm, pc);
}

void KevesVM::cmd_PUSH_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->keves_vals_ = nullptr;
  vm->gr1_ = *pc;
  return pushGr1ToArgument(vm, pc + 1);
}

void KevesVM::cmd_POP(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  registers->popArgument();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_TEST_QEV_EQUAL_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  return vm->acc_ == registers->lastArgument() ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_NUMBER_R(KevesVM* vm, const_KevesIterator pc) {
  return vm->acc_.isNumber() ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_NUMBER0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  vm->gr1_ = registers->lastArgument();
  return testNumber(vm, pc);
}

void KevesVM::cmd_TEST_NUMBER0_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());

  return arg0.isNumber() ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::testNumber(KevesVM* vm, const_KevesIterator pc) {
  return (vm->gr1_.isNumber() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_NUMBER_EQUAL_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());

  if (arg0.isFixnum()) {
    if (vm->acc_.isFixnum()) {
      return KevesFixnum(arg0) == KevesFixnum(vm->acc_) ?
	cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
    }

    if (vm->acc_.is<RationalNumberKev>()) {
      const RationalNumberKev* er_acc(vm->acc_);
      KevesValue int_acc(er_acc->toKevesFixnum());

      return (int_acc.isFixnum() && KevesFixnum(arg0) == KevesFixnum(int_acc)) ?
	cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
    }
    
    return cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
  }

  if (arg0.is<RationalNumberKev>()) {
    const RationalNumberKev* er_arg0(arg0);
    
    if (vm->acc_.isFixnum()) {
      KevesValue int_arg0(er_arg0->toKevesFixnum());
      
      return (int_arg0.isFixnum() && int_arg0 == KevesFixnum(vm->acc_)) ?
	cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
    }

    if (vm->acc_.is<RationalNumberKev>()) {
      const RationalNumberKev* er_acc(vm->acc_);

      return er_arg0->isEqualTo(*er_acc) ?
	cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
    }
    
    return cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
  }

  if (arg0.is<FlonumKev>()) {
    const FlonumKev* ir_arg0(arg0);

    if (vm->acc_.is<FlonumKev>()) {
      const FlonumKev* ir_acc(vm->acc_);

      return ir_arg0->isEqualTo(*ir_acc) ?
	cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
    }
    
    return cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
  }

  if (arg0.is<ExactComplexNumberKev>() &&
      vm->acc_.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* ir_arg0(arg0);
    const ExactComplexNumberKev* ec_acc(vm->acc_);
    
    return (ir_arg0->real().isEqualTo(ec_acc->real()) &&
	     ir_arg0->imag().isEqualTo(ec_acc->imag())) ?
      cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
  }

  if (arg0.is<InexactComplexNumberKev>() &&
	     vm->acc_.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* ir_arg0(arg0);
    const InexactComplexNumberKev* ic_acc(vm->acc_);
    return ir_arg0->isEqualTo(*ic_acc) ?
      cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
  }
  
  return cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_CHAR_R(KevesVM* vm, const_KevesIterator pc) {
  return vm->acc_.isChar() ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_CHAR0_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  return registers->lastArgument().isChar() ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_CHAR_EQUAL_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());

  return KevesChar(vm->acc_) == KevesChar(arg0) ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::cmd_TEST_SYMBOL(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.is<SymbolKev>() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_SYMBOL0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());
  return (arg0.is<SymbolKev>() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_SYMBOL_EQUAL_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (vm->acc_.isSymbol() && last.isSymbol()) {
    const SymbolKev* sym1(vm->acc_);
    const SymbolKev* sym2(last);

    return sym1->equals(*sym2) ?
      cmd_NOP(vm, pc + 2) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 2);
  }

  return cmd_NOP(vm, pc + KevesFixnum(*(pc + 1)) + 2);
}

void KevesVM::cmd_STRING_CLONE(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  StringKev copy;

  vm->checkStack(&copy, &cmd_STRING_CLONE, pc);

  if (str->size() > 0) copy.copyFrom(str->copy(&vm->gc_));
  vm->acc_ = &copy;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_STRING_APPEND(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  const StringKev* str1(registers->lastArgument());
  const StringKev* str2(vm->acc_);
  StringKev string;

  vm->checkStack(&string, &cmd_STRING_APPEND, pc);

  string.copyFrom(str1->append(&vm->gc_, *str2));
  vm->acc_ = &string;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_TEST_STRING_R(KevesVM* vm, const_KevesIterator pc) {
  return ((vm->acc_.isPtr() && vm->acc_.type() == STRING) ?
          cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1));
}

void KevesVM::cmd_TEST_STRING0_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());

  return ((arg0.isPtr() && arg0.type() == STRING) ?
          cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1));
}

void KevesVM::cmd_TEST_STRING_EQUAL_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (vm->acc_.isString() && last.isString()) {
    const StringKev* str1(vm->acc_);
    const StringKev* str2(last);

    return str1->equals(*str2) ?
      cmd_NOP(vm, pc + 2) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 2);
  }

  return cmd_NOP(vm, pc + KevesFixnum(*(pc + 1)) + 2);
}

void KevesVM::cmd_CONS(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(vm->registers_.lastArgument(), vm->acc_);
  vm->checkStack(&pair, &cmd_CONS, pc);
  vm->acc_ = &pair;
  return cmd_NOP(vm, pc);
}
      
void KevesVM::cmd_CONS_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(*pc, vm->acc_);
  vm->checkStack(&pair, &cmd_CONS_CONSTANT, pc);
  vm->acc_ = &pair;
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_CAR(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  vm->acc_ = pair->car();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_CAR0(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->registers_.lastArgument());
  vm->acc_ = pair->car();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_CDR(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  vm->acc_ = pair->cdr();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_CDR0(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->registers_.lastArgument());
  vm->acc_ = pair->cdr();
  return cmd_NOP(vm, pc);
}
      
void KevesVM::cmd_TEST_PAIR_R(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.isPair() ?
          cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1));
}

void KevesVM::cmd_TEST_PAIR0_R(KevesVM* vm, const_KevesIterator pc) {
  return (vm->registers_.lastArgument().isPair() ?
          cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1));
}
      
void KevesVM::raiseAssertReqRealNum(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common_->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
  vm->gr2_ = EMB_NULL;
  return raiseAssertCondition(vm, pc);
}

void KevesVM::cmd_TEST_NULL_R(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_ == EMB_NULL ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_NULL0_R(KevesVM* vm, const_KevesIterator pc) {
  return (vm->registers_.lastArgument() == EMB_NULL ?
	  cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_LIST_TO_VECTOR(KevesVM* vm, const_KevesIterator pc) {
  KevesValue temp(vm->acc_);
  int size(getListLength(temp));
  VectorKev* vector(vm->makeVector(size));
  
  for (KevesValue& elem : *vector ) {
    const PairKev* pair(temp);
    elem = pair->car();
    temp = pair->cdr();
  }

  vm->acc_ = vector;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_BEGIN(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &cmd_BEGIN, pc);

  LocalVarFrameKev clsr;
  const LambdaKev* proc(vm->acc_);

  StackFrameKev stack_frame;
  registers->wind(pc, &stack_frame, &arg_frame);

  registers->pushArgument(vm->acc_); // push procedure
  registers->pushArgument(vm->gr1_); // push code

  pc = proc->proc();

  // get program counter
  KevesValue num_local_variable(*pc);
  
  registers->setClosure(&clsr);
  registers->changeToBottomFrame();

  // prepare environment frame
  vm->gr1_ = num_local_variable;
  return cmd_BEGIN_helper(vm, pc + 1);
}

void KevesVM::cmd_BEGIN_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum frame_size(vm->gr1_);
  registers->makeEnvFrame(&vm->gc_, frame_size);
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_TEST_CONDITION(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.isCondition() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_SYNTAX(KevesVM* vm, const_KevesIterator pc) {
  KevesValue datum(*pc);

  if (datum.isPair() || datum.isSymbol()) {
    WrappedKev wrapped(*pc,
		       EMB_NULL,
		       EMB_NULL,
		       vm->curt_global_vars_.values());

    vm->keves_vals_ = nullptr;
    vm->gr1_ = &wrapped;
    return pushGr1ToArgumentSafe(vm, pc + 1);
  }

  return pushGr1ToArgumentSafe(vm, pc + 1);
}

void KevesVM::cmd_TERMINATE_EXPAND(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* expanded(vm->acc_);
  int size(expanded->size());
  int cnt(0);

  for (int i(0); i < size; ++i) {
    KevesValue temp(expanded->at(i));
    if (temp.isDestination()) ++cnt;
  }
  
  CodeKev* code(CodeKev::make(&vm->gc_, size - cnt));
  // CodeKev* code(CodeKev::makeWithPermanent(&vm->gc_, size - cnt));
  // CodeKev* code(CodeKev::makeWithTenured(&vm->gc_, size - cnt));
  KevesIterator itr(code->end());

  while (size-- > 0) {
    KevesValue temp(expanded->at(size));

    if (temp.isDestination()) {
      const DestinationKev* dest(temp);
      const JumpKev* jump_obj(dest->label());
      vm->gc_.toMutable(jump_obj)->set(itr);
    } else {
      *--itr = temp;
    }
  }

  Q_ASSERT(itr == code->begin());

  LambdaKev proc(code, 0);
  vm->acc_ = &proc;
  vm->gr1_ = code;
  // vm->executeGC(&cmd_NOP, pc);
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_ADD_GLOBAL_VARIABLE(KevesVM* vm, const_KevesIterator pc) {
  Q_ASSERT(vm->acc_.is<EnvironmentKev>());

  // const EnvironmentKev* env(vm->acc_);
  // vm->curt_global_vars_ = *env;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_EXPORT_SYMBOL(KevesVM* vm, const_KevesIterator pc) {
  const SymbolKev* id(*pc);
  KevesFixnum index(*(pc + 1));
  PairKev pair1(EMB_NULL, EMB_NULL);
  PairKev pair2(EMB_NULL, EMB_NULL);

  vm->prev_global_vars_.append(id,
			       vm->registers_.lastLocalVar(index),
			       &pair1,
			       &pair2);

  vm->acc_ = id;
  return pushAccToArgumentSafe(vm, pc + 2);
}

void KevesVM::cmd_TEST_ENVIRONMENT(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.is<EnvironmentKev>() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

// make &lexical exception
void KevesVM::makeLexicalException(KevesVM* vm, const_KevesIterator pc) {
  VectorKevWithArray<04> values(4);

  vm->checkStack(&values, &makeLexicalException, pc);

  SimpleConditionKev violation(vm->common_->builtin()->amp_lexical(), EMB_NULL);
  SimpleConditionKev who(vm->common_->builtin()->amp_who(), vm->acc_);
  SimpleConditionKev message(vm->common_->builtin()->amp_message(), vm->gr1_);
  SimpleConditionKev irritants(vm->common_->builtin()->amp_irritants(), vm->gr2_);
  CompoundConditionKev condition(&values);
  KevesIterator iter(values.begin());
  *iter++ = &violation;
  *iter++ = &who;
  *iter++ = &message;
  *iter++ = &irritants;
  vm->acc_ = &condition;
  return cmd_NOP(vm, pc);
}

// raise &assert
void KevesVM::raiseAssertCondition(KevesVM* vm, const_KevesIterator pc) {
  VectorKevWithArray<04> values(4);

  vm->checkStack(&values, &raiseAssertCondition, pc);

  SimpleConditionKev violation(vm->common_->builtin()->amp_lexical(), EMB_NULL);
  SimpleConditionKev who(vm->common_->builtin()->amp_who(), vm->acc_);
  SimpleConditionKev message(vm->common_->builtin()->amp_message(), vm->gr1_);
  SimpleConditionKev irritants(vm->common_->builtin()->amp_irritants(), vm->gr2_);
  CompoundConditionKev condition(&values);
  KevesIterator iter(values.begin());
  *iter++ = &violation;
  *iter++ = &who;
  *iter++ = &message;
  *iter++ = &irritants;
  vm->acc_ = &condition;
  return cmd_RAISE(vm, pc);
}

// find condition value
KevesValue KevesVM::findConditionValue(KevesValue condition, RecordKev* type) {
  if (!type || !condition.isCondition())
    return EMB_UNDEF;

  if (condition.is<SimpleConditionKev>()) {
    const SimpleConditionKev* simple(condition);

    if (simple->field() == type)
      return simple->value();
  }

  KevesStack stack;

  stack.push(condition);

  while (!stack.isEmpty()) {
    condition = stack.pop();

    if (condition.is<SimpleConditionKev>()) {
      const SimpleConditionKev* simple(condition);

      if(simple->field() == type)
	return simple->value();

    } else if (condition.is<CompoundConditionKev>()) {
      const CompoundConditionKev* compound(condition);
      const VectorKev* contentStack(compound->values());
      for (KevesValue elem : *contentStack) stack.push(elem);
    }
  }

  return EMB_UNDEF;
}


void KevesVM::cmd_UNWIND_CONTINUATION(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &cmd_UNWIND_CONTINUATION, pc);

  const ContinuationKev* conti_obj(vm->gr1_);

  // keep arguments as multiple values
  vm->keves_vals_ = registers->argp();
  vm->keves_vals_->assign(0, KevesFixnum(registers->argn()));

  // trace back to current-continuation
  // if WindKev object is found, set <after> of it and *this*
  // into <begin> sequence and return. 
  const StackFrameKev* fp(registers->fp());
  KevesValue first_obj(fp->argn() > 0 ? fp->argument(0) : EMB_NULL);

  for (;;) {
    if (StackFrameKev::isBottom(fp)) {
      pc = registers->unwind();
      vm->gc_.toMutable(conti_obj->btmp())->set_pc(pc);
      *registers = conti_obj->registers();
      pc = vm->common_->builtin()->code_REVERT_DYNAMIC_WIND_and_APPLY();
      vm->acc_ = conti_obj;
      vm->gr1_ = vm->keves_vals_->at(1);

      return pushGr1ToArgument(vm, pc); // return value
    }

    if (fp->argp() == conti_obj->registers().argp()) {
      pc = registers->unwind();
      *registers = conti_obj->registers();
      vm->gr1_ = vm->keves_vals_->at(1);
      return pushGr1ToArgument(vm, pc); // return value
    }
    
    pc = registers->unwind();

    if (first_obj.is<WindKev>()) {
      pc = registers->unwind();
      StackFrameKev stack_frame1;
      registers->windWithValues(pc, &stack_frame1, vm->keves_vals_);
      pc = vm->common_->builtin()->code_POP_APPLY();
      registers->replaceFirstArgument(conti_obj);

      StackFrameKev stack_frame2;
      registers->wind(pc, &stack_frame2, &arg_frame);
      const WindKev* wind(first_obj);
      registers->pushArgument(wind->after());
      return cmd_APPLY(vm, pc);
    }

    fp = registers->fp();
    first_obj = fp->argn() > 0 ? fp->argument(0) : EMB_NULL;
  }
}

void KevesVM::cmd_REVERT_DYNAMIC_WIND(KevesVM* vm, const_KevesIterator pc) {
  const ContinuationKev* conti_obj(vm->acc_);
  StackFrameKev frame(conti_obj->registers());
  vm->gr1_ = KevesValue(&frame);
  return revertDynamicWind(vm, pc);
}

void KevesVM::revertDynamicWind(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->checkStack(&arg_frame, &revertDynamicWind, pc);

  const ContinuationKev* conti_obj(vm->acc_);
  const StackFrameKev* temp_fp(vm->gr1_);

  if (temp_fp->fp() == conti_obj->btmp())
    return cmd_NOP(vm, pc);
  
  StackFrameKev stack_frame;
  temp_fp = temp_fp->fp();
  KevesValue first_obj(temp_fp->argn() > 0 ?
		       temp_fp->argument(0) : KevesValue(EMB_NULL));
  
  if (first_obj.is<WindKev>()) {
    const WindKev* wind(first_obj);
    pc = vm->common_->builtin()->code_POP_APPLY();
    registers->wind(pc, &stack_frame, &arg_frame);
    pc = vm->common_->builtin()->code_APPLY();
    registers->pushArgument(wind->before());
  }
  
  vm->gr1_ = KevesValue(temp_fp);
  return revertDynamicWind(vm, pc);
}

void KevesVM::cmd_SET_DYNAMIC_WIND(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame1;

  vm->checkStack(&arg_frame1, &cmd_SET_DYNAMIC_WIND, pc);

  ArgumentFrameKevWithArray<04> arg_frame2;
  KevesValue before(registers->argument(1));
  KevesValue thunk(registers->argument(2));
  KevesValue after(registers->argument(3));

  // wind
  StackFrameKev stack_frame1;
  registers->wind(pc, &stack_frame1, &arg_frame1);
  pc = vm->common_->builtin()->code_REMOVE_DYNAMIC_WIND();

  WindKev wind(before, thunk, after);
  registers->pushArgument(&wind);

  // wind
  StackFrameKev stack_frame2;
  registers->wind(pc, &stack_frame2, &arg_frame2);
  registers->pushArgument(thunk);
  return cmd_APPLY(vm, pc);
}

void KevesVM::cmd_REMOVE_DYNAMIC_WIND(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  StackFrameKev stack_frame;

  vm->checkStack(&stack_frame, &cmd_REMOVE_DYNAMIC_WIND, pc);
  const WindKev* first_obj(registers->argument(0));
  KevesValue result_of_thunk(registers->lastArgument());
  pc = registers->unwind();

  registers->replaceLastArgument(result_of_thunk);
  pc = vm->common_->builtin()->code_POP_RETURN();

  // wind and push *after*
  ArgumentFrameKevWithArray<04> arg_frame;
  registers->wind(pc, &stack_frame, &arg_frame);
  registers->pushArgument(first_obj->after());
  return cmd_APPLY(vm, pc);
}
