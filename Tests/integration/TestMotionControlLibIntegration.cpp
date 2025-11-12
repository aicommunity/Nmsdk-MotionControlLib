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

class MotionControlLibIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        InitTestLogging();
        storage = CreateStorageWithLibraries({&NMSDK::MotionControlLibrary});
        NMSDK::MotionControlLibrary.Upload(storage.get());
        NMSDK::MotionControlLibrary.CreateClassSamples(storage.get());
        environment = CreateTestEnvironment(storage);
    }

    void TearDown() override {
        environment.reset();
        storage.reset();
    }

    std::shared_ptr<UStorage> storage;
    std::shared_ptr<UEnvironment> environment;
};

TEST_F(MotionControlLibIntegrationTest, MotionControlLibEngineIntegration) {
    ASSERT_NE(storage, nullptr);
    ASSERT_NE(environment, nullptr);
    EXPECT_GT(storage->GetNumClasses(), 0);
}

TEST_F(MotionControlLibIntegrationTest, MotionControlLibEnvironmentIntegration) {
    if (!environment->IsInit()) {
        environment->Default();
        environment->Init();
    }

    EXPECT_TRUE(environment->IsInit());
    UStorage* envStorage = environment->GetStorage();
    ASSERT_NE(envStorage, nullptr);
    EXPECT_EQ(envStorage, storage.get());
}

TEST_F(MotionControlLibIntegrationTest, MotionControlLibModelCreation) {
    if (!environment->IsInit()) {
        environment->Default();
        environment->Init();
    }

    auto model = CreateTestModel(storage, environment);
    if (!model) {
        GTEST_SKIP() << "Cannot create model in environment";
        return;
    }

    const std::string controllerName =
        FindMotionComponentName(*storage, {"NPositionControlElement", "NNewPositionControlElement", "NNavMousePrimitive"},
                                {"NPosition", "NNav"});
    if (controllerName.empty()) {
        GTEST_SKIP() << "MotionControl library lacks control elements";
        return;
    }

    auto control = storage->TakeObject(controllerName);
    ASSERT_NE(control, nullptr);
    control->SetName("MotionController");
    ASSERT_TRUE(model->AddComponent(control));

    const std::string sensorName =
        FindMotionComponentName(*storage, {"NSignalEstimation", "NSimpleStatistic"}, {"NSignal", "NSimple"});
    if (!sensorName.empty()) {
        auto sensor = storage->TakeObject(sensorName);
        if (sensor) {
            sensor->SetName("MotionSensor");
            model->AddComponent(sensor);
        }
    }

    EXPECT_GE(model->GetNumComponents(), 1);
}

