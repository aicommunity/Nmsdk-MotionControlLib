#include "MotionControlAstaticGyroComponentControllerWidget.h"

#include <QVBoxLayout>

#include "../../../../Rdk/Deploy/Include/rdk_init.h"

#include "../../Core/NAstaticGyro.h"
#include "../../../Nmsdk-PulseLib/Core/NModel.h"

MotionControlAstaticGyroComponentControllerWidget::MotionControlAstaticGyroComponentControllerWidget(QWidget* parent,
                                                                                                   RDK::UApplication* app)
    : UVisualControllerWidget(parent, app)
{
    m_infoLabel = new QLabel(this);
    m_infoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_infoLabel);

    setAccessibleName("MotionControlAstaticGyroComponentControllerWidget");
}

void MotionControlAstaticGyroComponentControllerWidget::setComponentContext(const UComponentGuiContext& context)
{
    m_context = context;
}

void MotionControlAstaticGyroComponentControllerWidget::refreshFromModel(bool force)
{
    Q_UNUSED(force);

    if(m_context.componentLongName.isEmpty())
        return;

    RDK::UELockPtr<NMSDK::NModel> model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
    {
        m_infoLabel->setText("Model is not available.");
        return;
    }

    const auto* rawName = m_context.componentLongName.toLocal8Bit().constData();
    auto gyro = RDK::dynamic_pointer_cast<NMSDK::NAstaticGyro>(model->GetComponentL(rawName));
    if(!gyro)
    {
        m_infoLabel->setText("AstaticGyro component is not found in model.");
        return;
    }

    const double alpha = gyro->alpha;
    const double betta = gyro->betta;
    const double gamma = gyro->gamma;

    constexpr double kPi = 3.14159265358979323846;
    auto radToDeg = [](double rad) { return 180.0 * rad / kPi; };

    QString text;
    text += QString("Astatic Gyro: %1\n").arg(m_context.componentLongName);
    text += QString("radians:\n");
    text += QString("alpha = %1\n").arg(alpha, 0, 'f', 6);
    text += QString("betta = %1\n").arg(betta, 0, 'f', 6);
    text += QString("gamma = %1\n").arg(gamma, 0, 'f', 6);
    text += QString("degrees:\n");
    text += QString("alpha = %1\n").arg(radToDeg(alpha), 0, 'f', 6);
    text += QString("betta = %1\n").arg(radToDeg(betta), 0, 'f', 6);
    text += QString("gamma = %1\n").arg(radToDeg(gamma), 0, 'f', 6);

    m_infoLabel->setText(text);
}

QString MotionControlAstaticGyroComponentControllerWidget::componentGuiId() const
{
    return "motion.astaticgyro.controller";
}

