#ifndef BILLIARDSVIEW_H
#define BILLIARDSVIEW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include "billiardsmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BilliardsView; }
QT_END_NAMESPACE

class BilliardsView : public QMainWindow
{
    Q_OBJECT

public:
    BilliardsView(BilliardsModel&, QWidget *parent = nullptr);
    ~BilliardsView();

private:
    virtual void paintEvent(QPaintEvent*);
    Ui::BilliardsView *ui;
    const int worldToViewRatio = 262;
    int tableOffsetX;
    int tableOffsetY;
    BilliardsModel model;
    QTimer timer;
    int labelRadius;
    int score;
    QVector<QPoint> linePath;

    // Indexes for navigation stack widget
    int welcomeIndex  = 0;
    int levelIndex    = 1;
    int playIndex     = 2;
    int tutorialIndex = 3;

    float convertToTableCoordinatesX(float);
    float convertToTableCoordinatesY(float);
    void resetScore();

private slots:
    void updateCueBall(float, float);
    void updateRedBall(float, float);
    void updateYellowBall(float, float);
    void keyPressEvent(QKeyEvent*);
    void updatePowerMeter(float);
    void drawPath(QVector <QPoint> linePath);
    void addOnePoint();
    void on_startButton_clicked();
    void on_tutorialButton_clicked();
    void on_levelsButton_clicked();
    void on_menuButton_clicked();
    void on_menuButton_3_clicked();
    void on_menuButton_2_clicked();
    void on_level1Button_clicked();
    void on_restartButton_clicked();
    void on_level2Button_clicked();
    void on_level3Button_clicked();
    void on_level4Button_clicked();

signals:
    void shotTaken();
    void aimLeft();
    void aimRight();
    void increasePower();
    void decreasePower();
    void changeLevel(int);
    void restartLevel();
};
#endif // BILLIARDSVIEW_H
