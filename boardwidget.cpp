#include "BoardWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>
#include "Minimax.h"

BoardWidget::BoardWidget(QSharedPointer<Board> board,
                                       QWidget *parent) :
    QWidget(parent), showPossibleWhite(false), showPossibleBlack(false)
{
    this->setBoard(board);
}

BoardWidget::BoardWidget(QWidget *parent) :
    QWidget(parent), showPossibleWhite(false), showPossibleBlack(false)
{

}

QSize BoardWidget::sizeHint() const
{
    return QSize(800,800);
}

void BoardWidget::setBoard(QSharedPointer<Board> board)
{
    this->board = board;
    Board * raw = board.data();
    connect(raw,
            SIGNAL(moveMade(CELL_STATE,CELL_STATE)),
            this,
            SLOT(update()));

    this->update();
}

QSharedPointer<Board> BoardWidget::getBoard() const
{
    return this->board;
}

void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (this->board.isNull())
    {
        painter.fillRect(this->rect(),Qt::gray);
        return;
    }
    const QColor bgColor = QColor(0,0,0,255);
    const QColor boardColor = Qt::gray;
    const QColor gridColor = Qt::black;
    const QColor whiteChipColor = Qt::white;
    const QColor blackChipColor = Qt::black;

    const int size = qMin(this->width(),this->height())-1;
    const int numCells = this->board->getBoardSize();
    const int cellSize = size / numCells;

    if (this->board->getWhoIsNext() == WHITE)
        painter.fillRect(this->rect(),Qt::white);
    else
        painter.fillRect(this->rect(),Qt::black);

    painter.scale(1,-1.0);
    painter.translate(0,-1*size);

    painter.translate((this->width() - numCells*cellSize)/2,
                      -1*(this->height() - numCells*cellSize)/2);

    painter.fillRect(0,0,numCells*cellSize,numCells*cellSize,boardColor);


    painter.setPen(gridColor);
    for (int x = 0; x <= numCells; x++)
    {
        painter.drawLine(x*cellSize,0,x*cellSize,cellSize*numCells);
    }

    for (int y = 0; y <= numCells; y++)
    {
        painter.drawLine(0,y*cellSize,cellSize*numCells,y*cellSize);
    }

    for (int x = 0; x < numCells; x++)
    {
        for (int y = 0; y < numCells; y++)
        {
            const BoardPosition pos = {x,y};
            CELL_STATE cell = this->board->getCell(pos);

            if (cell == WHITE)
            {
                painter.setPen(whiteChipColor);
                painter.setBrush(QBrush(whiteChipColor));
                painter.drawEllipse(QPoint(cellSize*x + cellSize/2,cellSize*y + cellSize/2),cellSize/2-2,cellSize/2-2);
            }
            else if (cell == BLACK)
            {
                painter.setPen(blackChipColor);
                painter.setBrush(QBrush(blackChipColor));
                painter.drawEllipse(QPoint(cellSize*x + cellSize/2,cellSize*y + cellSize/2),cellSize/2-2,cellSize/2-2);
            }
        }
    }

    if (this->getBoard()->getWhoIsNext() == WHITE && !this->showPossibleWhite) {
        return;
    }
    else if (this->getBoard()->getWhoIsNext() == BLACK && !this->showPossibleBlack) {
        return;
    }
    else if (this->getBoard()->getWhoIsNext() == EMPTY) {
        return;
    }
    painter.setPen(Qt::green);
    painter.setBrush(QBrush(whiteChipColor));
    if (this->board->getWhoIsNext() == BLACK) {
        painter.setBrush(blackChipColor);
    }
    for (int x = 0; x < numCells; x++)
    {
        for (int y = 0; y < numCells; y++)
        {
            BoardPosition pos = {x,y};
            if (!this->board->isValidMove(pos,this->board->getWhoIsNext()))
                continue;
            painter.drawEllipse(QPoint(cellSize*x + cellSize/2, cellSize*y + cellSize/2),5,5);
        }
    }

    BoardPosition bestMove = this->getBoard()->getBestMove();
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(QPoint(cellSize*bestMove.x + cellSize/2, cellSize*bestMove.y + cellSize/2),5,5);
}

void BoardWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if ((event->pos() - this->lastMousePressPos).manhattanLength() > 4)
        return;
    QPoint pos = event->pos();

    const int size = qMin(this->width(),this->height())-1;
    const int numCells = this->board->getBoardSize();
    const int cellSize = size / numCells;
    QPoint transformedPos = pos;
    transformedPos.setY(transformedPos.y()*-1);
    transformedPos.setY(transformedPos.y() + size);
    transformedPos.setX(transformedPos.x() - (this->width() - numCells*cellSize)/2);
    transformedPos.setY(transformedPos.y() + (this->height() - numCells*cellSize)/2);


    if (transformedPos.x() < 0 || transformedPos.y() < 0 || transformedPos.x() > numCells*cellSize || transformedPos.y() > numCells*cellSize)
        return;

    int xCell = transformedPos.x() / cellSize;
    int yCell = transformedPos.y() / cellSize;
    const BoardPosition BoardPosition = {xCell,yCell};

    this->cellClicked(BoardPosition);
}

void BoardWidget::mousePressEvent(QMouseEvent * event)
{
    this->lastMousePressPos = event->pos();
}
