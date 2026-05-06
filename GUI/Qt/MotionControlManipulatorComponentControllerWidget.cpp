#include "MotionControlManipulatorComponentControllerWidget.h"

#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <cmath>

#include "../../../../Rdk/Deploy/Include/rdk_init.h"
#include "../../../Rdk-BasicLib/Core/UStatistic.h"
#include "../../Core/NDCEngine.h"
#include "../../Core/NEngineMotionControl.h"
#include "../../Core/NPendulumAndCart.h"
#include "../../../Nmsdk-PulseLib/Core/NModel.h"

namespace
{
constexpr int kMaxMotionElements = 50;
constexpr int kMaxControlLoops = 8;
constexpr int kMaxNumAfferent = 20;
constexpr int kSliderScale = 1000;
}

MotionControlManipulatorComponentControllerWidget::MotionControlManipulatorComponentControllerWidget(QWidget* parent, RDK::UApplication* app)
    : UVisualControllerWidget(parent, app)
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    m_titleLabel = new QLabel("Manipulator control (partial parity)", this);
    mainLayout->addWidget(m_titleLabel);

    m_statusLabel = new QLabel(this);
    mainLayout->addWidget(m_statusLabel);

    m_manipulatorLabel = new QLabel(this);
    mainLayout->addWidget(m_manipulatorLabel);

    auto* buttonsLayout = new QHBoxLayout();
    m_resetComponentButton = new QPushButton("Reset component", this);
    m_calculateComponentButton = new QPushButton("Calculate component", this);
    m_saveStatsButton = new QPushButton("Save stats", this);
    m_newStatsButton = new QPushButton("New stats", this);
    buttonsLayout->addWidget(m_resetComponentButton);
    buttonsLayout->addWidget(m_calculateComponentButton);
    buttonsLayout->addWidget(m_saveStatsButton);
    buttonsLayout->addWidget(m_newStatsButton);
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    auto* settingsForm = new QFormLayout();
    settingsForm->setLabelAlignment(Qt::AlignLeft);

    m_controlVoltageCheck = new QCheckBox("Enable control voltage", this);
    settingsForm->addRow("Control voltage", m_controlVoltageCheck);

    auto createSliderRow = [this](int min, int max, int step, QSlider*& slider, QLabel*& valueLabel) -> QWidget*
    {
        QWidget* row = new QWidget(this);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        slider = new QSlider(Qt::Horizontal, row);
        slider->setRange(min, max);
        slider->setSingleStep(step);
        valueLabel = new QLabel("-", row);
        valueLabel->setMinimumWidth(80);
        rowLayout->addWidget(slider, 1);
        rowLayout->addWidget(valueLabel);
        return row;
    };

    settingsForm->addRow("Num motion elements",
                         createSliderRow(1, kMaxMotionElements, 1, m_numMotionElementsSlider, m_numMotionElementsValue));
    settingsForm->addRow("Num control loops",
                         createSliderRow(1, kMaxControlLoops, 1, m_numControlLoopsSlider, m_numControlLoopsValue));
    settingsForm->addRow("II num afferent",
                         createSliderRow(1, kMaxNumAfferent, 1, m_iiNumAfferentSlider, m_iiNumAfferentValue));
    settingsForm->addRow("II afferent frequency",
                         createSliderRow(-kSliderScale, kSliderScale, 1, m_iiAfferentSlider, m_iiAfferentValue));
    settingsForm->addRow("Pac gain",
                         createSliderRow(0, kSliderScale, 1, m_pacGainSlider, m_pacGainValue));
    settingsForm->addRow("Pac secretion TC",
                         createSliderRow(0, kSliderScale, 1, m_pacSecretionSlider, m_pacSecretionValue));
    settingsForm->addRow("Pac dissociation TC",
                         createSliderRow(0, kSliderScale, 1, m_pacDissociationSlider, m_pacDissociationValue));

    m_sensorDivisionCombo = new QComboBox(this);
    m_sensorDivisionCombo->addItem("Signum", 0);
    m_sensorDivisionCombo->addItem("Range", 1);
    settingsForm->addRow("Afferent range mode", m_sensorDivisionCombo);
    m_branchModeCheck = new QCheckBox("Enable motoneuron branching", this);
    settingsForm->addRow("Motoneuron branch mode", m_branchModeCheck);
    m_renshowModeCheck = new QCheckBox("Enable Renshow cells", this);
    settingsForm->addRow("Renshow mode", m_renshowModeCheck);
    m_structuralAdaptationCheck = new QCheckBox("Enable structural adaptation", this);
    settingsForm->addRow("Structural adaptation", m_structuralAdaptationCheck);
    m_useSimpleAfferentsCheck = new QCheckBox("Use simple afferents (legacy advanced)", this);
    m_useSimpleAfferentsCheck->setEnabled(false);
    m_useSimpleAfferentsCheck->setToolTip("Not supported in Qt parity yet.");
    settingsForm->addRow("Use simple afferents", m_useSimpleAfferentsCheck);
    m_useNewNeuronsCheck = new QCheckBox("Use new neurons (legacy advanced)", this);
    m_useNewNeuronsCheck->setEnabled(false);
    m_useNewNeuronsCheck->setToolTip("Not supported in Qt parity yet.");
    settingsForm->addRow("Use new neurons", m_useNewNeuronsCheck);

    settingsForm->addRow("Moment",
                         createSliderRow(-kSliderScale, kSliderScale, 1, m_momentSlider, m_momentValue));
    settingsForm->addRow("Movement",
                         createSliderRow(-kSliderScale, kSliderScale, 1, m_movementSlider, m_movementValue));

    mainLayout->addLayout(settingsForm);

    auto* contoursFrame = new QFrame(this);
    contoursFrame->setFrameShape(QFrame::StyledPanel);
    auto* contoursFrameLayout = new QVBoxLayout(contoursFrame);
    contoursFrameLayout->setContentsMargins(6, 6, 6, 6);
    contoursFrameLayout->addWidget(new QLabel("Active contours", contoursFrame));
    m_contoursContainer = new QWidget(contoursFrame);
    m_contoursLayout = new QVBoxLayout(m_contoursContainer);
    m_contoursLayout->setContentsMargins(0, 0, 0, 0);
    m_contoursLayout->setSpacing(2);
    contoursFrameLayout->addWidget(m_contoursContainer);
    mainLayout->addWidget(contoursFrame);

    auto* hardwareLayout = new QGridLayout();
    m_hardwareLoadButton = new QPushButton("Load manipulator DLL", this);
    m_hardwareStartButton = new QPushButton("Hardware start", this);
    m_hardwareStopButton = new QPushButton("Hardware stop", this);
    m_hardwareResetButton = new QPushButton("Hardware reset", this);
    m_hardwareMoveServoButton = new QPushButton("Move servo", this);
    hardwareLayout->addWidget(m_hardwareLoadButton, 0, 0);
    hardwareLayout->addWidget(m_hardwareStartButton, 0, 1);
    hardwareLayout->addWidget(m_hardwareStopButton, 0, 2);
    hardwareLayout->addWidget(m_hardwareResetButton, 1, 0);
    hardwareLayout->addWidget(m_hardwareMoveServoButton, 1, 1);
    mainLayout->addLayout(hardwareLayout);
    setHardwareControlsEnabled(false);

    connect(m_resetComponentButton, &QPushButton::clicked, this, &MotionControlManipulatorComponentControllerWidget::onResetComponentClicked);
    connect(m_calculateComponentButton, &QPushButton::clicked, this, &MotionControlManipulatorComponentControllerWidget::onCalculateComponentClicked);
    connect(m_controlVoltageCheck, &QCheckBox::toggled, this, &MotionControlManipulatorComponentControllerWidget::onControlVoltageToggled);
    connect(m_numMotionElementsSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onNumMotionElementsChanged);
    connect(m_numControlLoopsSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onNumControlLoopsChanged);
    connect(m_iiNumAfferentSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onIINumAfferentChanged);
    connect(m_iiAfferentSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onIIAfferentChanged);
    connect(m_pacGainSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onPacGainChanged);
    connect(m_pacSecretionSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onPacSecretionChanged);
    connect(m_pacDissociationSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onPacDissociationChanged);
    connect(m_sensorDivisionCombo, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MotionControlManipulatorComponentControllerWidget::onSensorDivisionChanged);
    connect(m_branchModeCheck, &QCheckBox::toggled, this, &MotionControlManipulatorComponentControllerWidget::onBranchModeToggled);
    connect(m_renshowModeCheck, &QCheckBox::toggled, this, &MotionControlManipulatorComponentControllerWidget::onRenshowModeToggled);
    connect(m_structuralAdaptationCheck, &QCheckBox::toggled, this, &MotionControlManipulatorComponentControllerWidget::onStructuralAdaptationToggled);
    connect(m_momentSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onMomentChanged);
    connect(m_movementSlider, &QSlider::valueChanged, this, &MotionControlManipulatorComponentControllerWidget::onMovementChanged);
    connect(m_saveStatsButton, &QPushButton::clicked, this, &MotionControlManipulatorComponentControllerWidget::onSaveStatsClicked);
    connect(m_newStatsButton, &QPushButton::clicked, this, &MotionControlManipulatorComponentControllerWidget::onNewStatsClicked);

    setAccessibleName("MotionControlManipulatorComponentControllerWidget");
}

void MotionControlManipulatorComponentControllerWidget::setComponentContext(const UComponentGuiContext& context)
{
    m_context = context;
    refreshFromModel(true);
}

void MotionControlManipulatorComponentControllerWidget::refreshFromModel(bool force)
{
    Q_UNUSED(force);

    if(m_context.componentLongName.isEmpty())
    {
        m_statusLabel->setText("No active component context.");
        return;
    }

    auto model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
    {
        m_statusLabel->setText("Model lock timeout.");
        return;
    }

    m_titleLabel->setText(QString("Manipulator control for %1").arg(m_context.componentLongName));
    m_manipulatorLabel->setText(QString("Manipulator object: %1").arg(resolveManipulatorName(model)));

    setUiFromControlSystem();
}

QString MotionControlManipulatorComponentControllerWidget::componentGuiId() const
{
    return "motion.manipulator.control";
}

bool MotionControlManipulatorComponentControllerWidget::withControlSystem(
    const std::function<void(const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl*)>& fn) const
{
    if(m_context.componentLongName.isEmpty())
        return false;

    auto model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
        return false;

    const std::string controlSystemName = m_context.componentLongName.toStdString();
    auto controlSystem = RDK::dynamic_pointer_cast<NMSDK::NEngineMotionControl>(model->GetComponentL(controlSystemName));
    if(!controlSystem)
        return false;

    fn(model, controlSystem.Get());
    return true;
}

QString MotionControlManipulatorComponentControllerWidget::resolveManipulatorName(const RDK::UELockPtr<NMSDK::NModel>& model) const
{
    if(!model || m_context.componentLongName.isEmpty())
        return "N/A";

    const std::string controlSystemName = m_context.componentLongName.toStdString();
    auto controlSystem = RDK::dynamic_pointer_cast<NMSDK::NEngineMotionControl>(model->GetComponentL(controlSystemName));
    if(!controlSystem)
        return "not connected";

    const std::string manipName = controlSystem->MCNeuroObjectName.GetData();
    return manipName.empty() ? "not connected" : QString::fromStdString(manipName);
}

void MotionControlManipulatorComponentControllerWidget::rebuildContoursUi(int count)
{
    if(count < 0)
        count = 0;
    if(m_contourChecks.size() == count)
        return;

    qDeleteAll(m_contourChecks);
    m_contourChecks.clear();
    while(QLayoutItem* item = m_contoursLayout->takeAt(0))
    {
        delete item;
    }

    for(int i = 0; i < count; ++i)
    {
        auto* box = new QCheckBox(QString("Contour %1").arg(i), m_contoursContainer);
        connect(box, &QCheckBox::toggled, this, &MotionControlManipulatorComponentControllerWidget::onContourCheckboxToggled);
        m_contoursLayout->addWidget(box);
        m_contourChecks.push_back(box);
    }
    m_contoursLayout->addStretch(1);
}

void MotionControlManipulatorComponentControllerWidget::setUiFromControlSystem()
{
    bool found = withControlSystem([this](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        m_updatingUi = true;
        m_statusLabel->setText("Model is connected.");

        m_numMotionElementsSlider->setValue(cs->NumMotionElements.GetData());
        m_numMotionElementsValue->setText(QString::number(cs->NumMotionElements.GetData()));

        m_numControlLoopsSlider->setValue(cs->NumControlLoops.GetData());
        m_numControlLoopsValue->setText(QString::number(cs->NumControlLoops.GetData()));

        m_iiNumAfferentSlider->setValue(1);
        m_iiNumAfferentValue->setText(QString::number(m_iiNumAfferentSlider->value()));

        m_iiAfferentSlider->setValue(0);
        m_iiAfferentValue->setText("0.000");

        m_pacGainSlider->setValue(unitValueToSlider(cs->PacGain.GetData()));
        m_pacGainValue->setText(QString::number(cs->PacGain.GetData(), 'f', 3));

        m_pacSecretionSlider->setValue(unitValueToSlider(cs->PacSecretionTC.GetData()));
        m_pacSecretionValue->setText(QString::number(cs->PacSecretionTC.GetData(), 'f', 3));

        m_pacDissociationSlider->setValue(unitValueToSlider(cs->PacDissociationTC.GetData()));
        m_pacDissociationValue->setText(QString::number(cs->PacDissociationTC.GetData(), 'f', 3));

        int mode = cs->AfferentRangeMode.GetData();
        int modeIndex = m_sensorDivisionCombo->findData(mode);
        m_sensorDivisionCombo->setCurrentIndex(modeIndex >= 0 ? modeIndex : 0);
        m_branchModeCheck->setChecked(cs->MotoneuronBranchMode.GetData() != 0);
        m_renshowModeCheck->setChecked(cs->RenshowMode.GetData() != 0);
        m_structuralAdaptationCheck->setChecked(cs->AdaptiveStructureMode.GetData() != 0);

        rebuildContoursUi(cs->NumControlLoops.GetData());
        const std::vector<bool>& contours = cs->ActiveContours.GetData();
        for(int i = 0; i < m_contourChecks.size(); ++i)
        {
            const bool active = i < static_cast<int>(contours.size()) ? contours[static_cast<size_t>(i)] : false;
            m_contourChecks[i]->setChecked(active);
        }

        m_momentSlider->setValue(0);
        m_momentValue->setText("0.000");
        m_movementSlider->setValue(0);
        m_movementValue->setText("0.000");

        m_updatingUi = false;
    });
    if(!found)
    {
        m_statusLabel->setText("Control system component is unavailable.");
        return;
    }
}

void MotionControlManipulatorComponentControllerWidget::setHardwareControlsEnabled(bool enabled)
{
    const QString tip = enabled ? QString() : QString("WinAPI hardware control is not available in this build.");
    m_hardwareLoadButton->setEnabled(enabled);
    m_hardwareStartButton->setEnabled(enabled);
    m_hardwareStopButton->setEnabled(enabled);
    m_hardwareResetButton->setEnabled(enabled);
    m_hardwareMoveServoButton->setEnabled(enabled);
    m_hardwareLoadButton->setToolTip(tip);
    m_hardwareStartButton->setToolTip(tip);
    m_hardwareStopButton->setToolTip(tip);
    m_hardwareResetButton->setToolTip(tip);
    m_hardwareMoveServoButton->setToolTip(tip);
}

double MotionControlManipulatorComponentControllerWidget::sliderToUnitValue(int sliderValue) const
{
    return static_cast<double>(sliderValue) / static_cast<double>(kSliderScale);
}

int MotionControlManipulatorComponentControllerWidget::unitValueToSlider(double unitValue) const
{
    const double scaled = unitValue * static_cast<double>(kSliderScale);
    if(scaled < -kSliderScale)
        return -kSliderScale;
    if(scaled > kSliderScale)
        return kSliderScale;
    return static_cast<int>(scaled);
}

void MotionControlManipulatorComponentControllerWidget::onResetComponentClicked()
{
    if(m_context.componentLongName.isEmpty())
        return;
    Env_Reset(m_context.componentLongName.toLocal8Bit().constData());
    refreshFromModel(true);
}

void MotionControlManipulatorComponentControllerWidget::onCalculateComponentClicked()
{
    if(m_context.componentLongName.isEmpty())
        return;
    Env_Calculate(m_context.componentLongName.toLocal8Bit().constData());
    refreshFromModel(true);
}

void MotionControlManipulatorComponentControllerWidget::onControlVoltageToggled(bool checked)
{
    if(m_updatingUi || m_context.componentLongName.isEmpty())
        return;

    auto model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
        return;

    const std::string csName = m_context.componentLongName.toStdString();
    const std::string manipName = resolveManipulatorName(model).toStdString();
    if(manipName.empty() || manipName == "not connected" || manipName == "N/A")
        return;

    if(checked)
        model->CreateLink(csName + ".NManipulatorInput1", 0, manipName, 0);
    else
        model->BreakLink(csName + ".NManipulatorInput1", 0, manipName, 0);
}

void MotionControlManipulatorComponentControllerWidget::onNumMotionElementsChanged(int value)
{
    m_numMotionElementsValue->setText(QString::number(value));
    if(m_updatingUi)
        return;
    withControlSystem([value](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->NumMotionElements = value;
    });
}

void MotionControlManipulatorComponentControllerWidget::onNumControlLoopsChanged(int value)
{
    m_numControlLoopsValue->setText(QString::number(value));
    if(m_updatingUi)
        return;

    withControlSystem([this, value](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->NumControlLoops = value;
        rebuildContoursUi(value);
    });
}

void MotionControlManipulatorComponentControllerWidget::onIINumAfferentChanged(int value)
{
    m_iiNumAfferentValue->setText(QString::number(value));
    if(m_updatingUi)
        return;
    withControlSystem([this, value](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->ConnectInternalGenerators(m_iiAfferentSlider->value() < 0 ? 0 : 1, value, 0);
    });
}

void MotionControlManipulatorComponentControllerWidget::onIIAfferentChanged(int value)
{
    const double scaled = sliderToUnitValue(value);
    m_iiAfferentValue->setText(QString::number(scaled, 'f', 3));
    if(m_updatingUi)
        return;

    withControlSystem([this, value, scaled](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        const int direction = value < 0 ? 0 : 1;
        cs->SetInternalGeneratorFrequency(direction, m_iiNumAfferentSlider->value(), 0, std::abs(scaled));
    });
}

void MotionControlManipulatorComponentControllerWidget::onPacGainChanged(int value)
{
    const double scaled = sliderToUnitValue(value);
    m_pacGainValue->setText(QString::number(scaled, 'f', 3));
    if(m_updatingUi)
        return;
    withControlSystem([scaled](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->PacGain = scaled;
    });
}

void MotionControlManipulatorComponentControllerWidget::onPacSecretionChanged(int value)
{
    const double scaled = sliderToUnitValue(value);
    m_pacSecretionValue->setText(QString::number(scaled, 'f', 3));
    if(m_updatingUi)
        return;
    withControlSystem([scaled](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->PacSecretionTC = scaled;
    });
}

void MotionControlManipulatorComponentControllerWidget::onPacDissociationChanged(int value)
{
    const double scaled = sliderToUnitValue(value);
    m_pacDissociationValue->setText(QString::number(scaled, 'f', 3));
    if(m_updatingUi)
        return;
    withControlSystem([scaled](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->PacDissociationTC = scaled;
    });
}

void MotionControlManipulatorComponentControllerWidget::onSensorDivisionChanged(int index)
{
    if(m_updatingUi)
        return;
    withControlSystem([this, index](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->AfferentRangeMode = m_sensorDivisionCombo->itemData(index).toInt();
    });
}

void MotionControlManipulatorComponentControllerWidget::onContourCheckboxToggled()
{
    if(m_updatingUi)
        return;

    withControlSystem([this](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        std::vector<bool> contours(static_cast<size_t>(m_contourChecks.size()), false);
        for(int i = 0; i < m_contourChecks.size(); ++i)
            contours[static_cast<size_t>(i)] = m_contourChecks[i]->isChecked();
        cs->ActiveContours = contours;
    });
}

void MotionControlManipulatorComponentControllerWidget::onBranchModeToggled(bool checked)
{
    if(m_updatingUi)
        return;
    withControlSystem([checked](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->MotoneuronBranchMode = checked ? 1 : 0;
    });
}

void MotionControlManipulatorComponentControllerWidget::onRenshowModeToggled(bool checked)
{
    if(m_updatingUi)
        return;
    withControlSystem([checked](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->RenshowMode = checked ? 1 : 0;
    });
}

void MotionControlManipulatorComponentControllerWidget::onStructuralAdaptationToggled(bool checked)
{
    if(m_updatingUi)
        return;
    withControlSystem([checked](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        cs->AdaptiveStructureMode = checked ? 1 : 0;
    });
}

void MotionControlManipulatorComponentControllerWidget::onMomentChanged(int value)
{
    const double scaled = sliderToUnitValue(value);
    m_momentValue->setText(QString::number(scaled, 'f', 3));
    if(m_updatingUi || m_context.componentLongName.isEmpty())
        return;

    auto model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
        return;

    const QString manipName = resolveManipulatorName(model);
    if(manipName == "not connected" || manipName == "N/A")
        return;

    if(auto dc = RDK::dynamic_pointer_cast<NMSDK::NDCEngine>(model->GetComponentL(manipName.toStdString())))
        dc->OutMoment = scaled;
    if(auto pendulum = RDK::dynamic_pointer_cast<NMSDK::NPendulumAndCart>(model->GetComponentL(manipName.toStdString())))
        pendulum->ExtrenalMoment = scaled;
}

void MotionControlManipulatorComponentControllerWidget::onMovementChanged(int value)
{
    const double scaled = sliderToUnitValue(value);
    m_movementValue->setText(QString::number(scaled, 'f', 3));
    if(m_updatingUi || m_context.componentLongName.isEmpty())
        return;

    auto model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
        return;

    const QString manipName = resolveManipulatorName(model);
    if(manipName == "not connected" || manipName == "N/A")
        return;
    if(auto pendulum = RDK::dynamic_pointer_cast<NMSDK::NPendulumAndCart>(model->GetComponentL(manipName.toStdString())))
        pendulum->OutXMovement = scaled;
}

void MotionControlManipulatorComponentControllerWidget::onSaveStatsClicked()
{
    withControlSystem([](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        auto stats = cs->GetComponentL<RDK::UStatistic>("StatisticDoubleMatrix", true);
        if(stats)
            stats->ManualModeSwitch = true;
    });
}

void MotionControlManipulatorComponentControllerWidget::onNewStatsClicked()
{
    withControlSystem([](const RDK::UELockPtr<NMSDK::NModel>&, NMSDK::NEngineMotionControl* cs)
    {
        auto stats = cs->GetComponentL<RDK::UStatistic>("StatisticDoubleMatrix", true);
        if(stats)
        {
            stats->ManualModeSwitch = false;
            stats->ManualModeEnabled = true;
            stats->Reset();
        }
    });
}
