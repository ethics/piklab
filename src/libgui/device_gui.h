/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DEVICE_GUI_H
#define DEVICE_GUI_H

#include <memory>

#include "common/gui/container.h"
#include "common/gui/key_gui.h"
#include "common/gui/dialog.h"
#include "device_editor.h"
#include "devices/pic/base/pic.h"
namespace Programmer { class Group; }
namespace Tool { class Group; }

namespace DeviceChooser
{
class View;

BEGIN_DECLARE_ENUM(ListType)
  FamilyTree = 0, Flat
END_DECLARE_ENUM_KEY_LABEL(ListType)

//-----------------------------------------------------------------------------
class Config : public GenericConfig
{
public:
  Config() : GenericConfig("device_chooser") {}
  static const Programmer::Group *programmerGroup();
  static void writeProgrammerGroup(const Programmer::Group *group);
  static const Tool::Group *toolGroup();
  static void writeToolGroup(const Tool::Group *group);
  static QString package();
  static void writePackage(const QString &name);
  static uint nbPins();
  static void writeNbPins(uint nbPins);
};

//-----------------------------------------------------------------------------
template <typename Enum>
class EnumComboBox
{
public:
  EnumComboBox(const QString &key, QWidget *parent) : _key(key) {
    _combo = new KComboBox(parent);
    for (Enum type; type<Enum::Nb_Types; ++type) _combo->insertItem(type.label());
    Config config;
    Enum type = config.readEnumEntry(key, Enum(Enum::Nb_Types));
    if ( type!=Enum::Nb_Types ) _combo->setCurrentIndex(type.type());
  }
  EnumComboBox(const QString &emptyLabel, const QString &key, QWidget *parent) : _key(key) {
    _combo = new KComboBox(parent);
    _combo->insertItem(emptyLabel);
    for (Enum type; type<Enum::Nb_Types; ++type) _combo->insertItem(type.label());
    Config config;
    Enum type = config.readEnumEntry(key, Enum(Enum::Nb_Types));
    if ( type!=Enum::Nb_Types ) _combo->setCurrentIndex(type.type()+1);
  }
  ~EnumComboBox() { delete _combo; _combo = NULL; }

  QComboBox *combo() const { return _combo; }
  Enum value() const {
    if ( _combo->count()==Enum::Nb_Types ) return typename Enum::Type(_combo->currentItem());
    if ( _combo->currentIndex()==0 ) return Enum::Nb_Types;
    return typename Enum::Type(_combo->currentIndex()-1);
  }
  void reset() { _combo->setCurrentIndex(0); }
  void writeConfig() {
    Config config;
    config.writeEnumEntry(_key, value());
  }

private:
  QString    _key;
  KComboBox* _combo;
};

//-----------------------------------------------------------------------------
class Dialog : public ::Dialog
{
Q_OBJECT
public:
  Dialog(const QString &device, QWidget *parent);
  virtual ~Dialog();

  QString device() const;

private slots:
  void listDoubleClicked(Q3ListViewItem *item);
  void currentChanged(Q3ListViewItem *item);
  void deviceChange(const QString &device);
  void updateList();
  void resetFilters();

private:
  std::auto_ptr<KeyComboBox<QString> > _programmerCombo;
  std::auto_ptr<KeyComboBox<QString> > _toolCombo;
  std::auto_ptr<KeyComboBox<QString> > _packageCombo;
  std::auto_ptr<KeyComboBox<uint> >    _pinsCombo;
  std::auto_ptr<EnumComboBox<ListType> >                 _listTypeCombo;
  std::auto_ptr<EnumComboBox<Device::MemoryTechnology> > _memoryCombo;
  std::auto_ptr<EnumComboBox<Device::Status> >           _statusCombo;
  std::auto_ptr<EnumComboBox<Pic::Feature> >             _featureCombo;
  ListView* _listView;
  View*     _deviceView;

  void updateList(const QString &device);
  const Programmer::Group *programmerGroup() const;
  const Tool::Group *toolGroup() const;
};

//-----------------------------------------------------------------------------
class ComboBox : public KComboBox
{
Q_OBJECT
public:
  ComboBox(QWidget *parent);
  void setDevice(const QString &device);
  QString device() const { return currentText(); }
};

//-----------------------------------------------------------------------------
class Button : public QWidget
{
Q_OBJECT
public:
  Button(QWidget *parent);
  void setDevice(const QString &device) { _combo->setDevice(device); }
  QString device() const { return _combo->device(); }

signals:
  void changed();

private slots:
  void chooseDevice();

private:
  ComboBox *_combo;
};

//-----------------------------------------------------------------------------
class Browser : public KTextBrowser
{
Q_OBJECT
public:
  Browser(QWidget *parent);
  void setData(const Device::Data*);

signals:
  void deviceChanged(const QString &device);

protected:
  virtual void setSource(const QUrl& url);
  virtual QVariant loadResource(int type, const QUrl& url);

private:
  const Device::Data* _data;
};

//-----------------------------------------------------------------------------
class View : public TabWidget
{
Q_OBJECT
public:
  View(QWidget *parent);
  void clear();
  void setText(const QString &text);
  void setDevice(const QString &name, bool cannotChangeDevice);

signals:
  void deviceChanged(const QString &device);

private:
  Browser *_info, *_memory, *_vfg, *_pins;
};

//-----------------------------------------------------------------------------
class Editor : public DeviceEditor
{
Q_OBJECT
public:
  Editor(const QString &title, const QString &tag, QWidget *parent);
  virtual bool isModified() const { return false; }
  virtual bool isReadOnly() const { return true; }
  virtual void addGui() {}
  virtual void removeGui() {}
  virtual void setFocus() {}

signals:
  void deviceChanged(const QString &device);

private:
  virtual QWidget *createView(const Device::Data& data, QWidget *parent);
  virtual void setModifiedInternal(bool) {}
  virtual void setReadOnlyInternal(bool) {}
};

} // namespace

#endif
