/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "generator_check.h"

#include "devices/base/device_group.h"
#include "devices/pic/pic/pic_memory.h"
#include "tools/gputils/gputils.h"
#include "tools/gputils/gputils_generator.h"
#include "tools/sdcc/sdcc.h"
#include "tools/sdcc/sdcc_generator.h"
#include "devices/list/device_list.h"

//----------------------------------------------------------------------------
GeneratorCheckHelper::GeneratorCheckHelper()
  : _cprocess(0), _lprocess(0), _generator(0)
{}

GeneratorCheckHelper::~GeneratorCheckHelper()
{
  delete _generator;
}

void GeneratorCheckHelper::cleanup()
{
  delete _lprocess;
  _lprocess = 0;
  delete _cprocess;
  _cprocess = 0;
}

//----------------------------------------------------------------------------
GeneratorCheck::GeneratorCheck(GeneratorCheckHelper *helper)
  : _helper(helper), _fdest(0), _fhex(0), _memory1(0)
{
  _view = new CLI::View;
}

GeneratorCheck::~GeneratorCheck()
{
  delete _view;
  delete _helper;
}

int GeneratorCheck::runTest()
{
  _helper->initSupported();
  return DeviceTest::runTest();
}

bool GeneratorCheck::init(const Device::Data &data)
{
  PURL::Url dest(PURL::currentDirectory(), "test.xxx");
  dest = dest.toFileType(_helper->sourceFileType());
  _fdest.reset(new PURL::File(dest, *_view));
  _helper->init(data);
  PURL::Url hex(PURL::currentDirectory(), "test.hex");
  _fhex.reset(new PURL::File(hex, *_view));
  _memory1 = data.group().createMemory(data);
  return true;
}

bool GeneratorCheck::skip(const Device::Data &data) const
{
  return !_helper->isSupported(data);
}

void GeneratorCheck::cleanup(const Device::Data &)
{
//  _fdest->remove();
  _helper->cleanup();
}

bool GeneratorCheck::executeDevice(const Device::Data &data)
{
  // create asm file from template source code
  if ( !_fdest->openForWrite() ) TEST_FAILED_RETURN("");
  _fdest->appendText(_source);
  _fdest->close();

  // run compiler
  _helper->_cprocess->execute(2000); // 2s timeout
  if ( _helper->_cprocess->state()!=Process::Exited ) TEST_FAILED_RETURN("Error while running compilation")
  if ( _helper->_cprocess->exitCode()!=0 ) TEST_FAILED_RETURN(QString("Error in compilation for %1:\n%2%3").arg(data.name()).arg(_helper->_cprocess->allOutputs()).arg(QString::null))

  // run linker
  if (_helper->_lprocess != NULL) {
    _helper->_lprocess->execute(2000); // 2s timeout
    if ( _helper->_lprocess->state()!=Process::Exited ) TEST_FAILED_RETURN("Error while running linking")
    if ( _helper->_lprocess->exitCode()!=0 ) TEST_FAILED_RETURN(QString("Error in linking for %1:\n%2%3").arg(data.name()).arg(_helper->_lprocess->allOutputs()).arg(QString::null))
  }

  // load hex file
  if ( !_fhex->openForRead() ) TEST_FAILED_RETURN("")
  QStringList errors, warnings;
  Device::Memory::WarningTypes warningTypes;
  if ( !_memory1->load(_fhex->stream(), errors, warningTypes, warnings) ) TEST_FAILED_RETURN(QString("Error loading hex into memory: %1").arg(errors.join(" ")))
  //if ( warningTypes!=Device::Memory::NoWarning ) TEST_FAILED(QString("Warning loading hex into memory: %1").arg(warnings.join(" ")))

  TEST_PASSED
  return true;
}

//----------------------------------------------------------------------------
bool ConfigGeneratorCheck::init(const Device::Data &data)
{
  if ( !GeneratorCheck::init(data) ) return false;
  _memory2 = data.group().createMemory(data);
  return true;
}

bool ConfigGeneratorCheck::executeDevice(const Device::Data &data)
{
  // create configuration
  const Pic::Config &config = static_cast<const Pic::Data &>(data).config();
  for (int l=0; ; l++) {
    // set config bits
    bool ok = false;
    for (int i=0; i<config._words.count(); i++) {
      const Pic::Config::Word &cword = config._words[i];
      for (int k=0; k<cword.masks.count(); k++) {
        const Pic::Config::Mask &cmask = cword.masks[k];
        if ( l<cmask.values.count() ) {
          ok = true;
          if ( !cmask.values[l].name.isEmpty() )
              static_cast<Pic::Memory&>(*_memory2).setConfigValue(cmask.name, cmask.values[l].name);
        }
      }
    }
    if ( !ok ) break;

    // create source code
    PURL::SourceFamily sfamily = _helper->sourceFileType().data().sourceFamily;
    PURL::ToolType ttype = sfamily.data().toolType;
    SourceLine::List lines = _helper->generator()->includeLines(ttype, data);
    lines += _helper->generator()->configLines(ttype, *_memory2, ok);
    lines += _helper->configEndLines();
    _source = SourceLine::text(sfamily, lines, 2);
    if (!ok) TEST_FAILED_RETURN("Config lines generation incomplete")

    if ( !GeneratorCheck::executeDevice(data) ) return false;

    // check that config bits are the same
    uint nbChars = static_cast<const Pic::Data &>(data).nbCharsWord(Pic::MemoryRangeType::Config);
    for (int i=0; i<config._words.count(); i++) {
      const Pic::Config::Word &cword = config._words[i];
      BitValue word1 = static_cast<const Pic::Memory&>(*_memory1).word(Pic::MemoryRangeType::Config, i);
      BitValue word2 = static_cast<const Pic::Memory&>(*_memory2).word(Pic::MemoryRangeType::Config, i);
      if ( word1==word2 ) continue;
      for (int k=0; k<cword.masks.count(); k++) {
        const Pic::Config::Mask &cmask = cword.masks[k];
        if ( cmask.value.isInside(cword.pmask) ) continue;
        BitValue value1 = word1.maskWith(cmask.value);
        BitValue value2 = word2.maskWith(cmask.value);
        if ( value1==value2 ) continue;
        QString name1, name2;
        uint l2 = 0;
        for (int l=0; l<cmask.values.count(); l++) {
          const Pic::Config::Value &value = cmask.values[l];
          if ( value.value==value1 ) { name1 = value.name; }
          if ( value.value==value2 ) { name2 = value.name; l2 = l; }
        }
        if ( name1==name2 ) continue;
        TEST_FAILED_RETURN(QString("Config bits are different in %1: set\"%2\"=(%3) != compiled=%4)")
                           .arg(cmask.name).arg(cmask.values[l2].name)
                           .arg(toHexLabel(word2.maskWith(cmask.value), nbChars)).arg(toHexLabel(word1.maskWith(cmask.value), nbChars)))
      }
    }
  }

  TEST_PASSED
  return true;
}

void ConfigGeneratorCheck::cleanup(const Device::Data &data)
{
  GeneratorCheck::cleanup(data);
}

//----------------------------------------------------------------------------
bool TemplateGeneratorCheck::init(const Device::Data &data)
{
  if ( !GeneratorCheck::init(data) ) return false;
  bool ok;
  PURL::SourceFamily sfamily = _helper->sourceFileType().data().sourceFamily;
  PURL::ToolType ttype = sfamily.data().toolType;
  SourceLine::List lines = _helper->generator()->templateSourceFile(ttype, data, ok);
  _source = SourceLine::text(sfamily, lines, 2);
  if (!ok) TEST_FAILED_RETURN(QString("Incomplete template generator for %1").arg(data.name()))
  return true;
}

//----------------------------------------------------------------------------
GPUtilsGeneratorCheckHelper::GPUtilsGeneratorCheckHelper()
{
  _generator = new GPUtils::SourceGenerator;
}

void GPUtilsGeneratorCheckHelper::initSupported()
{
  Process::StringOutput p;
  QStringList options;
  options += "-l";
  p.setup("gpasm", options, false);
  p.execute(2000); // 2s timeout
  _supported = GPUtils::getSupportedDevices(p.string(Process::OutputType::Stdout));
}

bool GPUtilsGeneratorCheckHelper::init(const Device::Data &data)
{
  _cprocess = new Process::StringOutput;
  QStringList options;
  options += "-c";
  options += "-p" + GPUtils::toDeviceName(data.name());
  options += "test.asm";
  _cprocess->setup("gpasm", options, false);
  _lprocess = new Process::StringOutput;
  options.clear();
  options += "-o";
  options += "test.hex";
  options += "test.o";
  _lprocess->setup("gplink", options, false);
  return true;
}

SourceLine::List GPUtilsGeneratorCheckHelper::configEndLines() const
{
  SourceLine::List lines;
  lines.appendIndentedCode("end");
  return lines;
}

//----------------------------------------------------------------------------
SDCCGeneratorCheckHelper::SDCCGeneratorCheckHelper()
{
  _generator = new SDCC::SourceGenerator;
}

void SDCCGeneratorCheckHelper::initSupported()
{
  PURL::Url url(PURL::currentDirectory(), "test.c");
  Log::StringView view;
  PURL::File file(url, view);
  if ( file.openForWrite() ) file.appendText("void main(void) {}\n");
  file.close();
  _supported.clear();
  for (uint i=0; i<SDCC::Nb_Families; i++) {
    Process::StringOutput p;
    QStringList options;
    options += QString("-m") + SDCC::FAMILY_DATA[i].name;
    options += "-phelp";
    options += "test.c";
    p.setup("sdcc", options, false);
    p.execute(2000); // 2s timeout
    _supported += SDCC::getSupportedDevices(p.string(Process::OutputType::Stderr));
  }
}

bool SDCCGeneratorCheckHelper::init(const Device::Data &data)
{
  _cprocess = new Process::StringOutput;
  QStringList options;
  options += QString("-m") + SDCC::FAMILY_DATA[SDCC::family(data.name())].name;
  options += "-" + SDCC::toDeviceName(data.name());
  options += "test.c";
  options += "-I/usr/share/gputils/header";
  options += "-Wl-otext.hex";
  _cprocess->setup("sdcc", options, false);
  return true;
}

SourceLine::List SDCCGeneratorCheckHelper::configEndLines() const
{
  SourceLine::List lines;
  lines.appendIndentedCode("void main() {}");
  return lines;
}
