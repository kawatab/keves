// keves/lib/rnrs-base/rnrs-base.cpp - library of Keves base
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


#include "lib/rnrs-base/rnrs-base.hpp"

#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>
#include "keves_builtin_values.hpp"
#include "keves_common.hpp"
#include "keves_common-inl.hpp"
#include "keves_template.hpp"
#include "keves_vm.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
#include "kev/frame.hpp"
#include "kev/frame-inl.hpp"
#include "kev/number.hpp"
#include "kev/number-inl.hpp"
#include "kev/procedure-inl.hpp"
#include "kev/string.hpp"
#include "kev/string-inl.hpp"
#include "kev/symbol.hpp"
#include "kev/symbol-inl.hpp"
#include "kev/vector.hpp"
#include "kev/vector-inl.hpp"
#include "value/char.hpp"
#include "value/fixnum.hpp"


void LibRnrsBase::init(KevesCommon* common) {
  std::cout << "LibRnrsBase::init()" << std::endl;

  // setID("rnrs", "base-bin");
  // setVerNum(6);
					       
  sym_abs_ = SymbolKev::make(common, "abs");
  sym_add_ = SymbolKev::make(common, "+");
  // sym_and_ = SymbolKev::make(common, "and");
  sym_angle_ = SymbolKev::make(common, "angle");
  // sym_append_ = SymbolKev::make(common, "append");
  sym_apply_ = SymbolKev::make(common, "apply");
  sym_assertion_violation_ = SymbolKev::make(common, "assertion-violation");
  // sym_begin_ = SymbolKev::make(common, "begin");
  // sym_begin0_ = SymbolKev::make(common, "begin0");
  sym_boolean_q_ = SymbolKev::make(common, "boolean?");
  sym_call_with_cc_ = SymbolKev::make(common, "call/cc");
  sym_call_with_current_continuation_ = SymbolKev::make(common, "call-with-current-continuation");
  sym_call_with_values_ = SymbolKev::make(common, "call-with-values");
  sym_car_ = SymbolKev::make(common, "car");
  sym_cdr_ = SymbolKev::make(common, "cdr");
  sym_caar_ = SymbolKev::make(common, "caar");
  sym_cadr_ = SymbolKev::make(common, "cadr");
  sym_cdar_ = SymbolKev::make(common, "cdar");
  sym_cddr_ = SymbolKev::make(common, "cddr");
  sym_caaar_ = SymbolKev::make(common, "caaar");
  sym_caadr_ = SymbolKev::make(common, "caadr");
  sym_cadar_ = SymbolKev::make(common, "cadar");
  sym_caddr_ = SymbolKev::make(common, "caddr");
  sym_cdaar_ = SymbolKev::make(common, "cdaar");
  sym_cdadr_ = SymbolKev::make(common, "cdadr");
  sym_cddar_ = SymbolKev::make(common, "cddar");
  sym_cdddr_ = SymbolKev::make(common, "cdddr");
  sym_ceiling_ = SymbolKev::make(common, "ceiling");
  sym_char_eq_q_ = SymbolKev::make(common, "char=?");
  sym_char_gt_q_ = SymbolKev::make(common, "char>?");
  sym_char_gte_q_ = SymbolKev::make(common, "char>=?");
  sym_char_lt_q_ = SymbolKev::make(common, "char<?");
  sym_char_lte_q_ = SymbolKev::make(common, "char<=?");
  sym_char_q_ = SymbolKev::make(common, "char?");
  sym_char_to_integer_ = SymbolKev::make(common, "char->integer");
  sym_complex_q_ = SymbolKev::make(common, "complex?");
  sym_cons_ = SymbolKev::make(common, "cons");
  sym_denominator_ = SymbolKev::make(common, "denominator");
  sym_divide_ = SymbolKev::make(common, "/");
  sym_dynamic_wind_ = SymbolKev::make(common, "dynamic-wind");
  // sym_eq_q_ = SymbolKev::make(common, "eq?");
  sym_equal_ = SymbolKev::make(common, "=");
  // sym_eqv_q_ = SymbolKev::make(common, "eqv?");
  sym_exact_q_ = SymbolKev::make(common, "exact?");
  sym_flacos_ = SymbolKev::make(common, "flacos");
  sym_flasin_ = SymbolKev::make(common, "flasin");
  sym_flatan_ = SymbolKev::make(common, "flatan");
  sym_flcos_ = SymbolKev::make(common, "flcos");
  sym_flexact_ = SymbolKev::make(common, "flexact");
  sym_flexp_ = SymbolKev::make(common, "flexp");
  sym_flexpt_ = SymbolKev::make(common, "flexpt");
  sym_fllog_ = SymbolKev::make(common, "fllog");
  sym_floor_ = SymbolKev::make(common, "floor");
  sym_flsin_ = SymbolKev::make(common, "flsin");
  sym_flsqrt_ = SymbolKev::make(common, "flsqrt");
  sym_fltan_ = SymbolKev::make(common, "fltan");
  sym_greater_than_ = SymbolKev::make(common, ">");
  sym_greater_than_equal_ = SymbolKev::make(common, ">=");
  sym_even_q_ = SymbolKev::make(common, "even?");
  sym_finite_q_ = SymbolKev::make(common, "finite?");
  sym_imag_part_ = SymbolKev::make(common, "imag-part");
  sym_infinite_q_ = SymbolKev::make(common, "infinite?");
  sym_inexact_ = SymbolKev::make(common, "inexact");
  sym_inexact_q_ = SymbolKev::make(common, "inexact?");
  sym_integer_q_ = SymbolKev::make(common, "integer?");
  sym_integer_valued_q_ = SymbolKev::make(common, "integer-valued?");
  sym_integer_to_char_ = SymbolKev::make(common, "integer->char");
  sym_length_ = SymbolKev::make(common, "length");
  // sym_length_check_ = SymbolKev::make(common, "length-check");
  sym_less_than_ = SymbolKev::make(common, "<");
  sym_less_than_equal_ = SymbolKev::make(common, "<=");
  sym_list_ = SymbolKev::make(common, "list");
  sym_magnitude_ = SymbolKev::make(common, "magnitude");
  sym_make_polar_ = SymbolKev::make(common, "make-polar");
  sym_make_rectangular_ = SymbolKev::make(common, "make-rectangular");
  sym_make_string_ = SymbolKev::make(common, "make-string");
  sym_make_vector_ = SymbolKev::make(common, "make-vector");
  // sym_map_ = SymbolKev::make(common, "map");
  sym_max_ = SymbolKev::make(common, "max");
  sym_min_ = SymbolKev::make(common, "min");
  sym_multiply_ = SymbolKev::make(common, "*");
  sym_nan_q_ = SymbolKev::make(common, "nan?");
  sym_negative_q_ = SymbolKev::make(common, "negative?");
  sym_not_ = SymbolKev::make(common, "not");
  sym_null_q_ = SymbolKev::make(common, "null?");
  sym_number_q_ = SymbolKev::make(common, "number?");
  sym_number_to_string_ = SymbolKev::make(common, "number->string");
  sym_numerator_ = SymbolKev::make(common, "numerator");
  sym_odd_q_ = SymbolKev::make(common, "odd?");
  sym_pair_q_ = SymbolKev::make(common, "pair?");
  sym_positive_q_ = SymbolKev::make(common, "positive?");
  sym_procedure_q_ = SymbolKev::make(common, "procedure?");
  sym_rational_q_ = SymbolKev::make(common, "rational?");
  sym_rational_valued_q_ = SymbolKev::make(common, "rational-valued?");
  sym_real_part_ = SymbolKev::make(common, "real-part");
  sym_real_q_ = SymbolKev::make(common, "real?");
  sym_real_valued_q_ = SymbolKev::make(common, "real-valued?");
  sym_reverse_ = SymbolKev::make(common, "reverse");
  sym_round_ = SymbolKev::make(common, "round");
  // sym_separate_first_value_ = SymbolKev::make(common, "separate-first-value");
  sym_string_ = SymbolKev::make(common, "string");
  // sym_string_append_ = SymbolKev::make(common, "string-append");
  sym_string_copy_ = SymbolKev::make(common, "string-copy");
  sym_string_eq_q_ = SymbolKev::make(common, "string=?");
  sym_string_gt_q_ = SymbolKev::make(common, "string>?");
  sym_string_gte_q_ = SymbolKev::make(common, "string>=?");
  sym_string_length_ = SymbolKev::make(common, "string-length");
  sym_string_lt_q_ = SymbolKev::make(common, "string<?");
  sym_string_lte_q_ = SymbolKev::make(common, "string<=?");
  sym_string_q_ = SymbolKev::make(common, "string?");
  sym_string_ref_ = SymbolKev::make(common, "string-ref");
  sym_string_to_list_ = SymbolKev::make(common, "string->list");
  sym_string_to_number_ = SymbolKev::make(common, "string->number");
  sym_string_to_symbol_ = SymbolKev::make(common, "string->symbol");
  sym_substring_ = SymbolKev::make(common, "substring");
  sym_subtract_ = SymbolKev::make(common, "-");
  sym_symbol_eq_q_ = SymbolKev::make(common, "symbol=?");
  sym_symbol_q_ = SymbolKev::make(common, "symbol?");
  sym_symbol_to_string_ = SymbolKev::make(common, "symbol->string");
  // sym_transpose_ = SymbolKev::make(common, "transpose");
  sym_truncate_ = SymbolKev::make(common, "truncate");
  sym_values_ = SymbolKev::make(common, "values");
  sym_vector_ = SymbolKev::make(common, "vector");
  sym_vector_length_ = SymbolKev::make(common, "vector-length");
  sym_vector_q_ = SymbolKev::make(common, "vector?");
  sym_vector_ref_ = SymbolKev::make(common, "vector-ref");
  sym_vector_set_e_ = SymbolKev::make(common, "vector-set!");
  sym_vector_to_list_ = SymbolKev::make(common, "vector->list");
  sym_list_to_vector_ = SymbolKev::make(common, "list->vector");
  sym_zero_q_ = SymbolKev::make(common, "zero?");
  
  proc_procedure_q_.set(&Function::predicate<IsProcedure>, sym_procedure_q_);
  proc_number_q_.set(&Function::predicate<IsNumber>, sym_number_q_);
  proc_complex_q_.set(&Function::predicate<IsComplex>, sym_complex_q_);
  proc_real_q_.set(&Function::predicate<IsReal>, sym_real_q_);
  proc_rational_q_.set(&Function::predicate<IsRational>, sym_rational_q_);
  proc_integer_q_.set(&Function::predicate<IsInteger>, sym_integer_q_);
  proc_real_valued_q_.set(&Function::predicate<IsRealValued>, sym_real_valued_q_);
  proc_rational_valued_q_.set(&Function::predicate<IsRationalValued>, sym_rational_valued_q_);
  proc_integer_valued_q_.set(&Function::predicate<IsIntegerValued>, sym_integer_valued_q_);
  proc_exact_q_.set(&Function::predicate<IsNumber, IsExact>, sym_exact_q_);
  proc_inexact_q_.set(&Function::predicate<IsNumber, IsInexact>, sym_inexact_q_);
  proc_flexact_.set(&Function::make<IsNumber, Flexact>, sym_flexact_);
  proc_inexact_.set(&Function::make<IsNumber, Inexact>, sym_inexact_);
  proc_equal_.set(&Function::compare<IsNumber, IsEqualTo>, sym_equal_);
  proc_less_than_.set(&Function::compare<IsReal, IsLessThan>, sym_less_than_);
  proc_greater_than_.set(&Function::compare<IsReal, IsGreaterThan>, sym_greater_than_);
  proc_less_than_equal_.set(&Function::compare<IsReal, IsLessThanEqual>, sym_less_than_equal_);
  proc_greater_than_equal_.set(&Function::compare<IsReal, IsGreaterThanEqual>, sym_greater_than_equal_);
  proc_zero_q_.set(&Function::predicate<IsNumber, IsZero>, sym_zero_q_);
  proc_positive_q_.set(&Function::predicate<IsReal, IsPositive>, sym_positive_q_);
  proc_negative_q_.set(&Function::predicate<IsReal, IsNegative>, sym_negative_q_);
  proc_odd_q_.set(&Function::predicate<IsInteger, IsOdd>, sym_odd_q_);
  proc_even_q_.set(&Function::predicate<IsInteger, IsEven>, sym_even_q_);
  proc_finite_q_.set(&Function::predicate<IsReal, IsFinite>, sym_finite_q_);
  proc_infinite_q_.set(&Function::predicate<IsReal, IsInfinite>, sym_infinite_q_);
  proc_nan_q_.set(&Function::predicate<IsReal, IsNaN>, sym_nan_q_);
  proc_max_.set(&Function::composeTwoOrMore<IsReal, Max>, sym_max_);
  proc_min_.set(&Function::composeTwoOrMore<IsReal, Min>, sym_min_);
  proc_add_.set(&Arithmatic::func<Add, Add, fx_int, RationalNumberKevFromInt>, sym_add_);
  proc_multiply_.set(&Arithmatic::func<Multiply, Multiply, fx_long, RationalNumberKevFromLong>, sym_multiply_);
  proc_subtract_.set(&Arithmatic::func<Subtract, Add, fx_int, RationalNumberKevFromInt>, sym_subtract_);
  proc_divide_.set(&Arithmatic::func<Divide, Multiply, fx_long, RationalNumberKevFromLong>, sym_divide_);
  proc_abs_.set(&Function::make<IsReal, Abs>, sym_abs_);
  proc_numerator_.set(&Function::make<IsReal, Numerator>, sym_numerator_);
  proc_denominator_.set(&Function::make<IsReal, Denominator>, sym_denominator_);
  proc_floor_.set(&Function::make<IsReal, Floor>, sym_floor_);
  proc_ceiling_.set(&Function::make<IsReal, Ceiling>, sym_ceiling_);
  proc_truncate_.set(&Function::make<IsReal, Truncate>, sym_truncate_);
  proc_round_.set(&Function::make<IsReal, Round>, sym_round_);
  proc_make_rectangular_.set(&Function::make<IsReal, IsReal, MakeRectangular>, sym_make_rectangular_);
  proc_make_polar_.set(&Function::make<IsReal, IsReal, MakePolar>, sym_make_polar_);
  proc_real_part_.set(&Function::make<IsNumber, RealPart>, sym_real_part_);
  proc_imag_part_.set(&Function::make<IsNumber, ImagPart>, sym_imag_part_);
  proc_magnitude_.set(&Function::make<IsNumber, Magnitude>, sym_magnitude_);
  proc_angle_.set(&Function::make<IsNumber, Angle>, sym_angle_);
  proc_number_to_string_.set(&procNumberToString, sym_number_to_string_);
  proc_string_to_number_.set(&procStringToNumber, sym_string_to_number_);
  proc_flexp_.set(&Function::make<IsReal, Flexp>, sym_flexp_);
  proc_fllog_.set(&Function::make<IsReal, Fllog>, sym_fllog_);
  proc_flsin_.set(&Function::make<IsReal, Flsin>, sym_flsin_);
  proc_flcos_.set(&Function::make<IsReal, Flcos>, sym_flcos_);
  proc_fltan_.set(&Function::make<IsReal, Fltan>, sym_fltan_);
  proc_flasin_.set(&Function::make<IsReal, Flasin>, sym_flasin_);
  proc_flacos_.set(&Function::make<IsReal, Flacos>, sym_flacos_);
  proc_flatan_.set(&Function::make<IsReal, Flatan>, sym_flatan_);
  proc_flsqrt_.set(&Function::make<IsReal, Flsqrt>, sym_flsqrt_);
  proc_flexpt_.set(&procFlexpt, sym_flexpt_);
  proc_not_.set(&Function::make<Function::Anything, Not>, sym_not_);
  proc_boolean_q_.set(&Function::predicate<IsBoolean>, sym_boolean_q_);
  proc_pair_q_.set(&Function::predicate<Function::IsPair>, sym_pair_q_);
  proc_cons_.set(&Function::make<Function::Anything, Function::Anything, Cons>, sym_cons_);
  proc_car_.set(&Function::make<Function::IsPair, Car>, sym_car_);
  proc_cdr_.set(&Function::make<Function::IsPair, Cdr>, sym_cdr_);
  proc_caar_.set(&Function::make<Function::IsPair, Caar>, sym_caar_);
  proc_cadr_.set(&Function::make<Function::IsPair, Cadr>, sym_cadr_);
  proc_cdar_.set(&Function::make<Function::IsPair, Cdar>, sym_cdar_);
  proc_cddr_.set(&Function::make<Function::IsPair, Cddr>, sym_cddr_);
  proc_caaar_.set(&Function::make<Function::IsPair, Caaar>, sym_caaar_);
  proc_caadr_.set(&Function::make<Function::IsPair, Caadr>, sym_caadr_);
  proc_cadar_.set(&Function::make<Function::IsPair, Cadar>, sym_cadar_);
  proc_caddr_.set(&Function::make<Function::IsPair, Caddr>, sym_caddr_);
  proc_cdaar_.set(&Function::make<Function::IsPair, Cdaar>, sym_cdaar_);
  proc_cdadr_.set(&Function::make<Function::IsPair, Cdadr>, sym_cdadr_);
  proc_cddar_.set(&Function::make<Function::IsPair, Cddar>, sym_cddar_);
  proc_cdddr_.set(&Function::make<Function::IsPair, Cdddr>, sym_cdddr_);
  proc_null_q_.set(&Function::predicate<IsNull>, sym_null_q_);
  proc_list_.set(&procList, sym_list_);
  proc_length_.set(&Function::make<Function::Anything, Length>, sym_length_);
  proc_reverse_.set(&Function::make<Function::Anything, Reverse>, sym_reverse_);
  proc_symbol_q_.set(&Function::predicate<Function::IsSymbol>, sym_symbol_q_);
  proc_symbol_eq_q_.set(&Function::compare<Function::IsSymbol, IsSymbolEqual>, sym_symbol_eq_q_);
  proc_symbol_to_string_.set(&Function::make<Function::IsSymbol, SymbolToString>, sym_symbol_to_string_);
  proc_string_to_symbol_.set(&Function::make<Function::IsString, StringToSymbol>, sym_string_to_symbol_);
  proc_char_q_.set(&Function::predicate<Function::IsChar>, sym_char_q_);
  proc_char_to_integer_.set(&Function::make<Function::IsChar, CharToInteger>, sym_char_to_integer_);
  proc_integer_to_char_.set(&Function::make<Function::IsFixnum, IntegerToChar>, sym_integer_to_char_);
  proc_char_eq_q_.set(&Function::compare<Function::IsChar, IsCharEqual>, sym_char_eq_q_);
  proc_char_lt_q_.set(&Function::compare<Function::IsChar, IsCharLT>, sym_char_lt_q_);
  proc_char_gt_q_.set(&Function::compare<Function::IsChar, IsCharGT>, sym_char_gt_q_);
  proc_char_lte_q_.set(&Function::compare<Function::IsChar, IsCharLTE>, sym_char_lte_q_);
  proc_char_gte_q_.set(&Function::compare<Function::IsChar, IsCharGTE>, sym_char_gte_q_);
  proc_string_q_.set(&Function::predicate<Function::IsString>, sym_string_q_);
  proc_make_string_.set(&procMakeString, sym_make_string_);
  proc_string_.set(&procString, sym_string_);
  proc_string_length_.set(&Function::make<Function::IsString, StringLength>, sym_string_length_);
  proc_string_ref_.set(&Function::make<Function::IsString, Function::IsFixnum, StringRef>, sym_string_ref_);
  proc_string_eq_q_.set(&Function::compare<Function::IsString, IsStringEqual>, sym_string_eq_q_);
  proc_string_lt_q_.set(&Function::compare<Function::IsString, IsStringLT>, sym_string_lt_q_);
  proc_string_gt_q_.set(&Function::compare<Function::IsString, IsStringGT>, sym_string_gt_q_);
  proc_string_lte_q_.set(&Function::compare<Function::IsString, IsStringLTE>, sym_string_lte_q_);
  proc_string_gte_q_.set(&Function::compare<Function::IsString, IsStringGTE>, sym_string_gte_q_);
  proc_substring_.set(&Function::make<Function::IsString, Function::IsFixnum, Function::IsFixnum, Substring>, sym_substring_);
  proc_string_to_list_.set(&Function::make<Function::IsString, StringToList>, sym_string_to_list_);
  proc_string_copy_.set(&Function::make<Function::IsString, StringCopy>, sym_string_copy_);
  proc_vector_q_.set(&Function::predicate<IsVector>, sym_vector_q_);
  proc_make_vector_.set(&procMakeVector, sym_make_vector_);
  proc_vector_.set(&procVector, sym_vector_);
  proc_vector_length_.set(&Function::make<IsVector, VectorLength>, sym_vector_length_);
  proc_vector_ref_.set(&Function::make<IsVector, Function::IsFixnum, VectorRef>, sym_vector_ref_);
  proc_vector_set_e_.set(&Function::make<IsVector, Function::IsFixnum, Function::Anything, VectorSetE>, sym_vector_set_e_);
  proc_vector_to_list_.set(&Function::make<IsVector, VectorToList>, sym_vector_to_list_);
  proc_list_to_vector_.set(&procListToVector, sym_list_to_vector_);
  proc_call_with_cc_.set(&Function::make<IsProcedure, CallWithCC>, sym_call_with_cc_);
  proc_values_.set(&procValues, sym_values_);
  proc_call_with_values_.set(&Function::make<IsProcedure, IsProcedure, CallWithValues>, sym_call_with_values_);
  proc_dynamic_wind_.set(&Function::make<IsProcedure, IsProcedure, IsProcedure, DynamicWind>, sym_dynamic_wind_);
  proc_assertion_violation_.set(&procAssertionViolation, sym_assertion_violation_);
  proc_apply_.set(&procApply, sym_apply_);
  
  addBind("abs", &proc_abs_);
  addBind("+", &proc_add_);
  // addBind("and");
  addBind("angle", &proc_angle_);
  // addBind("append");
  addBind("apply", &proc_apply_);
  addBind("assertion-violation", &proc_assertion_violation_);
  // addBind("begin");
  // addBind("begin0");
  addBind("boolean?", &proc_boolean_q_);
  addBind("call/cc", &proc_call_with_cc_);
  addBind("call-with-current-continuation", &proc_call_with_cc_);
  addBind("call-with-values", &proc_call_with_values_);
  addBind("car", &proc_car_);
  addBind("cdr", &proc_cdr_);
  addBind("caar", &proc_caar_);
  addBind("cadr", &proc_cadr_);
  addBind("cdar", &proc_cdar_);
  addBind("cddr", &proc_cddr_);
  addBind("caaar", &proc_caaar_);
  addBind("caadr", &proc_caadr_);
  addBind("cadar", &proc_cadar_);
  addBind("caddr", &proc_caddr_);
  addBind("cdaar", &proc_cdaar_);
  addBind("cdadr", &proc_cdadr_);
  addBind("cddar", &proc_cddar_);
  addBind("cdddr", &proc_cdddr_);
  addBind("ceiling", &proc_ceiling_);
  addBind("char=?", &proc_char_eq_q_);
  addBind("char>?", &proc_char_gt_q_);
  addBind("char>=?", &proc_char_gte_q_);
  addBind("char<?", &proc_char_lt_q_);
  addBind("char<=?", &proc_char_lte_q_);
  addBind("char?", &proc_char_q_);
  addBind("char->integer", &proc_char_to_integer_);
  addBind("complex?", &proc_complex_q_);
  addBind("cons", &proc_cons_);
  addBind("denominator", &proc_denominator_);
  addBind("/", &proc_divide_);
  addBind("dynamic-wind", &proc_dynamic_wind_);
  // addBind("eq?");
  addBind("=", &proc_equal_);
  // addBind("eqv?");
  addBind("exact?", &proc_exact_q_);
  addBind("flacos", &proc_flacos_);
  addBind("flasin", &proc_flasin_);
  addBind("flatan", &proc_flatan_);
  addBind("flcos", &proc_flcos_);
  addBind("flexact", &proc_flexact_);
  addBind("flexp", &proc_flexp_);
  addBind("flexpt", &proc_flexpt_);
  addBind("fllog", &proc_fllog_);
  addBind("floor", &proc_floor_);
  addBind("flsin", &proc_flsin_);
  addBind("flsqrt", &proc_flsqrt_);
  addBind("fltan", &proc_fltan_);
  addBind(">", &proc_greater_than_);
  addBind(">=", &proc_greater_than_equal_);
  addBind("even?", &proc_even_q_);
  addBind("finite?", &proc_finite_q_);
  addBind("imag-part", &proc_imag_part_);
  addBind("infinite?", &proc_infinite_q_);
  addBind("inexact", &proc_inexact_);
  addBind("inexact?", &proc_inexact_q_);
  addBind("integer?", &proc_integer_q_);
  addBind("integer-valued?", &proc_integer_valued_q_);
  addBind("integer->char", &proc_integer_to_char_);
  addBind("length", &proc_length_);
  // addBind("length-check");
  addBind("<", &proc_less_than_);
  addBind("<=", &proc_less_than_equal_);
  addBind("list", &proc_list_);
  addBind("magnitude", &proc_magnitude_);
  addBind("make-polar", &proc_make_polar_);
  addBind("make-rectangular", &proc_make_rectangular_);
  addBind("make-string", &proc_make_string_);
  addBind("make-vector", &proc_make_vector_);
  // addBind("map");
  addBind("max", &proc_max_);
  addBind("min", &proc_min_);
  addBind("*", &proc_multiply_);
  addBind("nan?", &proc_nan_q_);
  addBind("negative?", &proc_negative_q_);
  addBind("not", &proc_not_);
  addBind("null?", &proc_null_q_);
  addBind("number?", &proc_number_q_);
  addBind("number->string", &proc_number_to_string_);
  addBind("numerator", &proc_numerator_);
  addBind("odd?", &proc_odd_q_);
  addBind("pair?", &proc_pair_q_);
  addBind("positive?", &proc_positive_q_);
  addBind("procedure?", &proc_procedure_q_);
  addBind("rational?", &proc_rational_q_);
  addBind("rational-valued?", &proc_rational_valued_q_);
  addBind("real-part", &proc_real_part_);
  addBind("real?", &proc_real_q_);
  addBind("real-valued?", &proc_real_valued_q_);
  addBind("reverse", &proc_reverse_);
  addBind("round", &proc_round_);
  // addBind("separate-first-value");
  addBind("string", &proc_string_);
  // addBind("string-append");
  addBind("string-copy", &proc_string_copy_);
  addBind("string=?", &proc_string_eq_q_);
  addBind("string>?", &proc_string_gt_q_);
  addBind("string>=?", &proc_string_gte_q_);
  addBind("string-length", &proc_string_length_);
  addBind("string<?", &proc_string_lt_q_);
  addBind("string<=?", &proc_string_lte_q_);
  addBind("string?", &proc_string_q_);
  addBind("string-ref", &proc_string_ref_);
  addBind("string->list", &proc_string_to_list_);
  addBind("string->number", &proc_string_to_number_);
  addBind("string->symbol", &proc_string_to_symbol_);
  addBind("substring", &proc_substring_);
  addBind("-", &proc_subtract_);
  addBind("symbol=?", &proc_symbol_eq_q_);
  addBind("symbol?", &proc_symbol_q_);
  addBind("symbol->string", &proc_symbol_to_string_);
  // addBind("transpose");
  addBind("truncate", &proc_truncate_);
  addBind("values", &proc_values_);
  addBind("vector", &proc_vector_);
  addBind("vector-length", &proc_vector_length_);
  addBind("vector?", &proc_vector_q_);
  addBind("vector-ref", &proc_vector_ref_);
  addBind("vector-set!", &proc_vector_set_e_);
  addBind("vector->list", &proc_vector_to_list_);
  addBind("list->vector", &proc_list_to_vector_);
  addBind("zero?", &proc_zero_q_);

}

bool LibRnrsBase::IsNumber::func(KevesValue kev) {
  return kev.isNumber();
}

bool LibRnrsBase::IsReal::func(KevesValue kev) {
  return kev.isRealNumber();
}

bool LibRnrsBase::IsComplex::func(KevesValue kev) {
  return kev.isComplexNumber();
}

bool LibRnrsBase::IsExact::func(KevesValue kev) {
  return kev.isExactNumber();
}

bool LibRnrsBase::IsInexact::func(KevesValue kev) {
  return kev.isInexactNumber();
}

bool LibRnrsBase::IsLessThan::func(KevesValue num1, KevesValue num2) {
  return NumberKev::isLT(num1, num2);
}

bool LibRnrsBase::IsLessThanEqual::func(KevesValue num1, KevesValue num2) {
  return NumberKev::isLTE(num1, num2);
}

bool LibRnrsBase::IsGreaterThan::func(KevesValue num1, KevesValue num2) {
  return NumberKev::isGT(num1, num2);
}

bool LibRnrsBase::IsGreaterThanEqual::func(KevesValue num1, KevesValue num2) {
  return NumberKev::isGTE(num1, num2);
}

bool LibRnrsBase::IsZero::func(KevesValue kev) {
  return NumberKev::isZero(kev);
}

bool LibRnrsBase::IsPositive::func(KevesValue kev) {
  return NumberKev::isPositive(kev);
}

bool LibRnrsBase::IsNegative::func(KevesValue kev) {
  return NumberKev::isNegative(kev);
}

bool LibRnrsBase::IsOdd::func(KevesValue kev) {
  if (kev.isFixnum()) {
    fx_int num(KevesFixnum { kev } );
    return num & 1;
  }

  if (kev.is<RationalNumberKev>()) {
    const RationalNumberKev* num(kev);
    return num->isOdd();
  }

  if (kev.is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isOdd();
  }

  return false;
}

bool LibRnrsBase::IsEven::func(KevesValue kev) {
  if (kev.isFixnum()) {
    fx_int num(KevesFixnum { kev } );
    return !(num & 1);
  }

  if (kev.is<RationalNumberKev>()) {
    const RationalNumberKev* num(kev);
    return num->isEven();
  }

  if (kev.is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isEven();
  }

  return false;
}

void LibRnrsBase::Flexact::func(KevesVM* vm, const_KevesIterator pc) {
  return
    vm->acc_.isExactNumber() ? KevesVM::returnValue(vm, pc) :
    vm->acc_.is<FlonumKev>() ? func_helper1(vm, pc) :
    func_helper2(vm, pc);
}

void LibRnrsBase::Flexact::func_helper1(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* num(vm->acc_);

  if (num->isNaN() || num->isInfinity())
    return KevesVM::returnValue(vm, pc);

  if (num->isZero()) {
    vm->acc_ = KevesFixnum(0);
    return KevesVM::returnValue(vm, pc);
  }
  
  RationalNumberKev rational(num->toExact(vm->gc()));
  vm->acc_ = &rational;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Flexact::func_helper2(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* num(vm->acc_);
  FlonumKev real(num->real());
  FlonumKev imag(num->imag());

  if (real.isNaN() || real.isInfinity() || imag.isNaN() || imag.isInfinity())
    return KevesVM::returnValue(vm, pc);

  RationalNumberKev rational_real(real.toExact(vm->gc()));
  
  vm->checkStack(&rational_real, &func_helper2, pc);
  
  if (imag.isZero()) {
    vm->acc_ = rational_real.optimize();
    return KevesVM::returnValue(vm, pc);
  }

  RationalNumberKev rational_imag(imag.toExact(vm->gc()));
  ExactComplexNumberKev complex(rational_real, rational_imag);
  vm->acc_ = &complex;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Inexact::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum()) {
    KevesFixnum fixnum(vm->acc_);
    FlonumKev flonum(fixnum);

    vm->checkStack(&flonum, &func, pc);
    
    vm->acc_ = &flonum;
    return KevesVM::returnValue(vm, pc);
  }

  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* num(vm->acc_);
    FlonumKev flonum(num->toInexact());

    vm->checkStack(&flonum, &func, pc);

    vm->acc_ = &flonum;
    return KevesVM::returnValue(vm, pc);
  }

  if (vm->acc_.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* num(vm->acc_);
    InexactComplexNumberKev inexact(num->toInexact());

    vm->checkStack(&inexact, &func, pc);

    vm->acc_ = &inexact;
    return KevesVM::returnValue(vm, pc);
  }

  return KevesVM::returnValue(vm, pc);
}

KevesValue LibRnrsBase::IsNumber::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqNum);
}

KevesValue LibRnrsBase::IsReal::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
}

KevesValue LibRnrsBase::IsReal::message1(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqRealNumAs1st);
}

KevesValue LibRnrsBase::IsReal::message2(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqRealNumAs2nd);
}

bool LibRnrsBase::IsRealValued::func(KevesValue kev) {
  if (kev.isRealNumber())
    return true;

  if (kev.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* exact(kev);
    return exact->isRealValued();
  }

  if (kev.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* inexact(kev);
    return inexact->isRealValued();
  }

  return false;
}

bool LibRnrsBase::IsRational::func(KevesValue kev) {
  if (kev.isRationalNumber())
    return true;

  if (kev.is<FlonumKev>()) {
    const FlonumKev* flonum(kev);
    return flonum->isInteger();
  }

  return false;
}

bool LibRnrsBase::IsRationalValued::func(KevesValue kev) {
  if (kev.isRealNumber())
    return true;

  if (kev.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* exact(kev);
    return exact->isRealValued();
  }

  if (kev.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* inexact(kev);
    return inexact->isRealValued();
  }

  return false;
}

bool LibRnrsBase::IsInteger::func(KevesValue kev) {
  if (kev.isFixnum())
    return true;

  if (kev.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(kev);
    return rational->isInteger();
  }

  if (kev.is<FlonumKev>()) {
    const FlonumKev* flonum(kev);
    return flonum->isInteger();
  }

  return false;
}

KevesValue LibRnrsBase::IsInteger::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqIntNum);
}

bool LibRnrsBase::IsIntegerValued::func(KevesValue kev) {
  if (kev.isFixnum())
    return true;

  if (kev.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(kev);
    return rational->isInteger();
  }

  if (kev.is<FlonumKev>()) {
    const FlonumKev* flonum(kev);
    return flonum->isInteger();
  }

  if (kev.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* exact(kev);
    return exact->isIntegerValued();
  }

  if (kev.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* inexact(kev);
    return inexact->isIntegerValued();
  }
  
  return false;
}

bool LibRnrsBase::IsEqualTo::func(KevesValue num1, KevesValue num2) {
  if (num1.isFixnum())
    return num2.isFixnum() && num1 == num2;

  if (num1.is<RationalNumberKev>()) {
    const RationalNumberKev* real1(num1);

    if (num2.is<RationalNumberKev>()) {
      const RationalNumberKev* real2(num2);
      return real1->isEqualTo(*real2);
    }
    
  } else if (num1.is<FlonumKev>()) {
    const FlonumKev* real1(num1);

    if (num2.is<FlonumKev>()) {
      const FlonumKev* real2(num2);
      return real1->isEqualTo(*real2);
    }

  } else if (num1.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* comple1(num1);

    if (num2.is<ExactComplexNumberKev>()) {
      const ExactComplexNumberKev* complex2(num2);

      return comple1->real().isEqualTo(complex2->real()) &&
	comple1->imag().isEqualTo(complex2->imag());
    }

  } else if (num1.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* comple1(num1);

    if (num2.is<InexactComplexNumberKev>()) {
      const InexactComplexNumberKev* complex2(num2);
      return comple1->isEqualTo(*complex2);
    }

  }

  return false;
}

bool LibRnrsBase::IsFinite::func(KevesValue kev) {
  if (kev.isRationalNumber())
    return true;

  if (kev.is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isFinite();
  }

  return false;
}

bool LibRnrsBase::IsInfinite::func(KevesValue kev) {
  if (kev.is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isInfinity();
  }

  return false;
}

bool LibRnrsBase::IsNaN::func(KevesValue kev) {
  if (kev.is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isNaN();
  }

  return false;
}

bool LibRnrsBase::IsProcedure::func(KevesValue kev) {
  return kev.isProcedure();
}

KevesValue LibRnrsBase::IsProcedure::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqProc);
}

KevesValue LibRnrsBase::IsProcedure::message1(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqProcAs1st);
}

KevesValue LibRnrsBase::IsProcedure::message2(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqProcAs2nd);
}

KevesValue LibRnrsBase::IsProcedure::message3(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqProcAs3rd);
}

KevesValue LibRnrsBase::Max::func(KevesGC* gc, KevesValue num1, KevesValue num2) {
  return NumberKev::max(gc, num1, num2);
}

KevesValue LibRnrsBase::Min::func(KevesGC* gc, KevesValue num1, KevesValue num2) {
  return NumberKev::min(gc, num1, num2);
}

void LibRnrsBase::Add::func0(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = KevesFixnum(0);
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::func1(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (last.isNumber()) {
    vm->acc_ = last;
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Add::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  fx_int sum(augend + addend);
  KevesFixnum fixnum(sum);

  if (fixnum == sum) {
    vm->acc_ = fixnum;
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = RationalNumberKev::makeFromInt(vm->gc(), sum);
  return KevesVM::returnValue(vm, pc);
}
  
void LibRnrsBase::Add::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  RationalNumberKev sum(augend->add(vm->gc(), addend));

  vm->checkStack(&sum, &funcRationalFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  FlonumKev sum(augend->add(addend));

  vm->checkStack(&sum, &funcFlonumFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  ExactComplexNumberKev sum(augend->add(vm->gc(), addend));

  vm->checkStack(&sum, &funcExactComplexFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  InexactComplexNumberKev sum(augend->add(addend));

  vm->checkStack(&sum, &funcInexactComplexFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  const RationalNumberKev* addend(vm->acc_);
  RationalNumberKev sum(addend->add(vm->gc(), augend));

  vm->checkStack(&sum, &funcFixnumRational, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcRationalRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, RationalNumberKev, RationalNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcFlonumRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, RationalNumberKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Add::funcExactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, RationalNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcInexactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, RationalNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcFixnumFlonum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  const FlonumKev* addend(vm->acc_);
  FlonumKev sum(addend->add(augend));

  vm->checkStack(&sum, &funcFixnumFlonum, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcRationalFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Add::funcFlonumFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Add::funcExactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcInexactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcFixnumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  const ExactComplexNumberKev* addend(vm->acc_);
  ExactComplexNumberKev sum(addend->add(vm->gc(), augend));

  vm->checkStack(&sum, &funcFixnumExactComplex, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcRationalExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcFlonumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcExactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcInexactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcFixnumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  const InexactComplexNumberKev* addend(vm->acc_);
  InexactComplexNumberKev sum(addend->add(augend));

  vm->checkStack(&sum, &funcFixnumInexactComplex, pc);

  vm->acc_ = &sum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Add::funcRationalInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcFlonumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcExactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Add::funcInexactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::func0(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = KevesFixnum(1);
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::func1(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (last.isNumber()) {
    vm->acc_ = last;
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  fx_long product(multiplicand * multiplier);
  KevesFixnum fixnum(static_cast<fx_int>(product));

  if (fixnum != product) {
    RationalNumberKevFromLong rational(product);
    
    vm->checkStack(&product, &funcFixnumFixnum, pc);
    
    vm->acc_ = &rational;
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->acc_ = fixnum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  RationalNumberKev product(multiplicand->multiply(vm->gc(), multiplier));

  vm->checkStack(&product, &funcRationalFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  FlonumKev product(multiplicand->multiply(multiplier));

  vm->checkStack(&product, &funcFlonumFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  ExactComplexNumberKev product(multiplicand->multiply(vm->gc(), multiplier));

  vm->checkStack(&product, &funcExactComplexFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  InexactComplexNumberKev product(multiplicand->multiply(multiplier));

  vm->checkStack(&product, &funcInexactComplexFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  const RationalNumberKev* multiplier(vm->acc_);
  RationalNumberKev product(multiplier->multiply(vm->gc(), multiplicand));

  vm->checkStack(&product, &funcFixnumRational, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcRationalRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, RationalNumberKev, RationalNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFlonumRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, RationalNumberKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcExactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, RationalNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcInexactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, RationalNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumFlonum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  const FlonumKev* multiplier(vm->acc_);
  FlonumKev product(multiplier->multiply(multiplicand));

  vm->checkStack(&product, &funcFixnumFlonum, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcRationalFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFlonumFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcExactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcInexactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  const ExactComplexNumberKev* multiplier(vm->acc_);
  ExactComplexNumberKev product(multiplier->multiply(vm->gc(), multiplicand));

  vm->checkStack(&product, &funcFixnumExactComplex, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcRationalExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFlonumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcExactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcInexactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  const FlonumKev* multiplier(vm->acc_);
  FlonumKev product(multiplier->multiply(multiplicand));

  vm->checkStack(&product, &funcFixnumInexactComplex, pc);

  vm->acc_ = &product;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcRationalInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcFlonumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcExactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Multiply::funcInexactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::func0(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req1OrMoreGot0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Subtract::func1(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.isFixnum())
    return func2(vm, pc);
  
  KevesFixnum num(last);
  fx_int inverse(-static_cast<fx_int>(num));
  
  if (KevesFixnum::isInRange(inverse)) {
    KevesFixnum fixnum(inverse);
    vm->acc_ = fixnum;
    return KevesVM::returnValue(vm, pc);
  }
  
  RationalNumberKevFromInt rational(inverse);

  vm->checkStack(&rational, &func1, pc);

  vm->acc_ = &rational;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::func2(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.is<RationalNumberKev>())
    return func3(vm, pc);
  
  const RationalNumberKev* num(last);
  RationalNumberKev inverse(num->inverse());

  vm->checkStack(&inverse, &func2, pc);

  vm->acc_ = inverse.optimize();
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::func3(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.is<FlonumKev>())
    return func4(vm, pc);
  
  const FlonumKev* num(last);
  FlonumKev inverse(num->inverse());

  vm->checkStack(&inverse, &func3, pc);

  vm->acc_ = &inverse;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::func4(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.is<ExactComplexNumberKev>())
    return func5(vm, pc);
  
  const ExactComplexNumberKev* num(last);
  ExactComplexNumberKev inverse(num->inverse());

  vm->checkStack(&inverse, &func4, pc);

  vm->acc_ = &inverse;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::func5(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.is<InexactComplexNumberKev>())
    return func6(vm, pc);
  
  const InexactComplexNumberKev* num(last);
  InexactComplexNumberKev inverse(num->inverse());

  vm->checkStack(&inverse, &func5, pc);

  vm->acc_ = &inverse;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::func6(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  fx_int difference(minuend - subtrahend);
  KevesFixnum fixnum(difference);

  if (fixnum != difference) {
    RationalNumberKevFromInt rational(difference);
    
    vm->checkStack(&rational, &funcFixnumFixnum, pc);
    
    vm->acc_ = &rational;
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = fixnum;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  RationalNumberKev difference(minuend->subtract(vm->gc(), subtrahend));

  vm->checkStack(&difference, &funcRationalFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  FlonumKev difference(minuend->subtract(subtrahend));

  vm->checkStack(&difference, &funcFlonumFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  ExactComplexNumberKev difference(minuend->subtract(vm->gc(), subtrahend));

  vm->checkStack(&difference, &funcExactComplexFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  InexactComplexNumberKev difference(minuend->subtract(subtrahend));

  vm->checkStack(&difference, &funcInexactComplexFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum minuend(vm->gr1_);
  const RationalNumberKev* subtrahend(vm->acc_);
  vm->acc_ = NumberKev::difference(vm->gc(), minuend, *subtrahend);
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcRationalRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, RationalNumberKev, RationalNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFlonumRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, RationalNumberKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcExactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, RationalNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcInexactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, RationalNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumFlonum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum minuend(vm->gr1_);
  const FlonumKev* subtrahend(vm->acc_);
  FlonumKev difference(NumberKev::difference(minuend, *subtrahend));

  vm->checkStack(&difference, &funcFixnumFlonum, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcRationalFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFlonumFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcExactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcInexactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum minuend(vm->gr1_);
  const ExactComplexNumberKev* subtrahend(vm->acc_);
  ExactComplexNumberKev difference(NumberKev::difference(vm->gc(), minuend, *subtrahend));

  vm->checkStack(&difference, &funcFixnumExactComplex, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcRationalExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFlonumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcExactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcInexactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum  minuend(vm->gr1_);
  const FlonumKev* subtrahend(vm->acc_);
  FlonumKev difference(NumberKev::difference(minuend, *subtrahend));

  vm->checkStack(&difference, &funcFixnumInexactComplex, pc);

  vm->acc_ = &difference;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcRationalInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcFlonumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcExactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Subtract::funcInexactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::func0(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req1OrMoreGot0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Divide::func1(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (last.isFixnum()) {
    fx_int numerator(1);
    fx_int denominator(KevesFixnum { last } );

    if (denominator == 0) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Div0);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }

    if (denominator == 1 || denominator == -1) {
      vm->acc_ = KevesFixnum(denominator);
      return KevesVM::returnValue(vm, pc);
    }

    RationalNumberKevFromInt quotient(numerator, denominator);
    vm->acc_ = &quotient;
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (last.is<RationalNumberKev>()) {
    const RationalNumberKev* num(last);
    RationalNumberKev reciprocal(num->reciprocal());
    vm->acc_ = reciprocal.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (last.is<FlonumKev>()) {
    const FlonumKev* divisor(last);
    FlonumKev quotient(divisor->reciprocal());
    vm->acc_ = &quotient;
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (last.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* divisor(last);
    ExactComplexNumberKev reciprocal(divisor->reciprocal(vm->gc()));
    vm->acc_ = &reciprocal;
    return KevesVM::returnExactComplexNumberKev(vm, pc);
  }

  if (last.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* divisor(last);
    InexactComplexNumberKev quotient(divisor->reciprocal());
    vm->acc_ = &quotient;
    return KevesVM::returnValueSafe(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  RationalNumberKevFromInt quotient(dividend, divisor);
  vm->acc_ = quotient.optimize();
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  RationalNumberKev quotient(dividend->divide(vm->gc(), divisor));
  vm->acc_ = quotient.optimize();
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  FlonumKev quotient(dividend->divide(divisor));
  vm->acc_ = &quotient;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  ExactComplexNumberKev quotient(dividend->divide(vm->gc(), divisor));
  vm->acc_ = &quotient;
  return KevesVM::returnExactComplexNumberKev(vm, pc);
}

void LibRnrsBase::Divide::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  InexactComplexNumberKev quotient(dividend->divide(divisor));
  vm->acc_ = &quotient;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  const RationalNumberKev* divisor(vm->acc_);
  RationalNumberKev quotient(NumberKev::quotient(vm->gc(), dividend, *divisor));
  vm->acc_ = quotient.optimize();
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcRationalRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, RationalNumberKev, RationalNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFlonumRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, RationalNumberKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Divide::funcExactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, RationalNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcInexactComplexRational(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, RationalNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumFlonum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  const FlonumKev* divisor(vm->acc_);
  FlonumKev quotient(NumberKev::quotient(dividend, *divisor));
  vm->acc_ = &quotient;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcRationalFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFlonumFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, FlonumKev, FlonumKev>(vm, pc);
}

void LibRnrsBase::Divide::funcExactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcInexactComplexFlonum(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, FlonumKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  const ExactComplexNumberKev* divisor(vm->acc_);
  ExactComplexNumberKev quotient(NumberKev::quotient(vm->gc(), dividend, *divisor));
  vm->acc_ = &quotient;
  return KevesVM::returnExactComplexNumberKev(vm, pc);
}

void LibRnrsBase::Divide::funcRationalExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<RationalNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFlonumExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcExactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithGC<ExactComplexNumberKev, ExactComplexNumberKev, ExactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcInexactComplexExactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, ExactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  const InexactComplexNumberKev* divisor(vm->acc_);
  InexactComplexNumberKev quotient(NumberKev::quotient(dividend, *divisor));
  vm->acc_ = &quotient;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcRationalInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<RationalNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcFlonumInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<FlonumKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcExactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<ExactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Divide::funcInexactComplexInexactComplex(KevesVM* vm, const_KevesIterator pc) {
  return funcWithoutGC<InexactComplexNumberKev, InexactComplexNumberKev, InexactComplexNumberKev>(vm, pc);
}

void LibRnrsBase::Abs::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum()) {
    KevesFixnum num(vm->acc_);
    vm->acc_ = KevesFixnum(abs(num));
    return KevesVM::returnValue(vm, pc);
  }

  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* num(vm->acc_);
    RationalNumberKev abs_num(num->abs());
    vm->acc_ = &abs_num;
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* org(vm->acc_);
    FlonumKev abs_num(org->abs());
    vm->acc_ = &abs_num;
    return KevesVM::returnValueSafe(vm, pc);
  }

  vm->acc_ = EMB_UNDEF;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Numerator::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    const Bignum* numerator(rational->numerator());
    BignumInt one(1);
    RationalNumberKev num(rational->isNegative(), numerator, &one);
    vm->acc_ = num.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* num(vm->acc_);

    if (num->isNaN() || num->isInfinity()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    if (num->isZero()) {
      vm->acc_ = KevesFixnum(0);
      return KevesVM::returnValue(vm, pc);
    }
    
    RationalNumberKev rational(num->toExact(vm->gc()));
    FlonumKev flonum(rational.makeMPQ().get_num().get_d());
    vm->acc_ = &flonum;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Denominator::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum()) {
    vm->acc_ = KevesFixnum(1);
    return KevesVM::returnValue(vm, pc);
  }

  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    const Bignum* denominator(rational->denominator());
    BignumInt one(1);
    RationalNumberKev den(false, denominator, &one);
    vm->acc_ = den.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* num(vm->acc_);
    
    if (num->isNaN() || num->isInfinity()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    if (num->isZero()) {
      vm->acc_ = KevesFixnum(1);
      return KevesVM::returnValue(vm, pc);
    }
    
    RationalNumberKev rational(num->toExact(vm->gc()));
    FlonumKev flonum(rational.makeMPQ().get_den().get_d());
    vm->acc_ = &flonum;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Floor::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev floor_num(rational->floor(vm->gc()));
    vm->acc_ = floor_num.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev floor_num(flonum->floor());
    vm->acc_ = &floor_num;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Ceiling::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev ceiling_num(rational->ceiling(vm->gc()));
    vm->acc_ = ceiling_num.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev ceiling_num(flonum->ceiling());
    vm->acc_ = &ceiling_num;
    return KevesVM::returnValueSafe(vm, pc);
  }

  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Truncate::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev truncate_num(rational->truncate(vm->gc()));
    vm->acc_ = truncate_num.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev truncate_num(flonum->truncate());
    vm->acc_ = &truncate_num;
    return KevesVM::returnValueSafe(vm, pc);
  }

  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Round::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isFixnum())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev round_num(rational->round(vm->gc()));
    vm->acc_ = round_num.optimize();
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev round_num(flonum->round());
    vm->acc_ = &round_num;
    return KevesVM::returnValueSafe(vm, pc);
  }

  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::MakeRectangular::func(KevesVM* vm, const_KevesIterator pc) {
  return KevesVM::makeRectangular(vm, pc);
}

void LibRnrsBase::MakePolar::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev mag(NumberKev::makeFlonum(vm->acc_));
  FlonumKev ang(NumberKev::makeFlonum(vm->gr1_));
  FlonumKev real_part(mag.multiply(ang.cos()));
  FlonumKev imag_part(mag.multiply(ang.sin()));
  vm->acc_ = &real_part;
  vm->gr1_ = &imag_part;
  return KevesVM::makeRectangular(vm, pc);
}

void LibRnrsBase::RealPart::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isRationalNumber() || vm->acc_.is<FlonumKev>())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* complex(vm->acc_);
    RationalNumberKev real(complex->real());
    vm->acc_ = &real;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  if (vm->acc_.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* complex(vm->acc_);
    FlonumKev real(complex->real());
    vm->acc_ = &real;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::ImagPart::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isRationalNumber()) {
    vm->acc_ = KevesFixnum(0);
    return KevesVM::returnValue(vm, pc);
  }

  if (vm->acc_.is<FlonumKev>()) {
    FlonumKev nan(std::numeric_limits<double>::quiet_NaN());
    vm->acc_ = &nan;
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* complex(vm->acc_);
    RationalNumberKev imag(complex->imag());
    vm->acc_ = &imag;
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (vm->acc_.is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* complex(vm->acc_);
    FlonumKev imag(complex->imag());
    vm->acc_ = &imag;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Magnitude::func(KevesVM* vm, const_KevesIterator pc) {
  if (!vm->acc_.isNumber())
    return KevesVM::returnValue(vm, pc);
  
  if (vm->acc_.isRealNumber())
    return Abs::func(vm, pc);
  
  auto mangitude = [&](const FlonumKev& real_part, const FlonumKev& imag_part) -> void {
    FlonumKev real_part2(real_part.multiply(real_part));
    FlonumKev imag_part2(imag_part.multiply(imag_part));
    FlonumKev mag(real_part2.add(imag_part2).sqrt());
    vm->acc_ = &mag;
    return KevesVM::returnValueSafe(vm, pc);
  };

  if (vm->acc_.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* complex(vm->acc_);
    RationalNumberKev rational_real_part(complex->real());
    RationalNumberKev rational_imag_part(complex->imag());

    return mangitude(NumberKev::makeFlonum(&rational_real_part),
		     NumberKev::makeFlonum(&rational_imag_part));
  }
  
  const InexactComplexNumberKev* complex(vm->acc_);
  return mangitude(complex->real(), complex->imag());
}

void LibRnrsBase::Angle::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.isRealNumber()) {
    FlonumKev ang(NumberKev::isNegative(vm->acc_) ? acos(-1.0) : 0.0);
    vm->acc_ = &ang;
    return KevesVM::returnValueSafe(vm, pc);
  }
  
  auto angle = [&](const FlonumKev& real_part, const FlonumKev& imag_part) {
    if (real_part.isZero()) {
      FlonumKev r(imag_part.isNegative() ? -acos(0.0) : acos(0.0));
      vm->acc_ = &r;
      return KevesVM::returnValueSafe(vm, pc);
    }
    
    if (real_part.isNegative()) {
      FlonumKev r(imag_part.divide(real_part).atan());
      FlonumKev pi(acos(-1.0));
      FlonumKev r2(r.isNegative() ? r.add(pi) : r.subtract(pi));
      vm->acc_ = &r2;
      return KevesVM::returnValueSafe(vm, pc);
    }
    
    FlonumKev r(imag_part.divide(real_part).atan());
    vm->acc_ = &r;
    return KevesVM::returnValueSafe(vm, pc);
  };

  if (vm->acc_.is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* complex(vm->acc_);
    RationalNumberKev rational_real_part(complex->real());
    RationalNumberKev rational_imag_part(complex->imag());
    
    return angle(NumberKev::makeFlonum(&rational_real_part),
		 NumberKev::makeFlonum(&rational_imag_part));
  }

  const InexactComplexNumberKev* complex(vm->acc_);
  return angle(complex->real(), complex->imag());
}

void LibRnrsBase::procNumberToString(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int radix(10);
  int precision(0);

  switch (argn) {
  case 4: {
    if (!registers->argument(3).isFixnum())
      return procNumberToString_err1(vm, pc);
    
    int arg3(KevesFixnum(registers->argument(3)));
    
    if (arg3 < 1)
      return procNumberToString_err1(vm, pc);
    
    precision = arg3;
  }
    
  case 3: {
    if (!registers->argument(2).isFixnum())
      return procNumberToString_err1(vm, pc);
    
    int arg2(KevesFixnum(registers->argument(2)));
    
    if (arg2 == 2 || arg2 == 8 || arg2 == 16) {
      if (precision != 0)
	return procNumberToString_err1(vm, pc);
      
      radix = arg2;
    } else {
      if (arg2 != 10)
	return procNumberToString_err1(vm, pc);
      
      radix = arg2;
    }
  }
    
  case 2:
    if (registers->argument(1).isNumber()) {
      KevesValue z(registers->argument(1));
      vm->acc_ = NumberKev::makeStringFromNumber(vm->gc(),
						 z,
						 radix,
						 precision);
      return KevesVM::returnValueSafe(vm, pc);
    }
    
    return procNumberToString_err2(vm, pc);

  default:
    return procNumberToString_err3(vm, pc);
  }
}

void LibRnrsBase::procNumberToString_err1(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqIntNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::procNumberToString_err2(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::procNumberToString_err3(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req2GotMore);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::procStringToNumber(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int radix(0);

  switch (argn) {

  case 3:
    if (!registers->lastArgument().isFixnum()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req2GotMore);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    radix = KevesFixnum(registers->lastArgument());
    
  case 2: {
    if (!registers->argument(1).isString()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqStr);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    const StringKev* str(registers->argument(1));
    vm->acc_ = str;
    vm->gr1_ = KevesFixnum(radix);
    return procStringToNumber_helper1(vm, pc);
  }
    
  default:
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(argn > 3 ?
					 KevesBuiltinValues::mesg_Req1Or2GotMore :
					 KevesBuiltinValues::mesg_Req1Or2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
}
  
void LibRnrsBase::procStringToNumber_helper1(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  int radix(KevesFixnum(vm->gr1_));
  bool exact(true);

  int pos(NumberKev::getPrefix(*str, &radix, &exact));

  if (pos < 0) {
    vm->acc_ = EMB_FALSE;
    return KevesVM::returnValueSafe(vm, pc);
  }

  KevesValue number1;
  StringKev str1(str->right(str->size() - pos));
  int pos1(0);

  if (str1.equals("+i")) {
    number1 = KevesFixnum(1);
    pos1 = 1;
  } else if (str1.equals("-i")) {
    number1 = KevesFixnum(-1);
    pos1 = 1;
  } else if (radix == 2 || radix == 8 || radix == 16) {
    number1 = NumberKev::makeRealNumberFromBinary(vm->gc(), str1, &pos1, radix);
  } else if (radix == 0 || radix == 10) {
    number1 = NumberKev::makeRealNumberFromDecimal(vm->gc(), str1, &pos1, &radix, &exact);
  } else {
    vm->acc_ = EMB_FALSE;
    return KevesVM::returnValueSafe(vm, pc);
  }

  if (!number1.isNumber()) {
    vm->acc_ = EMB_FALSE;
    return KevesVM::returnValueSafe(vm, pc);
  }

  // no imaginary part
  if (pos1 == str1.size()) {
    if (exact) {
      vm->acc_ = number1;
      return KevesVM::returnValueSafe(vm, pc);
    }
    
    FlonumKev flonum(NumberKev::makeFlonum(number1));
    vm->acc_ = &flonum;
    return KevesVM::returnValueSafe(vm, pc);
  }

  // no real part
  if(pos1 == str1.size() - 1 && str1.at(pos1) == QChar('i')) {
    if (str1.at(0) == QChar('+') || str1.at(0) == QChar('-')) {
      if (exact) {
	vm->acc_ = KevesFixnum(0);
	vm->gr1_ = number1;
	return KevesVM::makeRectangular(vm, pc);
      }
      
      FlonumKev flonum(NumberKev::makeFlonum(number1));
      vm->acc_ = KevesFixnum(0);
      vm->gr1_ = &flonum;
      return KevesVM::makeRectangular(vm, pc);
    }
    
    vm->acc_ = EMB_FALSE;
    return KevesVM::returnValueSafe(vm, pc);
  }

  KevesValue number2;
  QChar chr(str1.at(pos1));

  if (chr == QChar('+') || chr == QChar('-')) {
    const StringKev str2(str1.right(str1.size() - pos1));
    int pos2(0);
    if (radix == 2 || radix == 8 || radix == 16) {
      number2 = NumberKev::makeRealNumberFromBinary(vm->gc(), str2, &pos2, radix);
    } else if (radix == 0 || radix == 10) {
      number2 = NumberKev::makeRealNumberFromDecimal(vm->gc(), str2, &pos2, &radix, &exact);
    }

    if (!number2.isNumber()) {
      vm->acc_ = EMB_FALSE;
      return KevesVM::returnValueSafe(vm, pc);
    }
    
    if (pos2 == str2.size() - 1 && str2.at(pos2) == QChar('i')) {
      if (exact) {
	vm->acc_ = number1;
	vm->gr1_ = number2;
	return KevesVM::makeRectangular(vm, pc);
      }
      
      FlonumKev flonum1(NumberKev::makeFlonum(number1));
      FlonumKev flonum2(NumberKev::makeFlonum(number2));
      vm->acc_ = &flonum1;
      vm->gr1_ = &flonum2;
      return KevesVM::makeRectangular(vm, pc);
    }
  }

  if (chr == QChar('@')) {
    const StringKev str2(str1.right(str1.size() - pos1 - 1));
    int pos2(0);

    if (radix == 2 || radix == 8 || radix == 16) {
      number2 = NumberKev::makeFromBinary(vm->gc(), str2, radix, true);
    } else if (radix == 0 || radix == 10) {
      number2 = NumberKev::makeRealNumberFromDecimal(vm->gc(), str2, &pos2, &radix, &exact);
    }
    
    if (number2.isNumber() && pos2 == str2.size()) {
      if (number1.isRealNumber() && number2.isRealNumber()) {
	FlonumKev mag(NumberKev::makeFlonum(number1));
	FlonumKev ang(NumberKev::makeFlonum(number2));
	FlonumKev real_part(mag.multiply(ang.cos()));
	FlonumKev imag_part(mag.multiply(ang.sin()));
	vm->acc_ = &real_part;
	vm->gr1_ = &imag_part;
	return KevesVM::makeRectangular(vm, pc);
      }
    }
  }
  
  vm->acc_ = EMB_FALSE;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flexp::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.exp());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Fllog::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.log());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flsin::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.sin());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flcos::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.cos());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Fltan::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.tan());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flasin::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.asin());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flacos::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.acos());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flatan::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.atan());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::Flsqrt::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));

  if (flonum.isNegative()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }

  FlonumKev r(flonum.sqrt());
  vm->acc_ = &r;
  return KevesVM::returnValueSafe(vm, pc);
}

void LibRnrsBase::procFlexpt(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  if (registers->argn() == 3) {
    KevesValue x1(registers->lastArgument(1));
    KevesValue x2(registers->lastArgument());

    if (x1.isRealNumber() && x2.isRealNumber()) {
      FlonumKev fl_x1(NumberKev::makeFlonum(x1));
      FlonumKev fl_x2(NumberKev::makeFlonum(x2));
      FlonumKev r(fl_x1.expt(fl_x2));
      vm->acc_ = &r;
      return KevesVM::returnValueSafe(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqRealNum);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }

  if (registers->argn() == 1) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }

  if (registers->argn() == 2) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req2Got1);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_Req2GotMore);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

bool LibRnrsBase::IsBoolean::func(KevesValue kev) {
  return kev.isBool();
}

void LibRnrsBase::Not::func(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->acc_ == EMB_FALSE ? EMB_TRUE : EMB_FALSE;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Cons::func(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(vm->acc_, vm->gr1_);

  vm->checkStack(&pair, &func, pc);

  vm->acc_ = &pair;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Car::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  vm->acc_ = pair->car();
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Cdr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  vm->acc_ = pair->cdr();
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Cadr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.isPair()) {
    const PairKev* pair(cdr);
    vm->acc_ = pair->car();
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cddr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.isPair()) {
    const PairKev* pair(cdr);
    vm->acc_ = pair->cdr();
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caaar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.isPair()) {
    const PairKev* pair(car);
    KevesValue caar(pair->car());
    if (caar.isPair()) {
      const PairKev* pair(caar);
      vm->acc_ = pair->car();
      return KevesVM::returnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caadr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.isPair()) {
    const PairKev* pair(cdr);
    KevesValue cadr(pair->car());
    if (cadr.isPair()) {
      const PairKev* pair(cadr);
      vm->acc_ = pair->car();
      return KevesVM::returnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cadar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.isPair()) {
    const PairKev* pair(car);
    KevesValue cdar(pair->cdr());
    if (cdar.isPair()) {
      const PairKev* pair(cdar);
      vm->acc_ = pair->car();
      return KevesVM::returnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caddr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.isPair()) {
    const PairKev* pair(cdr);
    KevesValue cddr(pair->cdr());
    if (cddr.isPair()) {
      const PairKev* pair(cddr);
      vm->acc_ = pair->car();
      return KevesVM::returnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.isPair()) {
    const PairKev* pair(car);
    vm->acc_ = pair->car();
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.isPair()) {
    const PairKev* pair(car);
    vm->acc_ = pair->cdr();
    return KevesVM::returnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdaar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.isPair()) {
    const PairKev* pair(car);
    KevesValue caar(pair->car());
    if (caar.isPair()) {
      const PairKev* pair(caar);
      vm->acc_ = pair->cdr();
      return KevesVM::returnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdadr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.isPair()) {
    const PairKev* pair(cdr);
    KevesValue cadr(pair->car());
    if (cadr.isPair()) {
      const PairKev* pair(cadr);
      vm->acc_ = pair->cdr();
      return KevesVM::returnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cddar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.isPair()) {
    const PairKev* pair(car);
    KevesValue cdar(pair->cdr());
    if (cdar.isPair()) {
      const PairKev* pair(cdar);
      vm->acc_ = pair->cdr();
      return KevesVM::returnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdddr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.isPair()) {
    const PairKev* pair(cdr);
    KevesValue cddr(pair->cdr());
    if (cddr.isPair()) {
      const PairKev* pair(cddr);
      vm->acc_ = pair->cdr();
      return KevesVM::returnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

bool LibRnrsBase::IsNull::func(KevesValue kev) {
  return kev == EMB_NULL;
}

void LibRnrsBase::procList(KevesVM* vm, const_KevesIterator pc) {
  int argn(vm->registers_.argn());
  vm->acc_ = EMB_NULL;
  return (argn > 1 ? procList_helper : KevesVM::returnValue)(vm, pc);
}

void LibRnrsBase::procList_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  PairKev pair(registers->lastArgument(), vm->acc_);
  
  vm->checkStack(&pair, &procList_helper, pc);
  
  vm->acc_ = &pair;
  
  if (registers->argn() > 2) {
    registers->popArgument();
    return procList_helper(vm, pc);
  }
  
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::Length::func(KevesVM* vm, const_KevesIterator pc) {
  int length(KevesVM::getListLength(vm->acc_));

  if (length >= 0) {
    vm->acc_ = KevesFixnum(length);
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqProperList);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::Reverse::func(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = vm->acc_;
  vm->acc_ = EMB_NULL;
  return func_helper(vm, pc);
}

void LibRnrsBase::Reverse::func_helper(KevesVM* vm, const_KevesIterator pc) {
  if (vm->gr1_.isPair()) {
    const PairKev* last_pair(vm->gr1_);
    PairKev new_pair(last_pair->car(), vm->acc_);
    
    vm->checkStack(&new_pair, &func_helper, pc);
    
    vm->acc_ = &new_pair;
    vm->gr1_ = last_pair->cdr();
    return func_helper(vm, pc);
  }

  if (vm->gr1_ == EMB_NULL)
    return KevesVM::returnValue(vm, pc);

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqProperList);
  vm->gr2_ = EMB_NULL;
  return KevesVM::raiseAssertCondition(vm, pc);
}

bool LibRnrsBase::IsSymbolEqual::func(KevesValue kev1, KevesValue kev2) {
  const SymbolKev* sym1(kev1);
  const SymbolKev* sym2(kev2);
  return sym1->equals(*sym2);
}

void LibRnrsBase::SymbolToString::func(KevesVM* vm, const_KevesIterator pc) {
  const SymbolKev* sym(vm->acc_);
  StringKev str(sym->getKevesString());

  vm->checkStack(&str, &func, pc);

  vm->acc_ = &str;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::StringToSymbol::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  SymbolKev sym(*str);

  vm->checkStack(&sym, &func, pc);

  vm->acc_ = &sym;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::CharToInteger::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);
  vm->acc_ = KevesFixnum(static_cast<fx_int>(chr.unicode()));
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::IntegerToChar::func(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum fixnum(vm->acc_);
  QChar chr(fixnum);
  vm->gr1_ = vm->acc_;
  vm->acc_ = KevesChar(chr);
  return KevesVM::returnValue(vm, pc);
}

bool LibRnrsBase::IsCharEqual::func(KevesValue kev1, KevesValue kev2) {
  return kev1 == kev2;
}

bool LibRnrsBase::IsCharLT::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.compare(chr2) < 0;
}

bool LibRnrsBase::IsCharGT::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.compare(chr2) > 0;
}

bool LibRnrsBase::IsCharLTE::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.compare(chr2) <= 0;
}

bool LibRnrsBase::IsCharGTE::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.compare(chr2) >= 0;
}

void LibRnrsBase::procMakeString(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  QChar chr(' ');

  switch (argn) {
  case 3:
    if (!registers->lastArgument().isChar()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqChar);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    chr = KevesChar(registers->lastArgument());
    
  case 2:
    {
      KevesValue first(registers->argument(1));
      
      if (!first.isFixnum()) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqIntNum);
	vm->gr2_ = EMB_NULL;
	return KevesVM::raiseAssertCondition(vm, pc);
      }
      
      int size(KevesFixnum { first } );

      if (size < 0) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
	vm->gr2_ = EMB_NULL;
	return KevesVM::raiseAssertCondition(vm, pc);
      }
      
      vm->acc_ = vm->makeString(size, chr);
      return KevesVM::returnValueSafe(vm, pc);
    }

  default:
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(registers->argn() > 3 ?
					 KevesBuiltinValues::mesg_Req1Or2GotMore :
					 KevesBuiltinValues::mesg_Req1Or2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
}

void LibRnrsBase::procString(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int size(argn - 1);
  StringKev* string(vm->makeString(size));
  QChar* iter(string->data());

  for (int idx(1); idx < argn; ++idx, ++iter) {
    KevesValue current(registers->argument(idx));

    if (!current.isChar()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqChar);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }

    KevesChar chr(current);
    *iter = chr;
  }

  vm->acc_ = string;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::StringLength::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  vm->acc_ = KevesFixnum(str->size());
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::StringRef::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* string(vm->acc_);
  int k(KevesFixnum(vm->gr1_));

  if (k < 0 || k >= string->size()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }

  vm->acc_ = KevesChar(string->at(k));
  return KevesVM::returnValue(vm, pc);
}

bool LibRnrsBase::IsStringEqual::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->equals(*str2);
}

bool LibRnrsBase::IsStringLT::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->isLT(*str2);
}

bool LibRnrsBase::IsStringGT::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->isGT(*str2);
}

bool LibRnrsBase::IsStringLTE::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->isLTE(*str2);
}

bool LibRnrsBase::IsStringGTE::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->isGTE(*str2);
}

void LibRnrsBase::Substring::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  const StringKev* str(registers->argument(1));
  int start(KevesFixnum(registers->argument(2)));
  int end(KevesFixnum(registers->argument(3)));
  int len(str->size());
    
  if (start < 0 || start > end || end > len) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  // vm->acc_ = vm->keves_gc_.make(StringKev::makeSubstring(str, start, end));
  vm->acc_ = StringKev::makeSubstring(vm->gc(), str, start, end);

  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::StringToList::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* string(vm->acc_);
  int last_index(KevesFixnum(string->size() - 1));

  if (last_index < 0) {
    vm->acc_ = EMB_NULL;
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->gr1_ = EMB_NULL;
  vm->gr2_ = KevesFixnum(last_index);
  return func_helper(vm, pc);
}

void LibRnrsBase::StringToList::func_helper(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(EMB_NULL, EMB_NULL);

  vm->checkStack(&pair, &func_helper, pc);

  int index(KevesFixnum(vm->gr2_));
  const StringKev* string(vm->acc_);
  pair.set(KevesChar(string->at(index)), vm->gr1_);

  if (index == 0) {
    vm->acc_ = &pair;
    return KevesVM::returnValue(vm, pc);
  }
  
  vm->gr1_ = &pair;
  vm->gr2_ = KevesFixnum(index - 1);
  return func_helper(vm, pc);
}

void LibRnrsBase::StringCopy::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* original(vm->acc_);
  StringKev copy(original->copy(vm->gc()));

  vm->checkStack(&copy, &func, pc);

  vm->acc_ = &copy;
  return KevesVM::returnValue(vm, pc);
}

bool LibRnrsBase::IsVector::func(KevesValue kev) {
  return kev.isVector();
}

KevesValue LibRnrsBase::IsVector::message(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqVector);
}

KevesValue LibRnrsBase::IsVector::message1(KevesCommon* common) {
  return common->getMesgText(KevesBuiltinValues::mesg_ReqVectorAs1st);
}

void LibRnrsBase::procMakeVector(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  KevesValue obj(EMB_UNDEF);

  switch (argn) {
  case 3:
    obj = registers->lastArgument();
    
  case 2: {
    KevesValue first(registers->argument(1));
    
    if (!first.isFixnum()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqIntNum);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    int size(KevesFixnum { first } );
    
    if (size < 0) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
      vm->gr2_ = EMB_NULL;
      return KevesVM::raiseAssertCondition(vm, pc);
    }
    
    vm->acc_ = vm->makeVector(size, obj);
    return KevesVM::returnValue(vm, pc);
  }
    
  default:
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(registers->argn() > 3 ?
					 KevesBuiltinValues::mesg_Req1Or2GotMore :
					 KevesBuiltinValues::mesg_Req1Or2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
}

void LibRnrsBase::procVector(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int size(argn - 1);
  VectorKev* vector(vm->makeVector(size));
  KevesIterator iter(vector->begin());

  for (int idx(1); idx < argn; ++idx, ++iter) {
    *iter = registers->argument(idx);
  }

  vm->acc_ = vector;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::VectorLength::func(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  vm->acc_ = KevesFixnum(vector->size());
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::VectorRef::func(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  int k(KevesFixnum(vm->gr1_));

  if (k < 0 || k >= vector->size()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  vm->acc_ = vector->at(k);
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::VectorSetE::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  VectorKev* vector(MutableKevesValue(registers->argument(1)));
  int k(KevesFixnum(registers->argument(2)));

  if (k < 0 || k >= vector->size()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }

  KevesIterator iter(vector->begin());
  *(iter + k) = registers->argument(3);
  // if (vm->keves_gc_.isInTenuredWithoutMark(vector)) {
  //   vm->keves_gc_.pushToMarkedList(vector);
  // }
  vm->gc()->markLive(vector);
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::VectorToList::func(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  int last_index(KevesFixnum(vector->size() - 1));

  if (last_index < 0) {
    vm->acc_ = EMB_NULL;
    return KevesVM::returnValue(vm, pc);
  }

  vm->gr1_ = EMB_NULL;
  vm->gr2_ = KevesFixnum(last_index);
  return func_helper(vm, pc);
}

void LibRnrsBase::VectorToList::func_helper(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  int index(KevesFixnum(vm->gr2_));
  PairKev pair(vector->at(index), vm->gr1_);
  
  vm->checkStack(&pair, &func_helper, pc);
  
  if (index == 0) {
    vm->acc_ = &pair;
    return KevesVM::returnValue(vm, pc);
  }

  vm->gr1_ = &pair;
  vm->gr2_ = KevesFixnum(index - 1);
  return func_helper(vm, pc);
}

void LibRnrsBase::procListToVector(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());

  if (argn != 2)
    return Function::raiseNotGet1Arg(vm, pc);
  
  KevesValue temp(registers->lastArgument());
  int size(KevesVM::getListLength(temp));
  
  if (size < 0) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqProperList);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  VectorKev* vector(vm->makeVector(size));
  
  for (KevesValue& elem : *vector) {
    const PairKev* pair(temp);
    elem = pair->car();
    temp = pair->cdr();
  }
  
  vm->acc_ = vector;
  return KevesVM::returnValue(vm, pc);
}

void LibRnrsBase::CallWithCC::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;
  
  vm->checkStack(&arg_frame, &func, pc);
  
  pc = registers->unwind();
  
  ContinuationKev conti_obj(*registers);
  StackFrameKev stack_frame;
  registers->wind(pc, &stack_frame, &arg_frame);
  
  registers->pushArgument(vm->acc_);
  registers->pushArgument(&conti_obj);
  
  return KevesVM::cmd_APPLY(vm, pc);
}

void LibRnrsBase::CallWithValues::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame1;
  ArgumentFrameKevWithArray<04> arg_frame2;
  
  vm->checkStack(&arg_frame1, &func, pc);
  
  pc = registers->unwind();
  
  // wind and push *proc2*
  StackFrameKev stack_frame1;
  registers->wind(pc, &stack_frame1, &arg_frame1);
  pc = vm->common()->builtin()->code_APPLY_MULT_APPLY();
  registers->pushArgument(vm->gr1_);
  
  // wind and push *proc1*
  StackFrameKev stack_frame2;
  registers->wind(pc, &stack_frame2, &arg_frame2);
  registers->pushArgument(vm->acc_);
  vm->keves_vals_ = nullptr;
  vm->gr2_ = vm->gr1_;
  vm->gr1_ = vm->acc_;
  return KevesVM::cmd_APPLY(vm, pc);
}

void LibRnrsBase::procValues(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() > 1) {
    vm->acc_ = registers->argument(1);
    vm->keves_vals_ = registers->argp();
    vm->keves_vals_->replace(0, KevesFixnum(registers->argn()));
  } else {
    vm->keves_vals_ = nullptr;
    vm->acc_ = EMB_UNDEF;
  }

  return KevesVM::returnValue(vm, pc);

}

void LibRnrsBase::DynamicWind::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;
  
  vm->checkStack(&arg_frame, &func, pc);
  
  KevesValue before(registers->argument(1));
  pc = vm->common()->builtin()->code_SET_DYNAMIC_WIND();
  StackFrameKev stack_frame;
  registers->wind(pc, &stack_frame, &arg_frame);
  registers->pushArgument(before);
  vm->gr1_ = before;
  return KevesVM::cmd_APPLY(vm, pc);
}
  
void LibRnrsBase::procAssertionViolation(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() < 3) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(registers->argn() == 2 ?
					 KevesBuiltinValues::mesg_Req2Got1 :
					 KevesBuiltinValues::mesg_Req2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }
  
  return procAssertionViolation_helper(vm, pc);
}

void LibRnrsBase::procAssertionViolation_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  PairKev pair(registers->lastArgument(), vm->acc_);
  
  vm->checkStack(&pair, &procAssertionViolation_helper, pc);
  
  if (registers->argn() > 3) {
    registers->popArgument();
    vm->acc_ = &pair;
    return procAssertionViolation_helper(vm, pc);
  }

  vm->gr1_ = registers->lastArgument();
  vm->gr2_ = vm->acc_;
  vm->acc_ = registers->lastArgument(1);
  return KevesVM::raiseAssertCondition(vm, pc);
}

void LibRnrsBase::procApply(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() < 3) {
    vm->acc_ = vm->gr2_;

    vm->gr1_ = vm->common()->getMesgText(registers->argn() == 2 ?
				       KevesBuiltinValues::mesg_Req2Got1 :
				       KevesBuiltinValues::mesg_Req2Got0);

    vm->gr2_ = EMB_NULL;
    return KevesVM::raiseAssertCondition(vm, pc);
  }

  int argn(registers->argn());
  KevesValue last(registers->lastArgument());

  if (!last.isPair() && last != EMB_NULL) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqListAsLast);
    vm->gr2_ = EMB_NULL;
    return vm->raiseAssertCondition(vm, pc);
  }

  if (!registers->argument(1).isProcedure()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqProcAs1st);
    vm->gr2_ = EMB_NULL;
    return vm->raiseAssertCondition(vm, pc);
  }
  
  int num_args(argn - 2);
  int final_num_args(num_args + KevesVM::getListLength(last));
  ArgumentFrameKev* new_arg_frame(ArgumentFrameKev::make(vm->gc(), final_num_args));
  
  for (int i(0); i < num_args; ++i) {
    new_arg_frame->assign(i, registers->argument(i + 1));
  }
  
  registers->set_arg(new_arg_frame, num_args);
  vm->acc_ = last;
  return procApply_helper(vm, pc);
}

void LibRnrsBase::procApply_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue temp(vm->acc_);

  while (temp.isPair()) {
    const PairKev* pair(temp);
    KevesValue car(pair->car());
    registers->pushArgument(car);
    temp = pair->cdr();
  }

  if (temp != EMB_NULL) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->common()->getMesgText(KevesBuiltinValues::mesg_ReqProperList);
    vm->gr2_ = EMB_NULL;
    return vm->raiseAssertCondition(vm, pc);
  }

  vm->gr1_ = registers->argument(0);
  return KevesVM::applyProcedure(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* make(KevesCommon* common) {
    LibRnrsBase* library(new LibRnrsBase());
    library->init(common);
    return library;
  }
}
