/**
 * \file display.cpp
 * \brief RsVisa::Display implementation
 */


#include "display.hpp"


// RsVisa
#include "bool.hpp"
#include "helpers.hpp"
#include "znx.hpp"
using namespace RsVisa;


// std lib
#include <sstream>


Display::Display(Znx* znx) :
  _znx(znx)
{
  // no operations
}


bool Display::isOn()
{
  return toBool(updateSetting());
}


void Display::setOn(bool on)
{
  setUpdateSetting(on? "1" : "0");
}


void Display::setOff(bool off)
{
  setUpdateSetting(off? "0" : "1");
}


bool Display::isManualUpdate()
{
  return updateSetting() == "ONCE";
}


void Display::setManualUpdate(bool manual)
{
  setUpdateSetting("ONCE");
}


void Display::update()
{
  setUpdateSetting("ONCE");
}


void Display::local()
{
  _znx->write("@LOC");
}


void Display::remote()
{
  _znx->write("@REM");
}


// helpers

std::string Display::updateSetting()
{
  return rightTrim(_znx->query(":SYST:DISP:UPD?"));
}


void Display::setUpdateSetting(const char* value)
{
  const std::string value_str(value);
  setUpdateSetting(value_str);
}


void Display::setUpdateSetting(const std::string& value)
{
  std::stringstream scpi;
  scpi << ":SYST:DISP:UPD " << value;
  _znx->write(scpi.str());
}