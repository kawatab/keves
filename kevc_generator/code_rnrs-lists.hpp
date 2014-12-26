// keves/keves_generator/code_rnrs-lists.hpp - a code of of (rnrs base)
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


namespace Code_RnrsLists {
  void write(KevcGenerator* generator);

  KevesValue makeCode_proc_assq(KevcGenerator* generator,
				KevesValue proc_eq_q);

  KevesValue makeCode_proc_fold_left(KevcGenerator* generator,
				     KevesValue proc_apply,
				     KevesValue proc_car,
				     KevesValue proc_cdr,
				     KevesValue proc_cons,
				     KevesValue proc_map,
				     KevesValue proc_pair_q);
};
