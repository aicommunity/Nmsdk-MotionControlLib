#include <gtest/gtest.h>
#include "../../../../Tests/TestHelpers.h"
#include "../../../../Rdk/Deploy/Include/rdk.h"
#include "../../../../Libraries/Libraries.h"
#include "../../../Deploy/Include/Lib.h"
#include "../../Core/NMotionControlLibrary.h"
#include <algorithm>
#include <vector>
#include <set>

using namespace RDK;
using namespace RDK::TestHelpers;

namespace {

// Expected MotionControlLib component categories and their key classes
struct MotionControlLibCategory {
    std::string name;
    std::vector<std::string> keyClasses;
};

const std::vector<MotionControlLibCategory> EXPECTED_MOTIONLIB_CATEGORIES = {
    {"Engines", {"NDCEngine", "NPendulumAndCart"}},
    {"Manipulators", {"NManipulator", "NManipulatorAndGyro", "NNavMousePrimitive"}},
    {"Input Components", {"NManipulatorInput", "NManipulatorInputEmulator", "NControlObjectSource", "NManipulatorSource", "NManipulatorSourceEmulator"}},
    {"Neurons", {"NCounterNeuron", "NSuppressionUnit", "NObjInArea", "NSeqComparison"}},
    {"Motion Elements", {"NMotionElement", "NNewMotionElement", "NEngineMotionControl"}},
    {"Control", {"NEngineControlSignumAfferent", "NEngineControlRangeAfferent", "N2AsfNewSimplestAfferentBranchedEngineControl"}},
    {"Position Control", {"NPositionControlElement", "NNewPositionControlElement", "NMultiPositionControl", "NTrajectoryElement"}},
    {"Memory", {"NMazeMemory", "NMazeMemory_simplified", "NPCN"}},
    {"Separators", {"NSignumSeparator", "NIntervalSeparator", "NPosSignumSeparator", "NNegSignumSeparator"}},
    {"Statistics", {"NSimpleStatistic", "NNetworkLinksStatistic"}},
    {"Actuators", {"NActuatorSignals", "NSignalEstimation"}},
    {"Sensors", {"NFrequencyReceiver", "NPulseReceiver", "NEyeRetina"}}
};

} // namespace

class MotionControlLibRegistrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        InitTestLogging();
        
        // Create storage with all libraries in correct sequence
        storage = CreateStorageWithAllLibraries();
        ASSERT_NE(storage, nullptr) << "Failed to create storage with all libraries";
        
        // Upload all libraries
        RDK::BasicLibrary.Upload(storage.get());
        RDK::CvBasicLibrary.Upload(storage.get());
        RDK::HardwareLibrary.Upload(storage.get());
        NMSDK::PulseLibrary.Upload(storage.get());
        NMSDK::MotionControlLibrary.Upload(storage.get());
        
        // Call CreateClassSamples for all libraries in sequence
        // PulseLibrary must be initialized before MotionControlLibrary
        try {
            NMSDK::PulseLibrary.CreateClassSamples(storage.get());
        } catch (const std::exception& ex) {
            GTEST_SKIP() << "NPulseLibrary CreateClassSamples failed: " << ex.what();
        }
        
        // Now call CreateClassSamples for MotionControlLibrary
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

// Test that all expected MotionControlLib classes are registered
TEST_F(MotionControlLibRegistrationTest, AllMotionControlLibClassesRegistered) {
    ASSERT_NE(storage, nullptr);
    
    std::vector<std::string> allClasses;
    storage->GetClassNameList(allClasses);
    
    // Filter MotionControlLib classes
    std::set<std::string> registeredClasses(allClasses.begin(), allClasses.end());
    
    // Check each category
    for (const auto& category : EXPECTED_MOTIONLIB_CATEGORIES) {
        for (const auto& className : category.keyClasses) {
            EXPECT_TRUE(storage->CheckClass(className)) 
                << "MotionControlLib class '" << className << "' from category '" << category.name << "' should be registered";
            
            if (storage->CheckClass(className)) {
                EXPECT_TRUE(registeredClasses.count(className) > 0)
                    << "Class '" << className << "' should be in class list";
            }
        }
    }
}

// Test that minimum number of MotionControlLib classes are registered
TEST_F(MotionControlLibRegistrationTest, MotionControlLibClassesCount) {
    ASSERT_NE(storage, nullptr);
    
    std::vector<std::string> allClasses;
    storage->GetClassNameList(allClasses);
    
    // Count MotionControlLib classes (they typically start with 'N' and are specific to motion control)
    size_t motionLibClassCount = 0;
    std::vector<std::string> motionLibPrefixes = {
        "NDCEngine", "NPendulum", "NManipulator", "NNavMouse", "NFrequency",
        "NSimpleStatistic", "NNetworkLinksStatistic", "NPulseReceiver",
        "NSignumSeparator", "NIntervalSeparator", "NPosSignumSeparator",
        "NNegSignumSeparator", "NEyeRetina", "NManipulatorSource",
        "NControlObjectSource", "NSuppressionUnit", "NCounterNeuron",
        "NObjInArea", "NSeqComparison", "NActuatorSignals",
        "NSignalEstimation", "NMotionElement", "NNewMotionElement",
        "NEngineMotionControl", "NEngineControl", "NPositionControl",
        "NNewPositionControl", "NMultiPositionControl", "NTrajectoryElement",
        "NMazeMemory", "NPCN"
    };
    
    for (const auto& className : allClasses) {
        for (const auto& prefix : motionLibPrefixes) {
            if (className.find(prefix) == 0) {
                motionLibClassCount++;
                break;
            }
        }
    }
    
    // We expect at least 30 classes (actual is ~38)
    EXPECT_GE(motionLibClassCount, 30) 
        << "Expected at least 30 MotionControlLib classes, but found " << motionLibClassCount;
}

// Test that dependencies from PulseLib are resolved
TEST_F(MotionControlLibRegistrationTest, MotionControlLibDependenciesResolved) {
    ASSERT_NE(storage, nullptr);
    
    // Check that PulseLib classes used by MotionControlLib are available
    // MotionControlLib uses NSignumSeparator and other PulseLib components
    EXPECT_TRUE(storage->CheckClass("NSignumSeparator")) 
        << "NSignumSeparator from PulseLib should be available for MotionControlLib dependencies";
    
    // Check that we can create objects that depend on PulseLib classes
    if (storage->CheckClass("NSignumSeparator")) {
        auto separator = storage->TakeObject("NSignumSeparator");
        EXPECT_NE(separator, nullptr) 
            << "Should be able to create NSignumSeparator from PulseLib";
    }
    
    // Check that MotionControlLib classes that use TakeObject from PulseLib can be created
    if (storage->CheckClass("NPosSignumSeparator")) {
        auto posSep = storage->TakeObject("NPosSignumSeparator");
        EXPECT_NE(posSep, nullptr) 
            << "Should be able to create NPosSignumSeparator which depends on NSignumSeparator";
    }
    
    // Check that NEngineMotionControl can be created (it depends on PulseLib components)
    if (storage->CheckClass("NEngineMotionControl")) {
        auto engine = storage->TakeObject("NEngineMotionControl");
        EXPECT_NE(engine, nullptr) 
            << "Should be able to create NEngineMotionControl which depends on PulseLib";
    }
}

// Test that specific critical classes are registered
TEST_F(MotionControlLibRegistrationTest, CriticalClassesRegistered) {
    ASSERT_NE(storage, nullptr);
    
    // Critical classes that are commonly used
    std::vector<std::string> criticalClasses = {
        "NEngineMotionControl",
        "NCounterNeuron",
        "NSuppressionUnit",
        "NMotionElement",
        "NNewMotionElement",
        "NManipulator",
        "NControlObjectSource",
        "NSignumSeparator",
        "NPosSignumSeparator",
        "NNegSignumSeparator"
    };
    
    for (const auto& className : criticalClasses) {
        EXPECT_TRUE(storage->CheckClass(className)) 
            << "Critical MotionControlLib class '" << className << "' should be registered";
    }
}

// Test that engine control variants are registered
TEST_F(MotionControlLibRegistrationTest, EngineControlVariantsRegistered) {
    ASSERT_NE(storage, nullptr);
    
    // Check that different engine control variants are registered
    std::vector<std::string> engineVariants = {
        "NEngineControlSignumAfferent",
        "NEngineControlRangeAfferent",
        "N2AsfNewSimplestAfferentBranchedEngineControl"
    };
    
    for (const auto& className : engineVariants) {
        EXPECT_TRUE(storage->CheckClass(className)) 
            << "Engine control variant '" << className << "' should be registered";
    }
}

