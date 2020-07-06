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

#include "iceoryx_dds/dds/cyclone_data_reader.hpp"
#include "test.hpp"

#include <Mempool_DCPS.hpp>
#include <dds/dds.hpp>

using namespace ::testing;
using ::testing::_;

namespace iox
{
namespace dds
{
// ======================================== Helpers ======================================== //
using TestDataReader = CycloneDataReader;

// ======================================== Fixture ======================================== //
class CycloneDataReaderTest : public Test
{
  public:
    void SetUp(){};
    void TearDown(){};
};

// ======================================== Tests ======================================== //
TEST_F(CycloneDataReaderTest, DoesNotAttemptToReadWhenDisconnected)
{
    // ===== Setup
    uint64_t bufferSize = 1024;
    uint8_t buffer[bufferSize];
    uint64_t sampleSize = sizeof(uint64_t);

    // ===== Test
    TestDataReader reader{"", "", ""};
    auto result = reader.read(buffer, bufferSize, sampleSize);
    EXPECT_EQ(true, result.has_error());
    EXPECT_EQ(iox::dds::DataReaderError::NOT_CONNECTED, result.get_error());
}

TEST_F(CycloneDataReaderTest, ReturnsErrorWhenAttemptingToReadIntoANullBuffer)
{
    // ===== Setup
    uint64_t bufferSize = 0;
    uint8_t* buffer = nullptr;
    uint64_t sampleSize = sizeof(uint64_t);

    // ===== Test
    TestDataReader reader{"", "", ""};
    reader.connect();
    auto result = reader.read(buffer, bufferSize, sampleSize);

    EXPECT_EQ(true, result.has_error());
    EXPECT_EQ(iox::dds::DataReaderError::INVALID_RECV_BUFFER, result.get_error());
}

TEST_F(CycloneDataReaderTest, ReturnsErrorWhenReceiverBufferSmallerThanSampleSize)
{
    // ===== Setup
    uint64_t bufferSize = 0;
    uint8_t buffer[bufferSize];
    uint64_t sampleSize = sizeof(uint64_t);

    // ===== Test
    TestDataReader reader{"", "", ""};
    reader.connect();
    auto result = reader.read(buffer, bufferSize, sampleSize);

    EXPECT_EQ(true, result.has_error());
    EXPECT_EQ(iox::dds::DataReaderError::INVALID_RECV_BUFFER, result.get_error());
}

} // namespace dds
} // namespace iox
