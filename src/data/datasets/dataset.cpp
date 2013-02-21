/*
 * dataset.cpp
 *
 *  Created on: May 3, 2012
 *      Author: Ralph Schurade
 */
#include "dataset.h"

Dataset::Dataset( QDir fileName, Fn::DatasetType type ) :
    m_textureGLuint( 0 )
{
    // add standard properties
    m_properties.set( Fn::Property::ACTIVE, true );
    m_properties.set( Fn::Property::FILENAME, fileName.path() );
    m_properties.set( Fn::Property::TYPE, (int)type );
    m_properties.set( Fn::Property::NAME, fileName.path().split( "/" ).last(), true );
    m_properties.set( Fn::Property::SIZE, -1 );
    m_properties.set( Fn::Property::CREATED_BY, (int)Fn::Algo::NONE );

    m_properties.set( Fn::Property::RENDER_LOWER_X, 0 );
    m_properties.set( Fn::Property::RENDER_LOWER_Y, 0 );
    m_properties.set( Fn::Property::RENDER_LOWER_Z, 0 );
    m_properties.set( Fn::Property::RENDER_UPPER_X, 0 );
    m_properties.set( Fn::Property::RENDER_UPPER_Y, 0 );
    m_properties.set( Fn::Property::RENDER_UPPER_Z, 0 );

    m_properties.set( Fn::Property::HAS_TEXTURE, false );
}

Dataset::~Dataset()
{
}

GLuint Dataset::getTextureGLuint()
{
    if ( m_textureGLuint == 0 )
    {
        createTexture();
    }
    return m_textureGLuint;
}

PropertyGroup* Dataset::properties()
{
    return &m_properties;
}

void Dataset::draw( QMatrix4x4 mvpMatrix, QMatrix4x4 mvMatrixInverse, QAbstractItemModel* globalModel, QAbstractItemModel* roiModel, QAbstractItemModel* dataModel )
{
    // do nothing here
}

QString Dataset::getValueAsString( int x, int y, int z )
{
    return QString( "0" );
}


void Dataset::createTexture()
{
    // do nothing here
}
