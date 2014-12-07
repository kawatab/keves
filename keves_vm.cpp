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
#include <unistd.h>
#include "keves_base.hpp"
#include "keves_builtin_values.hpp"
// #include "keves_file_io.hpp"
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


/*
KevesVM::KevesVM(QObject *parent)
  : QObject(parent) {}
*/

KevesVM* KevesVM::Make(KevesBase* base) {
  return Make(base, base->default_result_field());
}

KevesVM* KevesVM::Make(KevesBase* base, KevesTextualOutputPort* result_field) {
  KevesVM* vm(new KevesVM());
  vm->base_ = base;
  vm->cmd_table_ = base->cmd_table();
  vm->gc_.Init(vm, base->shared_list());
  vm->result_field_ = result_field;
  return vm;
}

void KevesVM::run() {
  Execute("");
}
  
void KevesVM::CheckStack(size_t* size, vm_func func, const_KevesIterator pc) {
  if (static_cast<void*>(size)
      < static_cast<void*>(static_cast<char*>(stack_safety_limit_) + *size)) {
    return ExecuteGC(func, pc);
  }
}

void KevesVM::CheckStack(const void* obj, vm_func func, const_KevesIterator pc) {
  if (obj < stack_safety_limit_)
    return ExecuteGC(func, pc);
}

// for number
void KevesVM::MakeRectangular(KevesVM* vm, const_KevesIterator pc) {
  KevesValue real(vm->acc_);
  KevesValue imag(vm->gr1_);

  if (real.IsRealNumber() && imag.IsRealNumber()) {
    if (real.IsFixnum()) {
      KevesFixnum fixnum(real);
      RationalNumberKevFromInt real_part(fixnum);

      if (imag.IsFixnum()) {
	KevesFixnum fixnum(imag);

	if (fixnum.isZero()) {
	  vm->acc_ = real;
	  return ReturnValueSafe(vm, pc);
	}
	
	RationalNumberKevFromInt imag_part(fixnum);
	ExactComplexNumberKev complex(real_part, imag_part);
	vm->acc_ = &complex;
	return ReturnValueSafe(vm, pc);
      }

      if (imag.Is<RationalNumberKev>()) {
	const RationalNumberKev* imag_part(imag);

	if (imag_part->isZero()) {
	  vm->acc_ = real;
	  return ReturnValueSafe(vm, pc);
	}
	
	ExactComplexNumberKev complex(real_part, *imag_part);
	vm->acc_ = &complex;
	return ReturnValueSafe(vm, pc);
      }
      
      {
	FlonumKev real_part(fixnum);
	const FlonumKev* imag_part(imag);
	InexactComplexNumberKev complex(real_part, *imag_part);
	vm->acc_ = &complex;
	return ReturnValueSafe(vm, pc);
      }
    }

    if (real.Is<RationalNumberKev>()) {
      const RationalNumberKev* real_part(real);
      
      if (imag.IsFixnum()) {
	KevesFixnum fixnum(imag);
	
	if (fixnum.isZero()) {
	  vm->acc_ = real;
	  return ReturnValueSafe(vm, pc);
	}
	
	RationalNumberKevFromInt imag_part(fixnum);
	ExactComplexNumberKev complex(*real_part, imag_part);
	vm->acc_ = &complex;
	return ReturnValueSafe(vm, pc);
      }

      if (imag.Is<RationalNumberKev>()) {
	const RationalNumberKev* imag_part(imag);

	if (imag_part->isZero()) {
	  vm->acc_ = real;
	  return ReturnValueSafe(vm, pc);
	}
	
	ExactComplexNumberKev complex(*real_part, *imag_part);
	vm->acc_ = &complex;
	return ReturnValueSafe(vm, pc);
      }
      
      FlonumKev flonum(real_part->toInexact());
      const FlonumKev* imag_part(imag);
      InexactComplexNumberKev complex(flonum, *imag_part);
      vm->acc_ = &complex;
      return ReturnValueSafe(vm, pc);
    }
    
    const FlonumKev* real_part(real);
    
    if (imag.IsFixnum()) {
      KevesFixnum fixnum(imag);
      FlonumKev imag_part(fixnum);
      InexactComplexNumberKev complex(*real_part, imag_part);
      vm->acc_ = &complex;
      return ReturnValueSafe(vm, pc);
    }
    
    if (imag.Is<RationalNumberKev>()) {
      const RationalNumberKev* rational(imag);
      FlonumKev imag_part(rational->toInexact());
      InexactComplexNumberKev complex(*real_part, imag_part);
      vm->acc_ = &complex;
      return ReturnValueSafe(vm, pc);
    }
    
    const FlonumKev* imag_part(imag);
    InexactComplexNumberKev complex(*real_part, *imag_part);
    vm->acc_ = &complex;
    return ReturnValueSafe(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base_->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
  vm->gr2_ = EMB_NULL;
  return RaiseAssertCondition(vm, pc);
}

// for list
int KevesVM::GetListLength(KevesValue temp) {
  int count(0);
  
  while (temp.IsPair()) {
    const PairKev* pair(temp);
    temp = pair->cdr();
    ++count;
  }

  return temp == EMB_NULL ? count : -1;
}


// for strings
StringKev* KevesVM::MakeString(int size) {
  StringKev* string(StringKev::Make(&this->gc_, size));
  return string;
}

const StringKev* KevesVM::MakeString(int size, QChar chr) {
  StringKev* string(MakeString(size));
  string->Fill(chr);
  return string;
}

// for vectors
VectorKev* KevesVM::MakeVector(int size) {
  VectorKev* vector(VectorKev::Make(&this->gc_, size));
  return vector;
}

const VectorKev* KevesVM::MakeVector(int size, KevesValue obj) {
  VectorKev* vector(MakeVector(size));
  vector->Fill(obj);
  return vector;
}


// instructions
void KevesVM::cmd_NOP(KevesVM* vm, const_KevesIterator pc) {

#ifndef QT_NO_DEBUG
  int dummy;
  vm->CheckStack(&dummy, &cmd_NOP, pc);
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

void KevesVM::PushGr1ToArgumentSafe(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

  vm->CheckStack(&argn, &PushGr1ToArgumentSafe, pc);

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->gr1_);
  return cmd_NOP(vm, pc);
}

void KevesVM::PushAccToArgumentSafe(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

  vm->CheckStack(&argn, &PushAccToArgumentSafe, pc);

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->acc_);
  return cmd_NOP(vm, pc);
}

void KevesVM::PushGr1ToArgument(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

#ifndef QT_NO_DEBUG
  vm->CheckStack(&argn, &PushGr1ToArgument, pc);
#endif

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->gr1_);
  return cmd_NOP(vm, pc);
}

void KevesVM::PushAccToArgument(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argp_size(registers->argp()->size());
  int argn(registers->argn());

#ifndef QT_NO_DEBUG
  vm->CheckStack(&argn, &PushAccToArgument, pc);
#endif

  if (argn >= argp_size) registers->extendArgFrame(&vm->gc_);
  registers->pushArgument(vm->acc_);
  return cmd_NOP(vm, pc);
}

void KevesVM::ReturnValue(KevesVM* vm, const_KevesIterator) {
  return PushAccToArgument(vm, vm->registers_.unwind());
  // return PushAccToArgumentSafe(vm, registers->unwind());
}

void KevesVM::ReturnValueSafe(KevesVM* vm, const_KevesIterator) {
  return PushAccToArgumentSafe(vm, vm->registers_.unwind());
}

void KevesVM::ReturnTwoValues(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->CheckStack(&arg_frame, &ReturnTwoValues, pc);
  
  arg_frame.replace(0, KevesFixnum(3));
  arg_frame.replace(1, vm->acc_);
  arg_frame.replace(2, vm->gr1_);
  vm->keves_vals_ = &arg_frame;
  vm->gr1_ = vm->acc_;
  return PushGr1ToArgument(vm, vm->registers_.unwind());
}

void KevesVM::ContinueRationalNumberKev(KevesVM* vm,
					const_KevesIterator pc,
					const RationalNumberKev* real,
					vm_func for_fixnum,
					vm_func for_real) {
  KevesValue fixnum(real->toKevesFixnum());
  vm->gr1_ = vm->registers_.lastArgument();

  if (fixnum.IsFixnum()) {
    vm->acc_ = KevesFixnum(fixnum);
    return for_fixnum(vm, pc);
  }
  
  vm->acc_ = real;
  return for_real(vm, pc);
}

void KevesVM::ContinueExactComplexNumberKev(KevesVM* vm,
					    const_KevesIterator pc,
					    const ExactComplexNumberKev* complex,
					    vm_func for_fixnum,
					    vm_func for_real,
					    vm_func for_complex) {
  StackFrameKev* registers(&vm->registers_);

  if (complex->imag().isZero()) {
    RationalNumberKev real(complex->real());
    return ContinueRationalNumberKev(vm, pc, &real, for_fixnum, for_real);
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
    return ReturnValueSafe(vm, pc);
  }
  
  vm->acc_ = complex;
  return ReturnValueSafe(vm, pc);
}

void KevesVM::ExecuteGC(vm_func current_func, const_KevesIterator pc) {
  current_function_ = current_func;
  return gc_.Execute(pc);
}

int KevesVM::Execute(const QString& arg) {
  pthread_attr_t attr;
  pthread_getattr_np(pthread_self(), &attr);
  
  size_t stack_size(0);
  pthread_attr_getstack(&attr, &stack_lower_limit_, &stack_size);
  stack_higher_limit_ = static_cast<char*>(stack_lower_limit_) + stack_size;
  stack_safety_limit_ = static_cast<char*>(stack_lower_limit_) + 1024 * 1024;
  stack_higher_limit_ = reinterpret_cast<char*>(&attr);
  std::cout << "higher limit: " << stack_higher_limit_ << std::endl;

  sleep(2);
  return Execute_helper(arg);
}

int KevesVM::Execute_helper(const QString& arg) {
  // This is a bottom frame.
  registers_.set_fp(&registers_);

  registers_.changeToBottomFrame();
  KevesLibrary* lib_keves_base(base_->GetLibrary("keves", "base-bin"));

  if (!lib_keves_base) {
    std::cerr << "library is not found!!!\n";
    return 1;
  }
  
  // KevesValue code(lib_keves_base->GetCode());
  KevesValue code(lib_keves_base->FindBind("my-code"));

  if (code == EMB_NULL) {
    std::cerr << "bind: my-code is not found!!!\n";
    return 1;
  } else if (code.IsCode()) {
    std::cout << "find code" << std::endl;
  }
  
  const_KevesIterator pc(code.ToPtr<CodeKev>()->begin());
  Q_ASSERT(pc->IsInstruct());

  // prepare environment frame
  LocalVarFrameKevWithArray<04> env_frame;
  registers_.setEnvFrame(&env_frame);

  // prepare stack frame
  ArgumentFrameKevWithArray<04> arg_frame;
  registers_.setArgFrame(&arg_frame);
  FreeVarFrameKev clsr;
  registers_.setClosure(&clsr);
  keves_vals_ = nullptr;
  acc_ = gr1_ = gr2_ = gr3_ = EMB_UNDEF;
  
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
    std::cout << "gc time: " << gc_.GetElapsedTime() << std::endl;
    return 0;

  default:
    (*current_function_)(this, current_pc_);
    return -1; // normally, not arrive here
  }
}

void KevesVM::cmd_HALT(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  /*
  KevesValue temp(registers->lastArgument());

  if (registers->sfp()) {
    KevesValue ref(registers->fp()->envp()->at(2));

    if (ref.Is<EnvironmentKev>()) {
      vm->curt_global_vars_.CopyFrom(vm->prev_global_vars_);
      vm->prev_global_vars_.CopyFrom(*ref.ToPtr<EnvironmentKev>());
      vm->acc_ = temp;
      gr1_ = gr2_ = gr3_ = EMB_UNDEF;
      return ReturnValue(vm, pc);
    }
    
    std::cerr << "KevesVM::cmd_HALT(): Not handle global values properly\n";
    longjmp(vm->jmp_exit_, -2);
  }
  */

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

void KevesVM::cmd_TEST0(KevesVM* vm, const_KevesIterator pc) {
  return (vm->registers_.lastArgument() != EMB_FALSE ?
	  cmd_SKIP : cmd_JUMP)(vm, pc);
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

  vm->CheckStack(&arg_frame, &cmd_FRAME, pc);

  StackFrameKev stack_frame;
  const JumpKev* jump(*pc);
  vm->registers_.wind(jump->destination(), &stack_frame, &arg_frame);

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_FRAME_R(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->CheckStack(&arg_frame, &cmd_FRAME_R, pc);

  StackFrameKev stack_frame;
  KevesFixnum offset(*pc);
  vm->registers_.wind(pc + offset + 1, &stack_frame, &arg_frame);

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_SFRAME(KevesVM* vm, const_KevesIterator pc) {
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->CheckStack(&arg_frame, &cmd_SFRAME, pc);

  StackFrameKev stack_frame;
  const JumpKev* jump(*pc);

  vm->registers_.windWithExceptionHandler(jump->destination(),
				      &stack_frame,
				      &arg_frame);

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_APPLY(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = vm->registers_.argument(0);
  return ApplyProcedure(vm, pc);
}

void KevesVM::cmd_APPLY_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = *pc;
  return ApplyProcedure(vm, pc);
}

void KevesVM::ApplyProcedure(KevesVM* vm, const_KevesIterator pc) {
  KevesValue proc(vm->gr1_);

  if (proc.IsPtr()) {
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

  return RaiseAssertFirstObjNotProc(vm, pc);
}

void KevesVM::RaiseAssertFirstObjNotProc(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->base_->builtin()->sym_eval();
  vm->gr2_ = vm->gr1_;
  vm->gr1_ = vm->base_->GetMesgText(KevesBuiltinValues::mesg_1stObjNotProcOrSyn);
  return RaiseAssertCondition(vm, pc);
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
	  RaiseAssertLambdaReqLess : RaiseAssertLambdaReqMore)(vm, pc);
}

void KevesVM::cmd_CALL_LAMBDA_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  const LambdaKev* lambda(vm->acc_);
  registers->clearArgFrame();

  if (lambda->free_vars()) {
    registers->setClosure(vm->gc_.ToMutable(lambda->free_vars()));
  }

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_CALL_LAMBDA_VLA(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesFixnum num_arg(*pc);
  int num_arg_in_list(registers->argn() - num_arg - 1);
  
  if (num_arg_in_list < 0)
    return RaiseAssertLambdaReqMore(vm, pc);
  
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

    vm->CheckStack(&pair, &cmd_CALL_LAMBDA_VLA_helper, pc);

    vm->gr1_ = &pair;
    vm->gr2_ = KevesFixnum(num_arg_in_list - 1);
    return cmd_CALL_LAMBDA_VLA_helper(vm, pc);
  }
  
  registers->assignLocalVar(num_arg, vm->gr1_);
  return cmd_CALL_LAMBDA_helper(vm, pc);
}

void KevesVM::RaiseAssertLambdaReqLess(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base_->GetMesgText(KevesBuiltinValues::mesg_ReqLessGotMore);
  vm->gr2_ = EMB_NULL;
  return RaiseAssertCondition(vm, pc);
}

void KevesVM::RaiseAssertLambdaReqMore(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base_->GetMesgText(KevesBuiltinValues::mesg_ReqMoreGotLess);
  vm->gr2_ = EMB_NULL;
  return RaiseAssertCondition(vm, pc);
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
  return PushAccToArgumentSafe(vm, vm->registers_.unwind());
}

void KevesVM::cmd_RETURN_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = *pc;
  return PushAccToArgumentSafe(vm, vm->registers_.unwind());
}

void KevesVM::cmd_RETURN0(KevesVM* vm, const_KevesIterator) {
  StackFrameKev* registers(&vm->registers_);
  vm->acc_ = registers->lastArgument();
  return PushAccToArgumentSafe(vm, registers->unwind());
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
  return PushGr1ToArgument(vm, pc + 1);
}

void KevesVM::cmd_REFER_FREE(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum idx(*pc);
  vm->acc_ = vm->registers_.freeVar(idx);
  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_REFER_FREE0(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum idx(*pc);
  vm->gr1_ = vm->registers_.freeVar(idx);
  return PushGr1ToArgument(vm, pc + 1);
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

  while (list_obj.IsPair()) {
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
  FreeVarFrameKev* closure(registers->clsr()->prepend(&vm->gc_, num_local_var));
  const_KevesIterator body(pc + 2);
  Q_ASSERT(KevesValue(*body).IsInstruct());
  
  Q_ASSERT(num_local_var == registers->envn());
  registers->copyEnvFrameTo(closure);
  
  LambdaKev lambda(closure, vm->gc_.ToMutable(vm->current_code_), body);
  
  vm->keves_vals_ = nullptr;
  KevesFixnum offset(*pc);
  vm->gr1_ = &lambda;
  return PushGr1ToArgumentSafe(vm, pc + offset + 3);
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
    int actual_frame_size(registers->envFrameSize());
    int frame_size(size + registers->envn());
    
    if (frame_size > actual_frame_size) {
      if (actual_frame_size == 0) actual_frame_size = 4;

      do {
	actual_frame_size *= 2;
      } while (frame_size > actual_frame_size);

      LocalVarFrameKev* env_frame(LocalVarFrameKev::Make(&vm->gc_, actual_frame_size));

      registers->extendEnvFrame(env_frame);
    }

    auto ctor = [registers](void* ptr) {
      registers->pushLocal(new(ptr) ReferenceKev());
    };

  // vm->gc_.makeArray(StackFrameKev::InitializeEnvFrame(registers), size);
    vm->gc_.MakeArray(ctor, sizeof(ReferenceKev), size);
  }

  return cmd_NOP(vm, pc + 1);
}

void KevesVM::cmd_RAISE(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  *registers = *registers->sfp();

  if (StackFrameKev::isBottom(registers)) {
    pc = vm->base_->builtin()->code_HALT();
    std::cout << "cmd_RAISE: raise!!!!!!!!!!!!!" << std::endl; 
    vm->gr1_ = vm->acc_;
    return PushGr1ToArgumentSafe(vm, pc);
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
    pc = vm->base_->builtin()->code_HALT();
    std::cout << "cmd_RAISE_CONSTANT: raise!!!!!!!!!!!!!" << std::endl; 
    vm->gr1_ = condition;
    return PushGr1ToArgumentSafe(vm, pc);
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
    temp1 = vm->base_->ToString(vm->acc_);
      /*
    } catch (int i) {
      std::cerr << "KevesVM::cmd_DEBUG_CODE(): Allocation is failed!\n";
    }
    try {
      */
    if (registers->argn() > 0) temp2 = vm->base_->ToString(registers->lastArgument());
      /*
    } catch (int i) {
      std::cerr << "KevesVM::cmd_DEBUG_CODE(): Allocation is failed!\n";
    }
      */
    if (pc->IsInstruct()) {
      std::cout << "debug: "
		<< qPrintable(vm->base_->instruct_table()->GetLabel(KevesInstruct(*pc)))
		<< std::endl;
      std::cout << "acc: " << qPrintable(temp1) << std::endl;
      std::cout << "last argument: " << qPrintable(temp2) << std::endl;
    } else {
      std::cout << "debug: " << pc->ToPtr() << std::endl;
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
  return PushAccToArgumentSafe(vm, pc);
}

void KevesVM::cmd_PUSH_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  vm->keves_vals_ = nullptr;
  vm->gr1_ = *pc;
  return PushGr1ToArgument(vm, pc + 1);
}

void KevesVM::cmd_POP(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  registers->popArgument();
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_TEST_QEV_EQUAL(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  return (vm->acc_ == registers->lastArgument() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_NUMBER(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = vm->acc_;
  return TestNumber(vm, pc);
}

void KevesVM::cmd_TEST_NUMBER0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  vm->gr1_ = registers->lastArgument();
  return TestNumber(vm, pc);
}

void KevesVM::cmd_TEST_NUMBER0_R(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());
  return arg0.IsNumber() ?
    cmd_SKIP(vm, pc) : cmd_NOP(vm, pc + KevesFixnum(*pc) + 1);
}

void KevesVM::TestNumber(KevesVM* vm, const_KevesIterator pc) {
  return (vm->gr1_.IsNumber() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_NUMBER_EQUAL(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());

  if (arg0.IsFixnum()) {
    if (vm->acc_.IsFixnum()) {
      return (KevesFixnum(arg0) == KevesFixnum(vm->acc_) ?
	      cmd_SKIP : cmd_JUMP)(vm, pc);
    }

    if (vm->acc_.Is<RationalNumberKev>()) {
      const RationalNumberKev* er_acc(vm->acc_);
      KevesValue int_acc(er_acc->toKevesFixnum());

      return ((int_acc.IsFixnum() &&
	       KevesFixnum(arg0) == KevesFixnum(int_acc)) ?
	      cmd_SKIP : cmd_JUMP)(vm, pc);
    }
    
    return cmd_JUMP(vm, pc);
  }

  if (arg0.Is<RationalNumberKev>()) {
    const RationalNumberKev* er_arg0(arg0);
    
    if (vm->acc_.IsFixnum()) {
      KevesValue int_arg0(er_arg0->toKevesFixnum());
      
      return ((int_arg0.IsFixnum() && int_arg0 == KevesFixnum(vm->acc_)) ?
	      cmd_SKIP : cmd_JUMP)(vm, pc);
    }

    if (vm->acc_.Is<RationalNumberKev>()) {
      const RationalNumberKev* er_acc(vm->acc_);
      return (er_arg0->isEqualTo(*er_acc) ? cmd_SKIP : cmd_JUMP)(vm, pc);
    }
    
    return cmd_JUMP(vm, pc);
  }

  if (arg0.Is<FlonumKev>()) {
    const FlonumKev* ir_arg0(arg0);

    if (vm->acc_.Is<FlonumKev>()) {
      const FlonumKev* ir_acc(vm->acc_);
      return (ir_arg0->isEqualTo(*ir_acc) ? cmd_SKIP : cmd_JUMP)(vm, pc);
    }
    
    return cmd_JUMP(vm, pc);
  }

  if (arg0.Is<ExactComplexNumberKev>() &&
      vm->acc_.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* ir_arg0(arg0);
    const ExactComplexNumberKev* ec_acc(vm->acc_);
    
    return ((ir_arg0->real().isEqualTo(ec_acc->real()) &&
	     ir_arg0->imag().isEqualTo(ec_acc->imag())) ?
	    cmd_SKIP : cmd_JUMP)(vm, pc);
  }

  if (arg0.Is<InexactComplexNumberKev>() &&
	     vm->acc_.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* ir_arg0(arg0);
    const InexactComplexNumberKev* ic_acc(vm->acc_);
    return (ir_arg0->isEqualTo(*ic_acc) ? cmd_SKIP : cmd_JUMP)(vm, pc);
  }
  
  return cmd_JUMP(vm, pc);
}

void KevesVM::cmd_TEST_CHAR(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.IsChar() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_CHAR0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  return (registers->lastArgument().IsChar() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_CHAR_EQUAL(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());
  return (KevesChar(vm->acc_) == KevesChar(arg0) ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_SYMBOL(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.Is<SymbolKev>() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_SYMBOL0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());
  return (arg0.Is<SymbolKev>() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_SYMBOL_EQUAL(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (vm->acc_.IsSymbol() && last.IsSymbol()) {
    const SymbolKev* sym1(vm->acc_);
    const SymbolKev* sym2(last);
    return sym1->Equals(*sym2) ? cmd_NOP(vm, pc + 2) : cmd_JUMP(vm, pc);
  }

  return cmd_JUMP(vm, pc + 1);
}

void KevesVM::cmd_STRING_CLONE(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  StringKev copy;

  vm->CheckStack(&copy, &cmd_STRING_CLONE, pc);

  if (str->size() > 0) copy.CopyFrom(str->Copy(&vm->gc_));
  vm->acc_ = &copy;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_STRING_APPEND(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  const StringKev* str1(registers->lastArgument());
  const StringKev* str2(vm->acc_);
  StringKev string;

  vm->CheckStack(&string, &cmd_STRING_APPEND, pc);

  string.CopyFrom(str1->Append(&vm->gc_, *str2));
  vm->acc_ = &string;
  return cmd_NOP(vm, pc);
}

void KevesVM::cmd_TEST_STRING(KevesVM* vm, const_KevesIterator pc) {
  return ((vm->acc_.IsPtr() && vm->acc_.type() == STRING) ?
	  cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_STRING0(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue arg0(registers->lastArgument());

  return ((arg0.IsPtr() && arg0.type() == STRING) ?
	  cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_STRING_EQUAL(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (vm->acc_.IsString() && last.IsString()) {
    const StringKev* str1(vm->acc_);
    const StringKev* str2(last);
    return str1->Equals(*str2) ? cmd_NOP(vm, pc + 2) : cmd_JUMP(vm, pc);
  }

  return cmd_JUMP(vm, pc + 1);
}

void KevesVM::cmd_CONS(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(vm->registers_.lastArgument(), vm->acc_);
  vm->CheckStack(&pair, &cmd_CONS, pc);
  vm->acc_ = &pair;
  return cmd_NOP(vm, pc);
}
      
void KevesVM::cmd_CONS_CONSTANT(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(*pc, vm->acc_);
  vm->CheckStack(&pair, &cmd_CONS_CONSTANT, pc);
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
      
void KevesVM::cmd_TEST_PAIR(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.IsPair() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_PAIR0(KevesVM* vm, const_KevesIterator pc) {
  return (vm->registers_.lastArgument().IsPair() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}
      
void KevesVM::RaiseAssertReqRealNum(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base_->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
  vm->gr2_ = EMB_NULL;
  return RaiseAssertCondition(vm, pc);
}

void KevesVM::cmd_TEST_NULL(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_ == EMB_NULL ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_TEST_NULL0(KevesVM* vm, const_KevesIterator pc) {
  return (vm->registers_.lastArgument() == EMB_NULL ?
	  cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_LIST_TO_VECTOR(KevesVM* vm, const_KevesIterator pc) {
  KevesValue temp(vm->acc_);
  int size(GetListLength(temp));
  VectorKev* vector(vm->MakeVector(size));
  
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

  vm->CheckStack(&arg_frame, &cmd_BEGIN, pc);

  FreeVarFrameKev clsr;
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
  return (vm->acc_.IsCondition() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

void KevesVM::cmd_SYNTAX(KevesVM* vm, const_KevesIterator pc) {
  KevesValue datum(*pc);

  if (datum.IsPair() || datum.IsSymbol()) {
    WrappedKev wrapped(*pc,
		       EMB_NULL,
		       EMB_NULL,
		       vm->curt_global_vars_.values());

    vm->keves_vals_ = nullptr;
    vm->gr1_ = &wrapped;
    return PushGr1ToArgumentSafe(vm, pc + 1);
  }

  return PushGr1ToArgumentSafe(vm, pc + 1);
}

void KevesVM::cmd_TERMINATE_EXPAND(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* expanded(vm->acc_);
  int size(expanded->size());
  int cnt(0);

  for (int i(0); i < size; ++i) {
    KevesValue temp(expanded->At(i));
    if (temp.IsDestination()) ++cnt;
  }
  
  CodeKev* code(CodeKev::Make(&vm->gc_, size - cnt));
  // CodeKev* code(CodeKev::makeWithPermanent(&vm->gc_, size - cnt));
  // CodeKev* code(CodeKev::makeWithTenured(&vm->gc_, size - cnt));
  KevesIterator itr(code->end());

  while (size-- > 0) {
    KevesValue temp(expanded->At(size));

    if (temp.IsDestination()) {
      const DestinationKev* dest(temp);
      const JumpKev* jump_obj(dest->label());
      vm->gc_.ToMutable(jump_obj)->set(itr);
    } else {
      *--itr = temp;
    }
  }

  Q_ASSERT(itr == code->begin());

  LambdaKev proc(code, 0);
  vm->acc_ = &proc;
  vm->gr1_ = code;
  vm->ExecuteGC(&cmd_NOP, pc);
}

void KevesVM::cmd_ADD_GLOBAL_VARIABLE(KevesVM* vm, const_KevesIterator pc) {
  Q_ASSERT(vm->acc_.Is<EnvironmentKev>());

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
  return PushAccToArgumentSafe(vm, pc + 2);
}

void KevesVM::cmd_TEST_ENVIRONMENT(KevesVM* vm, const_KevesIterator pc) {
  return (vm->acc_.Is<EnvironmentKev>() ? cmd_SKIP : cmd_JUMP)(vm, pc);
}

// make &lexical exception
void KevesVM::MakeLexicalException(KevesVM* vm, const_KevesIterator pc) {
  VectorKevWithArray<04> values(4);

  vm->CheckStack(&values, &MakeLexicalException, pc);

  SimpleConditionKev violation(RecordKev::AMP_LEXICAL(), EMB_NULL);
  SimpleConditionKev who(RecordKev::AMP_WHO(), vm->acc_);
  SimpleConditionKev message(RecordKev::AMP_MESSAGE(), vm->gr1_);
  SimpleConditionKev irritants(RecordKev::AMP_IRRITANTS(), vm->gr2_);
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
void KevesVM::RaiseAssertCondition(KevesVM* vm, const_KevesIterator pc) {
  VectorKevWithArray<04> values(4);

  vm->CheckStack(&values, &RaiseAssertCondition, pc);

  SimpleConditionKev violation(RecordKev::AMP_ASSERT(), EMB_NULL);
  SimpleConditionKev who(RecordKev::AMP_WHO(), vm->acc_);
  SimpleConditionKev message(RecordKev::AMP_MESSAGE(), vm->gr1_);
  SimpleConditionKev irritants(RecordKev::AMP_IRRITANTS(), vm->gr2_);
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
KevesValue KevesVM::FindConditionValue(KevesValue condition, RecordKev* type) {
  if (!type || !condition.IsCondition())
    return EMB_UNDEF;

  if (condition.Is<SimpleConditionKev>()) {
    const SimpleConditionKev* simple(condition);

    if (simple->field() == type)
      return simple->value();
  }

  KevesStack stack;

  stack.push(condition);

  while (!stack.isEmpty()) {
    condition = stack.pop();

    if (condition.Is<SimpleConditionKev>()) {
      const SimpleConditionKev* simple(condition);

      if(simple->field() == type)
	return simple->value();

    } else if (condition.Is<CompoundConditionKev>()) {
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

  vm->CheckStack(&arg_frame, &cmd_UNWIND_CONTINUATION, pc);

  const ContinuationKev* conti_obj(vm->gr1_);

  // keep arguments as multiple values
  vm->keves_vals_ = registers->argp();
  vm->keves_vals_->replace(0, KevesFixnum(registers->argn()));

  // trace back to current-continuation
  // if WindKev object is found, set <after> of it and *this*
  // into <begin> sequence and return. 
  const StackFrameKev* fp(registers->fp());
  KevesValue first_obj(fp->argn() > 0 ? fp->argument(0) : EMB_NULL);

  for (;;) {
    if (StackFrameKev::isBottom(fp)) {
      pc = registers->unwind();
      vm->gc_.ToMutable(conti_obj->btmp())->set_pc(pc);
      *registers = conti_obj->registers();
      pc = vm->base_->builtin()->code_REVERT_DYNAMIC_WIND_and_APPLY();
      vm->acc_ = conti_obj;
      vm->gr1_ = vm->keves_vals_->at(1);

      return PushGr1ToArgument(vm, pc); // return value
    }

    if (fp->argp() == conti_obj->registers().argp()) {
      pc = registers->unwind();
      *registers = conti_obj->registers();
      vm->gr1_ = vm->keves_vals_->at(1);
      return PushGr1ToArgument(vm, pc); // return value
    }
    
    pc = registers->unwind();

    if (first_obj.Is<WindKev>()) {
      pc = registers->unwind();
      StackFrameKev stack_frame1;
      registers->windWithValues(pc, &stack_frame1, vm->keves_vals_);
      pc = vm->base_->builtin()->code_POP_APPLY();
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
  return RevertDynamicWind(vm, pc);
}

void KevesVM::RevertDynamicWind(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;

  vm->CheckStack(&arg_frame, &RevertDynamicWind, pc);

  const ContinuationKev* conti_obj(vm->acc_);
  const StackFrameKev* temp_fp(vm->gr1_);

  if (temp_fp->fp() == conti_obj->btmp())
    return cmd_NOP(vm, pc);
  
  StackFrameKev stack_frame;
  temp_fp = temp_fp->fp();
  KevesValue first_obj(temp_fp->argn() > 0 ?
		       temp_fp->argument(0) : KevesValue(EMB_NULL));
  
  if (first_obj.Is<WindKev>()) {
    const WindKev* wind(first_obj);
    pc = vm->base_->builtin()->code_POP_APPLY();
    registers->wind(pc, &stack_frame, &arg_frame);
    pc = vm->base_->builtin()->code_APPLY();
    registers->pushArgument(wind->before());
  }
  
  vm->gr1_ = KevesValue(temp_fp);
  return RevertDynamicWind(vm, pc);
}

void KevesVM::cmd_SET_DYNAMIC_WIND(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame1;

  vm->CheckStack(&arg_frame1, &cmd_SET_DYNAMIC_WIND, pc);

  ArgumentFrameKevWithArray<04> arg_frame2;
  KevesValue before(registers->argument(1));
  KevesValue thunk(registers->argument(2));
  KevesValue after(registers->argument(3));

  // wind
  StackFrameKev stack_frame1;
  registers->wind(pc, &stack_frame1, &arg_frame1);
  pc = vm->base_->builtin()->code_REMOVE_DYNAMIC_WIND();

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

  vm->CheckStack(&stack_frame, &cmd_REMOVE_DYNAMIC_WIND, pc);
  const WindKev* first_obj(registers->argument(0));
  KevesValue result_of_thunk(registers->lastArgument());
  pc = registers->unwind();

  registers->replaceLastArgument(result_of_thunk);
  pc = vm->base_->builtin()->code_POP_RETURN();

  // wind and push *after*
  ArgumentFrameKevWithArray<04> arg_frame;
  registers->wind(pc, &stack_frame, &arg_frame);
  registers->pushArgument(first_obj->after());
  return cmd_APPLY(vm, pc);
}
