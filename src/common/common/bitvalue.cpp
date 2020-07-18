/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "bitvalue.h"

//-----------------------------------------------------------------------------
const uint GenericValue::INVALID = 0xFFFFFFFFU;

//-----------------------------------------------------------------------------
BitValue BitValue::XORn(uint n) const
{
  uint nb = nbBits(_value);
  uint mask = maxValue(NumberBase::Bin, n);
  uint res = 0x0;
  for (uint i=0; i<nb; i +=n) {
    res ^= (_value >> i) & mask;
    //qDebug("%i %s %s", i, toHexLabel((value>>i) & mask, 4).latin1(), toHexLabel(res, 4).latin1());
  }
  return res;
}

BitValue BitValue::XNORn(uint n) const
{
  BitValue res = XORn(n);
  BitValue mask = maxValue(NumberBase::Bin, n);
  return res.complementInMask(mask);
}

//-----------------------------------------------------------------------------
ByteArray::ByteArray(uint length, uchar fillChar, PrintMode mode)
: _fillChar(fillChar), _mode(mode), _data(length)
{
  clear();
}

void ByteArray::clear()
{
  _data.fill(_fillChar);
}

void ByteArray::resize(uint size)
{
  _data.resize(size);
  clear();
}

QString ByteArray::pretty() const
{
  int end = _data.count() - 1;
  for (; end>=0; end--)
    if ( _data[end]!=_fillChar ) break;
    return pretty(0, end+1);
}

QString ByteArray::pretty(uint i, uint len) const
{
  QString s;
  for (uint k=0; k<len; ++k) s += toPrintable(_data[i + k], _mode);
  return s;
}

uint ByteArray::getValue(uint i, uint len) const
{
  uint v = 0;
  for (uint k=0; k<len; ++k) v |= _data[i + k] << (8*k);
  return v;
}

void ByteArray::setValue(GenericValue v, uint i, uint len)
{
  for (uint k = 0; k < len ; ++k) {
    _data[i+k] = v.byte(k);
  }
}

void ByteArray::setValueInc(GenericValue v, uint& i, uint len)
{
  for (uint k = 0; k < len ; ++k) {
    _data[i++] = v.byte(k);
  }
}
