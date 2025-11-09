#ifndef NS3_HA_INTERFERENCE_MODEL_H
#define NS3_HA_INTERFERENCE_MODEL_H

#include "ha-ppdu.h"

#include "ns3/spectrum-module.h"

#include <map>

namespace ns3
{

using Snr_u = double;

class HaInterferenceStatus : public SimpleRefCount<HaInterferenceStatus>
{
    friend class HaInterferenceModel;

  public:
    HaInterferenceStatus(Snr_u startSnr);
    Snr_u GetMinSnr();
    Snr_u GetMaxSnr();
    Snr_u GetMeanSnr();

  private:
    void UpdateNewSnr(Snr_u snrNow);
    void UpdateMaxSnr(Snr_u currentSnr);
    void UpdateMinSnr(Snr_u currentSnr);

    /**
     * Update snr value for status.
     *
     * To calculate the mean snr the formula is used:
     * \f[
     * SNR_{mean} = \frac{\sum_{i = 1}^{n} SNR_{i-1} \cdot (t_i - t_{i-1})}{t_n - t_0}
     * \f]
     *
     * To optimize the calculation, step-by-step calculation is used:
     * \f[
     * t_0: SNR_{mean} = SNR_0
     * \f]
     * \f[
     * t_i: SNR_{mean} = \frac{discreteSum + (SNR_{i-1} * (t_i-t_{i-1})) }{t_n - t_0}
     * \f]
     *
     * discreteSum = {@link m_discreteSum};
     * \f$ SNR_{i-1} \f$ = {@link m_lastSnr};
     * \f$ t_{i-1} \f$ = {@link m_lastTime};
     * \f$ t_0 \f$ = {@link m_startTime}.
     *
     * @see m_discreteSum
     * @see m_startTime
     * @see m_lastTime
     * @see m_lastSnr
     *
     * @param currentSnr calculated snr value at current time.
     */
    void UpdateMeanSnr(Snr_u currentSnr);

    Snr_u m_minSnr;  ///< Minimum snr value
    Snr_u m_maxSnr;  ///< Maximum snr value
    Snr_u m_meanSnr; ///< Mean snr value
    /**
     * Store time of first mean snr calculation.
     * @see UpdateMeanSnr()
     */
    Time m_startTime;
    /**
     * Store last time of update.
     * @see UpdateMeanSnr()
     */
    Time m_lastTime;
    /**
     * Store last calculated snr value.
     * @see UpdateMeanSnr()
     */
    Snr_u m_lastSnr;
    /**
     * Stores the discrete sum of products snr multiplied by the time interval.
     * @see UpdateMeanSnr()
     */
    double m_discreteSum;
};

class HaInterferenceModel : public Object
{
  public:
    static TypeId GetTypeId();

    /**
     * Create interference model with signals spectrum model.
     * @param model Spectrum model for all signals.
     */
    HaInterferenceModel(Ptr<const SpectrumModel> model);

    /**
     * Adds ppdu power to interference calculations.
     * @param ppdu Physical protocol data unit.
     * @param power Signal parameters.
     * @param duration Signal duration.
     * @return Interference status object.
     */
    Ptr<HaInterferenceStatus> AddPpdu(Ptr<const HaPpdu> ppdu,
                                      Ptr<const SpectrumValue> power,
                                      Time duration);

    /**
     * Add singal to interference calculations.
     * @param power signal parameters.
     * @param duration Signal duration.
     */
    void AddSignal(Ptr<const SpectrumValue> power, Time duration);

    void SetNoise(Ptr<const SpectrumValue> noise);

    bool CanGetInterferenceStatus(Ptr<const HaPpdu> ppdu) const;

    Ptr<HaInterferenceStatus> GetInterferenceStatus(Ptr<const HaPpdu> ppdu) const;

  private:
    Ptr<SpectrumValue> ConvertToMainModel(Ptr<const SpectrumValue> power) const;
    void AddSignalToOthers(Ptr<const SpectrumValue> signal);
    void RemoveSignalFromOthers(Ptr<const SpectrumValue> signal);
    void FinishCalculation(Ptr<const HaPpdu> ppdu);

    class HaInterferenceEntry : public SimpleRefCount<HaInterferenceEntry>
    {
      public:
        HaInterferenceEntry(Ptr<const SpectrumValue> signal,
                            Ptr<const SpectrumValue> signalInMainModel,
                            Ptr<const SpectrumValue> allSignals);

        void Update();
        Ptr<HaInterferenceStatus> GetStatus() const;

      private:
        Snr_u CalculateSnrNow();
        Ptr<const SpectrumValue> m_signal;
        Ptr<const SpectrumValue> m_signalInMainModel;
        Ptr<const SpectrumValue> m_allSignals;
        const SpectrumConverter m_signalConverter;

        Ptr<HaInterferenceStatus> m_status;
    };

    Ptr<SpectrumValue> m_allSignals;
    Ptr<const SpectrumValue> m_noise;
    std::map<Ptr<const HaPpdu>, Ptr<HaInterferenceEntry>> m_ppduInterferences{};
};

} // namespace ns3

#endif // NS3_HA_INTERFERENCE_MODEL_H
