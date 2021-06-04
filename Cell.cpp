#include "Cell.hpp"

Cell::Cell() : mX( 0 ), mY( 0 ), mAlive( false ) { }

void Cell::setPos( int x, int y )
{
    mX = x;
    mY = y;
}

int Cell::x() const
{
    return mX;
}

int Cell::y() const
{
    return mY;
}

bool Cell::isAlive() const
{
    return mAlive;
}

void Cell::setAlive( bool alive )
{
    mAlive = alive;
}

void Cell::addNeighbor( Cell *neighbor )
{
    mNeighbors.push_back( neighbor );
}

void Cell::clearNeighbors()
{
    mNeighbors.clear();
}

void Cell::calculate()
{
    int aliveNeighbors = 0;
    for( auto neighbor : mNeighbors )
    {
        if( neighbor->isAlive() )
            aliveNeighbors++;
    }

    // From https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
    // """
    // At each step in time, the following transitions occur:
    //   1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    //   2. Any live cell with two or three live neighbours lives on to the next generation.
    //   3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    //   4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    // """

    if( mAlive )
    {
        if( aliveNeighbors < 2 )
            mNextState = false;
        else if( aliveNeighbors > 3 )
            mNextState = false;
        else
            mNextState = true;
    }
    else
    {
        if( aliveNeighbors == 3 )
            mNextState = true;
        else
            mNextState = false;
    }
}

// Returns true if state changes
bool Cell::update()
{
    if( mAlive == mNextState )
        return false;

    mAlive = mNextState;
    return true;
}
