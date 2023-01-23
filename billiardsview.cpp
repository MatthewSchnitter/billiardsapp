#include "billiardsview.h"
#include "ui_billiardsview.h"

//constructor for the view, connect statements, timer, and offset values
BilliardsView::BilliardsView(BilliardsModel &model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BilliardsView)
{
    ui->setupUi(this);

    connect(&model,
            &BilliardsModel::updateCueBallPosition,
            this,
            &BilliardsView::updateCueBall);

    connect(&model,
            &BilliardsModel::updateRedBallPosition,
            this,
            &BilliardsView::updateRedBall);

    connect(&model,
            &BilliardsModel::updateYellowBallPosition,
            this,
            &BilliardsView::updateYellowBall);

    connect(&timer,
            &QTimer::timeout,
            &model,
            &BilliardsModel::stepSimulation);

    connect(this,
            &BilliardsView::shotTaken,
            &model,
            &BilliardsModel::onShot);

    connect(this,
            &BilliardsView::aimLeft,
            &model,
            &BilliardsModel::onChangeDirectionLeft);

    connect(this,
            &BilliardsView::aimRight,
            &model,
            &BilliardsModel::onChangeDirectionRight);

    connect(this,
            &BilliardsView::increasePower,
            &model,
            &BilliardsModel::onIncreasePower);

    connect(this,
            &BilliardsView::decreasePower,
            &model,
            &BilliardsModel::onDecreasePower);

    connect(&model,
            &BilliardsModel::updatePower,
            this,
            &BilliardsView::updatePowerMeter);

    connect(&model,
            &BilliardsModel::drawLine,
            this,
            &BilliardsView::drawPath);

    connect(&model,
            &BilliardsModel::addOneScore,
            this,
            &BilliardsView::addOnePoint);

    connect(this,
            &BilliardsView::changeLevel,
            &model,
            &BilliardsModel::switchLevel);

    connect(this,
            &BilliardsView::restartLevel,
            &model,
            &BilliardsModel::resetLevel);

   timer.start(16);

   tableOffsetX = ui->billiardTable->width() / 2;
   tableOffsetY = ui->billiardTable->height() / 2;
   labelRadius = ui->cueBall->width() / 2;

   score = 0;
}

BilliardsView::~BilliardsView() {
    delete ui;
}

//paints the raycasted line of the cue ball path
void BilliardsView::paintEvent(QPaintEvent*) {

    if (ui->navigationStack->currentIndex() != playIndex)
        return;

    QPixmap pixMap;
    pixMap.load(QString::fromUtf8(":/billiards table.png"));


    QPainter canvas (&pixMap);

    QPen linePen;
    linePen.setColor(Qt::white);
    linePen.setStyle(Qt::DashLine);
    linePen.setWidth(3);
    canvas.setPen(linePen);

    canvas.drawPolyline(linePath);
//    ui->artwork->setScaledContents(true);
    ui->artwork->setPixmap(pixMap);
}

//updates the position of the cue ball
void BilliardsView::updateCueBall(float x, float y) {
    ui->cueBall->move(convertToTableCoordinatesX(x), convertToTableCoordinatesY(y));
}

//updates the position of the red ball
void BilliardsView::updateRedBall(float x, float y){
    ui->redBall->move(convertToTableCoordinatesX(x), convertToTableCoordinatesY(y));
}

//updates the position of the yellow ball
void BilliardsView::updateYellowBall(float x, float y){
    ui->yellowBall->move(convertToTableCoordinatesX(x), convertToTableCoordinatesY(y));
}

//uses the size ratio and offset values to convert the coordinate
float BilliardsView::convertToTableCoordinatesX(float x) {
    return x * worldToViewRatio + tableOffsetX - labelRadius;
}

//uses the size ratio and offset values to convert the coordinate
float BilliardsView::convertToTableCoordinatesY(float y) {
    return y * worldToViewRatio + tableOffsetY - labelRadius;
}

// Naviagtion button connections
void BilliardsView::on_startButton_clicked() {
    ui->navigationStack->setCurrentIndex(playIndex);
    emit changeLevel(0);
}

//when the user presses the tutorial button
void BilliardsView::on_tutorialButton_clicked() {
    ui->navigationStack->setCurrentIndex(tutorialIndex);
}

//when the user presses the levels button
void BilliardsView::on_levelsButton_clicked() {
     ui->navigationStack->setCurrentIndex(levelIndex);
}

//when the user presses the main menu button
void BilliardsView::on_menuButton_clicked() {
    ui->navigationStack->setCurrentIndex(welcomeIndex);
}

//when the user presses the main menu button
void BilliardsView::on_menuButton_3_clicked() {
    ui->navigationStack->setCurrentIndex(welcomeIndex);
}

//when the user presses the main menu button
void BilliardsView::on_menuButton_2_clicked() {
    ui->navigationStack->setCurrentIndex(welcomeIndex);
}

//when the user presses a key to control the shot
void BilliardsView::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_E:
            emit shotTaken();
            break;

        case Qt::Key_A:
            emit aimLeft();
            break;

        case Qt::Key_D:
            emit aimRight();
            break;

        case Qt::Key_W:
            emit increasePower();
            break;

        case Qt::Key_S:
            emit decreasePower();
            break;
    }
}

//updates the power meter displayed to the user
void BilliardsView::updatePowerMeter(float power) {
    ui->powerMeter->setValue((int) power);
}

//updates the path of the cue ball
void BilliardsView::drawPath(QVector<QPoint> ballPath) {
    linePath = ballPath;
}

//adds one to the score and updates it
void BilliardsView::addOnePoint(){
    ui->score->setNum(++score);
}

//when user presses level one button
void BilliardsView::on_level1Button_clicked() {
    ui->navigationStack->setCurrentIndex(playIndex);
    emit changeLevel(1);
    resetScore();
}

//when user presses level two button
void BilliardsView::on_level2Button_clicked() {
    ui->navigationStack->setCurrentIndex(playIndex);
    emit changeLevel(2);
    resetScore();
}

//when user presses level three button
void BilliardsView::on_level3Button_clicked() {
    ui->navigationStack->setCurrentIndex(playIndex);
    emit changeLevel(3);
    resetScore();
}

//when user presses level four button
void BilliardsView::on_level4Button_clicked() {
    ui->navigationStack->setCurrentIndex(playIndex);
    emit changeLevel(4);
    resetScore();
}

//when user presses restart button
void BilliardsView::on_restartButton_clicked() {
    emit restartLevel();
    resetScore();
}

//resets the score and updates it
void BilliardsView::resetScore() {
    score = 0;
    ui->score->setNum(score);
}
