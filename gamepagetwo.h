#ifndef GAMEPAGETWO_H
#define GAMEPAGETWO_H

#include <QWidget>
#include "gamelogicone.h"
#include "man.h"
#include <QTextBrowser>
#include "gamesessiondata.h"
#include <QPair>
#include <memory>
#include "difficultylabel.h"
namespace Ui {
class GamePageTwo;
}
class Player2 : public Man
{
    Q_OBJECT
    friend class GamePageTwo;
public:
    explicit Player2(QObject* parent = nullptr) : Man(parent), isAnimating(false) {}

private:
    void initBoard(const Level& level, QWidget* parentWidget) override;
    void tryMove(int i, int j) override;
    TileButton* getOneTile(QPair<int, int> selected);
    std::shared_ptr<QPair<int, int>> selected;
    bool isAnimating;
};

class GamePageTwo : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageTwo(QWidget *parent = nullptr);
    void loadLevel(const Level& level);
    ~GamePageTwo();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void mainBtnClicked(int i);
private:
    Ui::GamePageTwo *ui;
    QTextBrowser* targetText;
    GameSessionData session;
    QTimer* gameTimer;
    Level level;
    Player2* playerR;
    Player2* playerL;
    QWidget* widgetR;
    QWidget* widgetL;
    std::shared_ptr<QPair<int, int>> selectedL;
    std::shared_ptr<QPair<int, int>> selectedR;
    bool isHandleKey;
    DifficultyLabel* difficultyBar;
    double hardness;

    void winEffect(Man* man);
    void updateUI(Man* man);
    void createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry = {290, 20, 270, 165});
    void tilesTrigger(const QPair<int, int>& pos, const QVector<TileButton*>& tiles);
    void triggerSelected(const QPair<int, int>& pos, Player2* player);
    void moveSelection(QPair<int, int> &pos, int dRow, int dCol, Player2* player);
};

#endif // GAMEPAGETWO_H
