// keves/keves_generator/code_keves-parse.hpp - a code of of (rnrs base)
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


#pragma once

#include "keves_value.hpp"


class KevcGenerator;


namespace Code_KevesParse {
  void write(KevcGenerator* generator);

  KevesValue makeLambda_qeves_parser(KevcGenerator* generator,
				     KevesValue proc_u8_list_to_vector,
				     KevesValue proc_add,
				     KevesValue proc_equal,
				     KevesValue proc_less_than,
				     KevesValue proc_less_than_equal,
				     KevesValue proc_greater_than,
				     KevesValue proc_greater_than_equal,
				     KevesValue proc_append,
				     KevesValue proc_apply,
				     KevesValue proc_assertion_violation,
				     KevesValue proc_call_with_values,
				     KevesValue proc_car,
				     KevesValue proc_caar,
				     KevesValue proc_caadr,
				     KevesValue proc_cdr,
				     KevesValue proc_cdar,
				     KevesValue proc_cdadr,
				     KevesValue proc_cddr,
				     KevesValue proc_char_eq_q,
				     KevesValue proc_char_lt_q,
				     KevesValue proc_char_gt_q,
				     KevesValue proc_cons,
				     KevesValue proc_integer_to_char,
				     KevesValue proc_list,
				     KevesValue proc_list_to_vector,
				     KevesValue proc_not,
				     KevesValue proc_null_q,
				     KevesValue proc_pair_q,
				     KevesValue proc_reverse,
				     KevesValue proc_string,
				     KevesValue proc_string_eq_q,
				     KevesValue proc_string_append,
				     KevesValue proc_string_length,
				     KevesValue proc_string_ref,
				     KevesValue proc_string_to_list,
				     KevesValue proc_string_to_number,
				     KevesValue proc_string_to_symbol,
				     KevesValue proc_substring,
				     KevesValue proc_symbol_eq_q,
				     KevesValue proc_values,
				     KevesValue proc_zero_q,
				     KevesValue proc_fold_left,
				     KevesValue proc_char_general_category,
				     KevesValue proc_char_whitespace_q,
				     KevesValue proc_char_ci_lt_q,
				     KevesValue proc_char_ci_gt_q);

  KevesValue makePair_abbr_list(KevcGenerator* generator);
  KevesValue makePair_char_name_list(KevcGenerator* generator);
  KevesValue makePair_escape_sequence_list(KevcGenerator* generator);
};
