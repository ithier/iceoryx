#pragma once

#include <iceoryx_posh/popo/gateway_generic.hpp>
#include <iceoryx_posh/popo/publisher.hpp>

#include "ioxdds/dds/dds_types.hpp";

namespace iox {
namespace dds {

template <typename gateway_t = iox::popo::GatewayGeneric,
          typename publisher_t = iox::popo::Publisher,
          typename data_reader_t = iox::dds::data_reader_t>
class DDS2IceoryxGateway : gateway_t
{

  public:
    DDS2IceoryxGateway();
    ~DDS2IceoryxGateway();

    DDS2IceoryxGateway(const DDS2IceoryxGateway&) = delete;
    DDS2IceoryxGateway& operator=(const DDS2IceoryxGateway&) = delete;
    DDS2IceoryxGateway(DDS2IceoryxGateway&&) = delete;
    DDS2IceoryxGateway& operator=(DDS2IceoryxGateway&&) = delete;

    void runMultithreaded() noexcept;
    void discover(const iox::capro::CaproMessage& msg) noexcept;
    void forward() noexcept;
    void shutdown() noexcept;

  private:
    std::atomic_bool m_isRunning{false};
    std::atomic_bool m_runForwardingLoop{false};
    std::atomic_bool m_runDiscoveryLoop{false};

    std::thread m_discoveryThread;
    std::thread m_forwardingThread;

    void forwardingLoop() noexcept;
    void discoveryLoop() noexcept;

};

} // namespace dds
} // namespace iox

#include "ioxdds/internal/gateway/dds_to_iox.inl"