#include "ha-mpdu.h"

namespace ns3
{

HaMpdu::HaMpdu(Ptr<Packet> packet, const HaHeader& header)
    : m_packet(packet),
      m_header(header)
{
}

HaHeader
HaMpdu::GetHeader() const
{
    return m_header;
}

Ptr<Packet>
HaMpdu::GetPacket() const
{
    return m_packet;
}

} // namespace ns3
