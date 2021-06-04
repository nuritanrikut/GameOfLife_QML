#include "TableModel.hpp"

#include <QRandomGenerator>
#include <QRect>

TableModel::TableModel( QObject *parent )
    : QAbstractTableModel( parent ),
      mGridWidth( 10 ),
      mGridHeight( 10 ),
      mInterval( 33 ),
      mCurrentGridWidth( 10 ),
      mCurrentGridHeight( 10 )
{
    createCells();
    generate();
    recalculateNeigbors();

    mTimer.setSingleShot( false );
    connect( &mTimer, &QTimer::timeout, this, &TableModel::singleIteration );
}

int TableModel::rowCount( const QModelIndex &parent ) const
{
    // If there is a parent, that means nesting. We don't want that.
    if( parent.isValid() )
        return 0;

    return mGridHeight;
}

int TableModel::columnCount( const QModelIndex &parent ) const
{
    if( parent.isValid() )
        return 0;

    return mGridWidth;
}

QVariant TableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    // We don't care about header names. They are not visible anyways.
    return QVariant();
}

QVariant TableModel::data( const QModelIndex &index, int role ) const
{
    // We only use Display role
    if( role == Qt::DisplayRole && index.isValid() )
    {
        int y = index.row();
        int x = index.column();
        return mCells[y * mCurrentGridWidth + x]->isAlive();
    }
    return QVariant();
}

bool TableModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if( role == Qt::DisplayRole && index.isValid() )
    {
        int y = index.row();
        int x = index.column();
        mCells[y * mCurrentGridWidth + x]->setAlive( value.toBool() );

        // Notify views that some data is changed
        Q_EMIT dataChanged( index, index, { Qt::DisplayRole } );
        return true;
    }
    return false;
}

Qt::ItemFlags TableModel::flags( const QModelIndex &index ) const
{
    // All cells are enabled and editable (in addition to defaults)
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | QAbstractTableModel::flags( index );
}

int TableModel::gridWidth() const
{
    return mGridWidth;
}

int TableModel::gridHeight() const
{
    return mGridHeight;
}

int TableModel::interval() const
{
    return mInterval;
}

void TableModel::setGridWidth( int gridWidth )
{
    if( mGridWidth == gridWidth )
        return;

    mGridWidth = gridWidth;

    // When size parameters are changed, remove all cells and re-create them
    clearCells();
    createCells();
    generate();
    recalculateNeigbors();

    Q_EMIT gridWidthChanged( mGridWidth );
}

void TableModel::setGridHeight( int gridHeight )
{
    if( mGridHeight == gridHeight )
        return;

    mGridHeight = gridHeight;

    clearCells();
    createCells();
    generate();
    recalculateNeigbors();

    Q_EMIT gridHeightChanged( mGridHeight );
}

void TableModel::setInterval( int interval )
{
    if( mInterval == interval )
        return;

    mInterval = interval;
    emit intervalChanged( mInterval );
}

void TableModel::generate()
{
    int top = mCurrentGridHeight;
    int left = mCurrentGridWidth;
    int bottom = 0;
    int right = 0;

    for( auto cell : mCells )
    {
        // Random
        quint32 randomValue = QRandomGenerator::global()->bounded( 2 );
        bool alive = randomValue == 1;
        bool changed = cell->isAlive() != alive;
        cell->setAlive( alive );
        if( changed )
        {
            top = qMin( top, cell->y() );
            left = qMin( left, cell->x() );
            bottom = qMax( bottom, cell->y() );
            right = qMax( right, cell->x() );
        }
    }

    // Here, instead of firing dataChanged signal for each cell, we calculate bounds and fire it once.
    Q_EMIT dataChanged( createIndex( top, left ), createIndex( bottom, right ), { Qt::DisplayRole } );
}

void TableModel::singleIteration()
{
    for( auto cell : mCells )
        cell->calculate();

    int top = mCurrentGridHeight;
    int left = mCurrentGridWidth;
    int bottom = 0;
    int right = 0;

    for( auto cell : mCells )
    {
        if( cell->update() )
        {
            top = qMin( top, cell->y() );
            left = qMin( left, cell->x() );
            bottom = qMax( bottom, cell->y() );
            right = qMax( right, cell->x() );
        }
    }

    // Here, instead of firing dataChanged signal for each cell, we calculate bounds and fire it once.
    Q_EMIT dataChanged( createIndex( top, left ), createIndex( bottom, right ), { Qt::DisplayRole } );
}

void TableModel::start()
{
    mTimer.start( mInterval );
}

void TableModel::stop()
{
    mTimer.stop();
}

void TableModel::clearCells()
{
    mCells.clear();
}

void TableModel::createCells()
{
    beginResetModel();

    mCurrentGridWidth = mGridWidth;
    mCurrentGridHeight = mGridHeight;

    for( auto cell : mCells )
        delete cell;

    mCells.resize( mCurrentGridWidth * mCurrentGridHeight );

    int index = 0;
    for( int y = 0; y < mCurrentGridHeight; y++ )
    {
        for( int x = 0; x < mCurrentGridWidth; x++ )
        {
            auto cell = mCells[index] = new Cell;
            cell->setPos( x, y );
            index++;
        }
    }

    endResetModel();
}

void TableModel::recalculateNeigbors()
{
    for( auto cell : mCells )
        cell->clearNeighbors();

    for( int y = 0; y < mCurrentGridHeight; y++ )
    {
        for( int x = 0; x < mCurrentGridWidth; x++ )
        {
            auto cell = mCells[y * mCurrentGridWidth + x];

            int left = y > 0 ? y - 1 : y;
            int right = y < mCurrentGridHeight - 1 ? y + 1 : y;
            int top = x > 0 ? x - 1 : x;
            int bottom = x < mCurrentGridWidth - 1 ? x + 1 : x;

            for( int ny = left; ny <= right; ny++ )
            {
                for( int nx = top; nx <= bottom; nx++ )
                {
                    if( ny == y && nx == x )
                        continue;

                    auto neighbor = mCells[ny * mCurrentGridWidth + nx];
                    cell->addNeighbor( neighbor );
                }
            }
        }
    }
}
