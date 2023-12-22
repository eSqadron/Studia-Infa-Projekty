//
// Created by OEM on 18.12.2020.
//
#include "gtest/gtest.h"
#include "gmock/gmock.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
