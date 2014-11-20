/* keves_char.hpp - character for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 * Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include <QString>
#include "keves_value.hpp"

class KevesChar {
public:
  KevesChar() = default;
  KevesChar(const KevesChar&) = default;
  KevesChar(KevesChar&&) = default;
  KevesChar& operator=(const KevesChar&) = default;
  KevesChar& operator=(KevesChar&&) = default;
  ~KevesChar() = default;

  explicit KevesChar(char chr) : chr_(chr) {
  }

  explicit KevesChar(QChar chr) : chr_(chr) {
  }

  explicit KevesChar(KevesValue kev)
  // : chr_(QChar(static_cast<qint32(kev.toUIntPtr() >> 8))) {
  {
    chr_ = static_cast<qint32>(kev.toUIntPtr()) >> 8;
  }
  
  template<typename T> explicit KevesChar(T) = delete;


  operator QChar() const {
    return chr_;
  }

  operator KevesValue() const {
    return KevesValue(reinterpret_cast<Kev*>(static_cast<quintptr>(chr_.unicode()) << 8 | 0xd));
  }

  QChar::Category category() const {
    return chr_.category();
  }

  bool isLetter() const {
    return chr_.isLetter();
  }

  bool isLower() const {
    return chr_.isLower();
  }

  bool IsNumber() const {
    return chr_.isNumber();
  }

  bool isSpace() const {
    return chr_.isSpace();
  }

  bool isTitleCase() const {
    return chr_.isTitleCase();
  }

  bool isUpper() const {
    return chr_.isUpper();
  }

  KevesChar toCaseFolded() const {
    return KevesChar(chr_.toCaseFolded());
  }

  KevesChar toLower() const {
    return KevesChar(chr_.toLower());
  }

  KevesChar toTitleCase() const {
    return KevesChar(chr_.toTitleCase());
  }

  ushort unicode() const {
    return chr_.unicode();
  }

  KevesChar toUpper() const {
    return KevesChar(chr_.toUpper());
  }

  int compare(QChar other) const {
    return this->chr_.unicode() - other.unicode();
  }

  int compare(KevesChar other) const {
    return this->compare(other.chr_);
  }

private:
  QChar chr_;
};
