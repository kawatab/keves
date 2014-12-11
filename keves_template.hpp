// keves/keves_template.hpp - templates for Keves
// Keves will be an R6RS Scheme implementation.
//
//  Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
// 
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
// 
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include "keves_base.hpp"
#include "keves_builtin_values.hpp"
#include "keves_vm.hpp"
#include "kev/number.hpp"


namespace Function {
  struct Anything {
    static constexpr bool func(KevesValue) {
      return true;
    }
    
    static constexpr KevesValue message(KevesBase* /*base*/) {
      return EMB_UNDEF;
    }

    static constexpr KevesValue message1(KevesBase* /*base*/) {
      return EMB_UNDEF;
    }

    static constexpr KevesValue message2(KevesBase* /*base*/) {
      return EMB_UNDEF;
    }

    static constexpr KevesValue message3(KevesBase* /*base*/) {
      return EMB_UNDEF;
    }
  };

  struct IsFixnum {
    static bool func(KevesValue);
    static KevesValue message(KevesBase* base);
    static KevesValue message1(KevesBase* base);
    static KevesValue message2(KevesBase* base);
    static KevesValue message3(KevesBase* base);
  };

  struct IsChar {
    static bool func(KevesValue);
    static KevesValue message(KevesBase* base);
    static KevesValue message1(KevesBase* base);
    static KevesValue message2(KevesBase* base);
  };

  struct IsString {
    static bool func(KevesValue);
    static KevesValue message(KevesBase* base);
    static KevesValue message1(KevesBase* base);
  };

  struct IsSymbol {
    static bool func(KevesValue);
    static KevesValue message(KevesBase* base);
  };
    
  struct IsPair {
    static bool func(KevesValue);
    static KevesValue message(KevesBase* base);
  };

  void raiseGetAnything(KevesVM*, const_KevesIterator);
  void raiseNotGet1Arg(KevesVM*, const_KevesIterator);
  void raiseNotGet2Args(KevesVM*, const_KevesIterator);
  void raiseNotGet2OrMoreArgs(KevesVM*, const_KevesIterator);
  void raiseNotGet3Args(KevesVM*, const_KevesIterator);

  template<class FUNC>
  void thunk(KevesVM* vm, const_KevesIterator pc)
  {
    return vm->registers_.argn() == 1 ?
      FUNC::func(vm, pc) : raiseGetAnything(vm, pc);
  }

  template<class TEST>
  void predicate(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() == 2) {
      vm->acc_ = TEST::func(registers->lastArgument()) ? EMB_TRUE : EMB_FALSE;
      return KevesVM::ReturnValue(vm, pc);
    }

    return raiseNotGet1Arg(vm, pc);
  }

  template<class PRETEST, class TEST>
  void predicate(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() != 2)
      return raiseNotGet1Arg(vm, pc);
    
    KevesValue last(registers->lastArgument());

    if (PRETEST::func(last)) {
      vm->acc_ = TEST::func(last) ? EMB_TRUE : EMB_FALSE;
      return KevesVM::ReturnValue(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = PRETEST::message(vm->base());
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  template<class TEST, class COMPARE>
  void compare_error(KevesVM* vm, const_KevesIterator pc)
  {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = TEST::message(vm->base());
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  template<class TEST, class COMPARE>
  void compare(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3)
      return raiseNotGet2OrMoreArgs(vm, pc);

    KevesValue last(registers->lastArgument());
    registers->popArgument();

    if (!TEST::func(last))
      return compare_error<TEST, COMPARE>(vm, pc);
    
    for (;;) {
      KevesValue current(registers->lastArgument());

      if (!TEST::func(current))
	return compare_error<TEST, COMPARE>(vm, pc);

      if (!COMPARE::func(current, last)) {
	while (registers->argn() > 2) {
	  registers->popArgument();

	  if (!TEST::func(registers->lastArgument()))
	    return compare_error<TEST, COMPARE>(vm, pc);
	}

	vm->acc_ = EMB_FALSE;
	return KevesVM::ReturnValue(vm, pc);
      }

      if (registers->argn() < 3)
	break;

      last = current;
      registers->popArgument();
    }

    vm->acc_ = EMB_TRUE;
    return KevesVM::ReturnValue(vm, pc);
  }

  template<class TEST, class COMPOSE>
  void compose(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() > 2) {
      KevesValue result(registers->lastArgument());
      registers->popArgument();

      if (!TEST::func(result)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST::message(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }
      
      for (;; registers->popArgument()) {
	KevesValue current(registers->lastArgument());

	if (!TEST::func(current)) {
	  vm->acc_ = vm->gr2_;
	  vm->gr1_ = TEST::message(vm->base());
	  vm->gr2_ = EMB_NULL;
	  return KevesVM::RaiseAssertCondition(vm, pc);
	}
	
	result = COMPOSE::func(vm->gc(), result, current);
	
	if (registers->argn() < 3)
	  break;
      }
      
      vm->acc_ = result;
      return KevesVM::ReturnValue(vm, pc);
    }

    if (registers->argn() == 2) {
      KevesValue kev(registers->lastArgument());
      
      if (!TEST::func(kev)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST::message(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }
      
      vm->acc_ = kev;
      return COMPOSE::func1(vm, pc);
    }
    
    return COMPOSE::func0(vm, pc);
  }

  template<class TEST, class COMPOSE>
  void composeTwoOrMore(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3)
      return raiseNotGet2OrMoreArgs(vm, pc);

    KevesValue result(registers->lastArgument());
    registers->popArgument();

    if (!TEST::func(result)) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = TEST::message(vm->base());
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    for (;; registers->popArgument()) {
      KevesValue current(registers->lastArgument());

      if (!TEST::func(current)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST::message(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }
      
      result = COMPOSE::func(vm->gc(), result, current);

      if (registers->argn() < 3) {
	vm->acc_ = result;
	return KevesVM::ReturnValue(vm, pc);
      }
    }
  }

  template<class TEST, class MAKE>
  void make(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() != 2)
      return raiseNotGet1Arg(vm, pc);
    
    KevesValue last(registers->lastArgument());
    
    if (TEST::func(last)) {
      vm->acc_ = last;
      return MAKE::func(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = TEST::message(vm->base());
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  template<class TEST1, class TEST2, class MAKE>
  void make(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() == 3) {
      KevesValue kev1(registers->lastArgument(1));
      KevesValue kev2(registers->lastArgument());

      if (!TEST1::func(kev1)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST1::message1(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }

      if (!TEST2::func(kev2)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST2::message2(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }

      vm->acc_ = kev1;
      vm->gr1_ = kev2;
      return MAKE::func(vm, pc);
    }

    return raiseNotGet2Args(vm, pc);
  }

  template<class TEST1, class TEST2, class TEST3, class MAKE>
  void make(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() == 4) {
      KevesValue kev1(registers->lastArgument(2));
      KevesValue kev2(registers->lastArgument(1));
      KevesValue kev3(registers->lastArgument());

      if (!TEST1::func(kev1)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST1::message1(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }

      if (!TEST2::func(kev2)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST2::message2(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }

      if (!TEST3::func(kev3)) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = TEST2::message3(vm->base());
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }

      return MAKE::func(vm, pc);
    }

    return raiseNotGet3Args(vm, pc);
  }
}

namespace Arithmatic {
  template<class, class, class, class>
  void func1(KevesVM*, const_KevesIterator);

  template<class, class, class, class>
  void func1a(KevesVM*, const_KevesIterator);

  template<class, class, class, class>
  void func2(KevesVM*, const_KevesIterator);

  template<class, class, class, class>
  void func2a(KevesVM*, const_KevesIterator);

  template<class, class>
  void func3(KevesVM*, const_KevesIterator);

  template<class, class>
  void func3a(KevesVM*, const_KevesIterator);

  template<class, class, class, class>
  void func4(KevesVM*, const_KevesIterator);

  template<class, class, class, class>
  void func4a(KevesVM*, const_KevesIterator);

  template<class, class> void func5(KevesVM*, const_KevesIterator);
  template<class, class> void func5a(KevesVM*, const_KevesIterator);

  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);
    int argn(registers->argn());

    if (argn > 2) {
      KevesValue last(registers->lastArgument());
      vm->acc_ = last;
      registers->popArgument();
      vm->gr1_ = registers->lastArgument();

      if (last.IsFixnum())
	return func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
      
      if (last.Is<RationalNumberKev>())
	return func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);

      if (last.Is<FlonumKev>())
	return func3<OPERATOR1, OPERATOR2>(vm, pc);

      if (last.Is<ExactComplexNumberKev>())
	return func4<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
      
      if (last.Is<InexactComplexNumberKev>())
	return func5<OPERATOR1, OPERATOR2>(vm, pc);
      
      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }

    return argn == 2 ? OPERATOR1::func1(vm, pc) : OPERATOR1::func0(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func1(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3) {
      KevesValue current(vm->gr1_);

      if (current.IsFixnum())
	return OPERATOR1::funcFixnumFixnum(vm, pc);

      if (current.Is<RationalNumberKev>())
	return OPERATOR1::funcRationalFixnum(vm, pc);

      if (current.Is<FlonumKev>())
	return OPERATOR1::funcFlonumFixnum(vm, pc);

      if (current.Is<ExactComplexNumberKev>())
	return OPERATOR1::funcExactComplexFixnum(vm, pc);

      if (current.Is<InexactComplexNumberKev>())
	return OPERATOR1::funcInexactComplexFixnum(vm, pc);

      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }

    return func1a<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func1a(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (vm->gr1_.IsFixnum()) {
      KevesFixnum num1(vm->gr1_);
      KevesFixnum num2(vm->acc_);
      FIXNUM result(OPERATOR2::func(num1, num2));

      if (KevesFixnum::isInRange(result)) {
	registers->popArgument();
	vm->acc_ = KevesFixnum(static_cast<fx_int>(result));
	vm->gr1_ = registers->lastArgument();
	return func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
      }

      registers->popArgument();
      RATIONAL rational(result);
      vm->acc_ = &rational;
      vm->gr1_ = registers->lastArgument();
      return func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
    }

    if (vm->gr1_.Is<RationalNumberKev>()) {
      KevesValue temp(vm->gr1_);
      vm->gr1_ = vm->acc_;
      vm->acc_ = temp;
      return func2a<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
    }
    
    if (vm->gr1_.Is<FlonumKev>()) {
      KevesValue temp(vm->gr1_);
      vm->gr1_ = vm->acc_;
      vm->acc_ = temp;
      return func3a<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (vm->gr1_.Is<ExactComplexNumberKev>()) {
      KevesValue temp(vm->gr1_);
      vm->gr1_ = vm->acc_;
      vm->acc_ = temp;
      return func4a<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
    }

    if (vm->gr1_.Is<InexactComplexNumberKev>()) {
      KevesValue temp(vm->gr1_);
      vm->gr1_ = vm->acc_;
      vm->acc_ = temp;
      return func5a<OPERATOR1, OPERATOR2>(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func2(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3) {
      KevesValue current(vm->gr1_);

      if (current.IsFixnum())
	return OPERATOR1::funcFixnumRational(vm, pc);

      if (current.Is<RationalNumberKev>())
	return OPERATOR1::funcRationalRational(vm, pc);

      if (current.Is<FlonumKev>())
	return OPERATOR1::funcFixnumRational(vm, pc);

      if (current.Is<ExactComplexNumberKev>())
	return OPERATOR1::funcExactComplexRational(vm, pc);

      if (current.Is<InexactComplexNumberKev>())
	return OPERATOR1::funcInexactComplexRational(vm, pc);
      
      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    return func2a<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func2a(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);
    const RationalNumberKev* num2(vm->acc_);
    KevesValue current(vm->gr1_);
    
    if (current.IsFixnum()) {
      KevesFixnum num1(current);
      RationalNumberKev result(OPERATOR2::func(vm->gc(), *num2, num1));
      registers->popArgument();
      
      return KevesVM::ContinueRationalNumberKev(vm, pc, &result,
						func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>);
    }

    if (current.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(current);
      RationalNumberKev result(OPERATOR2::func(vm->gc(), *num1, *num2));
      registers->popArgument();
      
      return KevesVM::ContinueRationalNumberKev(vm, pc, &result,
						func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>);
    }

    if (current.Is<FlonumKev>()) {
      const FlonumKev* num1(current);
      FlonumKev result(OPERATOR2::func(*num1, *num2));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func3<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (current.Is<ExactComplexNumberKev>()) {
      const ExactComplexNumberKev* num1(current);
      ExactComplexNumberKev result(OPERATOR2::func(vm->gc(), *num1, *num2));
      return KevesVM::ContinueExactComplexNumberKev(vm, pc, &result,
						    func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func4<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>);
    }

    if (current.Is<InexactComplexNumberKev>()) {
      const InexactComplexNumberKev* num1(current);
      InexactComplexNumberKev result(OPERATOR2::func(*num1, *num2));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func5<OPERATOR1, OPERATOR2>(vm, pc);
    }
    
    vm->acc_ = vm->gr2_;
    vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2>
  void func3(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3) {
      KevesValue current(vm->gr1_);

      if (current.IsFixnum())
	return OPERATOR1::funcFixnumFlonum(vm, pc);

      if (current.Is<RationalNumberKev>())
	return OPERATOR1::funcRationalFlonum(vm, pc);

      if (current.Is<FlonumKev>())
	return OPERATOR1::funcFlonumFlonum(vm, pc);

      if (current.Is<ExactComplexNumberKev>())
	return OPERATOR1::funcExactComplexFlonum(vm, pc);

      if (current.Is<InexactComplexNumberKev>())
	return OPERATOR1::funcInexactComplexFlonum(vm, pc);

      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    return func3a<OPERATOR1, OPERATOR2>(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2>
  void func3a(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);
    const FlonumKev* num2(vm->acc_);
    KevesValue current(vm->gr1_);

    if (current.IsFixnum()) {
      KevesFixnum num1(current);
      FlonumKev result(OPERATOR2::func(*num2, num1));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func3<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (current.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(current);
      FlonumKev result(OPERATOR2::func(*num2, *num1));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func3<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (current.Is<FlonumKev>()) {
      const FlonumKev* num1(current);
      FlonumKev result(OPERATOR2::func(*num1, *num2));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func3<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (current.Is<ExactComplexNumberKev>()) {
      const ExactComplexNumberKev* num1(current);
      InexactComplexNumberKev result(OPERATOR2::func(num1->toInexact(), *num2));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func5<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (current.Is<InexactComplexNumberKev>()) {
      const InexactComplexNumberKev* num1(current);
      InexactComplexNumberKev result(OPERATOR2::func(*num1, *num2));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func5<OPERATOR1, OPERATOR2>(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func4(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3) {
      KevesValue current(vm->gr1_);

      if (current.IsFixnum())
	return OPERATOR1::funcFixnumExactComplex(vm, pc);
      
      if (current.Is<RationalNumberKev>())
	return OPERATOR1::funcRationalExactComplex(vm, pc);
      
      if (current.Is<FlonumKev>())
	return OPERATOR1::funcFlonumExactComplex(vm, pc);
     
      if (current.Is<ExactComplexNumberKev>())
	return OPERATOR1::funcExactComplexExactComplex(vm, pc);
      
      if (current.Is<InexactComplexNumberKev>())
	return OPERATOR1::funcInexactComplexExactComplex(vm, pc);
      
      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }

    return func4a<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>(vm, pc);
  }

  template<class OPERATOR1, class OPERATOR2, class FIXNUM, class RATIONAL>
  void func4a(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);
    const ExactComplexNumberKev* num2(vm->acc_);
    KevesValue current(vm->gr1_);

    if (current.IsFixnum()) {
      KevesFixnum num1(current);
      ExactComplexNumberKev result(OPERATOR2::func(vm->gc(), *num2, num1));
      return KevesVM::ContinueExactComplexNumberKev(vm, pc, &result,
						    func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func4<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>);
    }

    if (current.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(current);
      ExactComplexNumberKev result(OPERATOR2::func(vm->gc(), *num2, *num1));
      return KevesVM::ContinueExactComplexNumberKev(vm, pc, &result,
						    func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func4<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>);
    }

    if (current.Is<FlonumKev>()) {
      const FlonumKev* num1(current);
      InexactComplexNumberKev result(OPERATOR2::func(num2->toInexact(), *num1));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func5<OPERATOR1, OPERATOR2>(vm, pc);
    }

    if (current.Is<ExactComplexNumberKev>()) {
      const ExactComplexNumberKev* num1(current);
      ExactComplexNumberKev result(OPERATOR2::func(vm->gc(), *num2, *num1));
      return KevesVM::ContinueExactComplexNumberKev(vm, pc, &result,
						    func1<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func2<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>,
						    func4<OPERATOR1, OPERATOR2, FIXNUM, RATIONAL>);
    }

    if (current.Is<InexactComplexNumberKev>()) {
      const InexactComplexNumberKev* num1(current);
      InexactComplexNumberKev result(OPERATOR2::func(num2->toInexact(), *num1));
      registers->popArgument();
      vm->acc_ = &result;
      vm->gr1_ = registers->lastArgument();
      return func5<OPERATOR1, OPERATOR2>(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2>
  void func5(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);

    if (registers->argn() < 3) {
      KevesValue current(vm->gr1_);

      if (current.IsFixnum())
	return OPERATOR1::funcFixnumInexactComplex(vm, pc);
      
      if (current.Is<RationalNumberKev>())
	return OPERATOR1::funcRationalInexactComplex(vm, pc);
      
      if (current.Is<FlonumKev>())
	return OPERATOR1::funcFlonumInexactComplex(vm, pc);
      
      if (current.Is<ExactComplexNumberKev>())
	return OPERATOR1::funcExactComplexInexactComplex(vm, pc);
      
      if (current.Is<InexactComplexNumberKev>())
	return OPERATOR1::funcInexactComplexInexactComplex(vm, pc);
      
      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }

    return func5a<OPERATOR1, OPERATOR2>(vm, pc);
  }
  
  template<class OPERATOR1, class OPERATOR2>
  void func5a(KevesVM* vm, const_KevesIterator pc)
  {
    StackFrameKev* registers(&vm->registers_);
    const InexactComplexNumberKev* num2(vm->acc_);
    InexactComplexNumberKev result { };
    KevesValue current(vm->gr1_);

    if (current.IsFixnum()) {
      KevesFixnum num1(current);
      result.CopyFrom(OPERATOR2::func(*num2, num1));
    } else if (current.Is<RationalNumberKev>()) {
      const RationalNumberKev* num1(current);
      result.CopyFrom(OPERATOR2::func(*num2, *num1));
    } else if (current.Is<FlonumKev>()) {
      const FlonumKev* num1(current);
      result.CopyFrom(OPERATOR2::func(*num2, *num1));
    } else if (current.Is<ExactComplexNumberKev>()) {
      const ExactComplexNumberKev* num1(current);
      result.CopyFrom(OPERATOR2::func(*num2, *num1));
    } else if (current.Is<InexactComplexNumberKev>()) {
      const InexactComplexNumberKev* num1(current);
      result.CopyFrom(OPERATOR2::func(*num2, *num1));
    } else {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = KevesValue(vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum));
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    registers->popArgument();
    vm->acc_ = &result;
    vm->gr1_ = registers->lastArgument();
    return func5<OPERATOR1, OPERATOR2>(vm, pc);
  }
}
