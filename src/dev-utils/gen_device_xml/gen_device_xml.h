/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GEN_DEVICE_XML_H
#define GEN_DEVICE_XML_H

#include "common/common/global.h"

#include <qtextstream.h>

//-----------------------------------------------------------------------------
class DomAttribute
{
public:
  DomAttribute() {}
  DomAttribute(const QString &name, const QString &value) : _name(name), _value(value) {}
  QString save() const { return _name + "=\"" + _value + "\" "; }

private:
  QString _name, _value;
};

//-----------------------------------------------------------------------------
class DomChild
{
public:
  virtual ~DomChild() {}
  virtual void save(QTextStream &, uint, const QValueList<uint> &align) const = 0;
  virtual bool isElement() const = 0;
};

class DomComment : public DomChild
{
public:
  DomComment(const QString &text) : _text(text) {}
    virtual void save(QTextStream &s, uint, const QValueList<uint> &) const;
  virtual bool isElement() const { return false; }

private:
  QString _text;
};

class DomEmptyLine : public DomChild
{
public:
  virtual void save(QTextStream &s, uint, const QValueList<uint> &) const { s << endl; }
  virtual bool isElement() const { return false; }
};

class DomParent
{
public:
  DomParent() {}
  virtual ~DomParent();
  void append(DomChild *child) { _childs += child; }

protected:
  QValueList<DomChild *> _childs;

  DomParent(const DomParent &) {}
  virtual DomParent &operator =(const DomParent &) { return *this; }
  void saveChilds(QTextStream &s, uint indent) const;
};

class DomElement : public DomChild, public DomParent
{
public:
  DomElement(const QString &tag) : _tag(tag) { Q_ASSERT( !tag.isEmpty() ); }
  QString tag() const { return _tag; }
  void appendAttribute(const QString &name, const QString &value) { _attributes += DomAttribute(name, value); }
  virtual void save(QTextStream &s, uint indent, const QValueList<uint> &align) const;
  virtual bool isElement() const { return true; }
  void computeAlign(QValueList<uint> &align) const;

private:
  QString _tag;
  QValueList<DomAttribute> _attributes;
};

//-----------------------------------------------------------------------------
class DomDocument : public DomParent
{
public:
  DomDocument(const QString &name) : _name(name) { Q_ASSERT( !name.isEmpty() ); }
  void save(QTextStream &s) const;

private:
  QString _name;
};

#endif
