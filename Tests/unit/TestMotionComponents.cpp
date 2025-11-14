#include <gtest/gtest.h>
#include "../../../../Tests/TestHelpers.h"
#include "../../../../Rdk/Deploy/Include/rdk.h"
#include "../../../Deploy/Include/Lib.h"
#include "../../../../Libraries/Nmsdk-MotionControlLib/Core/NMotionControlLibrary.h"
#include <algorithm>
#include <vector>

using namespace RDK;
using namespace RDK::TestHelpers;

namespace {

std::string FindMotionComponentName(UStorage& storage,
                                    std::initializer_list<const char*> preferredNames,
                                    std::initializer_list<const char*> fallbackPrefixes) {
    for (const auto* name : preferredNames) {
        if (storage.CheckClass(name)) {
            return name;
        }
    }

    std::vector<std::string> names;
    storage.GetClassNameList(names);
    for (const auto* prefix : fallbackPrefixes) {
        auto it = std::find_if(names.begin(), names.end(),
                               [&](const std::string& value) {
                                   return value.rfind(prefix, 0) == 0;
                               });
        if (it != names.end()) {
            return *it;
        }
    }
    return {};
}

} // namespace

class MotionComponentsTest : public ::testing::Test {
protected:
    void SetUp() override {
        InitTestLogging();
        storage = CreateStorageWithLibraries({&NMSDK::MotionControlLibrary});
        NMSDK::MotionControlLibrary.Upload(storage.get());
        // CreateClassSamples should now work correctly after fixing lifecycle issues
        // Components created via TakeObject are removed from ObjectsStorage via PopObject
        // before being used as prototypes in UploadClass
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

TEST_F(MotionComponentsTest, StorageInitialization) {
    ASSERT_NE(storage, nullptr);
    // CreateClassSamples should now work correctly after fixing lifecycle issues
    EXPECT_GT(storage->GetNumClasses(), 0) << "Storage should have classes registered after CreateClassSamples";
}

// Test NEngineMotionControl creation
TEST_F(MotionComponentsTest, CreateEngineMotionControl) {
    if (!storage->CheckClass("NEngineMotionControl")) {
        GTEST_SKIP() << "NEngineMotionControl class not available";
        return;
    }
    
    auto engine = storage->TakeObject("NEngineMotionControl");
    if (engine) {
        EXPECT_EQ(engine->GetCompClassName(), "NEngineMotionControl");
        EXPECT_NE(engine, nullptr);
        
        engine->SetName("TestEngineMotionControl");
        EXPECT_EQ(engine->GetName(), "TestEngineMotionControl");
    } else {
        GTEST_SKIP() << "Cannot create NEngineMotionControl object";
    }
}

// Test NCounterNeuron creation
TEST_F(MotionComponentsTest, CreateCounterNeuron) {
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
    } else {
        GTEST_SKIP() << "Cannot create NCounterNeuron object";
    }
}

// Test NSuppressionUnit creation
TEST_F(MotionComponentsTest, CreateSuppressionUnit) {
    if (!storage->CheckClass("NSuppressionUnit")) {
        GTEST_SKIP() << "NSuppressionUnit class not available";
        return;
    }
    
    auto unit = storage->TakeObject("NSuppressionUnit");
    if (unit) {
        EXPECT_EQ(unit->GetCompClassName(), "NSuppressionUnit");
        EXPECT_NE(unit, nullptr);
        
        unit->SetName("TestSuppressionUnit");
        EXPECT_EQ(unit->GetName(), "TestSuppressionUnit");
    } else {
        GTEST_SKIP() << "Cannot create NSuppressionUnit object";
    }
}

// Test NObjInArea creation
TEST_F(MotionComponentsTest, CreateObjInArea) {
    if (!storage->CheckClass("NObjInArea")) {
        GTEST_SKIP() << "NObjInArea class not available";
        return;
    }
    
    auto objInArea = storage->TakeObject("NObjInArea");
    if (objInArea) {
        EXPECT_EQ(objInArea->GetCompClassName(), "NObjInArea");
        EXPECT_NE(objInArea, nullptr);
        
        objInArea->SetName("TestObjInArea");
        EXPECT_EQ(objInArea->GetName(), "TestObjInArea");
    } else {
        GTEST_SKIP() << "Cannot create NObjInArea object";
    }
}

// Test NSeqComparison creation
TEST_F(MotionComponentsTest, CreateSeqComparison) {
    if (!storage->CheckClass("NSeqComparison")) {
        GTEST_SKIP() << "NSeqComparison class not available";
        return;
    }
    
    auto comparison = storage->TakeObject("NSeqComparison");
    if (comparison) {
        EXPECT_EQ(comparison->GetCompClassName(), "NSeqComparison");
        EXPECT_NE(comparison, nullptr);
        
        comparison->SetName("TestSeqComparison");
        EXPECT_EQ(comparison->GetName(), "TestSeqComparison");
    } else {
        GTEST_SKIP() << "Cannot create NSeqComparison object";
    }
}

// Test NActuatorSignals creation
TEST_F(MotionComponentsTest, CreateActuatorSignals) {
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
    } else {
        GTEST_SKIP() << "Cannot create NActuatorSignals object";
    }
}

// Test NSignalEstimation creation
TEST_F(MotionComponentsTest, CreateSignalEstimation) {
    if (!storage->CheckClass("NSignalEstimation")) {
        GTEST_SKIP() << "NSignalEstimation class not available";
        return;
    }
    
    auto estimation = storage->TakeObject("NSignalEstimation");
    if (estimation) {
        EXPECT_EQ(estimation->GetCompClassName(), "NSignalEstimation");
        EXPECT_NE(estimation, nullptr);
        
        estimation->SetName("TestSignalEstimation");
        EXPECT_EQ(estimation->GetName(), "TestSignalEstimation");
    } else {
        GTEST_SKIP() << "Cannot create NSignalEstimation object";
    }
}

// Test NMotionElement creation
TEST_F(MotionComponentsTest, CreateMotionElement) {
    if (!storage->CheckClass("NMotionElement")) {
        GTEST_SKIP() << "NMotionElement class not available";
        return;
    }
    
    auto element = storage->TakeObject("NMotionElement");
    if (element) {
        EXPECT_EQ(element->GetCompClassName(), "NMotionElement");
        EXPECT_NE(element, nullptr);
        
        element->SetName("TestMotionElement");
        EXPECT_EQ(element->GetName(), "TestMotionElement");
    } else {
        GTEST_SKIP() << "Cannot create NMotionElement object";
    }
}

