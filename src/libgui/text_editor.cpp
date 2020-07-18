/***************************************************************************
 *   Copyright (C) 2005-2012 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "text_editor.h"
#include "text_editor.moc"

#include <klibloader.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/smartinterface.h>
#include <ktexteditor/factory.h>
#include <ktexteditor/editor.h>
#include <kxmlguifactory.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>

#include "main_global.h"
#include "gui_debug_manager.h"
#include "editor_manager.h"
#include "global_config.h"
#include "toplevel.h"

//-----------------------------------------------------------------------------
const TextEditor::MarkTypeData TextEditor::MARK_TYPE_DATA[Breakpoint::Nb_MarkTypes] = {
  { KTextEditor::MarkInterface::Execution,          "program_counter"          },
  { KTextEditor::MarkInterface::markType08,         "program_counter_disabled" },
  { KTextEditor::MarkInterface::BreakpointActive,   "breakpoint_active"        },
  { KTextEditor::MarkInterface::BreakpointDisabled, "breakpoint_disabled"      },
  { KTextEditor::MarkInterface::BreakpointReached,  "breakpoint_reached"       },
  { KTextEditor::MarkInterface::Error,              "breakpoint_invalid"       }
};

QPixmap TextEditor::pixmap(Breakpoint::MarkType type)
{
  return SmallIcon(MARK_TYPE_DATA[type].pixmap);
}

TextEditor::TextEditor(const QString& title, const QString& tag, QWidget *parent, const char *name)
: Editor(title, tag, parent, name),
  _view(NULL)
{
  init(false);
}

TextEditor::TextEditor(bool withDebugger, QWidget *parent, const char *name)
: Editor(parent, name),
  _view(NULL)
{
  init(withDebugger);
}

void TextEditor::init(bool withDebugger)
{
  QFontMetrics fm(font());
  setMinimumHeight(5*fm.height());

  KPluginLoader loader("katepart");
  KPluginFactory *factory = loader.factory();
  if (factory == NULL) qFatal("Could not find katepart: Install Kate.");
  _document = factory->create<KTextEditor::Document>(this);
  ASSERT(_document != NULL);
  _oldModified = _document->isModified();
  _oldReadOnly = !_document->isReadWrite();

  QVBoxLayout *top = new QVBoxLayout(this, 0, 10);
  _split = new QSplitter(Qt::Horizontal, this);
  _split->setFrameStyle(QFrame::TabWidgetPanel |  QFrame::Sunken);
  top->addWidget(_split);

  connect(_document, SIGNAL(highlightingModeChanged(KTextEditor::Document*)), SLOT(highlightModeChanged()));
  connect(_document, SIGNAL(modeChanged(KTextEditor::Document*)), SLOT(modeChanged()));
  connect(_document, SIGNAL(modifiedChanged(KTextEditor::Document*)), SLOT(statusChanged()));
  setAcceptDrops(true);
  addView();

  KTextEditor::MarkInterface* markInterface = qobject_cast<KTextEditor::MarkInterface*>(_document);
  ASSERT(markInterface != NULL);
  for (uint i=0; i<Breakpoint::Nb_MarkTypes; i++)
    markInterface->setMarkPixmap(KTextEditor::MarkInterface::MarkTypes(MARK_TYPE_DATA[i].type),
				  pixmap(Breakpoint::MarkType(i)));

  if (withDebugger) QTimer::singleShot(0, this, SLOT(addToDebugManager()));
}

bool TextEditor::open(const PURL::Url &url)
{
  setReadOnly(url.fileType().data().properties & PURL::ReadOnly);
  if ( !_document->openUrl(url.kurl()) ) return false;

  //_view->setEol(url.isDosFile() ? Dos : Unix);
  highlightModeChanged();
  modeChanged();
  return true;
}

void TextEditor::addToDebugManager()
{
  static_cast<Debugger::GuiManager *>(Debugger::manager)->addTextEditor(*this);
}

bool TextEditor::eventFilter(QObject *, QEvent *e)
{
  if ( e->type()==QEvent::KeyPress ) {
    if ( static_cast<QKeyEvent *>(e)->key()==Qt::Key_Escape ) return true;
  }
  return false;
}

void TextEditor::addView()
{
    KTextEditor::View *v = _document->createView(_split);

    // use KXMLGuiClient::replaceXMLFile to provide katepart with restructured UI configuration
    const QString uiFile = KGlobal::mainComponent().componentName() + "/katepartui.rc";
    QStringList katePartUIs = KGlobal::mainComponent().dirs()->findAllResources("data", uiFile);
    if (!katePartUIs.isEmpty()) {
      const QString katePartUI = katePartUIs.last();
      const QString katePartLocalUI = KStandardDirs::locateLocal("data", uiFile);
      if (!QFile::exists(katePartLocalUI)) {
        // prevent warning: No such XML file ".../.kde/share/apps/piklab/katepartui.rc"
        QFile::copy(katePartUI, katePartLocalUI);
      }
      //v->replaceXMLFile(katePartUI, katePartLocalUI); // does not compile on CentOS 6 (kde 4.3.4)
      v->setLocalXMLFile(katePartLocalUI);
      v->setXMLFile(katePartUI);
    }

    v->actionCollection()->addAction("editor_toggle_breakpoint", Debugger::manager, SLOT(toggleBreakpoint()));
    v->actionCollection()->addAction("editor_enable_breakpoint", Debugger::manager, SLOT(toggleEnableBreakpoint()));

    if (_view == NULL) _view = v;
    ASSERT(v != NULL);
    connect(v, SIGNAL(focusIn(KTextEditor::View*)), SLOT(focusIn(KTextEditor::View*)));
    connect(v, SIGNAL(cursorPositionChanged(KTextEditor::View*, const KTextEditor::Cursor&)),
	    SLOT(statusChanged()));
    connect(v, SIGNAL(dropEventPass(QDropEvent*)), SIGNAL(dropEventPass(QDropEvent*)));
    connect(v, SIGNAL(viewModeChanged(KTextEditor::View*)), SLOT(statusChanged()));
    v->show();
    v->setFocus();
    QMenu* menu = Main::popup("ktexteditor_popup");
    ASSERT(menu != NULL);
    v->setContextMenu(menu);

    // divide available space equally between views
    QValueList<int> list = _split->sizes();
    int sum = 0;
    for (QValueList<int>::Iterator it = list.begin(); it != list.end(); ++it) sum += *it;
    sum /= list.size();
    for (QValueList<int>::Iterator it = list.begin(); it != list.end(); ++it) *it = sum;
    _split->setSizes(list);

    emit guiChanged();
}

void TextEditor::focusIn(KTextEditor::View *v)
{
  _view = v;
  setFocusProxy(v);
  statusChanged();
  emit guiChanged();
}

void TextEditor::addGui()
{
  MainWindow::self().guiFactory()->addClient(_view);
}

void TextEditor::removeGui()
{
  MainWindow::self().guiFactory()->removeClient(_view);
}

void TextEditor::removeCurrentView()
{
  delete _view;
  _view = _document->views().last();
  _document->views().last()->setFocus();
  emit guiChanged();
}

bool TextEditor::isReadOnly() const
{
  return !_document->isReadWrite();
}

void TextEditor::setReadOnlyInternal(bool ro)
{
  _oldReadOnly = ro;
  _document->setReadWrite(!ro);
}

bool TextEditor::isModified() const
{
  return _document->isModified();
}

void TextEditor::setModifiedInternal(bool m)
{
  _oldModified = m;
  _document->setModified(m);
}

void TextEditor::statusChanged()
{
  const KTextEditor::Cursor cursor = _view->cursorPosition();
  const uint line = cursor.line();
  const uint col = cursor.column();
  QString text = i18n("Line: %1  Col: %2").arg(line+1).arg(col+1);
  if( isReadOnly() ) text += "  " + i18n("R/O");
  emit statusTextChanged(" " + text + " ");
  if ( isReadOnly()!=_oldReadOnly || isModified()!=_oldModified ) emit guiChanged();
  if ( isModified()!=_oldModified ) emit modified();
  _oldModified = isModified();
  _oldReadOnly = isReadOnly();
  const Breakpoint::Data data(url(), line);
  updateBreakpointActions(&data);
}

void TextEditor::updateBreakpointActions(const Breakpoint::Data *data)
{
  const bool hasBreakpoint = (data ? Breakpoint::list().contains(*data) : false);
  _view->action("editor_toggle_breakpoint")->setText(hasBreakpoint ? i18n("Remove breakpoint") : i18n("Set breakpoint"));
  _view->action("editor_toggle_breakpoint")->setEnabled(data);
  const bool isActive = (hasBreakpoint ? Breakpoint::list().state(*data)==Breakpoint::Active : false);
  _view->action("editor_enable_breakpoint")->setText(!isActive ? i18n("Enable breakpoint") : i18n("Disable breakpoint"));
  _view->action("editor_enable_breakpoint")->setEnabled(Debugger::manager->coff() && hasBreakpoint);
}

void TextEditor::highlightModeChanged()
{
  if ( fileType()==PURL::Nb_FileTypes ) return;
  // managed by hand because of collisions with file extensions
  // used by other languages/softwares like other ASM and also PHP...
  const char *name = fileType().data().highlightModeName;
  if ( name==NULL || name==_document->highlightingMode() ) return;
  _document->setHighlightingMode(name);
}

void TextEditor::modeChanged()
{
  if ( fileType()==PURL::Nb_FileTypes ) return;
  // managed by hand because of collisions with file extensions
  // used by other languages/softwares like other ASM and also PHP...
  const char *name = fileType().data().highlightModeName;
  if ( name==NULL || name==_document->mode() ) return;
  _document->setMode(name);
}

uint TextEditor::cursorLine() const
{
  const KTextEditor::Cursor cursor = _view->cursorPosition();
  return cursor.line();
}

void TextEditor::setMark(uint line, Breakpoint::MarkType type)
{
  //_view->setIconBorder(true);
  KTextEditor::MarkInterface *markInterface = qobject_cast<KTextEditor::MarkInterface*>(_document);
  ASSERT(markInterface != NULL);
  markInterface->setMark(line, MARK_TYPE_DATA[type].type);
}

void TextEditor::clearMarks(uint type)
{
  KTextEditor::MarkInterface *markInterface = qobject_cast<KTextEditor::MarkInterface*>(_document);
  ASSERT(markInterface != NULL);
  const QHash<int, KTextEditor::Mark*> marks = markInterface->marks();
  for (QHash<int, KTextEditor::Mark*>::const_iterator it = marks.begin(); it != marks.end(); ++it)
    if ( it.value()->type==type ) markInterface->removeMark(it.key(), it.value()->type);
}

void TextEditor::clearBreakpointMarks()
{
  for (uint i=0; i<Breakpoint::Nb_MarkTypes; i++) clearMarks(MARK_TYPE_DATA[i].type);
}

QValueList<uint> TextEditor::bookmarkLines() const
{
  KTextEditor::MarkInterface *markInterface = qobject_cast<KTextEditor::MarkInterface*>(_document);
  ASSERT(markInterface != NULL);
  const QHash<int, KTextEditor::Mark*>& marks = markInterface->marks();
  QValueList<uint> lines;
  for (QHash<int, KTextEditor::Mark*>::const_iterator it = marks.begin(); it != marks.end(); ++it)
    if ( it.value()->type==KTextEditor::MarkInterface::Bookmark ) lines.append(it.key());
  return lines;
}

void TextEditor::setBookmarkLines(const QValueList<uint> &lines)
{
  clearMarks(KTextEditor::MarkInterface::Bookmark);

  KTextEditor::MarkInterface *markInterface = qobject_cast<KTextEditor::MarkInterface*>(_document);
  ASSERT(markInterface != NULL);
  for (QValueList<uint>::const_iterator it=lines.begin(); it!=lines.end(); ++it)
    markInterface->setMark(*it, KTextEditor::MarkInterface::Bookmark);
}

void TextEditor::configure()
{
  KPluginLoader loader("katepart");
  KPluginFactory *factory = loader.factory();
  KTextEditor::Factory* kteFactory = qobject_cast<KTextEditor::Factory*>(factory);
  if (kteFactory == NULL) qFatal("Could not find katepart: Install Kate.");
  KTextEditor::Editor* editor = kteFactory->editor();
  if (editor != NULL
    && editor->configDialogSupported()) {
    editor->configDialog(&MainWindow::self());
    editor->writeConfig();
  }
}

#if 0
void TextEditor::slotChangedText()
{
    //This slot runs the instrucion set help bar,
    // finds the current command word and compares it to the set of instrucions
    //Found in the descript Qstringlist.


    QString currentword;
    QString testval;

    Kate::View *v =  currentView();
    currentword = v->currentTextLine();
    //prepare the string for compareing
    currentword = currentword.simplifyWhiteSpace();
    currentword = currentword.upper();

    for ( QStringList::Iterator it = descript.begin(); it != descript.end(); ++it )
    {
        testval = *it;
        if(testval.startsWith(currentword.left(5)))
        {
            //if (testval.left(5) == currentword.left(5) ) {
            lab_curword->setText(*it);
        }
        //      else {
        //              lab_curword->setText("Pic Instruction Help");
        //      }
    }


}
#endif
#if 0
void TextEditor::populateList()
{
    //Populate the qstringlist with the pic instruction set, and the text to go along with them

    descript    += "CLRF  : Clear F     OPERANDS: f";
    descript    += "ADDWF : Add W to F  OPERANDS: f,d";
    descript    += "ANDWF : AND W with F OPERANDS: f,d";
    descript    += "CLRW  : Clear W     OPERANDS: NONE";
    descript    += "COMF  : Complement F OPERANDS: f,d";
    descript    += "DECF  : Decrement  F OPERANDS: f,d";
    descript    += "DECSSZ: Decrement F, Skip 0 OPERANDS: f,d";
    descript    += "INCF  : Increment F  OPERANDS: f,d";
    descript    += "INCFSZ: Increment F, Skip 0 OPERANDS: f,d";
    descript    += "IORWF : Inclusive OR W with F OPERANDS: f,d";
    descript    += "MOVF  : Move F,  OPERANDS: f,d";
    descript    += "MOVWF : Move W to F OPERANDS: f,d";
    descript    += "NOP   : No Operation OPERANDS: NONE";
    descript    += "RLF   : Rotate Left F through Carry OPERANDS: f,d";
    descript    += "RRF   : Rotate Right F through Carry OPERANDS: f,d";
    descript    += "SUBWF : Subtract W from F OPERANDS: f,d";
    descript    += "SWAPF : Swap Nibbles in F OPERANDS: f,d";
    descript    += "XORWF : Exclusive OR W with F OPERANDS: f,s";
    descript    += "BCF   : Bit Clear F OPERANDS: f,b";
    descript    += "BSF   : Bit Set F   OPERANDS: f,b";
    descript    += "BTFSC : Bit Test F, Skip if Clear OPERANDS: f,b";
    descript    += "BTFSS : Bit Test F, Skip if Set   OPERANDS: f,b";
    descript    += "ADDLW : Add Literal and W OPERANDS: k";
    descript    += "ANDLW : And Literal and W OPERANDS: k";
    descript    += "CLRWDT: Clear Watchdog Timer OPERANDS: NONE";
    descript    += "CALL  : Call Subroutine OPERANDS: k";
    descript    += "GOTO  : Goto address OPERANDS: k";
    descript    += "IORLW : Inclusive OR literal with W OPERANDS: k";
    descript    += "MOVLW : Move Literal with W  OPERANDS: k";
    descript    += "RETFIE: Return From Interrupt OPERANDS: NONE";
    descript    += "RETLW : Return with Literal in W OPERANDS: k";
    descript    += "RETURN: Return from subroutine OPERANDS: NONE";
    descript    += "SLEEP : Go into Standby Mode OPERANDS: NONE";
    descript    += "SUBLW : Subtract W from Literal OPERANDS: k";
    descript    += "XORLW : Exclusive OR Literal with W OPERANDS: k";

}
#endif

//-----------------------------------------------------------------------------
SimpleTextEditor::SimpleTextEditor(const QString& title, const QString& tag, QWidget *parent, const char *name)
: TextEditor(title, tag, parent, name),
  _type(PURL::Nb_FileTypes),
  _file(_sview)
{}

SimpleTextEditor::SimpleTextEditor(bool withDebugger, QWidget *parent, const char *name)
: TextEditor(withDebugger, parent, name),
  _type(PURL::Nb_FileTypes),
  _file(_sview)
{}

bool SimpleTextEditor::open(const PURL::Url &url)
{
  _type = url.fileType();
  return TextEditor::open(url);
}

bool SimpleTextEditor::setText(const QString &text)
{
  _file.openForWrite();
  _file.appendText(text);
  _file.close();
  if ( !_document->openUrl(_file.url().kurl()) ) return false;
  highlightModeChanged();
  modeChanged();
  return true;
}
