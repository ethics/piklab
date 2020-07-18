/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2004 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include <qpixmap.h>
#include <qcheckbox.h>
#include <qvariant.h>

#include "common/common/misc.h"
#include "global_ui.h"
#include "container.h"

//-----------------------------------------------------------------------------
class ConfigWidget : public Container::Grid
{
Q_OBJECT
public:
  ConfigWidget(QWidget *parent = 0) : Container::Grid(parent) {}
  virtual QString title() const  { return QString::null; }
  virtual QString header() const { return QString::null; }
  virtual QPixmap pixmap() const { return QPixmap(); }

public slots:
  virtual void loadConfig() = 0;
  virtual void saveConfig() = 0;
};

//-----------------------------------------------------------------------------
template <typename Type>
class BaseConfigWidget
{
public:
  BaseConfigWidget(ConfigWidget *widget) {
    _widgets.resize(Type::Nb_Types);
    for(Type type; type<Type::Nb_Types; ++type) _widgets[type.type()] = createWidget(type, widget);
  }
  void loadConfig() {
    for(Type type; type<Type::Nb_Types; ++type) load(type, _widgets[type.type()]);
  }
  void saveConfig() {
    for(Type type; type<Type::Nb_Types; ++type) save(type, _widgets[type.type()]);
  }
  void setDefault() {
    for(Type type; type<Type::Nb_Types; ++type) setDefault(type, _widgets[type.type()]);
  }

private:
  std::vector<QWidget *> _widgets;

  static QWidget *createWidget(Type type, ConfigWidget *widget) {
    QWidget *w = 0;
    uint row = widget->numRows();
    switch (type.data().defValue.type()) {
      case QVariant::Bool:
        w = new QCheckBox(type.label(), widget);
        widget->addWidget(w, row,row, 0,1);
        break;
      default: ASSERT(false); break;
    }
    return w;
  }
  void load(Type type, QWidget *widget) const {
    switch (type.data().defValue.type()) {
      case QVariant::Bool:
        static_cast<QCheckBox *>(widget)->setChecked(readConfigEntry(type).toBool());
        break;
      default: ASSERT(false); break;
    }
  }
  void save(Type type, QWidget *widget) {
    switch (type.data().defValue.type()) {
      case QVariant::Bool:
        writeConfigEntry(type, QVariant(static_cast<QCheckBox *>(widget)->isChecked(), 0));
        break;
      default: ASSERT(false); break;
    }
  }
  void setDefault(Type type, QWidget *widget) const {
    switch (type.data().defValue.type()) {
      case QVariant::Bool:
        static_cast<QCheckBox *>(widget)->setChecked(type.data().defValue.toBool());
        break;
      default: ASSERT(false); break;
    }
  }
};

//-----------------------------------------------------------------------------
#define BEGIN_DECLARE_CONFIG_WIDGET(ConfigType, ConfigWidgetType) \
class ConfigWidgetType : public ::ConfigWidget, public BaseConfigWidget<ConfigType> \
{ \
public: \
  ConfigWidgetType() : BaseConfigWidget<ConfigType>(this) {} \
  virtual void loadConfig() { BaseConfigWidget<ConfigType>::loadConfig(); } \
  virtual void saveConfig() { BaseConfigWidget<ConfigType>::saveConfig(); } \

#define END_DECLARE_CONFIG_WIDGET \
};

#endif
