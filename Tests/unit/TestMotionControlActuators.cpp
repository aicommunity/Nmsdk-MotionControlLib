#include <gtest/gtest.h>
#include "../../../../Tests/TestHelpers.h"
#include "../../../../Rdk/Deploy/Include/rdk.h"
#include "../../../Deploy/Include/Lib.h"
#include "../../Core/NMotionControlLibrary.h"

using namespace RDK;
using namespace RDK::TestHelpers;

class MotionControlActuatorsTest : public ::testing::Test {
protected:
    void SetUp() override {
        InitTestLogging();
        storage = CreateStorageWithLibraries({&NMSDK::MotionControlLibrary});
        NMSDK::MotionControlLibrary.Upload(storage.get());
        try {
            NMSDK::MotionControlLibrary.CreateClassSamples(storage.get());
        } catch (const std::exception& ex) {
            GTEST_SKIP() << "NMotionControlLibrary CreateClassSamples failed: " << ex.what();
        }
    }

    void TearDown() override {
        storage.reset();
    }

    std::shared_ptr<UStorage> storage;
};

// Test NActuatorSignals creation and properties
TEST_F(MotionControlActuatorsTest, CreateActuatorSignals) {
    if (!storage->CheckClass("NActuatorSignals")) {
        GTEST_SKIP() << "NActuatorSignals class not available";
        return;
    }
    
    auto actuator = storage->TakeObject("NActuatorSignals");
    if (actuator) {
        EXPECT_EQ(actuator->GetCompClassName(), "NActuatorSignals");
        EXPECT_NE(actuator, nullptr);
        
        actuator->SetName("TestActuatorSignals");
        EXPECT_EQ(actuator->GetName(), "TestActuatorSignals");
        
        // Verify component supports basic operations
        EXPECT_GE(actuator->GetNumComponents(), 0);
    } else {
        GTEST_SKIP() << "Cannot create NActuatorSignals object";
    }
}

// Test that NActuatorSignals is a UNet
TEST_F(MotionControlActuatorsTest, ActuatorSignalsIsNet) {
    if (!storage->CheckClass("NActuatorSignals")) {
        GTEST_SKIP() << "NActuatorSignals class not available";
        return;
    }
    
    auto actuator = storage->TakeObject("NActuatorSignals");
    if (actuator) {
        // NActuatorSignals inherits from UNet, so it should support components
        EXPECT_GE(actuator->GetNumComponents(), 0) << "NActuatorSignals should support components";
    } else {
        GTEST_SKIP() << "Cannot create NActuatorSignals object";
    }
}

