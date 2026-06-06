#ifndef MOTIONCONTROLASTATICGYROCOMPONENTCONTROLLERWIDGET_H
#define MOTIONCONTROLASTATICGYROCOMPONENTCONTROLLERWIDGET_H

#include <QLabel>

#include "../../../../Rdk/GUI/Qt/IComponentControllerWidget.h"
#include "../../../../Rdk/GUI/Qt/UVisualControllerWidget.h"
#include "../../../../Rdk/GUI/Qt/UComponentGuiContext.h"

class MotionControlAstaticGyroComponentControllerWidget : public UVisualControllerWidget, public IComponentControllerWidget
{
    Q_OBJECT
public:
    explicit MotionControlAstaticGyroComponentControllerWidget(QWidget* parent = nullptr, RDK::UApplication* app = nullptr);

    void setComponentContext(const UComponentGuiContext& context) override;
    void refreshFromModel(bool force) override;
    QString componentGuiId() const override;

private:
    UComponentGuiContext m_context;
    QLabel* m_infoLabel = nullptr;
};

#endif // MOTIONCONTROLASTATICGYROCOMPONENTCONTROLLERWIDGET_H

