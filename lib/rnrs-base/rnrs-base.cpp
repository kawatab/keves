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
#include "keves_base.hpp"
#include "keves_base-inl.hpp"
#include "keves_builtin_values.hpp"
#include "keves_template.hpp"
#include "keves_vm.hpp"
#include "kev/code.hpp"
#include "kev/code-inl.hpp"
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


void LibRnrsBase::Init(KevesBase* base) {
  std::cout << "LibRnrsBase::Init()" << std::endl;

  // SetID("rnrs", "base-bin");
  // SetVerNum(6);
					       
  sym_abs_ = SymbolKev::Make(base, "abs");
  sym_add_ = SymbolKev::Make(base, "+");
  // sym_and_ = SymbolKev::Make(base, "and");
  sym_angle_ = SymbolKev::Make(base, "angle");
  // sym_append_ = SymbolKev::Make(base, "append");
  sym_apply_ = SymbolKev::Make(base, "apply");
  sym_assertion_violation_ = SymbolKev::Make(base, "assertion-violation");
  // sym_begin_ = SymbolKev::Make(base, "begin");
  // sym_begin0_ = SymbolKev::Make(base, "begin0");
  sym_boolean_q_ = SymbolKev::Make(base, "boolean?");
  sym_call_with_cc_ = SymbolKev::Make(base, "call/cc");
  sym_call_with_current_continuation_ = SymbolKev::Make(base, "call-with-current-continuation");
  sym_call_with_values_ = SymbolKev::Make(base, "call-with-values");
  sym_car_ = SymbolKev::Make(base, "car");
  sym_cdr_ = SymbolKev::Make(base, "cdr");
  sym_caar_ = SymbolKev::Make(base, "caar");
  sym_cadr_ = SymbolKev::Make(base, "cadr");
  sym_cdar_ = SymbolKev::Make(base, "cdar");
  sym_cddr_ = SymbolKev::Make(base, "cddr");
  sym_caaar_ = SymbolKev::Make(base, "caaar");
  sym_caadr_ = SymbolKev::Make(base, "caadr");
  sym_cadar_ = SymbolKev::Make(base, "cadar");
  sym_caddr_ = SymbolKev::Make(base, "caddr");
  sym_cdaar_ = SymbolKev::Make(base, "cdaar");
  sym_cdadr_ = SymbolKev::Make(base, "cdadr");
  sym_cddar_ = SymbolKev::Make(base, "cddar");
  sym_cdddr_ = SymbolKev::Make(base, "cdddr");
  sym_ceiling_ = SymbolKev::Make(base, "ceiling");
  sym_char_eq_q_ = SymbolKev::Make(base, "char=?");
  sym_char_gt_q_ = SymbolKev::Make(base, "char>?");
  sym_char_gte_q_ = SymbolKev::Make(base, "char>=?");
  sym_char_lt_q_ = SymbolKev::Make(base, "char<?");
  sym_char_lte_q_ = SymbolKev::Make(base, "char<=?");
  sym_char_q_ = SymbolKev::Make(base, "char?");
  sym_char_to_integer_ = SymbolKev::Make(base, "char->integer");
  sym_complex_q_ = SymbolKev::Make(base, "complex?");
  sym_cons_ = SymbolKev::Make(base, "cons");
  sym_denominator_ = SymbolKev::Make(base, "denominator");
  sym_divide_ = SymbolKev::Make(base, "/");
  sym_dynamic_wind_ = SymbolKev::Make(base, "dynamic-wind");
  // sym_eq_q_ = SymbolKev::Make(base, "eq?");
  sym_equal_ = SymbolKev::Make(base, "=");
  // sym_eqv_q_ = SymbolKev::Make(base, "eqv?");
  sym_exact_q_ = SymbolKev::Make(base, "exact?");
  sym_flacos_ = SymbolKev::Make(base, "flacos");
  sym_flasin_ = SymbolKev::Make(base, "flasin");
  sym_flatan_ = SymbolKev::Make(base, "flatan");
  sym_flcos_ = SymbolKev::Make(base, "flcos");
  sym_flexact_ = SymbolKev::Make(base, "flexact");
  sym_flexp_ = SymbolKev::Make(base, "flexp");
  sym_flexpt_ = SymbolKev::Make(base, "flexpt");
  sym_fllog_ = SymbolKev::Make(base, "fllog");
  sym_floor_ = SymbolKev::Make(base, "floor");
  sym_flsin_ = SymbolKev::Make(base, "flsin");
  sym_flsqrt_ = SymbolKev::Make(base, "flsqrt");
  sym_fltan_ = SymbolKev::Make(base, "fltan");
  sym_greater_than_ = SymbolKev::Make(base, ">");
  sym_greater_than_equal_ = SymbolKev::Make(base, ">=");
  sym_even_q_ = SymbolKev::Make(base, "even?");
  sym_finite_q_ = SymbolKev::Make(base, "finite?");
  sym_imag_part_ = SymbolKev::Make(base, "imag-part");
  sym_infinite_q_ = SymbolKev::Make(base, "infinite?");
  sym_inexact_ = SymbolKev::Make(base, "inexact");
  sym_inexact_q_ = SymbolKev::Make(base, "inexact?");
  sym_integer_q_ = SymbolKev::Make(base, "integer?");
  sym_integer_valued_q_ = SymbolKev::Make(base, "integer-valued?");
  sym_integer_to_char_ = SymbolKev::Make(base, "integer->char");
  sym_length_ = SymbolKev::Make(base, "length");
  // sym_length_check_ = SymbolKev::Make(base, "length-check");
  sym_less_than_ = SymbolKev::Make(base, "<");
  sym_less_than_equal_ = SymbolKev::Make(base, "<=");
  sym_list_ = SymbolKev::Make(base, "list");
  sym_magnitude_ = SymbolKev::Make(base, "magnitude");
  sym_make_polar_ = SymbolKev::Make(base, "make-polar");
  sym_make_rectangular_ = SymbolKev::Make(base, "make-rectangular");
  sym_make_string_ = SymbolKev::Make(base, "make-string");
  sym_make_vector_ = SymbolKev::Make(base, "make-vector");
  // sym_map_ = SymbolKev::Make(base, "map");
  sym_max_ = SymbolKev::Make(base, "max");
  sym_min_ = SymbolKev::Make(base, "min");
  sym_multiply_ = SymbolKev::Make(base, "*");
  sym_nan_q_ = SymbolKev::Make(base, "nan?");
  sym_negative_q_ = SymbolKev::Make(base, "negative?");
  sym_not_ = SymbolKev::Make(base, "not");
  sym_null_q_ = SymbolKev::Make(base, "null?");
  sym_number_q_ = SymbolKev::Make(base, "number?");
  sym_number_to_string_ = SymbolKev::Make(base, "number->string");
  sym_numerator_ = SymbolKev::Make(base, "numerator");
  sym_odd_q_ = SymbolKev::Make(base, "odd?");
  sym_pair_q_ = SymbolKev::Make(base, "pair?");
  sym_positive_q_ = SymbolKev::Make(base, "positive?");
  sym_procedure_q_ = SymbolKev::Make(base, "procedure?");
  sym_rational_q_ = SymbolKev::Make(base, "rational?");
  sym_rational_valued_q_ = SymbolKev::Make(base, "rational-valued?");
  sym_real_part_ = SymbolKev::Make(base, "real-part");
  sym_real_q_ = SymbolKev::Make(base, "real?");
  sym_real_valued_q_ = SymbolKev::Make(base, "real-valued?");
  sym_reverse_ = SymbolKev::Make(base, "reverse");
  sym_round_ = SymbolKev::Make(base, "round");
  // sym_separate_first_value_ = SymbolKev::Make(base, "separate-first-value");
  sym_string_ = SymbolKev::Make(base, "string");
  // sym_string_append_ = SymbolKev::Make(base, "string-append");
  sym_string_copy_ = SymbolKev::Make(base, "string-copy");
  sym_string_eq_q_ = SymbolKev::Make(base, "string=?");
  sym_string_gt_q_ = SymbolKev::Make(base, "string>?");
  sym_string_gte_q_ = SymbolKev::Make(base, "string>=?");
  sym_string_length_ = SymbolKev::Make(base, "string-length");
  sym_string_lt_q_ = SymbolKev::Make(base, "string<?");
  sym_string_lte_q_ = SymbolKev::Make(base, "string<=?");
  sym_string_q_ = SymbolKev::Make(base, "string?");
  sym_string_ref_ = SymbolKev::Make(base, "string-ref");
  sym_string_to_list_ = SymbolKev::Make(base, "string->list");
  sym_string_to_number_ = SymbolKev::Make(base, "string->number");
  sym_string_to_symbol_ = SymbolKev::Make(base, "string->symbol");
  sym_substring_ = SymbolKev::Make(base, "substring");
  sym_subtract_ = SymbolKev::Make(base, "-");
  sym_symbol_eq_q_ = SymbolKev::Make(base, "symbol=?");
  sym_symbol_q_ = SymbolKev::Make(base, "symbol?");
  sym_symbol_to_string_ = SymbolKev::Make(base, "symbol->string");
  // sym_transpose_ = SymbolKev::Make(base, "transpose");
  sym_truncate_ = SymbolKev::Make(base, "truncate");
  sym_values_ = SymbolKev::Make(base, "values");
  sym_vector_ = SymbolKev::Make(base, "vector");
  sym_vector_length_ = SymbolKev::Make(base, "vector-length");
  sym_vector_q_ = SymbolKev::Make(base, "vector?");
  sym_vector_ref_ = SymbolKev::Make(base, "vector-ref");
  sym_vector_set_e_ = SymbolKev::Make(base, "vector-set!");
  sym_vector_to_list_ = SymbolKev::Make(base, "vector->list");
  sym_list_to_vector_ = SymbolKev::Make(base, "list->vector");
  sym_zero_q_ = SymbolKev::Make(base, "zero?");
  
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
  
  AddBind("abs", &proc_abs_);
  AddBind("+", &proc_add_);
  // AddBind("and");
  AddBind("angle", &proc_angle_);
  // AddBind("append");
  // AddBind("apply");
  AddBind("assertion-violation", &proc_assertion_violation_);
  // AddBind("begin");
  // AddBind("begin0");
  AddBind("boolean?", &proc_boolean_q_);
  AddBind("call/cc", &proc_call_with_cc_);
  AddBind("call-with-current-continuation", &proc_call_with_cc_);
  AddBind("call-with-values", &proc_call_with_values_);
  AddBind("car", &proc_car_);
  AddBind("cdr", &proc_cdr_);
  AddBind("caar", &proc_caar_);
  AddBind("cadr", &proc_cadr_);
  AddBind("cdar", &proc_cdar_);
  AddBind("cddr", &proc_cddr_);
  AddBind("caaar", &proc_caaar_);
  AddBind("caadr", &proc_caadr_);
  AddBind("cadar", &proc_cadar_);
  AddBind("caddr", &proc_caddr_);
  AddBind("cdaar", &proc_cdaar_);
  AddBind("cdadr", &proc_cdadr_);
  AddBind("cddar", &proc_cddar_);
  AddBind("cdddr", &proc_cdddr_);
  AddBind("ceiling", &proc_ceiling_);
  AddBind("char=?", &proc_char_eq_q_);
  AddBind("char>?", &proc_char_gt_q_);
  AddBind("char>=?", &proc_char_gte_q_);
  AddBind("char<?", &proc_char_lt_q_);
  AddBind("char<=?", &proc_char_lte_q_);
  AddBind("char?", &proc_char_q_);
  AddBind("char->integer", &proc_char_to_integer_);
  AddBind("complex?", &proc_complex_q_);
  AddBind("cons", &proc_cons_);
  AddBind("denominator", &proc_denominator_);
  AddBind("/", &proc_divide_);
  AddBind("dynamic-wind", &proc_dynamic_wind_);
  // AddBind("eq?");
  AddBind("=", &proc_equal_);
  // AddBind("eqv?");
  AddBind("exact?", &proc_exact_q_);
  AddBind("flacos", &proc_flacos_);
  AddBind("flasin", &proc_flasin_);
  AddBind("flatan", &proc_flatan_);
  AddBind("flcos", &proc_flcos_);
  AddBind("flexact", &proc_flexact_);
  AddBind("flexp", &proc_flexp_);
  AddBind("flexpt", &proc_flexpt_);
  AddBind("fllog", &proc_fllog_);
  AddBind("floor", &proc_floor_);
  AddBind("flsin", &proc_flsin_);
  AddBind("flsqrt", &proc_flsqrt_);
  AddBind("fltan", &proc_fltan_);
  AddBind(">", &proc_greater_than_);
  AddBind(">=", &proc_greater_than_equal_);
  AddBind("even?", &proc_even_q_);
  AddBind("finite?", &proc_finite_q_);
  AddBind("imag-part", &proc_imag_part_);
  AddBind("infinite?", &proc_infinite_q_);
  AddBind("inexact", &proc_inexact_);
  AddBind("inexact?", &proc_inexact_q_);
  AddBind("integer?", &proc_integer_q_);
  AddBind("integer-valued?", &proc_integer_valued_q_);
  AddBind("integer->char", &proc_integer_to_char_);
  AddBind("length", &proc_length_);
  // AddBind("length-check");
  AddBind("<", &proc_less_than_);
  AddBind("<=", &proc_less_than_equal_);
  AddBind("list", &proc_list_);
  AddBind("magnitude", &proc_magnitude_);
  AddBind("make-polar", &proc_make_polar_);
  AddBind("make-rectangular", &proc_make_rectangular_);
  AddBind("make-string", &proc_make_string_);
  AddBind("make-vector", &proc_make_vector_);
  // AddBind("map");
  AddBind("max", &proc_max_);
  AddBind("min", &proc_min_);
  AddBind("*", &proc_multiply_);
  AddBind("nan?", &proc_nan_q_);
  AddBind("negative?", &proc_negative_q_);
  AddBind("not", &proc_not_);
  AddBind("null?", &proc_null_q_);
  AddBind("number?", &proc_number_q_);
  AddBind("number->string", &proc_number_to_string_);
  AddBind("numerator", &proc_numerator_);
  AddBind("odd?", &proc_odd_q_);
  AddBind("pair?", &proc_pair_q_);
  AddBind("positive?", &proc_positive_q_);
  AddBind("procedure?", &proc_procedure_q_);
  AddBind("rational?", &proc_rational_q_);
  AddBind("rational-valued?", &proc_rational_valued_q_);
  AddBind("real-part", &proc_real_part_);
  AddBind("real?", &proc_real_q_);
  AddBind("real-valued?", &proc_real_valued_q_);
  AddBind("reverse", &proc_reverse_);
  AddBind("round", &proc_round_);
  // AddBind("separate-first-value");
  AddBind("string", &proc_string_);
  // AddBind("string-append");
  AddBind("string-copy", &proc_string_copy_);
  AddBind("string=?", &proc_string_eq_q_);
  AddBind("string>?", &proc_string_gt_q_);
  AddBind("string>=?", &proc_string_gte_q_);
  AddBind("string-length", &proc_string_length_);
  AddBind("string<?", &proc_string_lt_q_);
  AddBind("string<=?", &proc_string_lte_q_);
  AddBind("string?", &proc_string_q_);
  AddBind("string-ref", &proc_string_ref_);
  AddBind("string->list", &proc_string_to_list_);
  AddBind("string->number", &proc_string_to_number_);
  AddBind("string->symbol", &proc_string_to_symbol_);
  AddBind("substring", &proc_substring_);
  AddBind("-", &proc_subtract_);
  AddBind("symbol=?", &proc_symbol_eq_q_);
  AddBind("symbol?", &proc_symbol_q_);
  AddBind("symbol->string", &proc_symbol_to_string_);
  // AddBind("transpose");
  AddBind("truncate", &proc_truncate_);
  AddBind("values", &proc_values_);
  AddBind("vector", &proc_vector_);
  AddBind("vector-length", &proc_vector_length_);
  AddBind("vector?", &proc_vector_q_);
  AddBind("vector-ref", &proc_vector_ref_);
  AddBind("vector-set!", &proc_vector_set_e_);
  AddBind("vector->list", &proc_vector_to_list_);
  AddBind("list->vector", &proc_list_to_vector_);
  AddBind("zero?", &proc_zero_q_);

}

bool LibRnrsBase::IsNumber::func(KevesValue kev) {
  return kev.IsNumber();
}

bool LibRnrsBase::IsReal::func(KevesValue kev) {
  return kev.IsRealNumber();
}

bool LibRnrsBase::IsComplex::func(KevesValue kev) {
  return kev.IsComplexNumber();
}

bool LibRnrsBase::IsExact::func(KevesValue kev) {
  return kev.IsExactNumber();
}

bool LibRnrsBase::IsInexact::func(KevesValue kev) {
  return kev.IsInexactNumber();
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
  if (kev.IsFixnum()) {
    fx_int num(KevesFixnum { kev } );
    return num & 1;
  }

  if (kev.Is<RationalNumberKev>()) {
    const RationalNumberKev* num(kev);
    return num->isOdd();
  }

  if (kev.Is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isOdd();
  }

  return false;
}

bool LibRnrsBase::IsEven::func(KevesValue kev) {
  if (kev.IsFixnum()) {
    fx_int num(KevesFixnum { kev } );
    return !(num & 1);
  }

  if (kev.Is<RationalNumberKev>()) {
    const RationalNumberKev* num(kev);
    return num->isEven();
  }

  if (kev.Is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isEven();
  }

  return false;
}

void LibRnrsBase::Flexact::func(KevesVM* vm, const_KevesIterator pc) {
  return
    vm->acc_.IsExactNumber() ? KevesVM::ReturnValue(vm, pc) :
    vm->acc_.Is<FlonumKev>() ? func_helper1(vm, pc) :
    func_helper2(vm, pc);
}

void LibRnrsBase::Flexact::func_helper1(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* num(vm->acc_);

  if (num->isNaN() || num->isInfinity())
    return KevesVM::ReturnValue(vm, pc);

  if (num->isZero()) {
    vm->acc_ = KevesFixnum(0);
    return KevesVM::ReturnValue(vm, pc);
  }
  
  RationalNumberKev rational(num->toExact(vm->gc()));
  vm->acc_ = &rational;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Flexact::func_helper2(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* num(vm->acc_);
  FlonumKev real(num->real());
  FlonumKev imag(num->imag());

  if (real.isNaN() || real.isInfinity() || imag.isNaN() || imag.isInfinity())
    return KevesVM::ReturnValue(vm, pc);

  RationalNumberKev rational_real(real.toExact(vm->gc()));
  
  vm->CheckStack(&rational_real, &func_helper2, pc);
  
  if (imag.isZero()) {
    vm->acc_ = rational_real.optimize();
    return KevesVM::ReturnValue(vm, pc);
  }

  RationalNumberKev rational_imag(imag.toExact(vm->gc()));
  ExactComplexNumberKev complex(rational_real, rational_imag);
  vm->acc_ = &complex;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Inexact::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum()) {
    KevesFixnum fixnum(vm->acc_);
    FlonumKev flonum(fixnum);

    vm->CheckStack(&flonum, &func, pc);
    
    vm->acc_ = &flonum;
    return KevesVM::ReturnValue(vm, pc);
  }

  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* num(vm->acc_);
    FlonumKev flonum(num->toInexact());

    vm->CheckStack(&flonum, &func, pc);

    vm->acc_ = &flonum;
    return KevesVM::ReturnValue(vm, pc);
  }

  if (vm->acc_.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* num(vm->acc_);
    InexactComplexNumberKev inexact(num->toInexact());

    vm->CheckStack(&inexact, &func, pc);

    vm->acc_ = &inexact;
    return KevesVM::ReturnValue(vm, pc);
  }

  return KevesVM::ReturnValue(vm, pc);
}

KevesValue LibRnrsBase::IsNumber::message(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqNum);
}

KevesValue LibRnrsBase::IsReal::message(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
}

KevesValue LibRnrsBase::IsReal::message1(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqRealNumAs1st);
}

KevesValue LibRnrsBase::IsReal::message2(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqRealNumAs2nd);
}

bool LibRnrsBase::IsRealValued::func(KevesValue kev) {
  if (kev.IsRealNumber())
    return true;

  if (kev.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* exact(kev);
    return exact->isRealValued();
  }

  if (kev.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* inexact(kev);
    return inexact->isRealValued();
  }

  return false;
}

bool LibRnrsBase::IsRational::func(KevesValue kev) {
  if (kev.IsRationalNumber())
    return true;

  if (kev.Is<FlonumKev>()) {
    const FlonumKev* flonum(kev);
    return flonum->isInteger();
  }

  return false;
}

bool LibRnrsBase::IsRationalValued::func(KevesValue kev) {
  if (kev.IsRealNumber())
    return true;

  if (kev.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* exact(kev);
    return exact->isRealValued();
  }

  if (kev.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* inexact(kev);
    return inexact->isRealValued();
  }

  return false;
}

bool LibRnrsBase::IsInteger::func(KevesValue kev) {
  if (kev.IsFixnum())
    return true;

  if (kev.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(kev);
    return rational->isInteger();
  }

  if (kev.Is<FlonumKev>()) {
    const FlonumKev* flonum(kev);
    return flonum->isInteger();
  }

  return false;
}

KevesValue LibRnrsBase::IsInteger::message(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqIntNum);
}

bool LibRnrsBase::IsIntegerValued::func(KevesValue kev) {
  if (kev.IsFixnum())
    return true;

  if (kev.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(kev);
    return rational->isInteger();
  }

  if (kev.Is<FlonumKev>()) {
    const FlonumKev* flonum(kev);
    return flonum->isInteger();
  }

  if (kev.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* exact(kev);
    return exact->isIntegerValued();
  }

  if (kev.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* inexact(kev);
    return inexact->isIntegerValued();
  }
  
  return false;
}

bool LibRnrsBase::IsEqualTo::func(KevesValue num1, KevesValue num2) {
  if (num1.IsFixnum())
    return num2.IsFixnum() && num1 == num2;

  if (num1.Is<RationalNumberKev>()) {
    const RationalNumberKev* real1(num1);

    if (num2.Is<RationalNumberKev>()) {
      const RationalNumberKev* real2(num2);
      return real1->isEqualTo(*real2);
    }
    
  } else if (num1.Is<FlonumKev>()) {
    const FlonumKev* real1(num1);

    if (num2.Is<FlonumKev>()) {
      const FlonumKev* real2(num2);
      return real1->isEqualTo(*real2);
    }

  } else if (num1.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* comple1(num1);

    if (num2.Is<ExactComplexNumberKev>()) {
      const ExactComplexNumberKev* complex2(num2);

      return comple1->real().isEqualTo(complex2->real()) &&
	comple1->imag().isEqualTo(complex2->imag());
    }

  } else if (num1.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* comple1(num1);

    if (num2.Is<InexactComplexNumberKev>()) {
      const InexactComplexNumberKev* complex2(num2);
      return comple1->isEqualTo(*complex2);
    }

  }

  return false;
}

bool LibRnrsBase::IsFinite::func(KevesValue kev) {
  if (kev.IsRationalNumber())
    return true;

  if (kev.Is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isFinite();
  }

  return false;
}

bool LibRnrsBase::IsInfinite::func(KevesValue kev) {
  if (kev.Is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isInfinity();
  }

  return false;
}

bool LibRnrsBase::IsNaN::func(KevesValue kev) {
  if (kev.Is<FlonumKev>()) {
    const FlonumKev* num(kev);
    return num->isNaN();
  }

  return false;
}

bool LibRnrsBase::IsProcedure::func(KevesValue kev) {
  return kev.IsProcedure();
}

KevesValue LibRnrsBase::IsProcedure::message(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqProc);
}

KevesValue LibRnrsBase::IsProcedure::message1(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqProcAs1st);
}

KevesValue LibRnrsBase::IsProcedure::message2(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqProcAs2nd);
}

KevesValue LibRnrsBase::IsProcedure::message3(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqProcAs3rd);
}

KevesValue LibRnrsBase::Max::func(KevesGC* gc, KevesValue num1, KevesValue num2) {
  return NumberKev::max(gc, num1, num2);
}

KevesValue LibRnrsBase::Min::func(KevesGC* gc, KevesValue num1, KevesValue num2) {
  return NumberKev::min(gc, num1, num2);
}

void LibRnrsBase::Add::func0(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = KevesFixnum(0);
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Add::func1(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (last.IsNumber()) {
    vm->acc_ = last;
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Add::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  fx_int sum(augend + addend);
  KevesFixnum fixnum(sum);

  if (fixnum == sum) {
    vm->acc_ = fixnum;
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->acc_ = RationalNumberKev::makeFromInt(vm->gc(), sum);
  return KevesVM::ReturnValue(vm, pc);
}
  
void LibRnrsBase::Add::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  RationalNumberKev sum(augend->add(vm->gc(), addend));

  vm->CheckStack(&sum, &funcRationalFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Add::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  FlonumKev sum(augend->add(addend));

  vm->CheckStack(&sum, &funcFlonumFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Add::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  ExactComplexNumberKev sum(augend->add(vm->gc(), addend));

  vm->CheckStack(&sum, &funcExactComplexFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Add::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* augend(vm->gr1_);
  KevesFixnum addend(vm->acc_);
  InexactComplexNumberKev sum(augend->add(addend));

  vm->CheckStack(&sum, &funcInexactComplexFixnum, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Add::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum augend(vm->gr1_);
  const RationalNumberKev* addend(vm->acc_);
  RationalNumberKev sum(addend->add(vm->gc(), augend));

  vm->CheckStack(&sum, &funcFixnumRational, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&sum, &funcFixnumFlonum, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&sum, &funcFixnumExactComplex, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&sum, &funcFixnumInexactComplex, pc);

  vm->acc_ = &sum;
  return KevesVM::ReturnValue(vm, pc);
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
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Multiply::func1(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (last.IsNumber()) {
    vm->acc_ = last;
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  fx_long product(multiplicand * multiplier);
  KevesFixnum fixnum(static_cast<fx_int>(product));

  if (fixnum != product) {
    RationalNumberKevFromLong rational(product);
    
    vm->CheckStack(&product, &funcFixnumFixnum, pc);
    
    vm->acc_ = &rational;
    return KevesVM::ReturnValue(vm, pc);
  }
  
  vm->acc_ = fixnum;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  RationalNumberKev product(multiplicand->multiply(vm->gc(), multiplier));

  vm->CheckStack(&product, &funcRationalFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  FlonumKev product(multiplicand->multiply(multiplier));

  vm->CheckStack(&product, &funcFlonumFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  ExactComplexNumberKev product(multiplicand->multiply(vm->gc(), multiplier));

  vm->CheckStack(&product, &funcExactComplexFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* multiplicand(vm->gr1_);
  KevesFixnum multiplier(vm->acc_);
  InexactComplexNumberKev product(multiplicand->multiply(multiplier));

  vm->CheckStack(&product, &funcInexactComplexFixnum, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Multiply::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum multiplicand(vm->gr1_);
  const RationalNumberKev* multiplier(vm->acc_);
  RationalNumberKev product(multiplier->multiply(vm->gc(), multiplicand));

  vm->CheckStack(&product, &funcFixnumRational, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&product, &funcFixnumFlonum, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&product, &funcFixnumExactComplex, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&product, &funcFixnumInexactComplex, pc);

  vm->acc_ = &product;
  return KevesVM::ReturnValue(vm, pc);
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
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req1OrMoreGot0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Subtract::func1(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.IsFixnum())
    return func2(vm, pc);
  
  KevesFixnum num(last);
  fx_int inverse(-static_cast<fx_int>(num));
  
  if (KevesFixnum::isInRange(inverse)) {
    KevesFixnum fixnum(inverse);
    vm->acc_ = fixnum;
    return KevesVM::ReturnValue(vm, pc);
  }
  
  RationalNumberKevFromInt rational(inverse);

  vm->CheckStack(&rational, &func1, pc);

  vm->acc_ = &rational;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::func2(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.Is<RationalNumberKev>())
    return func3(vm, pc);
  
  const RationalNumberKev* num(last);
  RationalNumberKev inverse(num->inverse());

  vm->CheckStack(&inverse, &func2, pc);

  vm->acc_ = inverse.optimize();
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::func3(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.Is<FlonumKev>())
    return func4(vm, pc);
  
  const FlonumKev* num(last);
  FlonumKev inverse(num->inverse());

  vm->CheckStack(&inverse, &func3, pc);

  vm->acc_ = &inverse;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::func4(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.Is<ExactComplexNumberKev>())
    return func5(vm, pc);
  
  const ExactComplexNumberKev* num(last);
  ExactComplexNumberKev inverse(num->inverse());

  vm->CheckStack(&inverse, &func4, pc);

  vm->acc_ = &inverse;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::func5(KevesVM* vm, const_KevesIterator pc) {
  KevesValue last(vm->registers_.lastArgument());

  if (!last.Is<InexactComplexNumberKev>())
    return func6(vm, pc);
  
  const InexactComplexNumberKev* num(last);
  InexactComplexNumberKev inverse(num->inverse());

  vm->CheckStack(&inverse, &func5, pc);

  vm->acc_ = &inverse;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::func6(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  fx_int difference(minuend - subtrahend);
  KevesFixnum fixnum(difference);

  if (fixnum != difference) {
    RationalNumberKevFromInt rational(difference);
    
    vm->CheckStack(&rational, &funcFixnumFixnum, pc);
    
    vm->acc_ = &rational;
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->acc_ = fixnum;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  RationalNumberKev difference(minuend->subtract(vm->gc(), subtrahend));

  vm->CheckStack(&difference, &funcRationalFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  FlonumKev difference(minuend->subtract(subtrahend));

  vm->CheckStack(&difference, &funcFlonumFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcExactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const ExactComplexNumberKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  ExactComplexNumberKev difference(minuend->subtract(vm->gc(), subtrahend));

  vm->CheckStack(&difference, &funcExactComplexFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcInexactComplexFixnum(KevesVM* vm, const_KevesIterator pc) {
  const InexactComplexNumberKev* minuend(vm->gr1_);
  KevesFixnum subtrahend(vm->acc_);
  InexactComplexNumberKev difference(minuend->subtract(subtrahend));

  vm->CheckStack(&difference, &funcInexactComplexFixnum, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Subtract::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum minuend(vm->gr1_);
  const RationalNumberKev* subtrahend(vm->acc_);
  vm->acc_ = NumberKev::difference(vm->gc(), minuend, *subtrahend);
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&difference, &funcFixnumFlonum, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&difference, &funcFixnumExactComplex, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
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

  vm->CheckStack(&difference, &funcFixnumInexactComplex, pc);

  vm->acc_ = &difference;
  return KevesVM::ReturnValue(vm, pc);
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
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req1OrMoreGot0);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Divide::func1(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  KevesValue last(registers->lastArgument());

  if (last.IsFixnum()) {
    fx_int numerator(1);
    fx_int denominator(KevesFixnum { last } );

    if (denominator == 0) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Div0);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }

    if (denominator == 1 || denominator == -1) {
      vm->acc_ = KevesFixnum(denominator);
      return KevesVM::ReturnValue(vm, pc);
    }

    RationalNumberKevFromInt quotient(numerator, denominator);
    vm->acc_ = &quotient;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (last.Is<RationalNumberKev>()) {
    const RationalNumberKev* num(last);
    RationalNumberKev reciprocal(num->reciprocal());
    vm->acc_ = reciprocal.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (last.Is<FlonumKev>()) {
    const FlonumKev* divisor(last);
    FlonumKev quotient(divisor->reciprocal());
    vm->acc_ = &quotient;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (last.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* divisor(last);
    ExactComplexNumberKev reciprocal(divisor->reciprocal(vm->gc()));
    vm->acc_ = &reciprocal;
    return KevesVM::returnExactComplexNumberKev(vm, pc);
  }

  if (last.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* divisor(last);
    InexactComplexNumberKev quotient(divisor->reciprocal());
    vm->acc_ = &quotient;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumFixnum(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  RationalNumberKevFromInt quotient(dividend, divisor);
  vm->acc_ = quotient.optimize();
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcRationalFixnum(KevesVM* vm, const_KevesIterator pc) {
  const RationalNumberKev* dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  RationalNumberKev quotient(dividend->divide(vm->gc(), divisor));
  vm->acc_ = quotient.optimize();
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcFlonumFixnum(KevesVM* vm, const_KevesIterator pc) {
  const FlonumKev* dividend(vm->gr1_);
  KevesFixnum divisor(vm->acc_);
  FlonumKev quotient(dividend->divide(divisor));
  vm->acc_ = &quotient;
  return KevesVM::ReturnValueSafe(vm, pc);
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
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Divide::funcFixnumRational(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum dividend(vm->gr1_);
  const RationalNumberKev* divisor(vm->acc_);
  RationalNumberKev quotient(NumberKev::quotient(vm->gc(), dividend, *divisor));
  vm->acc_ = quotient.optimize();
  return KevesVM::ReturnValueSafe(vm, pc);
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
  return KevesVM::ReturnValueSafe(vm, pc);
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
  return KevesVM::ReturnValueSafe(vm, pc);
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
  if (vm->acc_.IsFixnum()) {
    KevesFixnum num(vm->acc_);
    vm->acc_ = KevesFixnum(abs(num));
    return KevesVM::ReturnValue(vm, pc);
  }

  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* num(vm->acc_);
    RationalNumberKev abs_num(num->abs());
    vm->acc_ = &abs_num;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* org(vm->acc_);
    FlonumKev abs_num(org->abs());
    vm->acc_ = &abs_num;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  vm->acc_ = EMB_UNDEF;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Numerator::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    const Bignum* numerator(rational->numerator());
    BignumInt one(1);
    RationalNumberKev num(rational->isNegative(), numerator, &one);
    vm->acc_ = num.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* num(vm->acc_);

    if (num->isNaN() || num->isInfinity()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    if (num->isZero()) {
      vm->acc_ = KevesFixnum(0);
      return KevesVM::ReturnValue(vm, pc);
    }
    
    RationalNumberKev rational(num->toExact(vm->gc()));
    FlonumKev flonum(rational.makeMPQ().get_num().get_d());
    vm->acc_ = &flonum;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Denominator::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum()) {
    vm->acc_ = KevesFixnum(1);
    return KevesVM::ReturnValue(vm, pc);
  }

  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    const Bignum* denominator(rational->denominator());
    BignumInt one(1);
    RationalNumberKev den(false, denominator, &one);
    vm->acc_ = den.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* num(vm->acc_);
    
    if (num->isNaN() || num->isInfinity()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    if (num->isZero()) {
      vm->acc_ = KevesFixnum(1);
      return KevesVM::ReturnValue(vm, pc);
    }
    
    RationalNumberKev rational(num->toExact(vm->gc()));
    FlonumKev flonum(rational.makeMPQ().get_den().get_d());
    vm->acc_ = &flonum;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Floor::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev floor_num(rational->floor(vm->gc()));
    vm->acc_ = floor_num.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev floor_num(flonum->floor());
    vm->acc_ = &floor_num;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Ceiling::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev ceiling_num(rational->ceiling(vm->gc()));
    vm->acc_ = ceiling_num.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev ceiling_num(flonum->ceiling());
    vm->acc_ = &ceiling_num;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Truncate::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev truncate_num(rational->truncate(vm->gc()));
    vm->acc_ = truncate_num.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev truncate_num(flonum->truncate());
    vm->acc_ = &truncate_num;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Round::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsFixnum())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.Is<RationalNumberKev>()) {
    const RationalNumberKev* rational(vm->acc_);
    RationalNumberKev round_num(rational->round(vm->gc()));
    vm->acc_ = round_num.optimize();
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    const FlonumKev* flonum(vm->acc_);
    FlonumKev round_num(flonum->round());
    vm->acc_ = &round_num;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::MakeRectangular::func(KevesVM* vm, const_KevesIterator pc) {
  return KevesVM::MakeRectangular(vm, pc);
}

void LibRnrsBase::MakePolar::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev mag(NumberKev::makeFlonum(vm->acc_));
  FlonumKev ang(NumberKev::makeFlonum(vm->gr1_));
  FlonumKev real_part(mag.multiply(ang.cos()));
  FlonumKev imag_part(mag.multiply(ang.sin()));
  vm->acc_ = &real_part;
  vm->gr1_ = &imag_part;
  return KevesVM::MakeRectangular(vm, pc);
}

void LibRnrsBase::RealPart::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsRationalNumber() || vm->acc_.Is<FlonumKev>())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* complex(vm->acc_);
    RationalNumberKev real(complex->real());
    vm->acc_ = &real;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  if (vm->acc_.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* complex(vm->acc_);
    FlonumKev real(complex->real());
    vm->acc_ = &real;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::ImagPart::func(KevesVM* vm, const_KevesIterator pc) {
  if (vm->acc_.IsRationalNumber()) {
    vm->acc_ = KevesFixnum(0);
    return KevesVM::ReturnValue(vm, pc);
  }

  if (vm->acc_.Is<FlonumKev>()) {
    FlonumKev nan(std::numeric_limits<double>::quiet_NaN());
    vm->acc_ = &nan;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<ExactComplexNumberKev>()) {
    const ExactComplexNumberKev* complex(vm->acc_);
    RationalNumberKev imag(complex->imag());
    vm->acc_ = &imag;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (vm->acc_.Is<InexactComplexNumberKev>()) {
    const InexactComplexNumberKev* complex(vm->acc_);
    FlonumKev imag(complex->imag());
    vm->acc_ = &imag;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Magnitude::func(KevesVM* vm, const_KevesIterator pc) {
  if (!vm->acc_.IsNumber())
    return KevesVM::ReturnValue(vm, pc);
  
  if (vm->acc_.IsRealNumber())
    return Abs::func(vm, pc);
  
  auto mangitude = [&](const FlonumKev& real_part, const FlonumKev& imag_part) -> void {
    FlonumKev real_part2(real_part.multiply(real_part));
    FlonumKev imag_part2(imag_part.multiply(imag_part));
    FlonumKev mag(real_part2.add(imag_part2).sqrt());
    vm->acc_ = &mag;
    return KevesVM::ReturnValueSafe(vm, pc);
  };

  if (vm->acc_.Is<ExactComplexNumberKev>()) {
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
  if (vm->acc_.IsRealNumber()) {
    FlonumKev ang(NumberKev::isNegative(vm->acc_) ? acos(-1.0) : 0.0);
    vm->acc_ = &ang;
    return KevesVM::ReturnValueSafe(vm, pc);
  }
  
  auto angle = [&](const FlonumKev& real_part, const FlonumKev& imag_part) {
    if (real_part.isZero()) {
      FlonumKev r(imag_part.isNegative() ? -acos(0.0) : acos(0.0));
      vm->acc_ = &r;
      return KevesVM::ReturnValueSafe(vm, pc);
    }
    
    if (real_part.isNegative()) {
      FlonumKev r(imag_part.divide(real_part).atan());
      FlonumKev pi(acos(-1.0));
      FlonumKev r2(r.isNegative() ? r.add(pi) : r.subtract(pi));
      vm->acc_ = &r2;
      return KevesVM::ReturnValueSafe(vm, pc);
    }
    
    FlonumKev r(imag_part.divide(real_part).atan());
    vm->acc_ = &r;
    return KevesVM::ReturnValueSafe(vm, pc);
  };

  if (vm->acc_.Is<ExactComplexNumberKev>()) {
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
    if (!registers->argument(3).IsFixnum())
      return procNumberToString_err1(vm, pc);
    
    int arg3(KevesFixnum(registers->argument(3)));
    
    if (arg3 < 1)
      return procNumberToString_err1(vm, pc);
    
    precision = arg3;
  }
    
  case 3: {
    if (!registers->argument(2).IsFixnum())
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
    if (registers->argument(1).IsNumber()) {
      KevesValue z(registers->argument(1));
      vm->acc_ = NumberKev::makeStringFromNumber(vm->gc(),
						 z,
						 radix,
						 precision);
      return KevesVM::ReturnValueSafe(vm, pc);
    }
    
    return procNumberToString_err2(vm, pc);

  default:
    return procNumberToString_err3(vm, pc);
  }
}

void LibRnrsBase::procNumberToString_err1(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqIntNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::procNumberToString_err2(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqNum);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::procNumberToString_err3(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req2GotMore);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::procStringToNumber(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int radix(0);

  switch (argn) {

  case 3:
    if (!registers->lastArgument().IsFixnum()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req2GotMore);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    radix = KevesFixnum(registers->lastArgument());
    
  case 2: {
    if (!registers->argument(1).IsString()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqStr);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    const StringKev* str(registers->argument(1));
    vm->acc_ = str;
    vm->gr1_ = KevesFixnum(radix);
    return procStringToNumber_helper1(vm, pc);
  }
    
  default:
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(argn > 3 ?
				       KevesBuiltinValues::mesg_Req1Or2GotMore :
				       KevesBuiltinValues::mesg_Req1Or2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
}
  
void LibRnrsBase::procStringToNumber_helper1(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  int radix(KevesFixnum(vm->gr1_));
  bool exact(true);

  int pos(NumberKev::getPrefix(*str, &radix, &exact));

  if (pos < 0) {
    vm->acc_ = EMB_FALSE;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  KevesValue number1;
  StringKev str1(str->Right(str->size() - pos));
  int pos1(0);

  if (str1.Equals("+i")) {
    number1 = KevesFixnum(1);
    pos1 = 1;
  } else if (str1.Equals("-i")) {
    number1 = KevesFixnum(-1);
    pos1 = 1;
  } else if (radix == 2 || radix == 8 || radix == 16) {
    number1 = NumberKev::makeRealNumberFromBinary(vm->gc(), str1, &pos1, radix);
  } else if (radix == 0 || radix == 10) {
    number1 = NumberKev::makeRealNumberFromDecimal(vm->gc(), str1, &pos1, &radix, &exact);
  } else {
    vm->acc_ = EMB_FALSE;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  if (!number1.IsNumber()) {
    vm->acc_ = EMB_FALSE;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  // no imaginary part
  if (pos1 == str1.size()) {
    if (exact) {
      vm->acc_ = number1;
      return KevesVM::ReturnValueSafe(vm, pc);
    }
    
    FlonumKev flonum(NumberKev::makeFlonum(number1));
    vm->acc_ = &flonum;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  // no real part
  if(pos1 == str1.size() - 1 && str1.At(pos1) == QChar('i')) {
    if (str1.At(0) == QChar('+') || str1.At(0) == QChar('-')) {
      if (exact) {
	vm->acc_ = KevesFixnum(0);
	vm->gr1_ = number1;
	return KevesVM::MakeRectangular(vm, pc);
      }
      
      FlonumKev flonum(NumberKev::makeFlonum(number1));
      vm->acc_ = KevesFixnum(0);
      vm->gr1_ = &flonum;
      return KevesVM::MakeRectangular(vm, pc);
    }
    
    vm->acc_ = EMB_FALSE;
    return KevesVM::ReturnValueSafe(vm, pc);
  }

  KevesValue number2;
  QChar chr(str1.At(pos1));

  if (chr == QChar('+') || chr == QChar('-')) {
    const StringKev str2(str1.Right(str1.size() - pos1));
    int pos2(0);
    if (radix == 2 || radix == 8 || radix == 16) {
      number2 = NumberKev::makeRealNumberFromBinary(vm->gc(), str2, &pos2, radix);
    } else if (radix == 0 || radix == 10) {
      number2 = NumberKev::makeRealNumberFromDecimal(vm->gc(), str2, &pos2, &radix, &exact);
    }

    if (!number2.IsNumber()) {
      vm->acc_ = EMB_FALSE;
      return KevesVM::ReturnValueSafe(vm, pc);
    }
    
    if (pos2 == str2.size() - 1 && str2.At(pos2) == QChar('i')) {
      if (exact) {
	vm->acc_ = number1;
	vm->gr1_ = number2;
	return KevesVM::MakeRectangular(vm, pc);
      }
      
      FlonumKev flonum1(NumberKev::makeFlonum(number1));
      FlonumKev flonum2(NumberKev::makeFlonum(number2));
      vm->acc_ = &flonum1;
      vm->gr1_ = &flonum2;
      return KevesVM::MakeRectangular(vm, pc);
    }
  }

  if (chr == QChar('@')) {
    const StringKev str2(str1.Right(str1.size() - pos1 - 1));
    int pos2(0);

    if (radix == 2 || radix == 8 || radix == 16) {
      number2 = NumberKev::makeFromBinary(vm->gc(), str2, radix, true);
    } else if (radix == 0 || radix == 10) {
      number2 = NumberKev::makeRealNumberFromDecimal(vm->gc(), str2, &pos2, &radix, &exact);
    }
    
    if (number2.IsNumber() && pos2 == str2.size()) {
      if (number1.IsRealNumber() && number2.IsRealNumber()) {
	FlonumKev mag(NumberKev::makeFlonum(number1));
	FlonumKev ang(NumberKev::makeFlonum(number2));
	FlonumKev real_part(mag.multiply(ang.cos()));
	FlonumKev imag_part(mag.multiply(ang.sin()));
	vm->acc_ = &real_part;
	vm->gr1_ = &imag_part;
	return KevesVM::MakeRectangular(vm, pc);
      }
    }
  }
  
  vm->acc_ = EMB_FALSE;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flexp::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.exp());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Fllog::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.log());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flsin::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.sin());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flcos::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.cos());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Fltan::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.tan());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flasin::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.asin());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flacos::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.acos());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flatan::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));
  FlonumKev r(flonum.atan());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::Flsqrt::func(KevesVM* vm, const_KevesIterator pc) {
  FlonumKev flonum(NumberKev::makeFlonum(vm->acc_));

  if (flonum.isNegative()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  FlonumKev r(flonum.sqrt());
  vm->acc_ = &r;
  return KevesVM::ReturnValueSafe(vm, pc);
}

void LibRnrsBase::procFlexpt(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  if (registers->argn() == 3) {
    KevesValue x1(registers->lastArgument(1));
    KevesValue x2(registers->lastArgument());

    if (x1.IsRealNumber() && x2.IsRealNumber()) {
      FlonumKev fl_x1(NumberKev::makeFlonum(x1));
      FlonumKev fl_x2(NumberKev::makeFlonum(x2));
      FlonumKev r(fl_x1.expt(fl_x2));
      vm->acc_ = &r;
      return KevesVM::ReturnValueSafe(vm, pc);
    }

    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqRealNum);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  if (registers->argn() == 1) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  if (registers->argn() == 2) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req2Got1);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_Req2GotMore);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

bool LibRnrsBase::IsBoolean::func(KevesValue kev) {
  return kev.IsBool();
}

void LibRnrsBase::Not::func(KevesVM* vm, const_KevesIterator pc) {
  vm->acc_ = vm->acc_ == EMB_FALSE ? EMB_TRUE : EMB_FALSE;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Cons::func(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(vm->acc_, vm->gr1_);

  vm->CheckStack(&pair, &func, pc);

  vm->acc_ = &pair;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Car::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  vm->acc_ = pair->car();
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Cdr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  vm->acc_ = pair->cdr();
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Cadr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.IsPair()) {
    const PairKev* pair(cdr);
    vm->acc_ = pair->car();
    return KevesVM::ReturnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cddr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.IsPair()) {
    const PairKev* pair(cdr);
    vm->acc_ = pair->cdr();
    return KevesVM::ReturnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caaar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.IsPair()) {
    const PairKev* pair(car);
    KevesValue caar(pair->car());
    if (caar.IsPair()) {
      const PairKev* pair(caar);
      vm->acc_ = pair->car();
      return KevesVM::ReturnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caadr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.IsPair()) {
    const PairKev* pair(cdr);
    KevesValue cadr(pair->car());
    if (cadr.IsPair()) {
      const PairKev* pair(cadr);
      vm->acc_ = pair->car();
      return KevesVM::ReturnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cadar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.IsPair()) {
    const PairKev* pair(car);
    KevesValue cdar(pair->cdr());
    if (cdar.IsPair()) {
      const PairKev* pair(cdar);
      vm->acc_ = pair->car();
      return KevesVM::ReturnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caddr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.IsPair()) {
    const PairKev* pair(cdr);
    KevesValue cddr(pair->cdr());
    if (cddr.IsPair()) {
      const PairKev* pair(cddr);
      vm->acc_ = pair->car();
      return KevesVM::ReturnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Caar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.IsPair()) {
    const PairKev* pair(car);
    vm->acc_ = pair->car();
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.IsPair()) {
    const PairKev* pair(car);
    vm->acc_ = pair->cdr();
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdaar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.IsPair()) {
    const PairKev* pair(car);
    KevesValue caar(pair->car());
    if (caar.IsPair()) {
      const PairKev* pair(caar);
      vm->acc_ = pair->cdr();
      return KevesVM::ReturnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdadr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.IsPair()) {
    const PairKev* pair(cdr);
    KevesValue cadr(pair->car());
    if (cadr.IsPair()) {
      const PairKev* pair(cadr);
      vm->acc_ = pair->cdr();
      return KevesVM::ReturnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cddar::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue car(pair->car());
  if (car.IsPair()) {
    const PairKev* pair(car);
    KevesValue cdar(pair->cdr());
    if (cdar.IsPair()) {
      const PairKev* pair(cdar);
      vm->acc_ = pair->cdr();
      return KevesVM::ReturnValue(vm, pc);
    }
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Cdddr::func(KevesVM* vm, const_KevesIterator pc) {
  const PairKev* pair(vm->acc_);
  KevesValue cdr(pair->cdr());
  if (cdr.IsPair()) {
    const PairKev* pair(cdr);
    KevesValue cddr(pair->cdr());
    if (cddr.IsPair()) {
      const PairKev* pair(cddr);
      vm->acc_ = pair->cdr();
      return KevesVM::ReturnValue(vm, pc);
    }
  }

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqPair);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

bool LibRnrsBase::IsNull::func(KevesValue kev) {
  return kev == EMB_NULL;
}

void LibRnrsBase::procList(KevesVM* vm, const_KevesIterator pc) {
  int argn(vm->registers_.argn());
  vm->acc_ = EMB_NULL;
  return (argn > 1 ? procList_helper : KevesVM::ReturnValue)(vm, pc);
}

void LibRnrsBase::procList_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  PairKev pair(registers->lastArgument(), vm->acc_);
  
  vm->CheckStack(&pair, &procList_helper, pc);
  
  vm->acc_ = &pair;
  
  if (registers->argn() > 2) {
    registers->popArgument();
    return procList_helper(vm, pc);
  }
  
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::Length::func(KevesVM* vm, const_KevesIterator pc) {
  int length(KevesVM::GetListLength(vm->acc_));

  if (length >= 0) {
    vm->acc_ = KevesFixnum(length);
    return KevesVM::ReturnValue(vm, pc);
  }
  
  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqProperList);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::Reverse::func(KevesVM* vm, const_KevesIterator pc) {
  vm->gr1_ = vm->acc_;
  vm->acc_ = EMB_NULL;
  return func_helper(vm, pc);
}

void LibRnrsBase::Reverse::func_helper(KevesVM* vm, const_KevesIterator pc) {
  if (vm->gr1_.IsPair()) {
    const PairKev* last_pair(vm->gr1_);
    PairKev new_pair(last_pair->car(), vm->acc_);
    
    vm->CheckStack(&new_pair, &func_helper, pc);
    
    vm->acc_ = &new_pair;
    vm->gr1_ = last_pair->cdr();
    return func_helper(vm, pc);
  }

  if (vm->gr1_ == EMB_NULL)
    return KevesVM::ReturnValue(vm, pc);

  vm->acc_ = vm->gr2_;
  vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqProperList);
  vm->gr2_ = EMB_NULL;
  return KevesVM::RaiseAssertCondition(vm, pc);
}

bool LibRnrsBase::IsSymbolEqual::func(KevesValue kev1, KevesValue kev2) {
  const SymbolKev* sym1(kev1);
  const SymbolKev* sym2(kev2);
  return sym1->Equals(*sym2);
}

void LibRnrsBase::SymbolToString::func(KevesVM* vm, const_KevesIterator pc) {
  const SymbolKev* sym(vm->acc_);
  StringKev str(sym->GetKevesString());

  vm->CheckStack(&str, &func, pc);

  vm->acc_ = &str;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::StringToSymbol::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  SymbolKev sym(*str);

  vm->CheckStack(&sym, &func, pc);

  vm->acc_ = &sym;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::CharToInteger::func(KevesVM* vm, const_KevesIterator pc) {
  KevesChar chr(vm->acc_);
  vm->acc_ = KevesFixnum(static_cast<fx_int>(chr.Unicode()));
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::IntegerToChar::func(KevesVM* vm, const_KevesIterator pc) {
  KevesFixnum fixnum(vm->acc_);
  QChar chr(fixnum);
  vm->gr1_ = vm->acc_;
  vm->acc_ = KevesChar(chr);
  return KevesVM::ReturnValue(vm, pc);
}

bool LibRnrsBase::IsCharEqual::func(KevesValue kev1, KevesValue kev2) {
  return kev1 == kev2;
}

bool LibRnrsBase::IsCharLT::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.Compare(chr2) < 0;
}

bool LibRnrsBase::IsCharGT::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.Compare(chr2) > 0;
}

bool LibRnrsBase::IsCharLTE::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.Compare(chr2) <= 0;
}

bool LibRnrsBase::IsCharGTE::func(KevesValue kev1, KevesValue kev2) {
  KevesChar chr1(kev1);
  KevesChar chr2(kev2);
  return chr1.Compare(chr2) >= 0;
}

void LibRnrsBase::procMakeString(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  QChar chr(' ');

  switch (argn) {
  case 3:
    if (!registers->lastArgument().IsChar()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqChar);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    chr = KevesChar(registers->lastArgument());
    
  case 2:
    {
      KevesValue first(registers->argument(1));
      
      if (!first.IsFixnum()) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqIntNum);
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }
      
      int size(KevesFixnum { first } );

      if (size < 0) {
	vm->acc_ = vm->gr2_;
	vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_OutOfRange);
	vm->gr2_ = EMB_NULL;
	return KevesVM::RaiseAssertCondition(vm, pc);
      }
      
      vm->acc_ = vm->MakeString(size, chr);
      return KevesVM::ReturnValueSafe(vm, pc);
    }

  default:
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(registers->argn() > 3 ?
				       KevesBuiltinValues::mesg_Req1Or2GotMore :
				       KevesBuiltinValues::mesg_Req1Or2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
}

void LibRnrsBase::procString(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int size(argn - 1);
  StringKev* string(vm->MakeString(size));
  QChar* iter(string->data());

  for (int idx(1); idx < argn; ++idx, ++iter) {
    KevesValue current(registers->argument(idx));

    if (!current.IsChar()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqChar);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }

    KevesChar chr(current);
    *iter = chr;
  }

  vm->acc_ = string;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::StringLength::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* str(vm->acc_);
  vm->acc_ = KevesFixnum(str->size());
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::StringRef::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* string(vm->acc_);
  int k(KevesFixnum(vm->gr1_));

  if (k < 0 || k >= string->size()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  vm->acc_ = KevesChar(string->At(k));
  return KevesVM::ReturnValue(vm, pc);
}

bool LibRnrsBase::IsStringEqual::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->Equals(*str2);
}

bool LibRnrsBase::IsStringLT::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->IsLT(*str2);
}

bool LibRnrsBase::IsStringGT::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->IsGT(*str2);
}

bool LibRnrsBase::IsStringLTE::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->IsLTE(*str2);
}

bool LibRnrsBase::IsStringGTE::func(KevesValue kev1, KevesValue kev2) {
  const StringKev* str1(kev1);
  const StringKev* str2(kev2);
  return str1->IsGTE(*str2);
}

void LibRnrsBase::Substring::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  const StringKev* str(registers->argument(1));
  int start(KevesFixnum(registers->argument(2)));
  int end(KevesFixnum(registers->argument(3)));
  int len(str->size());
    
  if (start < 0 || start > end || end > len) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  // vm->acc_ = vm->keves_gc_.make(StringKev::MakeSubstring(str, start, end));
  vm->acc_ = StringKev::MakeSubstring(vm->gc(), str, start, end);

  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::StringToList::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* string(vm->acc_);
  int last_index(KevesFixnum(string->size() - 1));

  if (last_index < 0) {
    vm->acc_ = EMB_NULL;
    return KevesVM::ReturnValue(vm, pc);
  }
  
  vm->gr1_ = EMB_NULL;
  vm->gr2_ = KevesFixnum(last_index);
  return func_helper(vm, pc);
}

void LibRnrsBase::StringToList::func_helper(KevesVM* vm, const_KevesIterator pc) {
  PairKev pair(EMB_NULL, EMB_NULL);

  vm->CheckStack(&pair, &func_helper, pc);

  int index(KevesFixnum(vm->gr2_));
  const StringKev* string(vm->acc_);
  pair.Set(KevesChar(string->At(index)), vm->gr1_);

  if (index == 0) {
    vm->acc_ = &pair;
    return KevesVM::ReturnValue(vm, pc);
  }
  
  vm->gr1_ = &pair;
  vm->gr2_ = KevesFixnum(index - 1);
  return func_helper(vm, pc);
}

void LibRnrsBase::StringCopy::func(KevesVM* vm, const_KevesIterator pc) {
  const StringKev* original(vm->acc_);
  StringKev copy(original->Copy(vm->gc()));

  vm->CheckStack(&copy, &func, pc);

  vm->acc_ = &copy;
  return KevesVM::ReturnValue(vm, pc);
}

bool LibRnrsBase::IsVector::func(KevesValue kev) {
  return kev.IsVector();
}

KevesValue LibRnrsBase::IsVector::message(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqVector);
}

KevesValue LibRnrsBase::IsVector::message1(KevesBase* base) {
  return base->GetMesgText(KevesBuiltinValues::mesg_ReqVectorAs1st);
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
    
    if (!first.IsFixnum()) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqIntNum);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    int size(KevesFixnum { first } );
    
    if (size < 0) {
      vm->acc_ = vm->gr2_;
      vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_OutOfRange);
      vm->gr2_ = EMB_NULL;
      return KevesVM::RaiseAssertCondition(vm, pc);
    }
    
    vm->acc_ = vm->MakeVector(size, obj);
    return KevesVM::ReturnValue(vm, pc);
  }
    
  default:
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(registers->argn() > 3 ?
				       KevesBuiltinValues::mesg_Req1Or2GotMore :
				       KevesBuiltinValues::mesg_Req1Or2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
}

void LibRnrsBase::procVector(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  int argn(registers->argn());
  int size(argn - 1);
  VectorKev* vector(vm->MakeVector(size));
  KevesIterator iter(vector->begin());

  for (int idx(1); idx < argn; ++idx, ++iter) {
    *iter = registers->argument(idx);
  }

  vm->acc_ = vector;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::VectorLength::func(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  vm->acc_ = KevesFixnum(vector->size());
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::VectorRef::func(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  int k(KevesFixnum(vm->gr1_));

  if (k < 0 || k >= vector->size()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  vm->acc_ = vector->At(k);
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::VectorSetE::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  VectorKev* vector(MutableKevesValue(registers->argument(1)));
  int k(KevesFixnum(registers->argument(2)));

  if (k < 0 || k >= vector->size()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_OutOfRange);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  KevesIterator iter(vector->begin());
  *(iter + k) = registers->argument(3);
  // if (vm->keves_gc_.isInTenuredWithoutMark(vector)) {
  //   vm->keves_gc_.pushToMarkedList(vector);
  // }
  vm->gc()->MarkLive(vector);
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::VectorToList::func(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  int last_index(KevesFixnum(vector->size() - 1));

  if (last_index < 0) {
    vm->acc_ = EMB_NULL;
    return KevesVM::ReturnValue(vm, pc);
  }

  vm->gr1_ = EMB_NULL;
  vm->gr2_ = KevesFixnum(last_index);
  return func_helper(vm, pc);
}

void LibRnrsBase::VectorToList::func_helper(KevesVM* vm, const_KevesIterator pc) {
  const VectorKev* vector(vm->acc_);
  int index(KevesFixnum(vm->gr2_));
  PairKev pair(vector->At(index), vm->gr1_);
  
  vm->CheckStack(&pair, &func_helper, pc);
  
  if (index == 0) {
    vm->acc_ = &pair;
    return KevesVM::ReturnValue(vm, pc);
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
  int size(KevesVM::GetListLength(temp));
  
  if (size < 0) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqProperList);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  VectorKev* vector(vm->MakeVector(size));
  
  for (KevesValue& elem : *vector) {
    const PairKev* pair(temp);
    elem = pair->car();
    temp = pair->cdr();
  }
  
  vm->acc_ = vector;
  return KevesVM::ReturnValue(vm, pc);
}

void LibRnrsBase::CallWithCC::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;
  
  vm->CheckStack(&arg_frame, &func, pc);
  
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
  
  vm->CheckStack(&arg_frame1, &func, pc);
  
  pc = registers->unwind();
  
  // wind and push *proc2*
  StackFrameKev stack_frame1;
  registers->wind(pc, &stack_frame1, &arg_frame1);
  pc = vm->base()->builtin()->code_APPLY_MULT_APPLY();
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

  return KevesVM::ReturnValue(vm, pc);

}

void LibRnrsBase::DynamicWind::func(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  ArgumentFrameKevWithArray<04> arg_frame;
  
  vm->CheckStack(&arg_frame, &func, pc);
  
  KevesValue before(registers->argument(1));
  pc = vm->base()->builtin()->code_SET_DYNAMIC_WIND();
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
    vm->gr1_ = vm->base()->GetMesgText(registers->argn() == 2 ?
				       KevesBuiltinValues::mesg_Req2Got1 :
				       KevesBuiltinValues::mesg_Req2Got0);
    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }
  
  return procAssertionViolation_helper(vm, pc);
}

void LibRnrsBase::procAssertionViolation_helper(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);
  PairKev pair(registers->lastArgument(), vm->acc_);
  
  vm->CheckStack(&pair, &procAssertionViolation_helper, pc);
  
  if (registers->argn() > 3) {
    registers->popArgument();
    vm->acc_ = &pair;
    return procAssertionViolation_helper(vm, pc);
  }

  vm->gr1_ = registers->lastArgument();
  vm->gr2_ = vm->acc_;
  vm->acc_ = registers->lastArgument(1);
  return KevesVM::RaiseAssertCondition(vm, pc);
}

void LibRnrsBase::procApply(KevesVM* vm, const_KevesIterator pc) {
  StackFrameKev* registers(&vm->registers_);

  if (registers->argn() < 3) {
    vm->acc_ = vm->gr2_;

    vm->gr1_ = vm->base()->GetMesgText(registers->argn() == 2 ?
				       KevesBuiltinValues::mesg_Req2Got1 :
				       KevesBuiltinValues::mesg_Req2Got0);

    vm->gr2_ = EMB_NULL;
    return KevesVM::RaiseAssertCondition(vm, pc);
  }

  int argn(registers->argn());
  KevesValue last(registers->lastArgument());

  if (!last.IsPair() && last != EMB_NULL) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqListAsLast);
    vm->gr2_ = EMB_NULL;
    return vm->RaiseAssertCondition(vm, pc);
  }

  if (!registers->argument(1).IsProcedure()) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqProcAs1st);
    vm->gr2_ = EMB_NULL;
    return vm->RaiseAssertCondition(vm, pc);
  }
  
  int num_args(argn - 2);
  int final_num_args(num_args + KevesVM::GetListLength(last));
  ArgumentFrameKev* new_arg_frame(ArgumentFrameKev::Make(vm->gc(), final_num_args));
  
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

  while (temp.IsPair()) {
    const PairKev* pair(temp);
    KevesValue car(pair->car());
    registers->pushArgument(car);
    temp = pair->cdr();
  }

  if (temp != EMB_NULL) {
    vm->acc_ = vm->gr2_;
    vm->gr1_ = vm->base()->GetMesgText(KevesBuiltinValues::mesg_ReqProperList);
    vm->gr2_ = EMB_NULL;
    return vm->RaiseAssertCondition(vm, pc);
  }

  vm->gr1_ = registers->argument(0);
  return KevesVM::ApplyProcedure(vm, pc);
}


////////////////////////////////////////////////////////////////
// for loading this as a dynamic library                      //
////////////////////////////////////////////////////////////////

extern "C" {
  KevesLibrary* Make(KevesBase* base) {
    LibRnrsBase* library(new LibRnrsBase());
    library->Init(base);
    return library;
  }
}
