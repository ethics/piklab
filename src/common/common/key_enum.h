/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef KEY_ENUM_H
#define KEY_ENUM_H

#include "misc.h"
#include "global.h"

//-----------------------------------------------------------------------------
class GenericEnum
{
public:
  GenericEnum(uint value) : _value(value) {}
  bool operator ==(GenericEnum e) const { return _value==e._value; }
  bool operator !=(GenericEnum e) const { return _value!=e._value; }
  bool operator <(GenericEnum e) const { return _value<e._value; }
  bool operator <=(GenericEnum e) const { return _value<=e._value; }
  bool operator >(GenericEnum e) const { return _value>e._value; }
  bool operator >=(GenericEnum e) const { return _value>=e._value; }
  GenericEnum &operator ++() { _value++; return *this; }

protected:
  uint _value;

private:
  friend QDataStream &operator >>(QDataStream &s, GenericEnum &e);
  friend QDataStream &operator <<(QDataStream &s, const GenericEnum &e);
};

inline QDataStream &operator <<(QDataStream &s, const GenericEnum &e)
{
  s << e._value;
  return s;
}
inline QDataStream &operator >>(QDataStream &s, GenericEnum &e)
{
  s >> e._value;
  return s;
}

//-----------------------------------------------------------------------------
#define BEGIN_DECLARE_ENUM(Enum) \
class Enum : public GenericEnum \
{ \
public: \
  enum Type {

#define DECLARE_KEY_METHODS \
public: \
  static Type fromKey(const QString &key) { \
    for (uint i=0; i<Nb_Types; i++) \
      if ( key==DATA[i].key ) return Type(i); \
    return Type(Nb_Types); \
  } \
  QString key() const { \
    if ( _value==Nb_Types ) return QString::null; \
    ASSERT(DATA[_value].key); \
    return DATA[_value].key; \
  }

#define DECLARE_LABEL_METHODS \
public: \
  QString label() const { \
    ASSERT( _value!=Nb_Types ); \
    ASSERT(DATA[_value].label); \
    return i18n(DATA[_value].label); \
  } \
  QString semicolonLabel() const { return i18n("%1:").arg(label()); } \
  QString labelWithValue(const QString &value) const { return i18n("%1: %2").arg(label()).arg(value); }

#define DECLARE_DATA \
public: \
  const Data &data() const { \
    ASSERT( _value!=Nb_Types ); \
    return DATA[_value]; \
  } \
  private: \
    static const Data DATA[Nb_Types]; \

#define DECLARE_ENUM_CLASS(Enum) \
public: \
  Enum(Type value = Type(0)) : GenericEnum(value) { ASSERT( value>=0 && value<=Type(Nb_Types)); } \
  Type type() const { return Type(_value); } \
};

#define END_DECLARE_TYPE \
    , Nb_Types \
  };

#define END_DECLARE_ENUM_DATA_ONLY(Enum, EnumData) \
  END_DECLARE_TYPE \
  typedef EnumData Data; \
  DECLARE_DATA \
  DECLARE_ENUM_CLASS(Enum)

#define END_DECLARE_ENUM(Enum, EnumData) \
  END_DECLARE_TYPE \
  typedef EnumData Data; \
  DECLARE_DATA \
  DECLARE_KEY_METHODS \
  DECLARE_LABEL_METHODS \
  DECLARE_ENUM_CLASS(Enum)

#define END_DECLARE_ENUM_KEY_ONLY(Enum) \
  END_DECLARE_TYPE \
  struct Data { const char *key; }; \
  DECLARE_DATA \
  DECLARE_KEY_METHODS \
  DECLARE_ENUM_CLASS(Enum)

#define END_DECLARE_ENUM_LABEL_ONLY(Enum) \
  END_DECLARE_TYPE \
  struct Data { const char *label; }; \
  DECLARE_DATA \
  DECLARE_LABEL_METHODS \
  DECLARE_ENUM_CLASS(Enum)

#define END_DECLARE_ENUM_KEY_LABEL(Enum) \
  END_DECLARE_TYPE \
  struct Data { const char *key, *label; }; \
  DECLARE_DATA \
  DECLARE_KEY_METHODS \
  DECLARE_LABEL_METHODS \
  DECLARE_ENUM_CLASS(Enum)

#define END_DECLARE_ENUM_NO_DATA(Enum) \
  END_DECLARE_TYPE \
  DECLARE_ENUM_CLASS(Enum)

//-----------------------------------------------------------------------------
#define DECLARE_ENUM_DATA(Enum, Typename, ClassName) \
class ClassName \
{ \
public: \
  typedef Typename Data; \
  enum { Nb_Types = Enum::Nb_Types }; \
  static const Data& data(Enum type) { \
    ASSERT( type.type()<Enum::Nb_Types ); \
    return DATA[type.type()]; \
  } \
\
private: \
  static const Data DATA[Enum::Nb_Types]; \
};

template <typename Enum, typename Type>
class EnumVector
{
public:
  EnumVector() : _vector(Enum::Nb_Types) {}
  const Type &operator [](Enum etype) const { return _vector[etype.type()]; }
  Type &operator [](Enum etype) { return _vector[etype.type()]; }
  const Type &operator [](typename Enum::Type type) const;
  Type &operator [](typename Enum::Type type);
private:
  std::vector<Type> _vector;
};

#define FOR_EACH(Enum, e) for(Enum e; e<Enum::Type(Enum::Nb_Types); ++e)

#endif
