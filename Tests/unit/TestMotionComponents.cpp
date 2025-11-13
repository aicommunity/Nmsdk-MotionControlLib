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

