#ifndef NS3_HA_PPDU_H
#define NS3_HA_PPDU_H

#include "ha-mpdu.h"
#include "ha-transmission-parameters.h"

#include "ns3/simple-ref-count.h"

namespace ns3
{

/**
 * @ingroup ha
 *
 * Hydroacoustic physical protocol data unit (PPDU).
 * Couples a MAC-level MPDU with its transmission parameters.
 */
class HaPpdu : public SimpleRefCount<HaPpdu>
{
  public:
    /**
     * Construct a PPDU from an MPDU and transmission parameters.
     * @param mpdu MAC protocol data unit to be transmitted.
     * @param parameters Transmission parameters used for this PPDU.
     */
    HaPpdu(Ptr<HaMpdu> mpdu, Ptr<HaTransmissionParameters> parameters);

    /**
     * @brief Get the MPDU contained in this PPDU.
     * @return pointer to the associated MPDU.
     */
    Ptr<HaMpdu> GetMpdu();

    /**
     * @brief Get the transmission parameters.
     * @return pointer to the transmission parameters.
     */
    Ptr<HaTransmissionParameters> GetParameters();

  private:
    Ptr<HaMpdu> m_mpdu;                           //!< Associated MAC protocol data unit.
    Ptr<HaTransmissionParameters> m_txParameters; //!< Transmission parameters.
};

} // namespace ns3

#endif // NS3_HA_PPDU_H
