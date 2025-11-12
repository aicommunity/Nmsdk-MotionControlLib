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
        NMSDK::MotionControlLibrary.CreateClassSamples(storage.get());
    }

    void TearDown() override {
        storage.reset();
    }

    std::shared_ptr<UStorage> storage;
};

TEST_F(MotionComponentsTest, RegistersManipulatorAndControlClasses) {
    ASSERT_NE(storage, nullptr);
    EXPECT_TRUE(storage->CheckClass("NManipulator")) << "Manipulator should be registered";
    EXPECT_TRUE(storage->CheckClass("NNavMousePrimitive")) << "Navigation primitive should be registered";
}

TEST_F(MotionComponentsTest, CreateManipulatorComponent) {
    const std::string className =
        FindMotionComponentName(*storage, {"NManipulator", "NManipulatorSource"}, {"NManip"});
    if (className.empty()) {
        GTEST_SKIP() << "MotionControl library did not expose manipulator classes";
        return;
    }

    auto component = storage->TakeObject(className);
    ASSERT_NE(component, nullptr);
    component->SetName("MotionManipulator");
    EXPECT_EQ(component->GetName(), "MotionManipulator");
    EXPECT_TRUE(storage->CheckObject(component));
    storage->ReturnObject(component);
}

TEST_F(MotionComponentsTest, CreateControlElement) {
    const std::string className =
        FindMotionComponentName(*storage, {"NNavMousePrimitive", "NPositionControlElement", "NNewPositionControlElement"},
                                {"NNav", "NPosition"});
    if (className.empty()) {
        GTEST_SKIP() << "MotionControl library did not expose control element classes";
        return;
    }

    auto control = storage->TakeObject(className);
    ASSERT_NE(control, nullptr);
    EXPECT_FALSE(control->GetCompClassName().empty());
    EXPECT_TRUE(control->Activity);
    storage->ReturnObject(control);
}

