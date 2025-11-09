#include "ha-interference-model.h"

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(HaInterferenceModel);

HaInterferenceStatus::HaInterferenceStatus(Snr_u startSnr)
{
    m_minSnr = startSnr;
    m_maxSnr = startSnr;
    m_meanSnr = startSnr;
    m_startTime = Simulator::Now();
    m_lastTime = Simulator::Now();
    m_lastSnr = startSnr;
    m_discreteSum = 0;
}

Snr_u
HaInterferenceStatus::GetMinSnr()
{
    if (m_lastTime != Simulator::Now())
    {
        UpdateNewSnr(m_lastSnr);
    }
    return m_minSnr;
}

Snr_u
HaInterferenceStatus::GetMaxSnr()
{
    if (m_lastTime != Simulator::Now())
    {
        UpdateNewSnr(m_lastSnr);
    }
    return m_maxSnr;
}

Snr_u
HaInterferenceStatus::GetMeanSnr()
{
    if (m_lastTime != Simulator::Now())
    {
        UpdateNewSnr(m_lastSnr);
    }
    return m_meanSnr;
}

void
HaInterferenceStatus::UpdateNewSnr(Snr_u snrNow)
{
    auto now = Simulator::Now();
    UpdateMaxSnr(m_lastSnr);
    UpdateMinSnr(m_lastSnr);
    UpdateMeanSnr(snrNow);
    m_lastTime = now;
    m_lastSnr = snrNow;
}

void
HaInterferenceStatus::UpdateMaxSnr(Snr_u currentSnr)
{
    if (currentSnr > m_maxSnr)
    {
        m_maxSnr = currentSnr;
    }
}

void
HaInterferenceStatus::UpdateMinSnr(Snr_u currentSnr)
{
    if (currentSnr < m_minSnr)
    {
        m_minSnr = currentSnr;
    }
}

void
HaInterferenceStatus::UpdateMeanSnr(Snr_u currentSnr)
{
    auto currentTime = Simulator::Now();
    // accumulate the sum of discrete quantities
    m_discreteSum += m_lastSnr * (currentTime - m_lastTime).GetDouble();
    // calculate mean snr with accumulated sum
    m_meanSnr = m_discreteSum / (currentTime - m_startTime).GetDouble();
    // update last SNR value
    m_lastSnr = currentSnr;
    m_lastTime = currentTime;
}

TypeId
HaInterferenceModel::GetTypeId()
{
    static TypeId tid = TypeId("ns3::HaInterferenceModel").SetParent<Object>().SetGroupName("Ha");
    return tid;
}

HaInterferenceModel::HaInterferenceModel(Ptr<const SpectrumModel> model)
{
    m_allSignals = Create<SpectrumValue>(model);
    m_noise = Create<SpectrumValue>(model);
}

Ptr<HaInterferenceStatus>
HaInterferenceModel::AddPpdu(Ptr<const HaPpdu> ppdu, Ptr<const SpectrumValue> power, Time duration)
{
    Ptr<const SpectrumValue> convertedSignal;
    if (power->GetSpectrumModel() == m_allSignals->GetSpectrumModel())
    {
        convertedSignal = power;
    }
    else
    {
        convertedSignal = ConvertToMainModel(power);
    }
    AddSignal(convertedSignal, duration);

    auto entry = Create<HaInterferenceEntry>(power, convertedSignal, m_allSignals);
    m_ppduInterferences[ppdu] = entry;

    Simulator::Schedule(duration, &HaInterferenceModel::FinishCalculation, this, ppdu);

    return entry->GetStatus();
}

void
HaInterferenceModel::AddSignal(Ptr<const SpectrumValue> power, Time duration)
{
    Ptr<const SpectrumValue> addedSignal;
    if (power->GetSpectrumModel() == m_allSignals->GetSpectrumModel())
    {
        addedSignal = power;
    }
    else
    {
        addedSignal = ConvertToMainModel(power);
    }

    AddSignalToOthers(addedSignal);
    Simulator::Schedule(duration, &HaInterferenceModel::RemoveSignalFromOthers, this, addedSignal);
}

void
HaInterferenceModel::SetNoise(Ptr<const SpectrumValue> noise)
{
    *m_allSignals -= *m_noise;
    m_noise = noise;
    AddSignalToOthers(m_noise);
}

bool
HaInterferenceModel::CanGetInterferenceStatus(Ptr<const HaPpdu> ppdu) const
{
    return m_ppduInterferences.contains(ppdu);
}

Ptr<HaInterferenceStatus>
HaInterferenceModel::GetInterferenceStatus(Ptr<const HaPpdu> ppdu) const
{
    return m_ppduInterferences.at(ppdu)->GetStatus();
}

Ptr<SpectrumValue>
HaInterferenceModel::ConvertToMainModel(Ptr<const SpectrumValue> power) const
{
    auto converter = SpectrumConverter(power->GetSpectrumModel(), m_allSignals->GetSpectrumModel());
    return converter.Convert(power);
}

void
HaInterferenceModel::AddSignalToOthers(Ptr<const SpectrumValue> signal)
{
    *m_allSignals += *signal;
    for (auto pair : m_ppduInterferences)
    {
        auto entity = pair.second;
        entity->Update();
    }
}

void
HaInterferenceModel::RemoveSignalFromOthers(Ptr<const SpectrumValue> signal)
{
    *m_allSignals -= *signal;
    for (auto pair : m_ppduInterferences)
    {
        auto entity = pair.second;
        entity->Update();
    }
}

void
HaInterferenceModel::FinishCalculation(Ptr<const HaPpdu> ppdu)
{
    m_ppduInterferences.at(ppdu)->Update();
    m_ppduInterferences.erase(ppdu);
}

HaInterferenceModel::HaInterferenceEntry::HaInterferenceEntry(
    Ptr<const SpectrumValue> signal,
    Ptr<const SpectrumValue> signalInMainModel,
    Ptr<const SpectrumValue> allSignals)
    : m_signal(signal),
      m_signalInMainModel(signalInMainModel),
      m_allSignals(allSignals),
      m_signalConverter(allSignals->GetSpectrumModel(), m_signal->GetSpectrumModel())
{
    bool validSignal = signal->GetSpectrumModel()->GetNumBands() == 1;
    NS_ASSERT(validSignal);
    bool isEqualModel = signalInMainModel->GetSpectrumModel() == m_allSignals->GetSpectrumModel();
    NS_ASSERT(isEqualModel);
    m_status = Create<HaInterferenceStatus>(CalculateSnrNow());
}

void
HaInterferenceModel::HaInterferenceEntry::Update()
{
    auto snr = CalculateSnrNow();
    m_status->UpdateNewSnr(snr);
}

Ptr<HaInterferenceStatus>
HaInterferenceModel::HaInterferenceEntry::GetStatus() const
{
    return m_status;
}

Snr_u
HaInterferenceModel::HaInterferenceEntry::CalculateSnrNow()
{
    auto snr = Create<SpectrumValue>(*m_signalInMainModel / (*m_allSignals - *m_signalInMainModel));
    auto converterSnr = m_signalConverter.Convert(snr);

    bool isOneBand = converterSnr->GetSpectrumModel()->GetNumBands() == 1;
    NS_ASSERT(isOneBand);

    return (*converterSnr)[0];
}

} // namespace ns3
