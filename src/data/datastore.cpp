/*
 * datastore.cpp
 *
 * Created on: May 4, 2012
 * @author Ralph Schurade
 */
#include "datastore.h"

#include "models.h"

#include "vptr.h"
#include "enums.h"

#include "datasets/dataset.h"

#include <QLocale>
#include <QDebug>

DataStore::DataStore()
{
}

DataStore::~DataStore()
{
}

void DataStore::updateGlobals( int row )
{
    if ( m_datasetList.size() > 0 && row == 0 )
    {
        Dataset* ds = VPtr<Dataset>::asPtr( m_datasetList.first() );

        if ( ds->properties( "maingl" )->get( Fn::Property::D_TYPE ).toInt() < (int)Fn::DatasetType::NIFTI_ANY )
        {

            Models::g()->setData( Models::g()->index( (int)Fn::Property::G_MAX_AXIAL,    0 ), ds->properties( "maingl" )->get( Fn::Property::D_NZ ).toInt() );
            Models::g()->setData( Models::g()->index( (int)Fn::Property::G_MAX_CORONAL,  0 ), ds->properties( "maingl" )->get( Fn::Property::D_NY ).toInt() );
            Models::g()->setData( Models::g()->index( (int)Fn::Property::G_MAX_SAGITTAL, 0 ), ds->properties( "maingl" )->get( Fn::Property::D_NX ).toInt() );
            Models::g()->setData( Models::g()->index( (int)Fn::Property::G_SLICE_DX,     0 ), ds->properties( "maingl" )->get( Fn::Property::D_DX ).toFloat() );
            Models::g()->setData( Models::g()->index( (int)Fn::Property::G_SLICE_DY,     0 ), ds->properties( "maingl" )->get( Fn::Property::D_DY ).toFloat() );
            Models::g()->setData( Models::g()->index( (int)Fn::Property::G_SLICE_DZ,     0 ), ds->properties( "maingl" )->get( Fn::Property::D_DZ ).toFloat() );

            bool dimsChanged = false;
            if ( m_datasetList.size() >= 2 )
            {
                Dataset* ds2 = VPtr<Dataset>::asPtr( m_datasetList[1] );

                dimsChanged = ds->properties( "maingl" )->get( Fn::Property::D_NX ).toInt() != ds2->properties( "maingl" )->get( Fn::Property::D_NX ).toInt();
            }

            if ( m_datasetList.size() == 1 || dimsChanged )
            {
                Models::g()->setData( Models::g()->index( (int)Fn::Property::G_AXIAL, 0 ), ds->properties( "maingl" )->get( Fn::Property::D_NZ ).toInt() / 2 );
                Models::g()->setData( Models::g()->index( (int)Fn::Property::G_CORONAL, 0 ), ds->properties( "maingl" )->get( Fn::Property::D_NY ).toInt() / 2 );
                Models::g()->setData( Models::g()->index( (int)Fn::Property::G_SAGITTAL, 0 ), ds->properties( "maingl" )->get( Fn::Property::D_NX ).toInt() / 2 );
            }

            Models::g()->submit();
        }
    }
}

int DataStore::rowCount( const QModelIndex &parent ) const
{
    return m_datasetList.size();
}

int DataStore::columnCount( const QModelIndex &parent ) const
{
    return 14;
}

QVariant DataStore::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    switch ( role )
    {
        case Qt::CheckStateRole:
        {
            if ( index.column() == 0 )
            {
                Dataset* ds = VPtr<Dataset>::asPtr( m_datasetList.at( index.row() ) );
                return ds->properties( "maingl" )->get( Fn::Property::D_ACTIVE ).toBool();
                break;
            }
            else
                return QVariant();
        }
        case Qt::DisplayRole:
            return getDatasetProperties( index );
            break;
        default:
            break;
    }

    return QVariant();
}

QVariant DataStore::getDatasetProperties( const QModelIndex &index ) const
{
    Dataset* ds = VPtr<Dataset>::asPtr( m_datasetList.at( index.row() ) );

    // handle props with special treatment
    switch ( (Fn::Property)index.column() )
    {
        case Fn::Property::D_TEXTURE_GLUINT:
            return ds->getTextureGLuint();
            break;
        case Fn::Property::D_DATASET_POINTER:
            return m_datasetList.at( index.row() );
            break;
        case Fn::Property::D_DATASET_LIST:
            return m_datasetList;
            break;
        default:
            break;
    }
    // everything else
    return ds->properties( "maingl" )->get( (Fn::Property)index.column() );
}

bool DataStore::setData( const QModelIndex &index, const QVariant &value, int role )
{
    //qDebug() << "row: " << index.row() << "column: " << index.column() << "role: " << role;
    if ( index.column() != (int)Fn::Property::D_NEW_DATASET && !index.isValid() )
    {
        return false;
    }

    if ( role == Qt::CheckStateRole )
    {
        Dataset* ds = VPtr<Dataset>::asPtr( m_datasetList.at( index.row() ) );
        ds->properties( "maingl" )->set( Fn::Property::D_ACTIVE, !ds->properties( "maingl" )->get( Fn::Property::D_ACTIVE ).toBool() );
        emit( dataChanged( index, index ) );
    }

    if ( role == Qt::DisplayRole )
    {
        // handle props with special treatment
        if ( index.column() == (int)Fn::Property::D_NEW_DATASET )
        {
            beginInsertRows( QModelIndex(), m_datasetList.size(), m_datasetList.size() );
            m_datasetList.push_back( value );
            endInsertRows();
            connect( VPtr<Dataset>::asPtr( value )->properties( "maingl" ), SIGNAL( signalPropChanged() ), this, SLOT( submit() ) );

            if ( VPtr<Dataset>::asPtr( value )->properties( "maingl2" )->get( Fn::Property::D_RENDER_TARGET).toString() == "maingl2" )
            {
                connect( VPtr<Dataset>::asPtr( value )->properties( "maingl2" ), SIGNAL( signalPropChanged() ), this, SLOT( submit() ) );
            }

            updateGlobals( m_datasetList.size() - 1 );
            emit( dataChanged( index, index ) );
            return true;
        }
        // everything else
        if ( index.row() >= 0 && index.row() < m_datasetList.size() )
        {
            VPtr<Dataset>::asPtr( m_datasetList.at( index.row() ) )->properties( "maingl" )->set( (Fn::Property)index.column(), value );
        }
        return true;
    }
    return false;
}

QVariant DataStore::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role == Qt::DisplayRole )
    {
        if ( orientation == Qt::Horizontal )
        {
            return QString( Fn::Prop2String::s( (Fn::Property)section ) );
        }
        else
        {
            return VPtr<Dataset>::asPtr( m_datasetList.at( section ) )->properties( "maingl" )->get( Fn::Property::D_NAME ).toString();
        }
    }
    return QVariant();
}

Qt::ItemFlags DataStore::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
    {
        qDebug() << "Item can be dropped here";
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled ;
    }

    Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if ( index.column() == 0 )
    {
        defaultFlags |= Qt::ItemIsUserCheckable;
    }

    return defaultFlags;
}


QModelIndex DataStore::index( int row, int column, const QModelIndex & parent ) const
{
    if ( row < m_datasetList.size() )
    {
        return createIndex( row, column, VPtr<Dataset>::asPtr( m_datasetList.at( row ) ) );
    }
    else if ( row == m_datasetList.size() && column == (int)Fn::Property::D_NEW_DATASET )
    {
        return createIndex( 0, column, 0 );
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex DataStore::parent( const QModelIndex & index ) const
{
    return QModelIndex();
}

void DataStore::moveItemUp( int row )
{
    if ( row > 0 && row < m_datasetList.size() )
    {
        beginMoveRows( index( row, 0 ), row, row, index( row - 1, 0 ), row - 1 );
        m_datasetList.swap( row, row - 1 );
        endMoveRows();
        updateGlobals( row - 1 );
        emit ( dataChanged( index( 0, 0 ), index( 0, 0 ) ) );
    }
}

void DataStore::moveItemDown( int row )
{
    if ( row >= 0 && row < m_datasetList.size() - 1 )
    {
        beginMoveRows( index( row, 0 ), row, row, index( row + 1, 0 ), row + 1 );
        m_datasetList.swap( row, row + 1 );
        endMoveRows();
        updateGlobals( row );
        emit ( dataChanged( index( 0, 0 ), index( 0, 0 ) ) );
    }
}

void DataStore::deleteItem( int row )
{
    if ( row >= 0 && row < m_datasetList.size() )
    {
        beginRemoveRows( index( row, 0 ), row, row );
        Dataset* toDelete = VPtr<Dataset>::asPtr( m_datasetList.at( row ) );
        m_datasetList.removeAt( row );
        endRemoveRows();
        beginResetModel();
        reset();
        endResetModel();
        emit ( dataChanged( index( 0, 0 ), index( 0, 0 ) ) );
        delete toDelete;
        updateGlobals( row );
    }
}

bool DataStore::submit()
{
    emit ( dataChanged( index( 0, 0 ), index( 0, 0 ) ) );
    return true;
}
