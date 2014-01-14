/*
 * slicerenderer.h
 *
 * Created on: 09.05.2012
 * @author Ralph Schurade
 */

#ifndef SLICERENDERER_H_
#define SLICERENDERER_H_

#include "objectrenderer.h"

class QGLShaderProgram;

class SliceRenderer : public ObjectRenderer
{
public:
	SliceRenderer();
	virtual ~SliceRenderer();

	void init();

	void draw( QMatrix4x4 p_matrix, QMatrix4x4 mv_matrix, int width, int height, int renderMode, QString target );

protected:
    void setupTextures();
    void setShaderVars( QString target );

    void initGeometry();

private:
	void drawAxial( QString target );
	void drawCoronal( QString target );
	void drawSagittal( QString target );

	GLuint vbo0;
	GLuint vbo1;
	GLuint vbo2;

	float m_x;
	float m_y;
	float m_z;
	float m_nx;
	float m_ny;
	float m_nz;
	float m_dx;
	float m_dy;
	float m_dz;

	float m_xOld;
    float m_yOld;
    float m_zOld;
    float m_xbOld;
    float m_ybOld;
    float m_zbOld;
};

#endif /* SLICERENDERER_H_ */
