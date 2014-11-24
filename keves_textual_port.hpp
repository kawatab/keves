// Keves/textual_port.hpp - textual port for Keves
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


class QString;

class KevesTextualOutputPort {
public:
  KevesTextualOutputPort() = default;
  KevesTextualOutputPort(const KevesTextualOutputPort&) = delete;
  KevesTextualOutputPort(KevesTextualOutputPort&&) = delete;
  KevesTextualOutputPort& operator=(const KevesTextualOutputPort&) = delete;
  KevesTextualOutputPort& operator=(KevesTextualOutputPort&&) = delete;
  virtual ~KevesTextualOutputPort() = default;

  virtual void Append(const QString& str);
};
