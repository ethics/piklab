/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "device_editor.h"
#include "device_editor.moc"

#include <qregexp.h>
#include <qscrollarea.h>

#include "devices/list/device_list.h"
#include "toplevel.h"
#include "gui_debug_manager.h"
#include "project_manager.h"
#include "common/global/pfile.h"
#include "main_global.h"

DeviceEditor::DeviceEditor(const QString &title, const QString &tag, QWidget *parent, const char *name)
  : Editor(title, tag, parent, name), _view(0)
{
  init();
}

DeviceEditor::DeviceEditor(QWidget *parent, const char *name)
  : Editor(parent, name), _view(0)
{
  init();
}

void DeviceEditor::init()
{
  QHBoxLayout *hbox = new QHBoxLayout(this, 0);
  QScrollArea *area = new QScrollArea(this);
  hbox->addWidget(area);
  _widget = new QWidget;
  _top = new QVBoxLayout(_widget, 0, 0);
  _labelDevice = new QLabel(_widget);
  _labelDevice->setMargin(10);
  _labelDevice->setTextFormat(Qt::RichText);
  _top->addWidget(_labelDevice);
  _labelWarning = new QLabel(_widget);
  _labelWarning->setMargin(10);
  _labelWarning->setTextFormat(Qt::RichText);
  _top->addWidget(_labelWarning);
  _vbox = new QVBoxLayout(_top);
  area->setWidgetResizable(true);
  area->setWidget(_widget);
  _widget->show();

  connect(&MainWindow::self(), SIGNAL(deviceChanged()), SLOT(updateDevice()));
}

void DeviceEditor::setDevice(bool force)
{
  if ( !force && &Main::deviceData()==_device ) return;
  _device = &Main::deviceData();
  _labelDevice->hide();
  if ( _view.get() != NULL && isModified() ) {
    if ( MessageBox::questionYesNo(i18n("File %1 not saved.").arg(filename()), KStandardGuiItem::save(), KStandardGuiItem::discard()) )
      Editor::save();
  }
  _labelWarning->hide();
  _view.reset(createView(*_device, _widget));
  if (_view.get() != NULL) {
    _view->show();
    _vbox->addWidget(_view.get());
    updateGeometry();
  }
  setModified(false);
  emit guiChanged();
}

PURL::Url DeviceEditor::findAsmFile(const PURL::Url &url)
{
  if ( url.isEmpty() ) return PURL::Url();
  PURL::SourceFamily family = url.fileType().data().sourceFamily;
  if ( family.data().toolType==PURL::ToolType::Assembler ) return url;
  FOR_EACH(PURL::FileType, i) {
    PURL::SourceFamily source = i.data().sourceFamily;
    if ( source==PURL::SourceFamily::Nb_Types ) continue;
    if ( source.data().toolType!=PURL::ToolType::Assembler ) continue;
    for (uint k=0; i.data().extensions[k]; k++) {
      PURL::Url src = url.toExtension(i.data().extensions[k]);
      if ( PURL::findExistingUrl(src) ) return src;
    }
  }
  return PURL::Url();
}

QString DeviceEditor::guessDeviceFromFile(const PURL::Url &url)
{
  PURL::Url src = findAsmFile(url);
  if ( src.isEmpty() ) return QString::null;
  Log::StringView sview;
  PURL::File file(src, sview);
  if ( !file.openForRead() ) return QString::null;

  QString device;
  //   QRegExp re1("^[ \\t]+(?:PROCESSOR|processor)[ \\t]+((?:p|sx|P|SX)[a-z0-9A-Z]+)" ) ;
  QRegExp re1("^[ \\t]+(?:PROCESSOR|processor)[ \\t]+([a-z0-9A-Z]+)" ) ;
  QRegExp re2("^[ \\t]+(?:LIST|list)[ \\t]+" ) ;
  for (;;) {
    QString line = file.readLine();
    if ( line.isNull() ) break;
    // search PROCESSOR directive
    if ( re1.search(line, 0)!=-1 ) {
      device = re1.cap(1);
      break;
    }
    // search LIST p=... directive
    int k = re2.search(line,0);
    if ( k!=-1 ) {
      //QRegExp re3("(?:p|P)[ \\t]*=[ \\t]*((?:p|sx|P|SX)[a-z0-9A-Z]+)") ;
      QRegExp re3("(?:p|P)[ \\t]*=[ \\t]*([a-z0-9A-Z]+)") ;
      if ( re3.search(line, k+5)!=-1 ) {
        device = re3.cap(1);
        break;
      }
    }
  }
  device = device.upper();
  if(!device.isEmpty() && device[0] == 'P') return device.mid(1);
  if (Device::Lister::instance().data(device) == NULL) return QString::null;
  return device;
}
