// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "helpers/fixture_dds_gateway.hpp"

#include "iceoryx_dds/gateway/channel.hpp"
#include "iceoryx_dds/gateway/dds_to_iox.hpp"

#include "mocks/chunk_mock.hpp"
#include "mocks/google_mocks.hpp"
#include "roudi_gtest.hpp"
#include "test.hpp"

using namespace ::testing;
using ::testing::_;

// ======================================== Helpers ======================================== //
using TestChannel = iox::dds::Channel<MockPublisher, MockDataReader>;
using TestGateway = iox::dds::DDS2IceoryxGateway<TestChannel, MockGenericDDSGateway<TestChannel>>;

// ======================================== Fixture ======================================== //

class DDS2IceoryxGatewayTest : public DDSGatewayTestFixture<MockPublisher, MockDataReader>
{};

// ======================================== Tests ======================================== //

TEST_F(DDS2IceoryxGatewayTest, ChannelsAreCreatedForConfiguredServices)
{
    // === Setup
    auto testService = iox::capro::ServiceDescription({"Radar", "Front-Right", "Reflections"});
    iox::dds::GatewayConfig config{};
    config.m_configuredServices.push_back(iox::dds::GatewayConfig::ServiceEntry{testService, 0});

    TestGateway gw{};
    EXPECT_CALL(gw, findChannel).WillOnce(Return(iox::cxx::nullopt_t()));
    EXPECT_CALL(gw, addChannel).WillOnce(Return(channelFactory(testService)));

    // === Test
    gw.loadConfiguration(config);
}