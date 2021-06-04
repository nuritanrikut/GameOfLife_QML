#pragma once

#include <QAbstractTableModel>
#include <QTimer>

#include "Cell.hpp"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

    // These are Qt properties. They are available to QML side.

    Q_PROPERTY( int gridWidth READ gridWidth WRITE setGridWidth NOTIFY gridWidthChanged )
    Q_PROPERTY( int gridHeight READ gridHeight WRITE setGridHeight NOTIFY gridHeightChanged )
    Q_PROPERTY( int interval READ interval WRITE setInterval NOTIFY intervalChanged )

public:
    explicit TableModel( QObject *parent = nullptr );

    int gridWidth() const;
    int gridHeight() const;
    int interval() const;

Q_SIGNALS:
    void gridWidthChanged( int gridWidth );
    void gridHeightChanged( int gridHeight );
    void intervalChanged( int interval );

public:
    // Virtual funtions from QAbstractTableModel interface
    // For writable and resizeable models following functions are required
    // Refer to Qt documentation for details

    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;

public Q_SLOTS:
    void setGridWidth( int gridWidth );
    void setGridHeight( int gridHeight );
    void setInterval( int interval );

    void generate();
    void singleIteration();

    void start();
    void stop();

private:
    void clearCells();
    void createCells();
    void recalculateNeigbors();

private:
    // GridWidth and GridHeight can be changed from QML side
    int mGridWidth;
    int mGridHeight;
    int mInterval;

    // CurrentGridWidth and CurrentGridHeight are only written in createCells()
    // So that we have the correct array boundaries.
    int mCurrentGridWidth;
    int mCurrentGridHeight;

    QList<Cell *> mCells;

    QTimer mTimer;
};
