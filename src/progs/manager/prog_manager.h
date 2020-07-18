/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROG_MANAGER_H
#define PROG_MANAGER_H

#include "common/global/log.h"
namespace Device { class Data; class Memory; class MemoryRange; }
namespace Port { class Description; }

namespace Programmer
{
class Generic;
class Group;
class HardwareDescription;

BEGIN_DECLARE_ENUM(ProgramAction)
  ProgramOnly = 0, Run, Step, Halt
END_DECLARE_ENUM_NO_DATA(ProgramAction)

class Manager : public QObject, public Log::Base
{
Q_OBJECT
public:
  Manager(QObject *parent);
  ::Programmer::Generic *programmer() { return _programmer.get(); }
  virtual ::Programmer::Generic *createProgrammer(const Device::Data *data) = 0;
  bool initProgramming(ProgramAction action);
  void endProgramming();
  void clear();
  void stop();
  bool program(const Device::Memory &memory, const Device::MemoryRange &range, bool allowRun);
  bool verify(const Device::Memory &memory, const Device::MemoryRange &range);
  bool read(Device::Memory &memory, const Device::MemoryRange &range);
  bool erase(const Device::MemoryRange &range);
  bool blankCheck(const Device::MemoryRange &range);
  bool setDevicePower(bool on);
  enum State { Idle, Programming };
  virtual void setState(State state) = 0;

public slots:
  bool connectDevice();
  bool disconnectDevice();
  bool run();
  bool halt();
  bool restart();
  bool step();

signals:
  void actionMessage(const QString &message);
  void showProgress(bool show);
  void setTotalProgress(uint steps);
  void setProgress(uint steps);

protected:
  std::auto_ptr< ::Programmer::Generic> _programmer;
  bool _halting;

  virtual const Group *group() const = 0;
  virtual bool internalInitProgramming(ProgramAction action);
  virtual const Device::Data *device() const = 0;
  virtual bool isTargetSelfPowered() const;
  virtual void internalCreateProgrammer(const Device::Data *data, const HardwareDescription &hd);
};

extern Manager *manager;

} // namespace

#endif
