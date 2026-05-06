#include "../../../../Rdk/GUI/Qt/UComponentFormRegistry.h"
#include "MotionControlComponentControllerWidget.h"
#include "MotionControlAstaticGyroComponentControllerWidget.h"
#include "MotionControlNewPositionControlElementControllerWidget.h"

namespace
{
UComponentFormDescriptor MakeMotionManipulatorDescriptor(const QString& id, const QString& title)
{
    UComponentFormDescriptor descriptor;
    descriptor.formId = id;
    descriptor.title = title;
    descriptor.singleInstance = true;
    descriptor.factory = [id, title](RDK::UApplication* app) -> UVisualControllerWidget*
    {
        Q_UNUSED(id);
        Q_UNUSED(title);
        return new MotionControlComponentControllerWidget(nullptr, app);
    };
    return descriptor;
}

UComponentFormDescriptor MakeMotionAstaticGyroDescriptor(const QString& id, const QString& title)
{
    UComponentFormDescriptor descriptor;
    descriptor.formId = id;
    descriptor.title = title;
    descriptor.singleInstance = true;
    descriptor.factory = [](RDK::UApplication* app) -> UVisualControllerWidget*
    {
        return new MotionControlAstaticGyroComponentControllerWidget(nullptr, app);
    };
    return descriptor;
}

UComponentFormDescriptor MakeMotionNewPositionControlElementDescriptor(const QString& id, const QString& title)
{
    UComponentFormDescriptor descriptor;
    descriptor.formId = id;
    descriptor.title = title;
    descriptor.singleInstance = true;
    descriptor.factory = [](RDK::UApplication* app) -> UVisualControllerWidget*
    {
        return new MotionControlNewPositionControlElementControllerWidget(nullptr, app);
    };
    return descriptor;
}
}

void RegisterMotionControlComponentGuiForms()
{
    UComponentFormRegistry& registry = UComponentFormRegistry::instance();

    const UComponentFormDescriptor manipDescriptor =
        MakeMotionManipulatorDescriptor("motion.manipulator.control", "Motion Control: Manipulator");
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
                                 MakeMotionManipulatorDescriptor("motion.newmanipulator.control", "Motion Control: New Manipulator"));
    registry.registerFormFactory("N2AsfSimplestAfferentBranchedEngineControl",
                                 MakeMotionManipulatorDescriptor("motion.newmanipulator.control", "Motion Control: New Manipulator"));
    registry.registerFormFactory("NNewPositionControlElement",
                                 MakeMotionNewPositionControlElementDescriptor("motion.position.control",
                                                                                   "Motion Control: Position Element"));
    registry.registerFormFactory("NAstaticGyro",
                                 MakeMotionAstaticGyroDescriptor("motion.gyro.control", "Motion Control: Astatic Gyro"));
}
