#include "ha-ppdu.h"

namespace ns3
{

HaPpdu::HaPpdu(Ptr<HaMpdu> mpdu, Ptr<HaTransmissionParameters> parameters)
    : m_mpdu(mpdu),
      m_txParameters(parameters)
{
    m_mpdu = mpdu;
}

Ptr<HaMpdu>
HaPpdu::GetMpdu()
{
    return m_mpdu;
}

Ptr<HaTransmissionParameters>
HaPpdu::GetParameters()
{
    return m_txParameters;
}
} // namespace ns3
