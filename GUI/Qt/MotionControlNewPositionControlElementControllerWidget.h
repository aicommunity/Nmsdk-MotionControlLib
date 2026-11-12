#ifndef MOTIONCONTROLNEWPOSITIONCONTROLELEMENTCONTROLLERWIDGET_H
#define MOTIONCONTROLNEWPOSITIONCONTROLELEMENTCONTROLLERWIDGET_H

#include <QPushButton>
#include <QTableWidget>

#include "../../../../Rdk/GUI/Qt/IComponentControllerWidget.h"
#include "../../../../Rdk/GUI/Qt/UComponentGuiContext.h"
#include "../../../../Rdk/GUI/Qt/UVisualControllerWidget.h"

namespace NMSDK
{
class NModel;
}

class MotionControlNewPositionControlElementControllerWidget : public UVisualControllerWidget, public IComponentControllerWidget
{
    Q_OBJECT
public:
    explicit MotionControlNewPositionControlElementControllerWidget(QWidget* parent = nullptr, RDK::UApplication* app = nullptr);

    void setComponentContext(const UComponentGuiContext& context) override;
    void refreshFromModel(bool force) override;
    QString componentGuiId() const override;

private slots:
    void onTrainPositionClicked();
    void onSelectedTableCellChanged(int row, int column);

private:
    void rebuildTables();
    void fillTablesFromModel(const RDK::UELockPtr<NMSDK::NModel>& model);

private:
    UComponentGuiContext m_context;
    QPushButton* m_trainPositionButton = nullptr;
    QTableWidget* m_trainedStateTable = nullptr;
    QTableWidget* m_selectedStateTable = nullptr;

    bool m_updatingSelectedTable = false;
    int m_numMotionElements = 0;
    int m_numControlLoops = 0;
};

#endif // MOTIONCONTROLNEWPOSITIONCONTROLELEMENTCONTROLLERWIDGET_H

