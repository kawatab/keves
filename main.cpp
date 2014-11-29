// Keves - a R6RS Scheme implementation
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
#include <QThreadPool>
#include "keves_base.hpp"
#include "keves_vm.hpp"


int main() {
  KevesBase base;
  KevesVM* vm(base.MakeVM());
  QThreadPool pool1;
  pool1.start(vm);
  KevesVM* vm2(base.MakeVM());
  QThreadPool pool2;
  pool2.start(vm2);

  for (int i(0); i < 100; ++i) std::cout << i << "\t" << std::flush;
  
  std::cout << std::endl;

  pool1.waitForDone();
  pool2.waitForDone();
  return 0;
}
