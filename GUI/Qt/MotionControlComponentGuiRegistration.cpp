#include "../../../../Rdk/GUI/Qt/UComponentFormRegistry.h"
#include "../../../../Rdk/GUI/Qt/UGenericComponentControllerWidget.h"

namespace
{
UComponentFormDescriptor MakeMotionDescriptor(const QString& id, const QString& title)
{
    UComponentFormDescriptor descriptor;
    descriptor.formId = id;
    descriptor.title = title;
    descriptor.singleInstance = true;
    descriptor.factory = [id, title](RDK::UApplication* app) -> UVisualControllerWidget*
    {
        return new UGenericComponentControllerWidget(id, title, nullptr, app);
    };
    return descriptor;
}
}

void RegisterMotionControlComponentGuiForms()
{
    UComponentFormRegistry& registry = UComponentFormRegistry::instance();

    const UComponentFormDescriptor manipDescriptor =
        MakeMotionDescriptor("motion.manipulator.control", "Motion Control: Manipulator");
    const char* manipClasses[] = {
        "NAslsNewSimplestAfferentBranchedEngineControlPM",
        "NAsfNewSimplestAfferentBranchedEngineControlPM",
        "NAslsNewSimplestAfferentBranchedEngineControl",
        "NAsfNewSimplestAfferentBranchedEngineControl",
        "NNewSimplestAfferentBranchedEngineControl",
        "NSimplestAfferentBranchedEngineControl",
        "NSimplestBranchedEngineControl",
        "NSimplestEngineControl",
        "NContinuesIndRangeBranchedCrosslinksEngineControlRangeAfferent",
        "NIndRangeBranchedCrosslinksEngineControlRangeAfferent",
        "NIndRangeBranchedEngineControlRangeAfferent",
        "NSimpleBranchedCrosslinksEngineControlRangeAfferent",
        "NBranchedCrosslinksEngineControlRangeAfferent",
        "NSimpleBranchedEngineControlRangeAfferent",
        "NBranchedEngineControlRangeAfferent",
        "NSimpleEngineControlRangeAfferent",
        "NSimpleEngineControlSignumAfferent",
        "NEngineControlRangeAfferent",
        "NEngineControlSignumAfferent"
    };
    for(const char* cls : manipClasses)
        registry.registerFormFactory(QString::fromLatin1(cls), manipDescriptor);

    registry.registerFormFactory("N2AsfNewSimplestAfferentBranchedEngineControl",
                                 MakeMotionDescriptor("motion.newmanipulator.control", "Motion Control: New Manipulator"));
    registry.registerFormFactory("N2AsfSimplestAfferentBranchedEngineControl",
                                 MakeMotionDescriptor("motion.newmanipulator.control", "Motion Control: New Manipulator"));
    registry.registerFormFactory("NNewPositionControlElement",
                                 MakeMotionDescriptor("motion.position.control", "Motion Control: Position Element"));
    registry.registerFormFactory("NAstaticGyro",
                                 MakeMotionDescriptor("motion.gyro.control", "Motion Control: Astatic Gyro"));
}
