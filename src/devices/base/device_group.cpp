/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "device_group.h"

#include "common/gui/global_ui.h"

QColor Device::statusColor(Status status)
{
  switch (status.type()) {
    case Status::Future:         return Qt::blue;
    case Status::InProduction:   return Qt::green;
    case Status::Mature:
    case Status::NotRecommended: return QColor("orange");
    case Status::EOL:            return Qt::red;
    case Status::Unknown:
    case Status::Nb_Types: break;
  }
  return Qt::black;
}

QString coloredString(const QString &text, QColor color)
{
  return QString("<font color=\"") + color.name() + "\">" + text + "</font>";
}

QString supportedString(bool supported)
{
  return coloredString(supported ? i18n("Supported") : i18n("Unsupported"),
                       supported ? Qt::green :  Qt::red);
}

class Tick {
public:
  Tick() {}
  Tick(double value, double oValue) {
    s = formatNumber(value, 1);
    min = oValue;
  }
  QString s;
  double min;
};

class TickMap : public QMap<double, Tick>
{
public:
  TickMap() {}
  void add(double value, double oValue) {
    insert(value, Tick(value, oValue), false);
    (*this)[value].min = QMIN((*this)[value].min, oValue);
  }
};

QPixmap drawGraph(const QValueVector<Device::RangeBox> &boxes)
{
  const uint w = 300, h = 200;
  QPixmap pixmap(w, h);
  pixmap.fill(Qt::white);
  QPainter p(&pixmap);
  QFontMetrics f(p.font());
  TickMap xTicks, yTicks;
  xTicks.add(0.0, 0.0);
  yTicks.add(0.0, 0.0);
  for (int i=0; i<boxes.count(); i++) {
//    qDebug("box #%i: %f=[%f %f] %f=[%f %f]", i, boxes[i].start.x, boxes[i].start.yMin,
//           boxes[i].start.yMax, boxes[i].end.x, boxes[i].end.yMin, boxes[i].end.yMax);
    xTicks.add(boxes[i].start.x, boxes[i].start.yMin);
    xTicks.add(boxes[i].start.x, boxes[i].start.yMax);
    xTicks.add(boxes[i].end.x, boxes[i].end.yMin);
    xTicks.add(boxes[i].end.x, boxes[i].end.yMax);
    yTicks.add(boxes[i].start.yMin, boxes[i].start.x);
    yTicks.add(boxes[i].start.yMax, boxes[i].start.x);
    yTicks.add(boxes[i].end.yMin, boxes[i].end.x);
    yTicks.add(boxes[i].end.yMax, boxes[i].end.x);
  }
  double xMax = 0.0, yMax = 0.0;
  int xStart = 0;
  int yStart = h-1 - f.lineSpacing();
  TickMap::const_iterator it = xTicks.begin();
  for (; it!=xTicks.end(); ++it) {
    xStart = QMAX(xStart, f.width(it.data().s));
    xMax = QMAX(xMax, it.key());
  }
  for (it = yTicks.begin(); it!=yTicks.end(); ++it)
    yMax = QMAX(yMax, it.key());
  int xEnd = w-1 - f.width(xTicks[xMax].s)/2;
  QRect rect = f.boundingRect(yTicks[yMax].s);
  int yEnd = rect.height()/2;

  // draw boxes
  p.setPen(Qt::lightGray);
  p.setBrush(Qt::lightGray);
  for (int i=0; i<boxes.count(); i++) {
    double ax = double(xEnd - xStart)/xMax;
    double ay = double(yEnd - yStart)/yMax;
    Q3PointArray pa(4);
    pa.setPoint(0, qRound(ax*boxes[i].start.x), qRound(ay*boxes[i].start.yMin));
    pa.setPoint(1, qRound(ax*boxes[i].end.x), qRound(ay*boxes[i].end.yMin));
    pa.setPoint(2, qRound(ax*boxes[i].end.x), qRound(ay*boxes[i].end.yMax));
    pa.setPoint(3, qRound(ax*boxes[i].start.x), qRound(ay*boxes[i].start.yMax));
    pa.translate(xStart, yStart);
    p.drawPolygon(pa);
  }

  // draw axis
  p.setPen(Qt::black);
  p.drawLine(xStart, yStart, w-1, yStart);
  p.drawLine(xStart, yStart, xStart, 0);

  // draw ticks and lines
  p.setPen(Qt::DotLine);
  for (it = yTicks.begin(); it!=yTicks.end(); ++it) {
    int y1 = yStart + qRound(it.key()*(yEnd-yStart)/yMax);
    QRect rect = f.boundingRect(it.data().s);
    p.drawText(xStart/2-rect.width()/2 , y1+rect.height()/2, it.data().s);
    int xmin = xStart + qRound(it.data().min*(xEnd-xStart)/xMax);
    p.drawLine(xStart, y1, xmin, y1);
  }
  for (it = xTicks.begin(); it!=xTicks.end(); ++it) {
    int x1 = xStart + qRound(it.key()*(xEnd-xStart)/xMax);
    QRect rect = f.boundingRect(it.data().s);
    p.drawText(x1-rect.width()/2, h-1, it.data().s);
    int ymin = yStart + qRound(it.data().min*(yEnd-yStart)/yMax);
    p.drawLine(x1, yStart, x1, ymin);
  }

  return pixmap;
}

QPixmap Device::vddGraph(const QString &xLabel, const QString &yLabel,
                         const QValueVector<Device::RangeBox> &boxes)
{
  QFontMetrics fm(qApp->font());
  uint sp = 10;
  const QPixmap graph = drawGraph(boxes);
  QPixmap pixmap(graph.width() + sp + fm.width(xLabel), graph.height() + sp + fm.lineSpacing());
  pixmap.fill(Qt::white);
  copyBlt(&pixmap, 0, fm.lineSpacing() + sp, &graph, 0, 0, graph.width(), graph.height());

  QPainter p(&pixmap);
  p.setPen(Qt::black);
  p.drawText(0, fm.lineSpacing(), yLabel);
  p.drawText(pixmap.width()-1-fm.width(xLabel), pixmap.height()-1, xLabel);
  return pixmap;
}

const Device::Package *Device::barPackage(const char *name, const Device::Data &data)
{
  for (int i=0; i<data.packages().count(); i++)
    if ( data.packages()[i].names().contains(name) ) return &data.packages()[i];
  return 0;
}

QPixmap Device::pinsGraph(const Device::Package &package)
{
  QFontMetrics fm(qApp->font());
  uint nb = package.nbPins();
  const int hspacing = 3, wspacing = 3, wmark = 10, wpin = 4;
  int theight = fm.ascent() + (fm.ascent()%2==0 ? 1 : 0);
  int height = hspacing + (nb/2)*(hspacing + theight);
  int wnumber = fm.width("1");
  wnumber = QMAX(wnumber, fm.width(QString::number(nb/2)));
  wnumber = QMAX(wnumber, fm.width(QString::number(nb/2+1)));
  wnumber = QMAX(wnumber, fm.width(QString::number(nb)));
  int bwidth = 4*wspacing + 2*wnumber + wmark;
  int lwidth = 0, rwidth = 0;
  for (uint k=0; k<nb/2; k++) {
    lwidth = QMAX(lwidth, fm.width(package.pinName(k)));
    rwidth = QMAX(rwidth, fm.width(package.pinName(nb-k-1)));
  }
  int bx = lwidth + wspacing + wpin;
  int width = bx + bwidth + wpin + wspacing + rwidth;

  QPixmap pixmap(width, height);
  pixmap.fill(Qt::white);
  QPainter p(&pixmap);
  p.setPen(QPen(Qt::black, 2));
  p.drawRect(bx, 1, bwidth, height-1);
  p.drawArc(bx+wspacing+wnumber+wspacing, -wmark/2+2, wmark, wmark, 0, -180*16);
  for (uint k=0; k<nb/2; k++) {
    int h = hspacing + theight/2 + k*(hspacing + theight);
    p.drawLine(bx-wpin-1, h, bx, h);
    p.drawLine(bx+bwidth, h, bx+bwidth+wpin, h);
    h += theight/2;
    QString label = package.pinName(k);
    p.drawText(bx-wpin-wspacing-fm.width(label), h, label);
    p.drawText(bx+bwidth+wpin+wspacing, h, package.pinName(nb-k-1));
    uint pin = (k+1);
    if ( pin==1 || pin==(nb/2) ) {
      p.drawText(bx+wspacing, h, QString::number(pin));
      label = QString::number(nb-k);
      p.drawText(bx+bwidth-wspacing-fm.width(label), h, label);
    }
  }

  return pixmap;
}

QString Device::htmlInfo(const Device::Data &data, const QString &deviceHref, const QString &documentHtml)
{
  QString doc;

  // title
  doc += "<h1>";
  bool first = true;
  FOR_EACH(Special, special) {
    for (int k=0; k<data.frequencyRanges().count(); k++) {
      if ( data.frequencyRanges()[k].special!=special ) continue;
      if (first) first = false;
      else doc += " / ";
      doc += data.fname(special);
      break;
    }
  }
  doc += "</h1>";

  doc += "<table>";
  QString status = coloredString(data.status().label(), statusColor(data.status()));
  doc += htmlTableRow(i18n("Status"), status);
  if ( data.alternatives().count() ) {
    QString s;
    for (int i=0; i<data.alternatives().count(); i++) {
      if ( i!=0 ) s += ", ";
      if ( deviceHref.isEmpty() ) s += data.alternatives()[i].upper();
      else {
        QString href = deviceHref.arg(data.alternatives()[i].upper());
        s += QString("<a href=\"%1\">%2</a>").arg(href).arg(data.alternatives()[i].upper());
      }
    }
    doc += htmlTableRow(i18n("Alternatives"), s);
  }
  doc += documentHtml;
  doc += "</table>";

  doc += "<hr />";
  doc += "<table>";
  doc += data.group().informationHtml(data);
  QString s;
  for (int i=0; i<data.packages().count(); i++) {
    QStringList names = data.packages()[i].names();
    for (uint k=0; k<uint(names.count()); k++)
      s += i18n(Package::data(names[k])->label) + QString("[%1] ").arg(data.packages()[i].nbPins());
  }
  doc += htmlTableRow(i18n("Packaging"), s);
  doc += "</table>";

  return doc;
}

const Device::Package* Device::preferredPackage(const Device::Data& data)
{
  for (uint i=0; Package::TYPE_DATA[i].name; i++) {
    if ( Package::TYPE_DATA[i].shape!=Package::Bar ) continue;
    const Package* package = barPackage(Package::TYPE_DATA[i].name, data);
    if (package) return package;
  }
  return NULL;
}

QString Device::pinsHtml(const Device::Data &data, const QString &imagePrefix)
{
  QString doc;
  const Package* package = preferredPackage(data);
  if (package) {
    doc += "<table cellpadding=\"3\"><tr bgcolor=\"gray\"><th align=\"center\">";
    QStringList names = package->names();
    for (int k=0; k<names.count(); k++) {
      if ( k!=0 ) doc += " ";
      doc += i18n(Package::data(names[k])->label);
      doc += "(" + QString::number(package->nbPins()) + ")";
    }
    doc += "</th></tr><tr><td align=\"center\">";
    doc += "<img src=\"" + imagePrefix + pinsPixmapLabel(data) + "\" />";
    doc += "</td></tr></table>";
  }
  return doc;
}

QString Device::pinsPixmapLabel(const Device::Data& data)
{
  return data.name() + "_pins_graph.png";
}

QPixmap Device::pinsPixmap(const Device::Data& data)
{
  const Package* package = preferredPackage(data);
  if (package) return pinsGraph(*package);
  return QPixmap();
}

QString Device::voltageFrequencyHtml(const Device::Data &data, const QString &imagePrefix)
{
  QString doc;
  for (int k=0; k<data.frequencyRanges().count(); k++) {
    const Device::FrequencyRange &fr = data.frequencyRanges()[k];
    doc += "<h3>" + data.fname(fr.special) + " - " + i18n("Temperature range: ") + fr.operatingCondition.label() + "</h3>";
    doc += "<img src=\"" + imagePrefix + voltagePixmapLabel(data, fr) + "\" />";
  }
  return doc;
}

QPixmap Device::voltageFrequencyPixmap(const Device::Data &data, const QString &name)
{
  for (int k=0; k<data.frequencyRanges().count(); k++) {
    const Device::FrequencyRange &fr = data.frequencyRanges()[k];
    if (voltagePixmapLabel(data, fr) == name) return Device::voltagePixmap(fr);
  }
  return QPixmap();
}

QString Device::voltagePixmapLabel(const Device::Data &data, const Device::FrequencyRange &fr)
{
  return data.name() + "_" + data.fname(fr.special) + "_" + fr.operatingCondition.key() + ".png";
}

QPixmap Device::voltagePixmap(const Device::FrequencyRange &fr)
{
  return Device::vddGraph(i18n("F (MHz)"), i18n("Vdd (V)"), fr.vdds);
}

bool Device::MemoryGraphData::operator<(const Device::MemoryGraphData &data) const
{
  return startAddress < data.startAddress;
}

QPixmap Device::memoryGraph(const std::list<MemoryGraphData> &r)
{
  // order
  std::list<MemoryGraphData> ranges = r;
  ranges.sort();
  // add empty ranges
  for (std::list<MemoryGraphData>::iterator it = ranges.begin(); it != ranges.end(); ) {
    std::list<MemoryGraphData>::iterator prev = it;
    ++it;
    if ( it==ranges.end() ) break;
    if ( (*prev).endAddress+1==(*it).startAddress ) continue;
    MemoryGraphData data;
    data.startAddress = (*prev).endAddress + 1;
    data.endAddress = (*it).startAddress-1;
    ranges.insert(it, data);
  }

  // compute widths and total height
  QFontMetrics fm(qApp->font());
  int theight = fm.ascent() + (fm.ascent()%2==0 ? 1 : 0);
  int hspacing = 5;
  int height = 1;
  int w1 = 0, w2 = 0;
  for (std::list<MemoryGraphData>::iterator it = ranges.begin(); it != ranges.end(); ++it) {
    w1 = QMAX(w1, fm.width((*it).start));
    w1 = QMAX(w1, fm.width((*it).end));
    w2 = QMAX(w2, fm.width((*it).label));
    (*it).height = 2*hspacing + theight;
    if ( (*it).startAddress!=(*it).endAddress ) (*it).height += 2*theight;
    height += (*it).height;
  }
  int wspacing = 4;
  int width = wspacing + w1 + wspacing + wspacing + w2;

  // draw
  QPixmap pixmap(width, height);
  pixmap.fill(Qt::white);
  QPainter p(&pixmap);
  int h = 0;
  // draw ranges
  for (std::list<MemoryGraphData>::const_iterator it = ranges.begin(); it != ranges.end(); ++it) {
    p.setPen(QPen(Qt::black, 1, Qt::DotLine));
    p.drawLine(0,h, width-1,h);
    p.setPen(QPen(Qt::black, 1));
    p.setBrush((*it).label.isEmpty() ? Qt::gray : Qt::white);
    p.drawRect(0,h, wspacing+w1+wspacing,(*it).height+1);
    int hmid = h+(*it).height/2+theight/2;
    p.drawText(wspacing+w1+wspacing+wspacing,hmid, (*it).label);
    if ( (*it).startAddress==(*it).endAddress ) p.drawText(wspacing,hmid, (*it).start);
    else {
      p.drawText(wspacing,h+theight, (*it).start);
      p.drawText(wspacing,h+(*it).height-3, (*it).end);
    }
    h += (*it).height;
    p.setPen(QPen(Qt::black, 1, Qt::DotLine));
    p.drawLine(0,h, width-1,h);
  }

  return pixmap;
}
