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

#include "kevc_generator.hpp"


void Code_RnrsExceptions::write(KevcGenerator* generator) {
  // import and export binds
  if (!generator->setImportLibrary("rnrs", "exceptions-bin", 6)) return;

  const char* binds[] = {
    "raise"
  };

  for (auto bind : binds)
    generator->exportBind(bind, generator->importBind(bind));

  generator->writeToFile();
}
