#include "MotionControlNewPositionControlElementControllerWidget.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QSignalBlocker>
#include <QHeaderView>
#include <QVBoxLayout>

#include "../../../../Rdk/Deploy/Include/rdk_init.h"

#include "../../Core/NNewPositionControlElement.h"

#include "../../../Nmsdk-PulseLib/Core/NModel.h"

#include "../../../Nmsdk-PulseLib/Core/NPulseGenerator.h"
#include "../../../Nmsdk-PulseLib/Core/NPulseNeuron.h"
#include "../../../Nmsdk-PulseLib/Core/NPulseLTZone.h"

MotionControlNewPositionControlElementControllerWidget::MotionControlNewPositionControlElementControllerWidget(QWidget* parent,
                                                                                                         RDK::UApplication* app)
    : UVisualControllerWidget(parent, app)
{
    m_trainPositionButton = new QPushButton("Train position", this);
    connect(m_trainPositionButton, &QPushButton::clicked, this, &MotionControlNewPositionControlElementControllerWidget::onTrainPositionClicked);

    m_selectedStateTable = new QTableWidget(this);
    m_trainedStateTable = new QTableWidget(this);

    m_selectedStateTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_selectedStateTable->setSelectionBehavior(QAbstractItemView::SelectItems);

    m_trainedStateTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_trainedStateTable->setSelectionBehavior(QAbstractItemView::SelectItems);

    // Default: allow editing only on selectedStateTable cells (we enforce editability in fill phase)
    m_selectedStateTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    m_trainedStateTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->addWidget(m_trainPositionButton);
    rootLayout->addWidget(m_selectedStateTable);
    rootLayout->addWidget(m_trainedStateTable);

    connect(m_selectedStateTable, &QTableWidget::cellChanged, this, &MotionControlNewPositionControlElementControllerWidget::onSelectedTableCellChanged,
            Qt::UniqueConnection);

    setAccessibleName("MotionControlNewPositionControlElementControllerWidget");
}

void MotionControlNewPositionControlElementControllerWidget::setComponentContext(const UComponentGuiContext& context)
{
    m_context = context;
}

QString MotionControlNewPositionControlElementControllerWidget::componentGuiId() const
{
    return "motion.position.control.newelement.controller";
}

void MotionControlNewPositionControlElementControllerWidget::refreshFromModel(bool force)
{
    Q_UNUSED(force);

    if(m_context.componentLongName.isEmpty())
        return;

    RDK::UELockPtr<NMSDK::NModel> model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
    {
        m_selectedStateTable->clear();
        m_trainedStateTable->clear();
        return;
    }

    fillTablesFromModel(model);
}

void MotionControlNewPositionControlElementControllerWidget::rebuildTables()
{
    // Ensure we keep the widget objects but reconfigure dimensions.
    m_selectedStateTable->clear();
    m_trainedStateTable->clear();

    if(m_numControlLoops <= 0 || m_numMotionElements <= 0)
        return;

    const int rowCount = m_numControlLoops + 1;
    const int colCount = m_numMotionElements * 2 + 1;

    m_selectedStateTable->setRowCount(rowCount);
    m_selectedStateTable->setColumnCount(colCount);

    m_trainedStateTable->setRowCount(rowCount);
    m_trainedStateTable->setColumnCount(colCount);

    m_trainedStateTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_selectedStateTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_trainedStateTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_selectedStateTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Make headers visually simpler
    m_selectedStateTable->horizontalHeader()->setVisible(false);
    m_selectedStateTable->verticalHeader()->setVisible(false);
    m_trainedStateTable->horizontalHeader()->setVisible(false);
    m_trainedStateTable->verticalHeader()->setVisible(false);
}

void MotionControlNewPositionControlElementControllerWidget::fillTablesFromModel(const RDK::UELockPtr<NMSDK::NModel>& model)
{
    auto* rawName = m_context.componentLongName.toLocal8Bit().constData();
    auto positionControl = RDK::dynamic_pointer_cast<NMSDK::NNewPositionControlElement>(model->GetComponentL(rawName));
    if(!positionControl)
    {
        m_numMotionElements = 0;
        m_numControlLoops = 0;
        m_selectedStateTable->clear();
        m_trainedStateTable->clear();
        return;
    }

    if(!positionControl->MotionControlElement)
        return;

    m_numMotionElements = positionControl->MotionControlElement->NumMotionElements;
    m_numControlLoops = positionControl->MotionControlElement->NumControlLoops;

    if(m_numMotionElements <= 0 || m_numControlLoops <= 0)
    {
        m_selectedStateTable->clear();
        m_trainedStateTable->clear();
        return;
    }

    rebuildTables();

    // Prevent cellChanged recursion while filling
    QSignalBlocker blockerSelected(m_selectedStateTable);
    QSignalBlocker blockerTrained(m_trainedStateTable);

    // Row 0: labels ML/MR
    for(int n = 0; n < m_numMotionElements; ++n)
    {
        const int leftCol = m_numMotionElements - n;         // corresponds to BCB: Cells[num_motion_elements-1-n+1][0]
        const int rightCol = n + m_numMotionElements + 1;    // corresponds to BCB: Cells[n+num_motion_elements+1][0]

        QTableWidgetItem* mlLeft = new QTableWidgetItem(QString("ML%1").arg(n));
        mlLeft->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_selectedStateTable->setItem(0, leftCol, mlLeft);

        QTableWidgetItem* mlLeftTr = new QTableWidgetItem(QString("ML%1").arg(n));
        mlLeftTr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_trainedStateTable->setItem(0, leftCol, mlLeftTr);

        QTableWidgetItem* mrRight = new QTableWidgetItem(QString("MR%1").arg(n));
        mrRight->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_selectedStateTable->setItem(0, rightCol, mrRight);

        QTableWidgetItem* mrRightTr = new QTableWidgetItem(QString("MR%1").arg(n));
        mrRightTr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_trainedStateTable->setItem(0, rightCol, mrRightTr);
    }

    for(int m = 0; m < m_numControlLoops; ++m)
    {
        const int row = m + 1;
        QTableWidgetItem* clSel = new QTableWidgetItem(QString("CL%1").arg(m));
        clSel->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_selectedStateTable->setItem(row, 0, clSel);

        QTableWidgetItem* clTr = new QTableWidgetItem(QString("CL%1").arg(m));
        clTr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        m_trainedStateTable->setItem(row, 0, clTr);
    }

    for(int m = 0; m < m_numControlLoops; ++m)
    {
        for(int n = 0; n < m_numMotionElements; ++n)
        {
            const int leftCol = m_numMotionElements - n;
            const int rightCol = n + m_numMotionElements + 1;

            // Selected state frequencies (editable)
            double leftFreq = 0.0;
            double rightFreq = 0.0;

            if(static_cast<int>(positionControl->LeftGenerators.size()) > m &&
               static_cast<int>(positionControl->LeftGenerators[m].size()) > (m_numMotionElements - n - 1))
            {
                auto* gen = dynamic_cast<NMSDK::NPulseGenerator*>(positionControl->LeftGenerators[m][m_numMotionElements - n - 1]);
                if(gen)
                    leftFreq = gen->Frequency.GetData();
            }

            if(static_cast<int>(positionControl->RightGenerators.size()) > m &&
               static_cast<int>(positionControl->RightGenerators[m].size()) > n)
            {
                auto* gen = dynamic_cast<NMSDK::NPulseGenerator*>(positionControl->RightGenerators[m][n]);
                if(gen)
                    rightFreq = gen->Frequency.GetData();
            }

            auto* leftItem = new QTableWidgetItem(QString::number(leftFreq, 'f', 3));
            auto* rightItem = new QTableWidgetItem(QString::number(rightFreq, 'f', 3));
            leftItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            rightItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

            m_selectedStateTable->setItem(m + 1, leftCol, leftItem);
            m_selectedStateTable->setItem(m + 1, rightCol, rightItem);

            // Trained state frequencies (read-only)
            double leftTrained = 0.0;
            double rightTrained = 0.0;

            if(static_cast<int>(positionControl->LeftPostInputNeurons.size()) > m &&
               static_cast<int>(positionControl->LeftPostInputNeurons[m].size()) > n)
            {
                auto* neuron = dynamic_cast<NMSDK::NPulseNeuron*>(positionControl->LeftPostInputNeurons[m][n]);
                if(neuron)
                {
                    auto ltzone = RDK::dynamic_pointer_cast<NMSDK::NPulseLTZone>(neuron->GetComponentL("LTZone"));
                    if(ltzone)
                        leftTrained = ltzone->OutputFrequency->As<double>(0);
                }
            }

            if(static_cast<int>(positionControl->RightPostInputNeurons.size()) > m &&
               static_cast<int>(positionControl->RightPostInputNeurons[m].size()) > n)
            {
                auto* neuron = dynamic_cast<NMSDK::NPulseNeuron*>(positionControl->RightPostInputNeurons[m][n]);
                if(neuron)
                {
                    auto ltzone = RDK::dynamic_pointer_cast<NMSDK::NPulseLTZone>(neuron->GetComponentL("LTZone"));
                    if(ltzone)
                        rightTrained = ltzone->OutputFrequency->As<double>(0);
                }
            }

            m_trainedStateTable->setItem(m + 1, leftCol, new QTableWidgetItem(QString::number(leftTrained, 'f', 3)));
            m_trainedStateTable->setItem(m + 1, rightCol, new QTableWidgetItem(QString::number(rightTrained, 'f', 3)));
        }
    }
}

void MotionControlNewPositionControlElementControllerWidget::onTrainPositionClicked()
{
    if(m_context.componentLongName.isEmpty())
        return;

    RDK::UELockPtr<NMSDK::NModel> model = RDK::GetModelLock<NMSDK::NModel>();
    if(!model)
        return;

    auto positionControl = RDK::dynamic_pointer_cast<NMSDK::NNewPositionControlElement>(
        model->GetComponentL(m_context.componentLongName.toLocal8Bit().constData()));
    if(!positionControl)
        return;

    positionControl->RememberState = true;
    refreshFromModel(true);
}

void MotionControlNewPositionControlElementControllerWidget::onSelectedTableCellChanged(int row, int column)
{
    if(m_updatingSelectedTable)
        return;

    if(!m_selectedStateTable)
        return;

    // Ignore non-data cells
    if(row <= 0 || column <= 0)
        return;
    if(row > m_numControlLoops)
        return;
    if(column > m_numMotionElements * 2)
        return;

    const int n = column - 1;
    const int m = row - 1;

    if(m < 0 || m >= m_numControlLoops || n < 0 || n >= m_numMotionElements * 2)
        return;

    const QTableWidgetItem* item = m_selectedStateTable->item(row, column);
    if(!item)
        return;

    bool ok = false;
    const double value = item->text().toDouble(&ok);
    if(!ok)
        return;

    m_updatingSelectedTable = true;

    RDK::UELockPtr<NMSDK::NModel> model = RDK::GetModelLock<NMSDK::NModel>();
    if(model)
    {
        auto positionControl = RDK::dynamic_pointer_cast<NMSDK::NNewPositionControlElement>(
            model->GetComponentL(m_context.componentLongName.toLocal8Bit().constData()));
        if(positionControl)
        {
            if(n < m_numMotionElements)
            {
                const int leftGenIdx = m_numMotionElements - n - 1;
                if(static_cast<int>(positionControl->LeftGenerators.size()) > m &&
                   static_cast<int>(positionControl->LeftGenerators[m].size()) > leftGenIdx)
                {
                    auto* gen = dynamic_cast<NMSDK::NPulseGenerator*>(positionControl->LeftGenerators[m][leftGenIdx]);
                    if(gen)
                        gen->Frequency = value;
                }
            }
            else
            {
                const int rightGenIdx = n - m_numMotionElements;
                if(static_cast<int>(positionControl->RightGenerators.size()) > m &&
                   static_cast<int>(positionControl->RightGenerators[m].size()) > rightGenIdx)
                {
                    auto* gen = dynamic_cast<NMSDK::NPulseGenerator*>(positionControl->RightGenerators[m][rightGenIdx]);
                    if(gen)
                        gen->Frequency = value;
                }
            }
        }
    }

    m_updatingSelectedTable = false;
    refreshFromModel(true);
}

