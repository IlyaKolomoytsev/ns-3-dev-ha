#ifndef NS3_HA_MPDU_H
#define NS3_HA_MPDU_H

#include "ha-header.h"

#include "ns3/packet.h"

namespace ns3
{

/**
 * @ingroup ha
 *
 * Hydroacoustic MAC protocol data unit (MPDU).
 * Encapsulates a MAC header and its associated payload packet.
 */
class HaMpdu : public SimpleRefCount<HaMpdu>
{
  public:
    /**
     * Construct an MPDU from payload and header.
     * @param packet Payload packet
     * @param header MAC header
     */
    HaMpdu(Ptr<Packet> packet, const HaHeader& header);

    /**
     * @brief Get the header stored in this item.
     * @return the header stored in this item.
     */
    HaHeader GetHeader() const;

    /**
     * @brief Get the packet stored in this item
     * @return the packet stored in this item.
     */
    Ptr<Packet> GetPacket() const;

  private:
    Ptr<Packet> m_packet; //!< Payload packet
    HaHeader m_header;    //!< MAC header
};

} // namespace ns3

#endif // NS3_HA_MPDU_H
