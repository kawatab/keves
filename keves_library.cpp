/* Keves/keves_library.cpp - library class for Keves
 * Keves will be an R6RS Scheme implementation.
 *
 * Copyright (C) 2014  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keves_library.hpp"

QDataStream& operator<<(QDataStream& out, const KevesLibrary& name) {
  out << name.id << name.ver_num << name.bind_list;
  return out;
}

QDataStream& operator>>(QDataStream& in, KevesLibrary& name) {
  in >> name.id >> name.ver_num >> name.bind_list;
  return in;
}
