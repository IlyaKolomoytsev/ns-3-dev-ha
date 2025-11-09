#include "interference-model.h"

using namespace ns3;

/* TEST CASES */
/**
 * Add only one HaPpdu signal.
 */
class AddOneHaPpduTest : public BaseInterferenceModelTestCase
{
  public:
    AddOneHaPpduTest();

    void DoRun() override;
};

AddOneHaPpduTest::AddOneHaPpduTest()
    : BaseInterferenceModelTestCase("AddOneHaPpdu")
{
}

void
AddOneHaPpduTest::DoRun()
{
    auto mainModel = Create<SpectrumModel>(Bands{BandInfo{1, 2, 3}});

    auto noizePowers = std::vector<double>{1.0};
    auto noise = Create<SpectrumValue>(mainModel);
    noise->SetValues(noizePowers);

    m_interference = CreateObject<HaInterferenceModel>(mainModel);
    m_interference->SetNoise(noise);

    auto ppdu = CreatePpdu();
    auto power = Create<SpectrumValue>(mainModel);
    auto powerValues = std::vector<double>{5.0};
    power->SetValues(powerValues);
    auto duration = Time(3);

    Simulator::Schedule(Time(2), &AddOneHaPpduTest::AddPpdu, this, ppdu, power, duration);
    Simulator::Schedule(Time(3), &AddOneHaPpduTest::CheckStatisticsByPpdu, this, ppdu, 5, 5, 5);
    Simulator::Schedule(Time(4), &AddOneHaPpduTest::CheckStatisticsByPpdu, this, ppdu, 5, 5, 5);
    Simulator::Schedule(Time(6), &AddOneHaPpduTest::CheckCantGetStatisticByPpdu, this, ppdu);
    Simulator::Run();
}

class AddOneHaPpduAndAnotherSignal : public BaseInterferenceModelTestCase
{
  public:
    AddOneHaPpduAndAnotherSignal();

    void DoRun() override;
};

AddOneHaPpduAndAnotherSignal::AddOneHaPpduAndAnotherSignal()
    : BaseInterferenceModelTestCase("AddOneHaPpduAndAnotherSignal")
{
}

void
AddOneHaPpduAndAnotherSignal::DoRun()
{
    auto mainModel = Create<SpectrumModel>(Bands{BandInfo{1, 2, 3}});

    auto noizePowers = std::vector<double>{2.0};
    auto noise = Create<SpectrumValue>(mainModel);
    noise->SetValues(noizePowers);

    m_interference = CreateObject<HaInterferenceModel>(mainModel);
    m_interference->SetNoise(noise);

    auto ppdu = CreatePpdu();
    auto powerPpdu = Create<SpectrumValue>(mainModel);
    powerPpdu->SetValues(std::vector<double>{20.0});
    auto durationPpdu = Time(5);

    auto powerAnotherSignal = Create<SpectrumValue>(mainModel);
    powerAnotherSignal->SetValues(std::vector<double>{8.0});
    auto durationAnotherSignal = Time(1);

    // add ppdu signal
    Simulator::Schedule(Time(10),
                        &AddOneHaPpduAndAnotherSignal::AddPpdu,
                        this,
                        ppdu,
                        powerPpdu,
                        durationPpdu);

    // add other signal
    Simulator::Schedule(Time(12),
                        &AddOneHaPpduAndAnotherSignal::AddSignal,
                        this,
                        powerAnotherSignal,
                        durationAnotherSignal);

    Simulator::Schedule(Time(11),
                        &AddOneHaPpduAndAnotherSignal::CheckStatisticsByPpdu,
                        this,
                        ppdu,
                        10,
                        10,
                        10);
    Simulator::Schedule(Time(12),
                        &AddOneHaPpduAndAnotherSignal::CheckStatisticsByPpdu,
                        this,
                        ppdu,
                        10,
                        10,
                        10);
    Simulator::Schedule(Time(13),
                        &AddOneHaPpduAndAnotherSignal::CheckStatisticsByPpdu,
                        this,
                        ppdu,
                        10,
                        2,
                        7.333333333);
    Simulator::Schedule(Time(14),
                        &AddOneHaPpduAndAnotherSignal::CheckStatisticsByPpdu,
                        this,
                        ppdu,
                        10,
                        2,
                        8);
    Simulator::Schedule(Time(15),
                        &AddOneHaPpduAndAnotherSignal::CheckStatisticsByPpdu,
                        this,
                        ppdu,
                        10,
                        2,
                        8.4);
    Simulator::Schedule(Time(16),
                        &AddOneHaPpduAndAnotherSignal::CheckCantGetStatisticByPpdu,
                        this,
                        ppdu);
    Simulator::Run();
}

/* TEST SUITE */

class HaInterferenceTestSuite : public TestSuite
{
  public:
    HaInterferenceTestSuite();
};

HaInterferenceTestSuite::HaInterferenceTestSuite()
    : TestSuite("ha-interference-model", Type::UNIT)
{
    AddTestCase(new AddOneHaPpduAndAnotherSignal);
}

static HaInterferenceTestSuite g_haInterferenceTestSuite;
