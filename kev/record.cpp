// keves/kev/record.cpp - conditions for Keves
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


#include "kev/record.hpp"


RecordKev::RecordKev(const QString& a) : MutableKev(TYPE), name_(a) {
}

RecordKev* RecordKev::AMP_LEXICAL() {
  static RecordKev recordKev(QString("&lexical"));
  return &recordKev;
}

RecordKev* RecordKev::AMP_SYNTAX() {
  static RecordKev recordKev(QString("&syntax"));
  return &recordKev;
}

RecordKev* RecordKev::AMP_ASSERT() {
  static RecordKev recordKev(QString("&assert"));
  return &recordKev;
}

RecordKev* RecordKev::AMP_IRRITANTS() {
  static RecordKev recordKev(QString("&irritants"));
  return &recordKev;
}

RecordKev* RecordKev::AMP_WHO() {
  static RecordKev recordKev(QString("&who"));
  return &recordKev;
}

RecordKev* RecordKev::AMP_MESSAGE() {
  static RecordKev recordKev(QString("&message"));
  return &recordKev;
}
