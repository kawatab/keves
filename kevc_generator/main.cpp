// kevc_generator/main.cpp - generator of binary code for Keves
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


#include <iostream>
#include "keves_base.hpp"
#include "test_code.hpp"


int main() {
  KevesBase base;

  TestCode::Code02::Write(&base, "test02.kevc");
  TestCode::Code02::Read(&base, "test02.kevc");

  std::cout << std::endl;
  
  TestCode::Code03::Write(&base, "test03.kevc");
  TestCode::Code03::Read(&base, "test03.kevc");

  TestCode::KevesBaseCode::Write(&base, "lib/keves/base.kevc");
  TestCode::KevesBaseCode::Read(&base, "lib/keves/base.kevc");
  return 0;
}
