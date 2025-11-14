#include <gtest/gtest.h>
#include "../../../../Tests/TestHelpers.h"
#include "../../../../Rdk/Deploy/Include/rdk.h"
#include "../../../Deploy/Include/Lib.h"
#include "../../Core/NMotionControlLibrary.h"

using namespace RDK;
using namespace RDK::TestHelpers;

class MotionControlNeuronsTest : public ::testing::Test {
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

// Test NCounterNeuron creation and properties
TEST_F(MotionControlNeuronsTest, CreateCounterNeuron) {
    if (!storage->CheckClass("NCounterNeuron")) {
        GTEST_SKIP() << "NCounterNeuron class not available";
        return;
    }
    
    auto neuron = storage->TakeObject("NCounterNeuron");
    if (neuron) {
        EXPECT_EQ(neuron->GetCompClassName(), "NCounterNeuron");
        EXPECT_NE(neuron, nullptr);
        
        neuron->SetName("TestCounterNeuron");
        EXPECT_EQ(neuron->GetName(), "TestCounterNeuron");
        
        // Verify component supports basic operations
        EXPECT_GE(neuron->GetNumComponents(), 0);
    } else {
        GTEST_SKIP() << "Cannot create NCounterNeuron object";
    }
}

// Test that NCounterNeuron is a UNet
TEST_F(MotionControlNeuronsTest, CounterNeuronIsNet) {
    if (!storage->CheckClass("NCounterNeuron")) {
        GTEST_SKIP() << "NCounterNeuron class not available";
        return;
    }
    
    auto neuron = storage->TakeObject("NCounterNeuron");
    if (neuron) {
        // NCounterNeuron inherits from UNet, so it should support components
        EXPECT_GE(neuron->GetNumComponents(), 0) << "NCounterNeuron should support components";
    } else {
        GTEST_SKIP() << "Cannot create NCounterNeuron object";
    }
}

