#ifndef NS3_HA_PHY_H
#define NS3_HA_PHY_H

#include "ha-mpdu.h"
#include "ha-phy-common.h"
#include "ha-ppdu.h"
#include "ha-units.h"

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"

#include <unordered_map>

namespace ns3
{

class HaPhy;
class HaPhyEntity;
class HaSpectrumValue;

using ReadyTransmitCallback = Callback<void, Ptr<HaPhy>>;
using ReceiveMpduCallback = Callback<void, Ptr<HaPhy>, Ptr<HaMpdu>>;

class HaPhy : public Object
{
  public:
    static TypeId GetTypeId();

    HaPhy();
    ~HaPhy() override;

    /* Set callbacks */
    void SetReceiveMpduCallback(ReceiveMpduCallback cb);
    void SetReadyTransmitCallback(ReadyTransmitCallback cb);

    // data path
    bool Send(Ptr<HaMpdu> mpdu);
    virtual void Send(Ptr<HaPpdu> psdu) = 0;
    void Receive(Ptr<HaPpdu> ppdu, dBm_u power, Vector transmitterVelocity);

    /* State transitions */
    void MoveToTx();
    void MoveToRx();
    void MoveToIdle();
    void MoveToOff();

    /* State queries */
    bool IsStateTx() const;
    bool IsStateRx() const;
    bool IsStateIdle() const;
    bool IsStateOff() const;

    // accessors / parameters
    Ptr<HaPhyEntity> GetPhyEntity(HaSignalType type) const;
    dBm_u GetRxSensitivity() const;
    dBm_u GetTxPower(Ptr<HaPpdu> ppdu) const;
    Ptr<MobilityModel> GetMobility() const;

    enum State
    {
        Off,
        Idle,
        Transmitting,
        Receiving,
    };

  private:
    std::unordered_map<HaSignalType, Ptr<HaPhyEntity>> m_phyEntities;
    HaSignalType m_activeEntityType;
    State m_state = Off;

    ReadyTransmitCallback m_readyTx;
    ReceiveMpduCallback m_rxMpdu;
};

} // namespace ns3

#endif // NS3_HA_PHY_H
