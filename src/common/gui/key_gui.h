/***************************************************************************
 *   Copyright (C) 2006-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef KEY_GUI_H
#define KEY_GUI_H

#include <map>

#include "common/gui/misc_gui.h"
#include "common/common/misc.h"

//-----------------------------------------------------------------------------
template <typename KeyType, typename Type, typename WidgetType>
class KeyWidget
{
public:
  KeyWidget(QWidget* parent) { _widget = new WidgetType(parent); }
  virtual ~KeyWidget() { delete _widget; _widget = NULL; }
  WidgetType *widget() const { return _widget; }

  typedef std::map<KeyType, int> Map;
  typedef typename Map::const_iterator const_iterator;
  const_iterator begin() const { return _ids.begin(); }
  const_iterator end() const { return _ids.end(); }
  size_t size() const { return _ids.size(); }

  virtual void clear() { _ids.clear(); }
  void appendItem(const KeyType &key, Type type) {
    ASSERT(!contains(key));
    _ids[key] = append(type);
  }
  KeyType currentItem() const { return key(currentId()); }
  void setCurrentItem(const KeyType &key) {
    const_iterator it = _ids.find(key);
    if (it != _ids.end()) setCurrentId(it->second);
    else ASSERT(false);
  }
  bool contains(const KeyType &key) const { return (_ids.find(key) != _ids.end()); }
  Type item(const KeyType &key) const {
    const_iterator it = _ids.find(key);
    if (it != _ids.end()) return get(it->second);
    ASSERT(false);
    return Type();
  }
  KeyType key(int id) const {
    for (const_iterator it = begin(); it != end(); ++it)
      if ( it->second==id ) return it->first;
    ASSERT(false);
    return KeyType();
  }

protected:
  virtual int append(Type type) = 0;
  virtual int currentId() const = 0;
  virtual void setCurrentId(int id) = 0;
  virtual Type get(int id) const = 0;

private:
  Map                  _ids;
  QPointer<WidgetType> _widget;
};

//-----------------------------------------------------------------------------
template <typename KeyType>
class KeyComboBox : public KeyWidget<KeyType, QString, KComboBox>
{
public:
  typedef KeyWidget<KeyType, QString, KComboBox> ParentType;
  KeyComboBox(QWidget* parent) : ParentType(parent) {}
  virtual void clear() {
    ParentType::clear();
    if (ParentType::widget() != NULL) ParentType::widget()->clear();
  }
  void fixMinimumWidth() {
    if (ParentType::widget() != NULL)
      ParentType::widget()->setMinimumWidth(ParentType::widget()->sizeHint().width());
  }

protected:
  virtual int append(QString label) {
    ParentType::widget()->insertItem(label);
    return ParentType::widget()->count()-1;
  }
  virtual int currentId() const { return ParentType::widget()->currentIndex(); }
  virtual void setCurrentId(int id) { ParentType::widget()->setCurrentIndex(id); }
  virtual QString get(int id) const { return ParentType::widget()->itemText(id); }
};

//-----------------------------------------------------------------------------
template <typename KeyType>
class KeyWidgetStack : public KeyWidget<KeyType, QWidget *, Q3WidgetStack>
{
public:
  typedef KeyWidget<KeyType, QWidget *, Q3WidgetStack> ParentType;
  KeyWidgetStack(QWidget* parent) : ParentType(parent) {}

protected:
  virtual int append(QWidget *widget) { return ParentType::widget()->addWidget(widget); }
  virtual int currentId() const { return ParentType::widget()->id(ParentType::widget()->visibleWidget()); }
  virtual void setCurrentId(int id) { ParentType::widget()->raiseWidget(id); }
  virtual QWidget *get(int id) const { return ParentType::widget()->widget(id); }
};

//-----------------------------------------------------------------------------
template <typename KeyType>
class KeyPopupButton : public KeyWidget<KeyType, QString, PopupButton>
{
public:
  typedef KeyWidget<KeyType, QString, PopupButton> ParentType;
  KeyPopupButton(QWidget* parent) : ParentType(parent) {}

protected:
  virtual int append(QString label) { return ParentType::widget()->appendItem(label, QPixmap()); }
  virtual QString get(int id) const { return ParentType::widget()->popup()->text(id); }

private: // disable
  QString currentItem() const;
  void setCurrentItem(const QString &key);
  virtual int currentId() const { return 0; }
  virtual void setCurrentId(int) {}
};

//-----------------------------------------------------------------------------
template <typename Lister>
class ListerComboBox : public KeyComboBox<QString>
{
public:
  ListerComboBox(QWidget* parent) : KeyComboBox<QString>(parent) {
    for (typename Lister::ConstIterator it = Lister::instance().begin();
         it != Lister::instance().end();
         ++it) {
      appendItem(it.key(), it.data()->label());
    }
  }

  const typename Lister::Group& group() const { return *Lister::instance().group(ParentType::currentItem()); }
  void setGroup(const typename Lister::Group& group) { ParentType::setCurrentItem(group.name()); }

private: // disable
  QString currentItem() const;
  void setCurrentItem(const QString& key);
};

#endif
