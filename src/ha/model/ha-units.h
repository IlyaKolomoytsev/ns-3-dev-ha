#ifndef NS3_HA_UNITS_H
#define NS3_HA_UNITS_H

#include <cstdint>

/**
 * @file
 * @ingroup ha
 * Declaration of the SI units (as weak types aliases) used across ha module
 */

namespace ns3
{

using mWatt_u = double;       //!< mWatt weak type
using Watt_u = double;        //!< Watt weak type
using dBW_u = double;         //!< dBW weak type
using dBm_u = double;         //!< dBm weak type
using dB_u = double;          //!< dB weak type
using dBr_u = dB_u;           //!< dBr weak type
using Hz_u = double;          //!< Hz weak type
using MHz_u = double;         //!< MHz weak type
using meter_u = double;       //!< meter weak type
using ampere_u = double;      //!< ampere weak type
using volt_u = double;        //!< volt weak type
using degree_u = double;      //!< degree weak type (angle)
using joule_u = double;       //!< joule weak type
using dBm_per_Hz_u = double;  //!< dBm/Hz weak type
using dBm_per_MHz_u = double; //!< dBm/MHz weak type

} // namespace ns3

#endif // NS3_HA_UNITS_H
