/*
 * datasetmesh.h
 *
 * Created on: Jul 19, 2012
 * @author Ralph Schurade
 */

#ifndef DATASETMESH_H_
#define DATASETMESH_H_

#include "dataset.h"

#include <QString>

class TriangleMesh2;
class MeshRenderer;

class DatasetMesh: public Dataset
{
Q_OBJECT

public:
    DatasetMesh( TriangleMesh2* mesh, QDir filename = QDir( "new mesh" ) );
    DatasetMesh( QDir filename, Fn::DatasetType type );
    virtual ~DatasetMesh();

    void initProperties();
    void finalizeProperties();

    virtual void draw( QMatrix4x4 pMatrix, QMatrix4x4 mvMatrix, int width, int height, int renderMode, QString target );
    QString getValueAsString( int x, int y, int z );

    bool mousePick( int pickId, QVector3D pos, Qt::KeyboardModifiers modifiers, QString target );

    TriangleMesh2* getMesh();

protected:
    QVector<TriangleMesh2*> m_mesh;
    MeshRenderer* m_renderer;

private slots:
    void paintModeChanged( QVariant mode );
    void makePermanent();

};

#endif /* DATASETMESH_H_ */
