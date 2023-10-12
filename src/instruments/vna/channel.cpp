/**
 * \file channel.cpp
 * \brief RsVisa::Channel implementation
 */


#include "channel.hpp"
#include "preserve_data_format.hpp"
#include "to_vector.hpp"
using namespace RsVisa;


// std lib
#include <sstream>


Channel::Channel(Znx *znx, unsigned int index) :
  _znx(znx),
  _index(index)
{
  // no operations
}


unsigned int Channel::index() const
{
  return _index;
}


unsigned int Channel::points()
{
  // :SENS<ch>:SWE:POIN?
  std::stringstream scpi;
  scpi << ":SENS" << index();
  scpi << ":SWE:POIN?";
  return std::stoi(_znx->query(scpi.str()));
}


void Channel::setPoints(unsigned int points)
{
  // :SENS<ch>:SWE:POIN <points>
  std::stringstream scpi;
  scpi << ":SENS"      << index();
  scpi << ":SWE:POIN " << points;
  _znx->write(scpi.str());
}


double Channel::startFrequency_Hz()
{
  // :SENS<ch>:FREQ:STAR?
  std::stringstream scpi;
  scpi << ":SENS" << _index;
  scpi << ":FREQ:STAR?";
  return std::stod(_znx->query(scpi.str()));
}


void Channel::setStartFrequency(double frequency_Hz)
{
  // :SENS<ch>:FREQ:STAR <frequency_Hz>
  std::stringstream scpi;
  scpi << ":SENS"       << _index;
  scpi << ":FREQ:STAR " << frequency_Hz;
  _znx->write(scpi.str());
}


double Channel::stopFrequency_Hz()
{
  // :SENS<ch>:FREQ:STOP?
  std::stringstream scpi;
  scpi << ":SENS" << _index;
  scpi << ":FREQ:STOP?";
  return std::stod(_znx->query(scpi.str()));
}


void Channel::setStopFrequency(double frequency_Hz)
{
  // :SENS<ch>:FREQ:STOP <frequency_Hz>
  std::stringstream scpi;
  scpi << ":SENS"       << _index;
  scpi << ":FREQ:STOP " << frequency_Hz;
  _znx->write(scpi.str());
}


std::vector<double> Channel::frequencies_Hz()
{
  // calculate buffer size (Bytes)
  // note: requires points() scpi query
  using uint = unsigned int;
  const uint points      = this->points();
  const uint bytes_per_point = 8;
  const uint payload     = points * bytes_per_point;
  const uint header      = 2 + uint(std::to_string(payload).size());
  const uint buffer_size = header + payload;

  // construct scpi command
  std::stringstream stream;
  stream << ":CALC" << _index;
  stream << ":DATA:STIM?";
  const std::string scpi = stream.str();

  // set data format to
  // binary 64-bit, little endian
  PreserveDataFormat preserve_data_format(_znx);
  DataFormat format = _znx->dataFormat();
  format.setBinary64Bit();
  format.setLittleEndian();

  // query frequencies block data
  BlockData block_data = _znx->queryBlockData(scpi, buffer_size + 1);
  if (!block_data.isComplete())
  {
    // query failed
    return std::vector<double>();
  }

  // return frequencies vector
  return toVector<double>(block_data.payload(), block_data.payloadSize_B());
}