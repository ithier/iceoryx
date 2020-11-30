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

#include "iceoryx_utils/cxx/type_traits.hpp"
#include "iceoryx_utils/cxx/expected.hpp"
#include "iceoryx_utils/cxx/optional.hpp"
#include "test.hpp"

using namespace ::testing;
using namespace iox::cxx;

class ExpectedOptionalChainingTest : public Test
{

};


enum class DummyError
{
    UH_OH
};

enum class DummyErrorTwo
{
    UH_OH
};

// expected<optional<T>>
TEST_F(ExpectedOptionalChainingTest, ExpectedOptionalHasValue)
{
    auto andThenWasCalled = false;
    auto orElseWasCalled = false;

    auto expectedOptional = expected<optional<int>, DummyError>::create_value(make_optional<int>(42));
    expectedOptional.and_then([&andThenWasCalled](int& val){
        andThenWasCalled = true;
        ASSERT_EQ(val, 42);
    })
    .or_else([&orElseWasCalled](DummyError){
        orElseWasCalled = true;
    });

    ASSERT_TRUE(andThenWasCalled);
    ASSERT_FALSE(orElseWasCalled);
}

TEST_F(ExpectedOptionalChainingTest, ExpectedOptionalHasError)
{
    auto andThenWasCalled = false;
    auto orElseWithErrorWasCalled = false;

    auto expectedOptional = expected<optional<int>, DummyError>::create_error(DummyError::UH_OH);
    expectedOptional.and_then([&andThenWasCalled](int& val){
        andThenWasCalled = true;
        ASSERT_EQ(val, 42);
    })
    .or_else([&orElseWithErrorWasCalled](DummyError){
        orElseWithErrorWasCalled = true;
    });

    ASSERT_FALSE(andThenWasCalled);
    ASSERT_TRUE(orElseWithErrorWasCalled);
}

TEST_F(ExpectedOptionalChainingTest, ExpectedOptionalIsEmpty)
{
    auto andThenWasCalled = false;
    auto orElseWithErrorWasCalled = false;
    auto orElseWithoutErrorWasCalled = false;

    auto expectedOptional = expected<optional<int>, DummyError>::create_value(optional<int>(nullopt));
    expectedOptional.and_then([&andThenWasCalled](int& val){
        andThenWasCalled = true;
        ASSERT_EQ(val, 42);
    })
    .or_else([&orElseWithErrorWasCalled](DummyError){
        orElseWithErrorWasCalled = true;
    })
    .or_else([&orElseWithoutErrorWasCalled]{
        orElseWithoutErrorWasCalled = true;
    });

    ASSERT_FALSE(andThenWasCalled);
    ASSERT_FALSE(orElseWithErrorWasCalled);
    ASSERT_TRUE(orElseWithoutErrorWasCalled);
}

// optional<expected<T>>
TEST_F(ExpectedOptionalChainingTest, OptionalExpectedHasValue)
{
    auto andThenWasCalled = false;
    auto orElseWithErrorWasCalled = false;
    auto orElseWithoutErrorWasCalled = false;

    auto optionalExpectedError = make_optional<expected<int, DummyError>>(expected<int, DummyError>::create_value(42));
    optionalExpectedError.and_then([&andThenWasCalled](int&){
        andThenWasCalled = true;
    }).or_else([&orElseWithErrorWasCalled](DummyError&){
        orElseWithErrorWasCalled = true;
    }).or_else([&orElseWithoutErrorWasCalled](){
        orElseWithoutErrorWasCalled = true;
    });

    ASSERT_TRUE(andThenWasCalled);
    ASSERT_FALSE(orElseWithErrorWasCalled);
    ASSERT_FALSE(orElseWithoutErrorWasCalled);
}

TEST_F(ExpectedOptionalChainingTest, OptionalExpectedHasError)
{
    auto andThenWasCalled = false;
    auto orElseWithErrorWasCalled = false;
    auto orElseWithoutErrorWasCalled = false;

    auto optionalExpectedError = make_optional<expected<int, DummyError>>(expected<int, DummyError>::create_error(DummyError::UH_OH));
    optionalExpectedError.and_then([&andThenWasCalled](int&){
        andThenWasCalled = true;
    }).or_else([&orElseWithErrorWasCalled](DummyError&){
        orElseWithErrorWasCalled = true;
    }).or_else([&orElseWithoutErrorWasCalled](){
        orElseWithoutErrorWasCalled = true;
    });

    ASSERT_FALSE(andThenWasCalled);
    ASSERT_TRUE(orElseWithErrorWasCalled);
    ASSERT_FALSE(orElseWithoutErrorWasCalled);
}

TEST_F(ExpectedOptionalChainingTest, OptionalExpectedIsEmpty)
{
    auto andThenWasCalled = false;
    auto orElseWithErrorWasCalled = false;
    auto orElseWithoutErrorWasCalled = false;

    auto optionalExpectedError = optional<expected<int, DummyError>>(nullopt);
    optionalExpectedError.and_then([&andThenWasCalled](int&){
        andThenWasCalled = true;
    }).or_else([&orElseWithErrorWasCalled](DummyError&){
        orElseWithErrorWasCalled = true;
    }).or_else([&orElseWithoutErrorWasCalled](){
        orElseWithoutErrorWasCalled = true;
    });

    ASSERT_FALSE(andThenWasCalled);
    ASSERT_FALSE(orElseWithErrorWasCalled);
    ASSERT_TRUE(orElseWithoutErrorWasCalled);
}

// optional<optional<T>>
TEST_F(ExpectedOptionalChainingTest, OptionalOptionalHasValue)
{
    auto andThenWasCalled = false;
    auto orElseWasCalled = false;

    auto optionalOptional = make_optional<optional<int>>(make_optional<int>(42));
    optionalOptional.and_then([&andThenWasCalled](int& val){
        andThenWasCalled = true;
        ASSERT_EQ(val, 42);
    })
    .or_else([&orElseWasCalled](){
        orElseWasCalled = true;
    });

    ASSERT_TRUE(andThenWasCalled);
    ASSERT_FALSE(orElseWasCalled);
}

TEST_F(ExpectedOptionalChainingTest, OptionalOptionalIsEmpty)
{
    auto andThenWasCalled = false;
    auto orElseWasCalled = false;

    auto optionalOptional = optional<optional<int>>(nullopt);
    optionalOptional.and_then([&andThenWasCalled](int& val){
        andThenWasCalled = true;
        ASSERT_EQ(val, 42);
    })
    .or_else([&orElseWasCalled](){
        orElseWasCalled = true;
    });

    ASSERT_FALSE(andThenWasCalled);
    ASSERT_TRUE(orElseWasCalled);
}

//// optional<expected<expected<T>>>
//TEST_F(ExpectedOptionalChainingTest, OptionalExpectedExpectedHasErrorInFirstExpected)
//{
//    auto andThenWasCalled = false;
//    auto orElseWasCalledWithDummyError = false;
//    auto orElseWasCalledWithDummyErrorTwo = false;

//    auto optionalExpectedError =
//            make_optional<
//                expected<expected<int, DummyErrorTwo>, DummyError>>(expected<expected<int, DummyErrorTwo>, DummyError>::create_value(
//                        expected<int, DummyErrorTwo>::create_error()));

//    optionalExpectedError.and_then([&andThenWasCalled](int&){
//        andThenWasCalled = true;
//    }).or_else([&orElseWasCalledWithDummyError](DummyError&){
//        orElseWasCalledWithDummyError = true;
//    }).or_else([&orElseWasCalledWithDummyError](DummyErrorTwo&){
//        orElseWasCalledWithDummyError = true;
//    });

//    ASSERT_TRUE(true);
//}

//TEST_F(ExpectedOptionalChainingTest, OptionalExpectedExpectedHasErrorInSecondExpected)
//{

//}