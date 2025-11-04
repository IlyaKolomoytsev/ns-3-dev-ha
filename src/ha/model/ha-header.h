#ifndef NS3_HA_HEADER_H
#define NS3_HA_HEADER_H

#include "ns3/mac8-address.h"

namespace ns3
{

/**
 * @ingroup ha
 * @brief Alias for the hydroacoustic MAC address type.
 *
 * The hydroacoustic stack uses eight-octet MAC addresses.
 */
using HaMacAddress = Mac8Address;

/**
 * @ingroup ha
 * @brief Value-type header holding source and destination addresses.
 */
class HaHeader
{
  public:
    /**
     * @brief Construct a header from source and destination addresses.
     * @param source Source MAC address.
     * @param destination Destination MAC address.
     */
    HaHeader(HaMacAddress source, HaMacAddress destination);

    /**
     * @brief Get the source address.
     * @return A copy of the stored source MAC address.
     */
    HaMacAddress getSource() const;

    /**
     * @brief Get the destination address.
     * @return A copy of the stored destination MAC address.
     */
    HaMacAddress getDestination() const;

  private:
    HaMacAddress m_source;      //!< Source MAC address.
    HaMacAddress m_destination; //!< Destination MAC address.
};

} // namespace ns3

#endif // NS3_HA_HEADER_H
