#pragma once

#include <vector>

class Cell
{
public:
    Cell();

    void setPos( int x, int y );
    int x() const;
    int y() const;

    bool isAlive() const;
    void setAlive( bool alive );

    void addNeighbor( Cell * );
    void clearNeighbors();

    void calculate();
    bool update();

private:
    int mX;
    int mY;
    bool mAlive;
    bool mNextState;
    std::vector<Cell *> mNeighbors;
};
