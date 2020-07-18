/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "device_gui.h"
#include "device_gui.moc"

#include <set>

#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "devices/gui/device_group_ui.h"
#include "progs/list/prog_list.h"
#include "progs/list/device_info.h"
#include "tools/list/tool_list.h"
#include "common/gui/container.h"

//-----------------------------------------------------------------------------
void DeviceChooser::Config::writeProgrammerGroup(const Programmer::Group *group)
{
  Config config;
  config.writeEntry("programmer", group ? group->name() : QString::null);
}
const Programmer::Group *DeviceChooser::Config::programmerGroup()
{
  Config config;
  QString name = config.readEntry("programmer", QString::null);
  return Programmer::Lister::instance().group(name);
}

void DeviceChooser::Config::writeToolGroup(const Tool::Group *group)
{
  Config config;
  config.writeEntry("tool", group ? group->name() : QString::null);
}
const Tool::Group *DeviceChooser::Config::toolGroup()
{
  Config config;
  QString name = config.readEntry("tool", QString::null);
  return Tool::Lister::instance().group(name);
}

void DeviceChooser::Config::writePackage(const QString &name)
{
  Config config;
  config.writeEntry("package", name);
}
QString DeviceChooser::Config::package()
{
  Config config;
  return config.readEntry("package", "<all>");
}

void DeviceChooser::Config::writeNbPins(uint nb)
{
  Config config;
  config.writeEntry("nb_pins", nb);
}
uint DeviceChooser::Config::nbPins()
{
  Config config;
  return config.readUIntEntry("nb_pins", 0);
}

//-----------------------------------------------------------------------------
namespace DeviceChooser
{

class ListItem : public KListViewItem
{
public:
  ListItem(KListView *list, const QString &name, bool selectable, bool isDevice)
    : KListViewItem(list, name), _device(isDevice) {
    setSelectable(selectable);
  }
  ListItem(KListViewItem *item, const QString &name)
    : KListViewItem(item, name), _device(true) {}

  bool isDevice() const { return _device; }
  virtual void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align) {
    QColorGroup ncg = cg;
    if (_device) {
      const Device::Data *device = Device::Lister::instance().data(text(0));
      ASSERT(device);
      ncg.setColor(QColorGroup::Text, Device::statusColor(device->status()));
    }
    KListViewItem::paintCell(p, ncg, column, width, align);
  }

private:
  bool _device;
};

} // namespace

//-----------------------------------------------------------------------------
const DeviceChooser::ListType::Data DeviceChooser::ListType::DATA[Nb_Types] = {
  { "family_tree", I18N_NOOP("Family Tree") },
  { "flat",        I18N_NOOP("Flat List")   }
};

DeviceChooser::Dialog::Dialog(const QString &device, QWidget *parent)
  : ::Dialog(parent, "device_chooser_dialog", true, i18n("Select a device"),
             Ok|Close, Close, false, QSize(400, 300))
{
  setButtonGuiItem(Ok, KGuiItem(i18n( "&Select"), "dialog-ok"));
  QVBoxLayout *top = new QVBoxLayout(mainWidget(), 0, 10);

  // view
  QHBoxLayout *hbox = new QHBoxLayout(top, 10);
  QVBoxLayout *vbox = new QVBoxLayout(hbox);
  _listTypeCombo.reset(new EnumComboBox<ListType>("list_type", mainWidget()));
  connect(_listTypeCombo->combo(), SIGNAL(activated(int)), SLOT(updateList()));
  vbox->addWidget(_listTypeCombo->combo());
  QPushButton *button = new KPushButton(KGuiItem(i18n("Reset Filters"), "view-refresh"), mainWidget());
  connect(button, SIGNAL(clicked()), SLOT(resetFilters()));
  vbox->addWidget(button);
  vbox->addStretch(1);

  // filters
  Container::Grid *container = new Container::Grid(mainWidget(), Container::Raised);
  hbox->addWidget(container);
  QLabel *label = new QLabel(i18n("Filters:"), container);
  container->addWidget(label, 0,0, 0,0);
  Container::Shrinkable *fcontainer = new Container::Shrinkable(container);
  container->addWidget(fcontainer, 0,0, 1,1);

  // programmer filter
  _programmerCombo.reset(new KeyComboBox<QString>(fcontainer));
  _programmerCombo->appendItem("<all>", i18n("<Programmer>"));
  for (Programmer::Lister::ConstIterator pit = Programmer::Lister::instance().begin();
       pit != Programmer::Lister::instance().end();
       ++pit)
    _programmerCombo->appendItem(pit.key(), pit.data()->label());
  const Programmer::Group *pgroup = Config::programmerGroup();
  if (pgroup) _programmerCombo->setCurrentItem(pgroup->name());
  connect(_programmerCombo->widget(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_programmerCombo->widget());

  // tool filter
  _toolCombo.reset(new KeyComboBox<QString>(fcontainer));
  _toolCombo->appendItem("<all>", i18n("<Toolchain>"));
  for (Tool::Lister::ConstIterator tit = Tool::Lister::instance().begin();
       tit != Tool::Lister::instance().end();
       ++tit) {
    if ( tit.data()->isCustom() ) continue;
    _toolCombo->appendItem(tit.key(), tit.data()->label());
  }
  const Tool::Group *tgroup = Config::toolGroup();
  if (tgroup) _toolCombo->setCurrentItem(tgroup->name());
  connect(_toolCombo->widget(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_toolCombo->widget());

  // memory filter
  _memoryCombo.reset(new EnumComboBox<Device::MemoryTechnology>(i18n("<Memory Type>"), "memory_technology", fcontainer));
  connect(_memoryCombo->combo(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_memoryCombo->combo());

  // status filter
  _statusCombo.reset(new EnumComboBox<Device::Status>(i18n("<Status>"), "status", fcontainer));
  connect(_statusCombo->combo(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_statusCombo->combo());

  // features filter
  _featureCombo.reset(new EnumComboBox<Pic::Feature>(i18n("<Feature>"), "feature", fcontainer));
  connect(_featureCombo->combo(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_featureCombo->combo());

  // package filter
  _packageCombo.reset(new KeyComboBox<QString>(fcontainer));
  _packageCombo->appendItem("<all>", i18n("<Package>"));
  QStringList names;
  for (uint i=0; Device::Package::TYPE_DATA[i].name; i++) names += Device::Package::TYPE_DATA[i].name;
  qSort(names);
  for (int i=0; i<names.count(); i++) _packageCombo->appendItem(names[i], i18n(Device::Package::data(names[i])->label));
  _packageCombo->setCurrentItem(Config::package());
  connect(_packageCombo->widget(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_packageCombo->widget());

  // no. of pins filter
  _pinsCombo.reset(new KeyComboBox<uint>(fcontainer));
  _pinsCombo->appendItem(0, i18n("<No. of Pins>"));
  std::set<uint> set;
  for (uint i=0; Device::Package::TYPE_DATA[i].name; i++) {
    const Device::Package::TypeData &tdata = Device::Package::TYPE_DATA[i];
    for (uint k=0; k<Device::Package::MAX_NB; k++) {
      if ( tdata.nbPins[k]==0 ) continue;
      set.insert(tdata.nbPins[k]);
    }
  }
  for (std::set<uint>::const_iterator it = set.begin(); it != set.end(); ++it) {
    _pinsCombo->appendItem(*it, QString::number(*it));
  }
  _pinsCombo->setCurrentItem(Config::nbPins());
  connect(_pinsCombo->widget(), SIGNAL(activated(int)), SLOT(updateList()));
  fcontainer->appendWidget(_pinsCombo->widget());

  // list view
  QValueList<int> widths;
  widths += 80;
  widths += 500;
  Splitter *splitter = new Splitter(widths, Qt::Horizontal, mainWidget(), "device_shooser_splitter");
  top->addWidget(splitter, 1);
  _listView = new ListView(splitter);
  connect(_listView, SIGNAL(currentChanged(Q3ListViewItem *)),
          SLOT(currentChanged(Q3ListViewItem *)));
  connect(_listView, SIGNAL(doubleClicked(Q3ListViewItem *, const QPoint &, int)),
          SLOT(listDoubleClicked(Q3ListViewItem *)));
  _listView->setAllColumnsShowFocus(true);
  _listView->setRootIsDecorated(true);
  _listView->setSorting(-1);
  _listView->addColumn(i18n("Device"));
  _listView->setResizeMode(Q3ListView::LastColumn);

  // device view
  _deviceView = new View(splitter);
  connect(_deviceView, SIGNAL(deviceChanged(const QString &)),
          SLOT(deviceChange(const QString &)));

  updateList(device);
}

DeviceChooser::Dialog::~Dialog()
{
  Config::writeProgrammerGroup(programmerGroup());
  Config::writeToolGroup(toolGroup());
  _listTypeCombo->writeConfig();
  _memoryCombo->writeConfig();
  _statusCombo->writeConfig();
  _featureCombo->writeConfig();
  Config::writePackage(_packageCombo->currentItem());
  Config::writeNbPins(_pinsCombo->currentItem());
}

QString DeviceChooser::Dialog::device() const
{
  Q3ListViewItem *item = _listView->selectedItem();
  if ( item==0 ) return QString::null;
  return item->text(0);
}

void DeviceChooser::Dialog::listDoubleClicked(Q3ListViewItem *item)
{
  if ( item==0 ) return;
  if ( !static_cast<ListItem *>(item)->isDevice() ) item->setOpen(!item->isOpen());
  else accept();
}

void DeviceChooser::Dialog::currentChanged(Q3ListViewItem *item)
{
  if ( item==0 || !static_cast<ListItem *>(item)->isDevice() ) _deviceView->clear();
  else _deviceView->setDevice(item->text(0), false);
}

void DeviceChooser::Dialog::deviceChange(const QString &name)
{
  Q3ListViewItemIterator it(_listView);
  for (; it.current(); ++it)
    if ( it.current()->text(0)==name ) {
      _listView->setSelected(it.current(), true);
      _listView->ensureItemVisible(it.current());
      break;
    }
}

void DeviceChooser::Dialog::resetFilters()
{
  _programmerCombo->setCurrentItem("<all>");
  _toolCombo->setCurrentItem("<all>");
  _memoryCombo->reset();
  _statusCombo->reset();
  _featureCombo->reset();
  _packageCombo->setCurrentItem("<all>");
  _pinsCombo->setCurrentItem(0);
  updateList();
}

void DeviceChooser::Dialog::updateList()
{
  Q3ListViewItem *item = _listView->selectedItem();
  QString device = (item ? item->text(0) : QString::null);
  _listView->clear();
  updateList(device);
}

const Programmer::Group *DeviceChooser::Dialog::programmerGroup() const
{
  return Programmer::Lister::instance().group(_programmerCombo->currentItem());
}

const Tool::Group *DeviceChooser::Dialog::toolGroup() const
{
  return Tool::Lister::instance().group(_toolCombo->currentItem());
}

void DeviceChooser::Dialog::updateList(const QString &device)
{
  const Programmer::Group *pgroup = programmerGroup();
  if ( pgroup && pgroup->supportedDevices().isEmpty() && pgroup->isSoftware() ) {
    _deviceView->setText(i18n("Could not detect supported devices for \"%1\". Please check installation.").arg(pgroup->label()));
    return;
  }
  const Tool::Group *tgroup = toolGroup();
  if ( tgroup && tgroup->supportedDevices().isEmpty() ) {
    _deviceView->setText(i18n("Could not detect supported devices for toolchain \"%1\". Please check installation.").arg(tgroup->label()));
    return;
  }

  QMap<QString, KListViewItem *> groups;
  Q3ListViewItem *selected = NULL;
  const QStringList list = Device::Lister::instance().supportedDevices();
  QStringList::const_iterator it = list.end();
  for (it--; it!=list.begin(); it--) {
    if ( pgroup && !pgroup->isSupported(*it) ) continue;
    if ( tgroup && !tgroup->isSupported(*it) ) continue;
    const Device::Data *data = Device::Lister::instance().data(*it);
    ASSERT(data);
    if ( _memoryCombo->value()!=Device::MemoryTechnology::Nb_Types && data->memoryTechnology()!=_memoryCombo->value() ) continue;
    if ( _statusCombo->value()!=Device::Status::Nb_Types && data->status()!=_statusCombo->value() ) continue;
    if ( _featureCombo->value()!=Pic::Feature::Nb_Types ) {
      if ( data->group().name()!="pic" ) continue;
      if ( !static_cast<const Pic::Data *>(data)->hasFeature(_featureCombo->value()) ) continue;
    }
    if ( _packageCombo->currentItem()!="<all>" ) {
      QValueVector<Device::Package>::const_iterator it;
      for (it=data->packages().begin(); it!=data->packages().end(); ++it)
        if ( (*it).names().contains(_packageCombo->currentItem()) ) break;
      if ( it==data->packages().end() ) continue;
    }
    if ( _pinsCombo->currentItem()!=0 ) {
      QValueVector<Device::Package>::const_iterator it;
      for (it=data->packages().begin(); it!=data->packages().end(); ++it)
        if ( (*it).nbPins()==_pinsCombo->currentItem() ) break;
      if ( it==data->packages().end() ) continue;
    }
    KListViewItem *item = 0;
    switch (_listTypeCombo->value().type()) {
      case ListType::FamilyTree: {
        QString gname = data->listViewGroup();
        if ( !groups.contains(gname) )
          groups[gname] = new ListItem(_listView, gname, false, false);
        item = new ListItem(groups[gname], *it);
        break;
      }
      case ListType::Flat:
        item = new ListItem(_listView, *it, true, true);
        break;
      case ListType::Nb_Types: ASSERT(false); break;
    }
    if ( device==(*it) ) selected = item;
  }
  if ( selected==0 ) selected = _listView->firstChild();
  if (selected) {
    _listView->setSelected(selected, true);
    _listView->ensureItemVisible(selected);
    currentChanged(selected);
  }
}

//-----------------------------------------------------------------------------
DeviceChooser::ComboBox::ComboBox(QWidget *parent)
  : KComboBox(parent)
{
  setObjectName("device_chooser_combobox");
  for (Device::Lister::ConstIterator it = Device::Lister::instance().begin();
       it != Device::Lister::instance().end();
       ++it) {
    QStringList devices = it.data()->supportedDevices();
    qSort(devices);
    for (QStringList::const_iterator it = devices.begin(); it != devices.end(); ++it) insertItem(*it);
  }
}

void DeviceChooser::ComboBox::setDevice(const QString &device)
{
  if ( !device.isEmpty() ) setCurrentText(device);
}

//-----------------------------------------------------------------------------
DeviceChooser::Button::Button(QWidget *parent)
  : QWidget(parent, "device_chooser_button")
{
  QHBoxLayout *hbox = new QHBoxLayout(this, 0, 10);
  _combo = new ComboBox(this);
  connect(_combo, SIGNAL(activated(int)), SIGNAL(changed()));
  hbox->addWidget(_combo);
  KPushButton *button = new KPushButton(KIcon("document-open"), QString::null, this);
  connect(button, SIGNAL(clicked()), SLOT(chooseDevice()));
  hbox->addWidget(button);
}

void DeviceChooser::Button::chooseDevice()
{
  Dialog dialog(_combo->device(), this);
  if ( !dialog.exec() || dialog.device().isEmpty() ) return;
  _combo->setDevice(dialog.device());
  emit changed();
}

//-----------------------------------------------------------------------------
DeviceChooser::Browser::Browser(QWidget *parent)
 : KTextBrowser(parent),
   _data(NULL)
{}

PURL::Url findDocumentUrl(const QString &prefix, const QString &baseName)
{
  PURL::Url previous = KUrl(prefix + baseName + ".pdf");
  bool previousExists = previous.exists();
  for (uint i=0; i<26; i++) {
    PURL::Url url = KUrl(prefix + baseName + QChar('a' + i) + ".pdf");
    bool exists = url.exists();
    if ( !exists && previousExists ) return previous;
    previous = url;
    previousExists = exists;
  }
  return previous;
}

void DeviceChooser::Browser::setSource(const QUrl& url)
{
  BusyCursorStarter bc;
  if (url.scheme() == "device") emit deviceChanged(url.toString(QUrl::RemoveScheme));
  else if (url.scheme() == "document") {
    QString prefix = "http://ww1.microchip.com/downloads/en/DeviceDoc/";
    PURL::Url foundUrl = findDocumentUrl(prefix, url.toString(QUrl::RemoveScheme));
    KTextBrowser::setSource(foundUrl.kurl());
  } else KTextBrowser::setSource(url);
}

void DeviceChooser::Browser::setData(const Device::Data* data)
{
  _data = data;
}

QVariant DeviceChooser::Browser::loadResource(int type, const QUrl& url)
{
  if (type != QTextDocument::ImageResource) return KTextBrowser::loadResource(type, url);
  QString label = url.path();
  if (label.startsWith("vf_")) return voltageFrequencyPixmap(*_data, label.mid(3));
  else if (label.startsWith("pins_")) return pinsPixmap(*_data);
  else return _data->group().memoryGraph(*_data);
}

//-----------------------------------------------------------------------------
DeviceChooser::View::View(QWidget *parent)
  : TabWidget(parent, "device_view")
{
  // Information
  _info = new Browser(this);
  insertTab(_info, i18n("Information"));
  connect(_info, SIGNAL(deviceChanged(const QString &)),
          SIGNAL(deviceChanged(const QString &)));

  // Memory Map
  _memory = new Browser(this);
  insertTab(_memory, i18n("Memory Map"));

  // Voltage-Frequency Graphs
  _vfg = new Browser(this);
  insertTab(_vfg, i18n("Voltage-Frequency Graphs"));

  // Pin Diagrams
  _pins = new Browser(this);
  insertTab(_pins, i18n("Pin Diagrams"));
}

void DeviceChooser::View::setDevice(const QString &name, bool cannotChangeDevice)
{
  const Device::Data *data = Device::Lister::instance().data(name);
  if (data == NULL) return;
  QString doc = htmlInfo(*data, (cannotChangeDevice ? "" : "device:%1"), Device::documentHtml(*data));
  doc += Device::supportedHtmlInfo(*data);
  _info->setText("<html><body>" + doc + "</body></html>");
  _info->setData(data);
  doc = voltageFrequencyHtml(*data, "vf_");
  QString label = data->name() + "_memory_map.png";
  _memory->setText("<html><body><img src=\"" + label + "\" /></body></html>");
  _memory->setData(data);
  _vfg->setText("<html><body>" + doc + "</body></html>");
  _vfg->setData(data);
  doc = pinsHtml(*data, "pins_");
  _pins->setText("<html><body>" + doc + "</body></html>");
  _pins->setData(data);
}

void DeviceChooser::View::setText(const QString &text)
{
  _info->setText(text);
  _vfg->setText(text);
  _pins->setText(text);
}

void DeviceChooser::View::clear()
{
  _info->clear();
  _vfg->clear();
  _pins->clear();
}

//-----------------------------------------------------------------------------
DeviceChooser::Editor::Editor(const QString &title, const QString &tag, QWidget *widget)
  : DeviceEditor(title, tag, widget, "device_view_editor")
{}

QWidget *DeviceChooser::Editor::createView(const Device::Data& data, QWidget *parent)
{
  DeviceChooser::View *view = new DeviceChooser::View(parent);
  connect(view, SIGNAL(deviceChanged(const QString &)), SIGNAL(deviceChanged(const QString &)));
  view->setDevice(data.name(), true);
  return view;
}
