#ifndef NS3_INTERFERENCE_MODEL_H
#define NS3_INTERFERENCE_MODEL_H
#include "ns3/ha-interference-model.h"
#include "ns3/test.h"

namespace ns3
{

class BaseInterferenceModelTestCase : public TestCase
{
  protected:
    BaseInterferenceModelTestCase(std::string name);
    Ptr<HaPpdu> CreatePpdu();
    void AddPpdu(Ptr<HaPpdu> ppdu, Ptr<SpectrumValue> power, Time duration);
    void AddSignal(Ptr<SpectrumValue> power, Time duration);
    void CheckStatisticsByPpdu(Ptr<const HaPpdu> ppdu,
                               Snr_u expectMaximum,
                               Snr_u expectMinimum,
                               Snr_u expectMean);
    void CheckStatisticsByStatus(Ptr<HaInterferenceStatus> status,
                                 Snr_u expectMaximum,
                                 Snr_u expectMinimum,
                                 Snr_u expectMean);
    void CheckCantGetStatisticByPpdu(Ptr<const HaPpdu> ppdu);
    void SetNoise(Ptr<SpectrumValue> noise);
    Ptr<HaInterferenceModel> m_interference;
};

inline BaseInterferenceModelTestCase::BaseInterferenceModelTestCase(std::string name)
    : TestCase(name)
{
}

inline Ptr<HaPpdu>
BaseInterferenceModelTestCase::CreatePpdu()
{
    auto macHeader = HaHeader(HaMacAddress::Allocate(), HaMacAddress::Allocate());
    auto mpdu = Create<HaMpdu>(Create<Packet>(), macHeader);
    auto signalParameters =
        Create<HaTransmissionParameters>(HaSignalType::HA_SIGNAL_UNKNOWN, 2, 1, Time(1));
    auto ppdu = Create<HaPpdu>(mpdu, signalParameters);
    return ppdu;
}

inline void
BaseInterferenceModelTestCase::AddPpdu(Ptr<HaPpdu> ppdu, Ptr<SpectrumValue> power, Time duration)
{
    m_interference->AddPpdu(ppdu, power, duration);
}

inline void
BaseInterferenceModelTestCase::AddSignal(Ptr<SpectrumValue> power, Time duration)
{
    m_interference->AddSignal(power, duration);
}

inline void
BaseInterferenceModelTestCase::CheckStatisticsByPpdu(Ptr<const HaPpdu> ppdu,
                                                     Snr_u expectMaximum,
                                                     Snr_u expectMinimum,
                                                     Snr_u expectMean)
{
    bool exist = m_interference->CanGetInterferenceStatus(ppdu);
    NS_TEST_ASSERT_MSG_EQ(exist, true, "Can't get interference status for this ppdu: " << ppdu);
    if (exist)
    {
        auto status = m_interference->GetInterferenceStatus(ppdu);
        CheckStatisticsByStatus(status, expectMaximum, expectMinimum, expectMean);
    }
}

inline void
BaseInterferenceModelTestCase::CheckStatisticsByStatus(Ptr<HaInterferenceStatus> status,
                                                       Snr_u expectMaximum,
                                                       Snr_u expectMinimum,
                                                       Snr_u expectMean)
{
    auto max = status->GetMaxSnr();
    auto min = status->GetMinSnr();
    auto mean = status->GetMeanSnr();
    NS_TEST_ASSERT_MSG_EQ_TOL(max, expectMaximum, 0.000001, "Incorrect max snr");
    NS_TEST_ASSERT_MSG_EQ_TOL(min, expectMinimum, 0.000001, "Incorrect min snr");
    NS_TEST_ASSERT_MSG_EQ_TOL(mean, expectMean, 0.000001, "Incorrect mean snr");
}

inline void
BaseInterferenceModelTestCase::CheckCantGetStatisticByPpdu(Ptr<const HaPpdu> ppdu)
{
    bool exist = m_interference->CanGetInterferenceStatus(ppdu);
    NS_TEST_ASSERT_MSG_EQ(exist,
                          false,
                          "Interference model has finished processing this ppdu: "
                              << ppdu << " but statistics are still available.");

    bool thrown = false;
    try
    {
        auto status = m_interference->GetInterferenceStatus(ppdu);
    }
    catch (...)
    {
        thrown = true;
    }
    NS_TEST_ASSERT_MSG_EQ(thrown,
                          true,
                          "Interference model return 'Status' for ppdu: "
                              << ppdu << " but earlier, it said that the status does not exist");
}

inline void
BaseInterferenceModelTestCase::SetNoise(Ptr<SpectrumValue> noise)
{
    m_interference->SetNoise(noise);
}
} // namespace ns3

#endif // NS3_INTERFERENCE_MODEL_H
