/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gen_device_xml.h"

#include <qfile.h>

#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "devices/pic/base/pic.h"
#include "devices/mem24/base/mem24.h"
#include "devices/pic/base/pic_config.h"

//-----------------------------------------------------------------------------
void DomComment::save(QTextStream &s, uint, const QValueList<uint> &) const
{
  Q_ASSERT( !_text.contains("--") );
  s << "<!--*" << _text.leftJustify(79-9, '*') << "*-->" << endl;
}

DomParent::~DomParent()
{
  QValueList<DomChild *>::const_iterator cit;
  for (cit=_childs.begin(); cit!=_childs.end(); ++cit) delete (*cit);
}

void DomElement::computeAlign(QValueList<uint> &align) const
{
  for (uint k=0; k<_attributes.count(); k++) {
    uint width = _attributes[k].save().length();
    if ( k==align.count() ) align.append(width);
    else align[k] = qMax(align[k], width) ;
  }
}

void DomParent::saveChilds(QTextStream &s, uint indent) const
{
  QMap<QString, QValueList<uint> > aligns;
  QValueList<DomChild *>::const_iterator cit;
  for (cit=_childs.begin(); cit!=_childs.end(); ++cit) {
    if ( !(*cit)->isElement() ) continue;
    const DomElement *element = static_cast<const DomElement *>(*cit);
    element->computeAlign(aligns[element->tag()]);
  }
  for (cit=_childs.begin(); cit!=_childs.end(); ++cit) {
    QValueList<uint> align;
    if ( (*cit)->isElement() ) align = aligns[static_cast<const DomElement *>(*cit)->tag()];
    (*cit)->save(s, indent, align);
  }
}

void DomElement::save(QTextStream &s, uint indent, const QValueList<uint> &align) const
{
  Q_ASSERT( !_tag.contains("<") && !_tag.contains(">") );
  s << repeat(" ", indent) << "<" << _tag << " ";
  Q_ASSERT( align.count()>=_attributes.count() );
  for (uint i=0; i<_attributes.count(); i++)
    s << _attributes[i].save().leftJustify(align[i], ' ');
  if ( _childs.isEmpty() ) s << "/>" << endl;
  else s << ">" << endl;
  saveChilds(s, indent+2);
  if ( !_childs.isEmpty() ) s << repeat(" ", indent) << "</" << _tag << ">" << endl;
}

void DomDocument::save(QTextStream &s) const
{
  Q_ASSERT( !_name.contains("<") && !_name.contains(">") );
  s << "<!DOCTYPE " << _name << ">" << endl;
  saveChilds(s, 0);
}

//-----------------------------------------------------------------------------
DomDocument *createDocument(const Device::Data &data)
{
  DomDocument *doc = new DomDocument("piklab_device");
  doc->append(new DomComment(""));
  doc->append(new DomComment(" Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>            "));
  doc->append(new DomComment("                                                                      "));
  doc->append(new DomComment(" This program is free software; you can redistribute it and/or modify "));
  doc->append(new DomComment(" it under the terms of the GNU General Public License as published by "));
  doc->append(new DomComment(" the Free Software Foundation; either version 2 of the License, or    "));
  doc->append(new DomComment(" (at your option) any later version.                                  "));
  doc->append(new DomComment(""));

  DomElement *root = new DomElement("device");
  doc->append(root);
  root->appendAttribute("name", data.name());
  root->appendAttribute("status", data.status().key());
  if ( !data.alternatives().isEmpty() ) root->appendAttribute("alternatives", data.alternatives().join(" "));
  root->appendAttribute("memory_technology", data.memoryTechnology().key());
  root->append(new DomEmptyLine);
  root->append(new DomComment(" Operating characteristics "));
  for (uint i=0; i<data.frequencyRanges().count(); i++) {
    const Device::FrequencyRange &fr = data.frequencyRanges()[i];
    DomElement *fre = new DomElement("frequency_range");
    root->append(fre);
    fre->appendAttribute("name", fr.operatingCondition.key());
    if ( fr.special!=Device::Special::Normal ) fre->appendAttribute("special", fr.special.key());
    for (uint k=0; k<fr.vdds.count(); k++) {
      const Device::RangeBox &vdd = fr.vdds[k];
      DomElement *rbe = new DomElement("frequency");
      fre->append(rbe);
      rbe->appendAttribute("start", QString::number(vdd.start.x));
      rbe->appendAttribute("end", QString::number(vdd.end.x));
      rbe->appendAttribute("vdd_min", QString::number(vdd.start.yMin));
      rbe->appendAttribute("vdd_max", QString::number(vdd.start.yMax));
      if ( vdd.end.yMin!=vdd.start.yMin ) rbe->appendAttribute("vdd_min_end", QString::number(vdd.end.yMin));
      if ( !vdd.osc.isEmpty() ) rbe->appendAttribute("osc", vdd.osc);
      if ( !vdd.mode.isEmpty() ) rbe->appendAttribute("mode", vdd.mode);
      if ( !vdd.special.isEmpty() ) rbe->appendAttribute("special", vdd.special);
    }
  }

  DomElement *de = new DomElement("documents");
  de->appendAttribute("webpage", data.documents().webpage);
  de->appendAttribute("datasheet", data.documents().datasheet);
  de->appendAttribute("progsheet", data.documents().progsheet);
  de->appendAttribute("erratas", data.documents().erratas.join(" "));
  root->append(de);

  if ( data.group().name()=="pic" ) {
    const Pic::Data &pdata = static_cast<const Pic::Data &>(data);
    root->appendAttribute("architecture", pdata.architecture().key());
    if ( pdata.id()!=0 ) root->appendAttribute("id", toHexLabel(pdata.id(), 4));
    root->append(new DomEmptyLine);
    DomElement *ce = new DomElement("checksums");
    root->append(ce);
    QMap<QString, Pic::Checksum::Data>::const_iterator it;
    for (it=pdata.checksums().begin(); it!=pdata.checksums().end(); ++it) {
      DomElement *cde = new DomElement("checksum");
      ce->append(cde);
      cde->appendAttribute("protected", it.key());
      if ( it.data().constant!=0x0000 ) cde->appendAttribute("constant", toHexLabel(it.data().constant, 4));
      if ( it.data().algorithm!=Pic::Checksum::Algorithm::Normal ) cde->appendAttribute("type", it.data().algorithm.key());
      cde->appendAttribute("bchecksum", toHexLabel(it.data().blankChecksum, 4));
      cde->appendAttribute("cchecksum", toHexLabel(it.data().checkChecksum, 4));
    }
    root->append(new DomEmptyLine);
    FOR_EACH(Pic::ProgVoltageType, type) {
      const Pic::VoltageData &vd = pdata.voltage(type);
      if ( type==Pic::ProgVoltageType::VddWrite && vd==pdata.voltage(Pic::ProgVoltageType::VddBulkErase) ) continue;
      if ( vd==pdata.voltage(Pic::ProgVoltageType::VddWrite) ) continue;
      DomElement *ve = new DomElement("voltages");
      root->append(ve);
      ve->appendAttribute("name", type.key());
      ve->appendAttribute("min", QString::number(vd.min));
      ve->appendAttribute("max", QString::number(vd.max));
      ve->appendAttribute("nominal", QString::number(vd.nominal));
    }
    root->append(new DomEmptyLine);
    root->append(new DomComment(" Memory "));
    FOR_EACH(Pic::MemoryRangeType, type) {
      if ( !pdata.isPresent(type) ) continue;
      const Pic::MemoryRangeData &mrd = pdata.range(type);
      DomElement *mre = new DomElement("memory");
      root->append(mre);
      mre->appendAttribute("name", type.key());
      mre->appendAttribute("start", toHexLabel(mrd.start, pdata.nbCharsAddress()));
      mre->appendAttribute("end", toHexLabel(mrd.end, pdata.nbCharsAddress()));
      if ( mrd.hexFileOffset!=0 ) mre->appendAttribute("hexfile_offset", toHexLabel(mrd.hexFileOffset, pdata.nbCharsAddress()));
      uint nbCharsWord = pdata.nbCharsWord(type);
      if ( type==Pic::MemoryRangeType::UserId ) mre->appendAttribute("rmask", toHexLabel(mrd.rmask, nbCharsWord));
      if ( type==Pic::MemoryRangeType::Cal && !pdata.is18Family() ) {
        mre->appendAttribute("cal_opmask", toHexLabel(pdata.calibrationData().opcodeMask, nbCharsWord));
        mre->appendAttribute("cal_opcode", toHexLabel(pdata.calibrationData().opcode, nbCharsWord));
      }
    }
    root->append(new DomEmptyLine);
    root->append(new DomComment(" Configuration bits "));
    bool first = true;
    for (uint i=0; i<pdata.config()._words.count(); i++) {
      const Pic::Config::Word &cword = pdata.config()._words[i];
      if ( cword.name.isNull() ) continue;
      if (first) first = false;
      else root->append(new DomEmptyLine);
      DomElement *cwe = new DomElement("config");
      root->append(cwe);
      cwe->appendAttribute("offset", toHexLabel(i * pdata.addressIncrement(Pic::MemoryRangeType::Config), 1));
      cwe->appendAttribute("name", cword.name);
      uint nbCharsWord = pdata.nbCharsWord(Pic::MemoryRangeType::Config);
      cwe->appendAttribute("wmask", toHexLabel(cword.wmask, nbCharsWord));
      cwe->appendAttribute("bvalue", toHexLabel(cword.bvalue, nbCharsWord));
      if ( cword.pmask!=0 ) cwe->appendAttribute("pmask", toHexLabel(cword.pmask, nbCharsWord));
      if ( !cword.ignoredCNames.isEmpty() ) cwe->appendAttribute("icnames", cword.ignoredCNames.join(" "));
      if ( cword.cmask!=cword.usedMask() ) cwe->appendAttribute("cmask", toHexLabel(cword.cmask, nbCharsWord));
      for (uint k=0; k<cword.masks.count(); k++) {
        const Pic::Config::Mask &cmask = cword.masks[k];
        DomElement *cme = new DomElement("mask");
        cwe->append(cme);
        cme->appendAttribute("name", cmask.name);
        cme->appendAttribute("value", toHexLabel(cmask.value, nbCharsWord));
        for (uint l=0; l<cmask.values.count(); l++) {
          const Pic::Config::Value &cvalue = cmask.values[l];
          DomElement *cve = new DomElement("value");
          cme->append(cve);
          cve->appendAttribute("value", toHexLabel(cvalue.value, nbCharsWord));
          cve->appendAttribute("name", cvalue.name);
          if ( !cvalue.name.isNull() ) cve->appendAttribute("cname", cvalue.cnames.join(" "));
          if ( !cvalue.extraCnames.isEmpty() ) cve->appendAttribute("ecnames", cvalue.extraCnames.join(" "));
        }
      }
    }
  } else if ( data.group().name()=="mem24" ) {
    root->append(new DomEmptyLine);
    root->append(new DomComment(" Memory "));
    const Mem24::Data &mdata = static_cast<const Mem24::Data &>(data);
    DomElement *me = new DomElement("memory");
    root->append(me);
    me->appendAttribute("size", toHexLabelAbs(mdata.nbBytes()));
    me->appendAttribute("nb_blocks", QString::number(mdata.nbBlocks()));
    me->appendAttribute("page_size", QString::number(mdata.nbBytesPage()));
  }

  // packages
  root->append(new DomEmptyLine);
  root->append(new DomComment(" Packages "));
  for (uint i=0; i<data.packages().count(); i++) {
    if ( i!=0 ) root->append(new DomEmptyLine);
    const Device::Package &pack = data.packages()[i];
    DomElement *pe = new DomElement("package");
    root->append(pe);
    QStringList types;
    for (uint k=0; k<pack.types.count(); k++) types += Device::Package::TYPE_DATA[pack.types[k]].name;
    pe->appendAttribute("types", types.join(" "));
    pe->appendAttribute("nb_pins", QString::number(pack.pins.count()));
    for (uint k=0; k<pack.pins.count(); k++) {
      DomElement *pine = new DomElement("pin");
      pe->append(pine);
      pine->appendAttribute("index", QString::number(k+1));
      pine->appendAttribute("name", pack.pins[k]);
    }
  }

  root->append(new DomEmptyLine);
  return doc;
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  if ( argc!=2 ) qFatal("Need directory as argument.\n");
  QStringList all = Device::lister().supportedDevices();
  for (uint i=0; i<all.count(); i++) {
    qDebug("%s", all[i].latin1());
    DomDocument *doc = createDocument(*Device::lister().data(all[i]));
    QFile file(QString(argv[1]) + "/" + all[i] + ".xml");
    if ( !file.open(IO_WriteOnly) ) qFatal("Could not open file \"%s\"", file.name().latin1());
    QTextStream stream(&file);
    doc->save(stream);
    delete doc;
  }
  return 0;
}
