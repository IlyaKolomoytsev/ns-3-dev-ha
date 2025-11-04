#ifndef NS3_HA_TRANSMISSION_PARAMETERS_H
#define NS3_HA_TRANSMISSION_PARAMETERS_H
#include "ha-phy-common.h"
#include "ha-units.h"

#include "ns3/core-module.h"

namespace ns3
{

/**
 * @ingroup ha
 *
 * Transmission parameters for a hydroacoustic signal.
 */
class HaTransmissionParameters
{
  public:
    /**
     * Construct parameters with given values.
     * @param signalType       Type of transmitted signal.
     * @param carrierFrequency Carrier frequency [Hz].
     * @param bandwidth        Signal bandwidth [Hz].
     * @param duration         Transmission duration.
     */
    HaTransmissionParameters(HaSignalType signalType,
                             Hz_u carrierFrequency,
                             Hz_u bandwidth,
                             Time duration);

    /**
     * Get the signal type.
     * @return Signal type value.
     */
    HaSignalType GetSignalType() const;

    /**
     * Get the carrier frequency.
     * @return Carrier frequency in Hz.
     */
    Hz_u GetCarrierFrequency() const;

    /**
     * Get the signal bandwidth.
     * @return Bandwidth in Hz.
     */
    Hz_u GetBandWidth() const;

    /**
     * Get the signal duration.
     * @return Transmission duration.
     */
    Time GetDuration() const;

  private:
    HaSignalType m_signalType; //!< Signal type identifier.
    Hz_u m_carrierFrequency;   //!< Carrier frequency [Hz].
    Hz_u m_bandwidth;          //!< Signal bandwidth [Hz].
    Time duration;             //!< Transmission duration.
};

} // namespace ns3

#endif // NS3_HA_TRANSMISSION_PARAMETERS_H
