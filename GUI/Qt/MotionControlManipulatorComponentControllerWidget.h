#ifndef MOTIONCONTROLMANIPULATORCOMPONENTCONTROLLERWIDGET_H
#define MOTIONCONTROLMANIPULATORCOMPONENTCONTROLLERWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVector>
#include <QVBoxLayout>
#include <functional>

#include "../../../../Rdk/GUI/Qt/IComponentControllerWidget.h"
#include "../../../../Rdk/GUI/Qt/UComponentGuiContext.h"
#include "../../../../Rdk/GUI/Qt/UVisualControllerWidget.h"

namespace NMSDK
{
class NEngineMotionControl;
class NModel;
}

class MotionControlManipulatorComponentControllerWidget : public UVisualControllerWidget, public IComponentControllerWidget
{
    Q_OBJECT
public:
    explicit MotionControlManipulatorComponentControllerWidget(QWidget* parent = nullptr, RDK::UApplication* app = nullptr);

    void setComponentContext(const UComponentGuiContext& context) override;
    void refreshFromModel(bool force) override;
    QString componentGuiId() const override;

private slots:
    void onResetComponentClicked();
    void onCalculateComponentClicked();
    void onControlVoltageToggled(bool checked);
    void onNumMotionElementsChanged(int value);
    void onNumControlLoopsChanged(int value);
    void onIINumAfferentChanged(int value);
    void onIIAfferentChanged(int value);
    void onPacGainChanged(int value);
    void onPacSecretionChanged(int value);
    void onPacDissociationChanged(int value);
    void onSensorDivisionChanged(int index);
    void onContourCheckboxToggled();
    void onBranchModeToggled(bool checked);
    void onRenshowModeToggled(bool checked);
    void onStructuralAdaptationToggled(bool checked);
    void onMomentChanged(int value);
    void onMovementChanged(int value);
    void onSaveStatsClicked();
    void onNewStatsClicked();

private:
    bool withControlSystem(const std::function<void(const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl*)>& fn) const;
    QString resolveManipulatorName(const RDK::UELockPtr<NMSDK::NModel>& model) const;
    void rebuildContoursUi(int count);
    void setUiFromControlSystem();
    void setHardwareControlsEnabled(bool enabled);
    double sliderToUnitValue(int sliderValue) const;
    int unitValueToSlider(double unitValue) const;

private:
    UComponentGuiContext m_context;
    bool m_updatingUi = false;

    QLabel* m_titleLabel = nullptr;
    QLabel* m_statusLabel = nullptr;
    QLabel* m_manipulatorLabel = nullptr;

    QPushButton* m_resetComponentButton = nullptr;
    QPushButton* m_calculateComponentButton = nullptr;
    QPushButton* m_saveStatsButton = nullptr;
    QPushButton* m_newStatsButton = nullptr;

    QCheckBox* m_controlVoltageCheck = nullptr;

    QSlider* m_numMotionElementsSlider = nullptr;
    QLabel* m_numMotionElementsValue = nullptr;

    QSlider* m_numControlLoopsSlider = nullptr;
    QLabel* m_numControlLoopsValue = nullptr;

    QSlider* m_iiNumAfferentSlider = nullptr;
    QLabel* m_iiNumAfferentValue = nullptr;

    QSlider* m_iiAfferentSlider = nullptr;
    QLabel* m_iiAfferentValue = nullptr;

    QSlider* m_pacGainSlider = nullptr;
    QLabel* m_pacGainValue = nullptr;

    QSlider* m_pacSecretionSlider = nullptr;
    QLabel* m_pacSecretionValue = nullptr;

    QSlider* m_pacDissociationSlider = nullptr;
    QLabel* m_pacDissociationValue = nullptr;

    QComboBox* m_sensorDivisionCombo = nullptr;
    QCheckBox* m_branchModeCheck = nullptr;
    QCheckBox* m_renshowModeCheck = nullptr;
    QCheckBox* m_structuralAdaptationCheck = nullptr;
    QCheckBox* m_useSimpleAfferentsCheck = nullptr;
    QCheckBox* m_useNewNeuronsCheck = nullptr;

    QSlider* m_momentSlider = nullptr;
    QLabel* m_momentValue = nullptr;

    QSlider* m_movementSlider = nullptr;
    QLabel* m_movementValue = nullptr;

    QWidget* m_contoursContainer = nullptr;
    QVBoxLayout* m_contoursLayout = nullptr;
    QVector<QCheckBox*> m_contourChecks;

    QPushButton* m_hardwareLoadButton = nullptr;
    QPushButton* m_hardwareStartButton = nullptr;
    QPushButton* m_hardwareStopButton = nullptr;
    QPushButton* m_hardwareResetButton = nullptr;
    QPushButton* m_hardwareMoveServoButton = nullptr;
};

#endif // MOTIONCONTROLMANIPULATORCOMPONENTCONTROLLERWIDGET_H
