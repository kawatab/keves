// keves/kevc_generator/code_rnrs-exceptions.cpp - a code of (rnrs exceptions)
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


#include "code_rnrs-exceptions.hpp"

#include <iostream>
#include <QList>
#include "keves_base.hpp"
#include "keves_library.hpp"


void Code_RnrsExceptions::write(KevesBase* base, const char* file_name) {
  // Library Header
  QStringList id;
  QList<ver_num_t> ver_num;
  id << "rnrs" << "exceptions";
  ver_num << 6;
  KevesLibrary this_lib(id, ver_num);

  // import binds
  KevesImportLibraryList import_libs(base);

  // from (rnrs exceptions)
  QStringList id_rnrs_exceptions_bin;
  id_rnrs_exceptions_bin << "rnrs" << "exceptions-bin";
  QList<ver_num_t> ver_rnrs_exceptions_bin;
  ver_rnrs_exceptions_bin << 6;

  if (!import_libs.setLibrary(id_rnrs_exceptions_bin, ver_rnrs_exceptions_bin)) {
    std::cerr << "Aborted writing the file: " << file_name << ".\n";
    return;
  }

  // export binds
  const char* binds[] = {
    "raise"
  };

  for (auto bind : binds)
    this_lib.addBind(bind, import_libs.nominateBind(bind));

  this_lib.writeToFile(base, file_name, import_libs);
}

void Code_RnrsExceptions::read(KevesBase* base, const char* file_name) {
  std::cout << file_name << ":" << std::endl;
  
  KevesLibrary* lib(KevesLibrary::readFromFile(file_name, base));

  lib->displayProperty(base);
  
  delete lib;
}
