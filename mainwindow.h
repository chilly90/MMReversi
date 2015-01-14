#include <QMainWindow>

#include "board.h"
#include "game.h"
#include "Settings.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void countChange(int white, int black);

    void gameOver(CELL_STATE);

    void on_playVsPlayer_triggered();

    void on_playAsWhite_triggered();

    void on_playAsBlack_triggered();

    void on_settings_triggered();

    void changeOptions(Options options);

    void on_exit_triggered();

private:
    void setGame(QSharedPointer<Game> game);
    Ui::MainWindow *ui;
    QSharedPointer<Game> game;
    Options options;
};
