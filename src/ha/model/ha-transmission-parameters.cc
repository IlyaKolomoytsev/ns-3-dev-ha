#include "ha-transmission-parameters.h"

namespace ns3
{
HaTransmissionParameters::HaTransmissionParameters(HaSignalType signalType,
                                                   Hz_u carrierFrequency,
                                                   Hz_u bandwidth,
                                                   Time duration)
    : m_signalType(signalType),
      m_carrierFrequency(carrierFrequency),
      m_bandwidth(bandwidth),
      m_duration(duration)
{
}

HaSignalType
HaTransmissionParameters::GetSignalType() const
{
    return m_signalType;
}

Hz_u
HaTransmissionParameters::GetCarrierFrequency() const
{
    return m_carrierFrequency;
}

Hz_u
HaTransmissionParameters::GetBandWidth() const
{
    return m_bandwidth;
}

Time
HaTransmissionParameters::GetDuration() const
{
    return m_duration;
}
} // namespace ns3
