#include "ha-header.h"

namespace ns3
{
HaHeader::HaHeader(HaMacAddress source, HaMacAddress destination)
    : m_source(source),
      m_destination(destination)
{
}

HaMacAddress
HaHeader::getSource() const
{
    return m_source;
}

HaMacAddress
HaHeader::getDestination() const
{
    return m_destination;
}
} // namespace ns3
