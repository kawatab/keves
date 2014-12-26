// keves/lib/rnrs-base/rnrs-base.hpp - library of Keves base
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


#pragma once

#include "keves_library.hpp"
#include "keves_vm.hpp"
#include "kev/number.hpp"
#include "kev/procedure.hpp"
#include "value/fixnum.hpp"


class KevesBase;
class SymbolKev;

class LibRnrsBase : public KevesLibrary {
public:
  LibRnrsBase() = default;
  LibRnrsBase(const LibRnrsBase&) = delete;
  LibRnrsBase(LibRnrsBase&&) = delete;
  LibRnrsBase& operator=(const LibRnrsBase&) = delete;
  LibRnrsBase& operator=(LibRnrsBase&&) = delete;
  ~LibRnrsBase() = default;

  void init(KevesBase* base);

  SymbolKev* sym_char_upcase_;
  CPSKev proc_char_upcase_;
  SymbolKev* sym_char_downcase_;
  CPSKev proc_char_downcase_;
  SymbolKev* sym_char_titlecase_;
  CPSKev proc_char_titlecase_;
  SymbolKev* sym_char_foldcase_;
  CPSKev proc_char_foldcase_;
  SymbolKev* sym_char_ci_eq_q_;
  CPSKev proc_char_ci_eq_q_;
  SymbolKev* sym_char_ci_lt_q_;
  CPSKev proc_char_ci_lt_q_;
  SymbolKev* sym_char_ci_gt_q_;
  CPSKev proc_char_ci_gt_q_;
  SymbolKev* sym_char_ci_lte_q_;
  CPSKev proc_char_ci_lte_q_;
  SymbolKev* sym_char_ci_gte_q_;
  CPSKev proc_char_ci_gte_q_;
  SymbolKev* sym_char_alphabetic_q_;
  CPSKev proc_char_alphabetic_q_;
  SymbolKev* sym_char_numeric_q_;
  CPSKev proc_char_numeric_q_;
  SymbolKev* sym_char_whitespace_q_;
  CPSKev proc_char_whitespace_q_;
  SymbolKev* sym_char_upper_case_q_;
  CPSKev proc_char_upper_case_q_;
  SymbolKev* sym_char_lower_case_q_;
  CPSKev proc_char_lower_case_q_;
  SymbolKev* sym_char_title_case_q_;
  CPSKev proc_char_title_case_q_;
  SymbolKev* sym_char_general_category_;
  CPSKev proc_char_general_category_;
  SymbolKev* sym_string_upcase_;
  CPSKev proc_string_upcase_;
  SymbolKev* sym_string_downcase_;
  CPSKev proc_string_downcase_;
  SymbolKev* sym_string_titlecase_;
  CPSKev proc_string_titlecase_;
  SymbolKev* sym_string_foldcase_;
  CPSKev proc_string_foldcase_;

  // SymbolKev* sym_eqv_q_;
  // LambdaKev proc_eqv_q_;
  // SymbolKev* sym_eq_q_;
  // LambdaKev proc_eq_q_;
  SymbolKev* sym_procedure_q_;
  CPSKev proc_procedure_q_;
  SymbolKev* sym_number_q_;
  CPSKev proc_number_q_;
  SymbolKev* sym_complex_q_;
  CPSKev proc_complex_q_;
  SymbolKev* sym_real_q_;
  CPSKev proc_real_q_;
  SymbolKev* sym_rational_q_;
  CPSKev proc_rational_q_;
  SymbolKev* sym_integer_q_;
  CPSKev proc_integer_q_;
  SymbolKev* sym_real_valued_q_;
  CPSKev proc_real_valued_q_;
  SymbolKev* sym_rational_valued_q_;
  CPSKev proc_rational_valued_q_;
  SymbolKev* sym_integer_valued_q_;
  CPSKev proc_integer_valued_q_;
  SymbolKev* sym_exact_q_;
  CPSKev proc_exact_q_;
  SymbolKev* sym_inexact_q_;
  CPSKev proc_inexact_q_;
  SymbolKev* sym_flexact_;
  CPSKev proc_flexact_;
  SymbolKev* sym_inexact_;
  CPSKev proc_inexact_;
  SymbolKev* sym_equal_;
  CPSKev proc_equal_;
  SymbolKev* sym_less_than_;
  CPSKev proc_less_than_;
  SymbolKev* sym_greater_than_;
  CPSKev proc_greater_than_;
  SymbolKev* sym_less_than_equal_;
  CPSKev proc_less_than_equal_;
  SymbolKev* sym_greater_than_equal_;
  CPSKev proc_greater_than_equal_;
  SymbolKev* sym_zero_q_;
  CPSKev proc_zero_q_;
  SymbolKev* sym_positive_q_;
  CPSKev proc_positive_q_;
  SymbolKev* sym_negative_q_;
  CPSKev proc_negative_q_;
  SymbolKev* sym_odd_q_;
  CPSKev proc_odd_q_;
  SymbolKev* sym_even_q_;
  CPSKev proc_even_q_;
  SymbolKev* sym_finite_q_;
  CPSKev proc_finite_q_;
  SymbolKev* sym_infinite_q_;
  CPSKev proc_infinite_q_;
  SymbolKev* sym_nan_q_;
  CPSKev proc_nan_q_;
  SymbolKev* sym_max_;
  CPSKev proc_max_;
  SymbolKev* sym_min_;
  CPSKev proc_min_;
  SymbolKev* sym_add_;
  CPSKev proc_add_;
  SymbolKev* sym_multiply_;
  CPSKev proc_multiply_;
  SymbolKev* sym_subtract_;
  CPSKev proc_subtract_;
  SymbolKev* sym_divide_;
  CPSKev proc_divide_;
  SymbolKev* sym_abs_;
  CPSKev proc_abs_;
  SymbolKev* sym_numerator_;
  CPSKev proc_numerator_;
  SymbolKev* sym_denominator_;
  CPSKev proc_denominator_;
  SymbolKev* sym_floor_;
  CPSKev proc_floor_;
  SymbolKev* sym_ceiling_;
  CPSKev proc_ceiling_;
  SymbolKev* sym_truncate_;
  CPSKev proc_truncate_;
  SymbolKev* sym_round_;
  CPSKev proc_round_;
  SymbolKev* sym_make_rectangular_;
  CPSKev proc_make_rectangular_;
  SymbolKev* sym_make_polar_;
  CPSKev proc_make_polar_;
  SymbolKev* sym_real_part_;
  CPSKev proc_real_part_;
  SymbolKev* sym_imag_part_;
  CPSKev proc_imag_part_;
  SymbolKev* sym_magnitude_;
  CPSKev proc_magnitude_;
  SymbolKev* sym_angle_;
  CPSKev proc_angle_;
  SymbolKev* sym_number_to_string_;
  CPSKev proc_number_to_string_;
  SymbolKev* sym_string_to_number_;
  CPSKev proc_string_to_number_;
  SymbolKev* sym_flexp_;
  CPSKev proc_flexp_;
  SymbolKev* sym_fllog_;
  CPSKev proc_fllog_;
  SymbolKev* sym_flsin_;
  CPSKev proc_flsin_;
  SymbolKev* sym_flcos_;
  CPSKev proc_flcos_;
  SymbolKev* sym_fltan_;
  CPSKev proc_fltan_;
  SymbolKev* sym_flasin_;
  CPSKev proc_flasin_;
  SymbolKev* sym_flacos_;
  CPSKev proc_flacos_;
  SymbolKev* sym_flatan_;
  CPSKev proc_flatan_;
  SymbolKev* sym_flsqrt_;
  CPSKev proc_flsqrt_;
  SymbolKev* sym_flexpt_;
  CPSKev proc_flexpt_;
  SymbolKev* sym_not_;
  CPSKev proc_not_;
  SymbolKev* sym_boolean_q_;
  CPSKev proc_boolean_q_;
  SymbolKev* sym_pair_q_;
  CPSKev proc_pair_q_;
  SymbolKev* sym_cons_;
  CPSKev proc_cons_;
  SymbolKev* sym_car_;
  CPSKev proc_car_;
  SymbolKev* sym_cdr_;
  CPSKev proc_cdr_;
  SymbolKev* sym_caar_;
  CPSKev proc_caar_;
  SymbolKev* sym_cadr_;
  CPSKev proc_cadr_;
  SymbolKev* sym_cdar_;
  CPSKev proc_cdar_;
  SymbolKev* sym_cddr_;
  CPSKev proc_cddr_;
  SymbolKev* sym_caaar_;
  CPSKev proc_caaar_;
  SymbolKev* sym_caadr_;
  CPSKev proc_caadr_;
  SymbolKev* sym_cadar_;
  CPSKev proc_cadar_;
  SymbolKev* sym_caddr_;
  CPSKev proc_caddr_;
  SymbolKev* sym_cdaar_;
  CPSKev proc_cdaar_;
  SymbolKev* sym_cdadr_;
  CPSKev proc_cdadr_;
  SymbolKev* sym_cddar_;
  CPSKev proc_cddar_;
  SymbolKev* sym_cdddr_;
  CPSKev proc_cdddr_;
  SymbolKev* sym_null_q_;
  CPSKev proc_null_q_;
  SymbolKev* sym_list_;
  CPSKev proc_list_;
  SymbolKev* sym_length_;
  CPSKev proc_length_;
  // SymbolKev* sym_append_;
  // LambdaKev proc_append_;
  SymbolKev* sym_reverse_;
  CPSKev proc_reverse_;
  SymbolKev* sym_symbol_q_;
  CPSKev proc_symbol_q_;
  SymbolKev* sym_symbol_eq_q_;
  CPSKev proc_symbol_eq_q_;
  SymbolKev* sym_symbol_to_string_;
  CPSKev proc_symbol_to_string_;
  SymbolKev* sym_string_to_symbol_;
  CPSKev proc_string_to_symbol_;
  SymbolKev* sym_char_q_;
  CPSKev proc_char_q_;
  SymbolKev* sym_char_to_integer_;
  CPSKev proc_char_to_integer_;
  SymbolKev* sym_integer_to_char_;
  CPSKev proc_integer_to_char_;
  SymbolKev* sym_char_eq_q_;
  CPSKev proc_char_eq_q_;
  SymbolKev* sym_char_lt_q_;
  CPSKev proc_char_lt_q_;
  SymbolKev* sym_char_gt_q_;
  CPSKev proc_char_gt_q_;
  SymbolKev* sym_char_lte_q_;
  CPSKev proc_char_lte_q_;
  SymbolKev* sym_char_gte_q_;
  CPSKev proc_char_gte_q_;
  SymbolKev* sym_string_q_;
  CPSKev proc_string_q_;
  SymbolKev* sym_make_string_;
  CPSKev proc_make_string_;
  SymbolKev* sym_string_;
  CPSKev proc_string_;
  SymbolKev* sym_string_length_;
  CPSKev proc_string_length_;
  SymbolKev* sym_string_ref_;
  CPSKev proc_string_ref_;
  SymbolKev* sym_string_eq_q_;
  CPSKev proc_string_eq_q_;
  SymbolKev* sym_string_lt_q_;
  CPSKev proc_string_lt_q_;
  SymbolKev* sym_string_gt_q_;
  CPSKev proc_string_gt_q_;
  SymbolKev* sym_string_lte_q_;
  CPSKev proc_string_lte_q_;
  SymbolKev* sym_string_gte_q_;
  CPSKev proc_string_gte_q_;
  SymbolKev* sym_substring_;
  CPSKev proc_substring_;
  // SymbolKev* sym_string_append_;
  // LambdaKev proc_string_append_;
  SymbolKev* sym_string_to_list_;
  CPSKev proc_string_to_list_;
  SymbolKev* sym_string_copy_;
  CPSKev proc_string_copy_;
  SymbolKev* sym_vector_q_;
  CPSKev proc_vector_q_;
  SymbolKev* sym_make_vector_;
  CPSKev proc_make_vector_;
  SymbolKev* sym_vector_;
  CPSKev proc_vector_;
  SymbolKev* sym_vector_length_;
  CPSKev proc_vector_length_;
  SymbolKev* sym_vector_ref_;
  CPSKev proc_vector_ref_;
  SymbolKev* sym_vector_set_e_;
  CPSKev proc_vector_set_e_;
  SymbolKev* sym_vector_to_list_;
  CPSKev proc_vector_to_list_;
  SymbolKev* sym_list_to_vector_;
  CPSKev proc_list_to_vector_;
  SymbolKev* sym_call_with_current_continuation_;
  SymbolKev* sym_call_with_cc_;
  CPSKev proc_call_with_cc_;
  SymbolKev* sym_values_;
  CPSKev proc_values_;
  SymbolKev* sym_call_with_values_;
  CPSKev proc_call_with_values_;
  SymbolKev* sym_dynamic_wind_;
  CPSKev proc_dynamic_wind_;
  SymbolKev* sym_assertion_violation_;
  CPSKev proc_assertion_violation_;
  SymbolKev* sym_apply_;
  CPSKev proc_apply_;
  // SymbolKev* sym_length_check_;
  // LambdaKev proc_length_check_;
  // SymbolKev* sym_map_;
  // LambdaKev proc_map_;
  // SymbolKev* sym_separate_first_value_;
  // LambdaKev proc_separate_first_value_;
  // SymbolKev* sym_transpose_;
  // LambdaKev proc_transpose_;

private:
  struct Flexact {
    static void func(KevesVM*, const_KevesIterator);
    static void func_helper1(KevesVM*, const_KevesIterator);
    static void func_helper2(KevesVM*, const_KevesIterator);
  };

  struct Inexact {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct IsComplex {
    static bool func(KevesValue);
  };

  struct IsEqualTo {
    static bool func(KevesValue, KevesValue);
  };

  struct IsExact {
    static bool func(KevesValue);
  };

  struct IsFinite {
    static bool func(KevesValue);
  };

  struct IsGreaterThan {
    static bool func(KevesValue, KevesValue);
  };

  struct IsGreaterThanEqual {
    static bool func(KevesValue, KevesValue);
  };

  struct IsInexact {
    static bool func(KevesValue);
  };

  struct IsInfinite {
    static bool func(KevesValue);
  };

  struct IsInteger {
    static bool func(KevesValue);
    static KevesValue message(KevesBase*);
  };

  struct IsIntegerValued {
    static bool func(KevesValue);
  };

  struct IsLessThan {
    static bool func(KevesValue, KevesValue);
  };

  struct IsLessThanEqual {
    static bool func(KevesValue, KevesValue);
  };

  struct IsNaN {
    static bool func(KevesValue);
  };

  struct IsNegative {
    static bool func(KevesValue);
  };

  struct IsNumber {
    static bool func(KevesValue);
    static KevesValue message(KevesBase*);
  };

  struct IsPositive {
    static bool func(KevesValue);
  };

  struct IsProcedure {
    static bool func(KevesValue);
    static KevesValue message(KevesBase*);
    static KevesValue message1(KevesBase*);
    static KevesValue message2(KevesBase*);
    static KevesValue message3(KevesBase*);
  };

  struct IsRational {
    static bool func(KevesValue);
  };

  struct IsRationalValued {
    static bool func(KevesValue);
  };

  struct IsReal {
    static bool func(KevesValue);
    static KevesValue message(KevesBase*);
    static KevesValue message1(KevesBase*);
    static KevesValue message2(KevesBase*);
  };

  struct IsRealValued {
    static bool func(KevesValue);
  };

  struct IsZero {
    static bool func(KevesValue);
  };

  struct IsOdd {
    static bool func(KevesValue);
  };

  struct IsEven {
    static bool func(KevesValue);
  };

  struct Max {
    static KevesValue func(KevesGC*, KevesValue, KevesValue);
  };

  struct Min {
    static KevesValue func(KevesGC*, KevesValue, KevesValue);
  };

  struct Add {
    static void func0(KevesVM*, const_KevesIterator);
    static void func1(KevesVM*, const_KevesIterator);
    static void funcFixnumFixnum(KevesVM*, const_KevesIterator);
    static void funcRationalFixnum(KevesVM*, const_KevesIterator);
    static void funcFlonumFixnum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcFixnumRational(KevesVM*, const_KevesIterator);
    static void funcRationalRational(KevesVM*, const_KevesIterator);
    static void funcFlonumRational(KevesVM*, const_KevesIterator);
    static void funcExactComplexRational(KevesVM*, const_KevesIterator);
    static void funcInexactComplexRational(KevesVM*, const_KevesIterator);
    static void funcFixnumFlonum(KevesVM*, const_KevesIterator);
    static void funcRationalFlonum(KevesVM*, const_KevesIterator);
    static void funcFlonumFlonum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcFixnumExactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalExactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumExactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcFixnumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalInexactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexInexactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexInexactComplex(KevesVM*, const_KevesIterator);

    template<class AUGEND, class ADDEND, class SUM>
    static void funcWithGC(KevesVM* vm, const_KevesIterator pc) {
      const AUGEND* augend(vm->gr1_);
      const ADDEND* addend(vm->acc_);
      SUM sum(augend->add(vm->gc(), *addend));
      vm->acc_ = &sum;
      return KevesVM::returnValueSafe(vm, pc);
    }

    template<class AUGEND, class ADDEND, class SUM>
    static void funcWithoutGC(KevesVM* vm, const_KevesIterator pc) {
      const AUGEND* augend(vm->gr1_);
      const ADDEND* addend(vm->acc_);
      SUM sum { augend->add(*addend) };
      vm->acc_ = &sum;
      return KevesVM::returnValueSafe(vm, pc);
    }

    static fx_int func(KevesFixnum num1, KevesFixnum num2) {
      return num1 + num2;
    }

    template<class T>
    static RationalNumberKev func(KevesGC* gc, RationalNumberKev num1, T num2) {
      return num1.add(gc, num2);
    }

    template<class T>
    static FlonumKev func(FlonumKev num1, T num2) {
      return num1.add(num2);
    }

    template<class T>
    static ExactComplexNumberKev func(KevesGC* gc, ExactComplexNumberKev num1, T num2) {
      return num1.add(gc, num2);
    }

    template<class T>
    static InexactComplexNumberKev func(InexactComplexNumberKev num1, T num2) {
      return num1.add(num2);
    }
  };
    
  struct Multiply {
    static void func0(KevesVM*, const_KevesIterator);
    static void func1(KevesVM*, const_KevesIterator);
    static void funcFixnumFixnum(KevesVM*, const_KevesIterator);
    static void funcRationalFixnum(KevesVM*, const_KevesIterator);
    static void funcFlonumFixnum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcFixnumRational(KevesVM*, const_KevesIterator);
    static void funcRationalRational(KevesVM*, const_KevesIterator);
    static void funcFlonumRational(KevesVM*, const_KevesIterator);
    static void funcExactComplexRational(KevesVM*, const_KevesIterator);
    static void funcInexactComplexRational(KevesVM*, const_KevesIterator);
    static void funcFixnumFlonum(KevesVM*, const_KevesIterator);
    static void funcRationalFlonum(KevesVM*, const_KevesIterator);
    static void funcFlonumFlonum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcFixnumExactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalExactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumExactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcFixnumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalInexactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexInexactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexInexactComplex(KevesVM*, const_KevesIterator);

    template<class MULTIPLICAND, class MULTIPLIER, class PRODUCT>
    static void funcWithGC(KevesVM* vm, const_KevesIterator pc) {
      const MULTIPLICAND* multiplicand(vm->gr1_);
      const MULTIPLIER* multiplier(vm->acc_);
      PRODUCT product { multiplicand->multiply(vm->gc(), *multiplier) };
      vm->acc_ = &product;
      return KevesVM::returnValueSafe(vm, pc);
    }

    template<class MULTIPLICAND, class MULTIPLIER, class PRODUCT>
    static void funcWithoutGC(KevesVM* vm, const_KevesIterator pc) {
      const MULTIPLICAND* multiplicand(vm->gr1_);
      const MULTIPLIER* multiplier(vm->acc_);
      PRODUCT product { multiplicand->multiply(*multiplier) };
      vm->acc_ = &product;
      return KevesVM::returnValueSafe(vm, pc);
    }

    static fx_long func(KevesFixnum num1, KevesFixnum num2) {
      return static_cast<fx_long>(num1) * static_cast<fx_long>(num2);
    }

    template<class T>
    static RationalNumberKev func(KevesGC* gc, RationalNumberKev num1, T num2) {
      return num1.multiply(gc, num2);
    }

    template<class T>
    static FlonumKev func(FlonumKev num1, T num2) {
      return num1.multiply(num2);
    }

    template<class T>
    static ExactComplexNumberKev func(KevesGC* gc, ExactComplexNumberKev num1, T num2) {
      return num1.multiply(gc, num2);
    }

    template<class T>
    static InexactComplexNumberKev func(InexactComplexNumberKev num1, T num2) {
      return num1.multiply(num2);
    }
  };
    
  struct Subtract {
    static void func0(KevesVM*, const_KevesIterator);
    static void func1(KevesVM*, const_KevesIterator);
    static void func2(KevesVM*, const_KevesIterator);
    static void func3(KevesVM*, const_KevesIterator);
    static void func4(KevesVM*, const_KevesIterator);
    static void func5(KevesVM*, const_KevesIterator);
    static void func6(KevesVM*, const_KevesIterator);
    static void funcFixnumFixnum(KevesVM*, const_KevesIterator);
    static void funcRationalFixnum(KevesVM*, const_KevesIterator);
    static void funcFlonumFixnum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcFixnumRational(KevesVM*, const_KevesIterator);
    static void funcRationalRational(KevesVM*, const_KevesIterator);
    static void funcFlonumRational(KevesVM*, const_KevesIterator);
    static void funcExactComplexRational(KevesVM*, const_KevesIterator);
    static void funcInexactComplexRational(KevesVM*, const_KevesIterator);
    static void funcFixnumFlonum(KevesVM*, const_KevesIterator);
    static void funcRationalFlonum(KevesVM*, const_KevesIterator);
    static void funcFlonumFlonum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcFixnumExactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalExactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumExactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcFixnumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalInexactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexInexactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexInexactComplex(KevesVM*, const_KevesIterator);

    template<class MINUEND, class SUBTRAHEND, class DIFFERENCE>
    static void funcWithGC(KevesVM* vm, const_KevesIterator pc) {
      const MINUEND* minuend(vm->gr1_);
      const SUBTRAHEND* subtrahend(vm->acc_);
      DIFFERENCE difference { minuend->subtract(vm->gc(), *subtrahend) };
      vm->acc_ = &difference;
      return KevesVM::returnValueSafe(vm, pc);
    }

    template<class MINUEND, class SUBTRAHEND, class DIFFERENCE>
    static void funcWithoutGC(KevesVM* vm, const_KevesIterator pc) {
      const MINUEND* minuend(vm->gr1_);
      const SUBTRAHEND* subtrahend(vm->acc_);
      DIFFERENCE difference { minuend->subtract(*subtrahend) };
      vm->acc_ = &difference;
      return KevesVM::returnValueSafe(vm, pc);
    }
  };

  struct Divide {
    static void func0(KevesVM*, const_KevesIterator);
    static void func1(KevesVM*, const_KevesIterator);
    static void funcFixnumFixnum(KevesVM*, const_KevesIterator);
    static void funcRationalFixnum(KevesVM*, const_KevesIterator);
    static void funcFlonumFixnum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFixnum(KevesVM*, const_KevesIterator);
    static void funcFixnumRational(KevesVM*, const_KevesIterator);
    static void funcRationalRational(KevesVM*, const_KevesIterator);
    static void funcFlonumRational(KevesVM*, const_KevesIterator);
    static void funcExactComplexRational(KevesVM*, const_KevesIterator);
    static void funcInexactComplexRational(KevesVM*, const_KevesIterator);
    static void funcFixnumFlonum(KevesVM*, const_KevesIterator);
    static void funcRationalFlonum(KevesVM*, const_KevesIterator);
    static void funcFlonumFlonum(KevesVM*, const_KevesIterator);
    static void funcExactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcInexactComplexFlonum(KevesVM*, const_KevesIterator);
    static void funcFixnumExactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalExactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumExactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexExactComplex(KevesVM*, const_KevesIterator);
    static void funcFixnumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcRationalInexactComplex(KevesVM*, const_KevesIterator);
    static void funcFlonumInexactComplex(KevesVM*, const_KevesIterator);
    static void funcExactComplexInexactComplex(KevesVM*, const_KevesIterator);
    static void funcInexactComplexInexactComplex(KevesVM*, const_KevesIterator);

    template<class DIVIDEND, class DIVISOR, class QUOTIENT>
    static void funcWithGC(KevesVM* vm, const_KevesIterator pc) {
      const DIVIDEND* dividend(vm->gr1_);
      const DIVISOR* divisor(vm->acc_);
      QUOTIENT quotient { dividend->divide(vm->gc(), *divisor) };
      vm->acc_ = &quotient;
      return KevesVM::returnValueSafe(vm, pc);
    }

    template<class DIVIDEND, class DIVISOR, class QUOTIENT>
    static void funcWithoutGC(KevesVM* vm, const_KevesIterator pc) {
      const DIVIDEND* dividend(vm->gr1_);
      const DIVISOR* divisor(vm->acc_);
      QUOTIENT quotient { dividend->divide(*divisor) };
      vm->acc_ = &quotient;
      return KevesVM::returnValueSafe(vm, pc);
    }
  };

  struct Abs {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Numerator {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Denominator {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Floor {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Ceiling {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Truncate {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Round {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct MakeRectangular {
    static void func(KevesVM*, const_KevesIterator);
  };
    
  struct MakePolar {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct RealPart {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct ImagPart {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Magnitude {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Angle {
    static void func(KevesVM*, const_KevesIterator);
  };

  static void procNumberToString(KevesVM*, const_KevesIterator);
  static void procNumberToString_err1(KevesVM*, const_KevesIterator);
  static void procNumberToString_err2(KevesVM*, const_KevesIterator);
  static void procNumberToString_err3(KevesVM*, const_KevesIterator);
  static void procStringToNumber(KevesVM*, const_KevesIterator);
  static void procStringToNumber_helper1(KevesVM*, const_KevesIterator);

  struct Flexp {
    static void func(KevesVM*, const_KevesIterator);
  };
  
  struct Fllog {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Flsin {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Flcos {
    static void func(KevesVM*, const_KevesIterator);
  };
  
  struct Fltan {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Flasin {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Flacos {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Flatan {
    static void func(KevesVM*, const_KevesIterator);
  };
  
  struct Flsqrt {
    static void func(KevesVM*, const_KevesIterator);
  };

  static void procFlexpt(KevesVM*, const_KevesIterator);

  struct IsBoolean {
    static bool func(KevesValue);
  };

  struct Not {
    static void func(KevesVM*, const_KevesIterator);
  };

public:
  struct Cons {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Car {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Cdr {
    static void func(KevesVM*, const_KevesIterator);
  };

private:
  struct Caar {
    static void func(KevesVM*, const_KevesIterator);
  };

public:
  struct Cadr {
    static void func(KevesVM*, const_KevesIterator);
  };

private:
  struct Cdar {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Cddr {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Caaar {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Caadr {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Cadar {
    static void func(KevesVM*, const_KevesIterator);
  };

public:
  struct Caddr {
    static void func(KevesVM*, const_KevesIterator);
  };

private:
  struct Cdaar {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Cdadr {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct Cddar {
    static void func(KevesVM*, const_KevesIterator);
  };

public:
  struct Cdddr {
    static void func(KevesVM*, const_KevesIterator);
  };

private:
  struct IsNull {
    static bool func(KevesValue);
  };

  static void procList(KevesVM*, const_KevesIterator);
  static void procList_helper(KevesVM*, const_KevesIterator);

  struct Length {
    static void func(KevesVM*, const_KevesIterator);
  };
  
  struct Reverse {
    static void func(KevesVM*, const_KevesIterator);
    static void func_helper(KevesVM*, const_KevesIterator);
  };

  struct IsSymbolEqual {
    static bool func(KevesValue, KevesValue);
  };

  struct SymbolToString {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringToSymbol {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct CharToInteger {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct IntegerToChar {
    static void func(KevesVM*, const_KevesIterator);
  };
  
  struct IsCharEqual {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharLT {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharGT {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharLTE {
    static bool func(KevesValue, KevesValue);
  };

  struct IsCharGTE {
    static bool func(KevesValue, KevesValue);
  };

  static void procMakeString(KevesVM*, const_KevesIterator);
  static void procString(KevesVM*, const_KevesIterator);

  struct StringLength {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringRef {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct IsStringEqual {
    static bool func(KevesValue, KevesValue);
  };

  struct IsStringLT {
    static bool func(KevesValue, KevesValue);
  };

  struct IsStringGT {
    static bool func(KevesValue, KevesValue);
  };

  struct IsStringLTE {
    static bool func(KevesValue, KevesValue);
  };

  struct IsStringGTE {
    static bool func(KevesValue, KevesValue);
  };

  struct Substring {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct StringToList {
    static void func(KevesVM*, const_KevesIterator);
    static void func_helper(KevesVM*, const_KevesIterator);
  };

  struct StringCopy {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct IsVector {
    static bool func(KevesValue);
    static KevesValue message(KevesBase*);
    static KevesValue message1(KevesBase*);
  };

  static void procMakeVector(KevesVM*, const_KevesIterator);
  static void procVector(KevesVM*, const_KevesIterator);

  struct VectorLength {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct VectorRef {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct VectorSetE {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct VectorToList {
    static void func(KevesVM*, const_KevesIterator);
    static void func_helper(KevesVM*, const_KevesIterator);
  };

  static void procListToVector(KevesVM*, const_KevesIterator);

  struct CallWithCC {
    static void func(KevesVM*, const_KevesIterator);
  };

  static void procValues(KevesVM*, const_KevesIterator);

  struct CallWithValues {
    static void func(KevesVM*, const_KevesIterator);
  };

  struct DynamicWind {
    static void func(KevesVM*, const_KevesIterator);
  };

  static void procAssertionViolation(KevesVM*, const_KevesIterator);
  static void procAssertionViolation_helper(KevesVM*, const_KevesIterator);
  static void procApply(KevesVM*, const_KevesIterator);
  static void procApply_helper(KevesVM*, const_KevesIterator);
};
