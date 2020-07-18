/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "log_view.h"
#include "log_view.moc"

#include <qeventloop.h>

#include "global_config.h"
#include "common/gui/purl_gui.h"
#include "common/gui/misc_gui.h"

Log::Widget::Widget(QWidget *parent, const char *name)
  : KTextEdit(parent)
{
  setObjectName(name);
  setTextFormat(Qt::RichText);
  setLineWrapMode(WidgetWidth);
  setWordWrapMode(QTextOption::WrapAnywhere);
  setUndoRedoEnabled(false);
  QPalette p = palette();
  setReadOnly(true);
  setPalette(p);
  setMinimumWidth(300);
  setDebugLevel(GlobalConfig::debugLevel());

  connect(this, SIGNAL(aboutToShowContextMenu(QMenu*)), this, SLOT(slotAboutToShowContextMenu(QMenu*)));
}

void Log::Widget::slotAboutToShowContextMenu(QMenu* popup)
{
  setDebugLevel(GlobalConfig::debugLevel());

  popup->insertItem(KIcon("document-save"), i18n("Save As..."), this, SLOT(saveAs()));
  popup->insertItem(KIcon("edit-clear"), i18n("Clear"), this, SLOT(clear()));
  popup->insertSeparator();
  FOR_EACH(DebugLevel, level) {
    _id[level.type()] = popup->insertItem(level.label());
    popup->setItemChecked(_id[level.type()], _debugLevel==level);
  }
  popup->insertSeparator();
  int id = popup->insertItem(i18n("Output in console"), this, SLOT(toggleConsoleOutput()));
  popup->setItemChecked(id, GlobalConfig::logOutputType()==GuiConsole);
  connect(popup, SIGNAL(activated(int)), SLOT(toggleVisible(int)));

}

void Log::Widget::logExtra(const QString &text)
{
  _text += text;
  if ( GlobalConfig::logOutputType()==GuiConsole ) fprintf(stdout, "%s", text.latin1());
}

void Log::Widget::doLog(LineType type, const QString &text, Action action)
{
  doLog(text, type.data().color, type.data().bold, action);
}

void Log::Widget::doLog(DebugLevel level, const QString &text, Action action)
{
  doLog(text, level.data().color, false, action);
}

void Log::Widget::doLog(const QString &text, const QString &color, bool bold, Action action)
{
  if (action == LogFileOnly) return;
  logExtra(text + "\n");
  QString s = QString("<font color=%1>").arg(color);
  if (bold) s += "<b>";
  s += escapeXml(text) + "&nbsp;";
  if (bold) s += "</b>";
  s += "</font>";
  append(s);
  // ### DANGER
  if ( action==Immediate)
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInput);
}

void Log::Widget::toggleVisible(int id)
{
  FOR_EACH(DebugLevel, level) {
    if ( _id[level.type()]==id ) {
      _debugLevel = level;
      GlobalConfig::writeDebugLevel(level);
      break;
    }
  }
}

void Log::Widget::toggleConsoleOutput()
{
  GlobalConfig::writeLogOutputType(GlobalConfig::logOutputType()==GuiOnly ? GuiConsole : GuiOnly);
}

void Log::Widget::sorry(const QString &message, const QString &details)
{
  logExtra(message + " [" + details + "]\n");
  MessageBox::detailedSorry(message, details, Log::Show);
}

bool Log::Widget::askContinue(const QString &message)
{
  bool ok = MessageBox::askContinue(message);
  logExtra(message + " [" + (ok ? "continue" : "cancel") + "]\n");
  return ok;
}

void Log::Widget::clear()
{
  KTextEdit::clear();
  _text = QString::null;
}

void Log::Widget::saveAs()
{
  PURL::Url url = PURL::getSaveUrl("kfiledialog:///save_log", "text/x-log", this, i18n("Save log to file"), PURL::AskOverwrite);
  if ( url.isEmpty() ) return;
  url.write(_text, *this);
}

void Log::Widget::mouseReleaseEvent(QMouseEvent *e)
{
  KTextEdit::mouseReleaseEvent(e);
  if ( e->button()!=Qt::LeftButton ) return;
  QTextBlockFormat format = _cursor.blockFormat();
  format.clearBackground();
  _cursor.setBlockFormat(format);
  _cursor = cursorForPosition(e->pos());
  format = _cursor.blockFormat();
  QBrush brush = palette().brush(QPalette::AlternateBase);
  format.setBackground(brush);
  _cursor.setBlockFormat(format);
  lineClicked(_cursor.block().firstLineNumber());
}
