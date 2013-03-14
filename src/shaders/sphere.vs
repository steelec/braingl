#include uniforms_vs
#include lighting_vs

void main()
{
    v_normal = a_position;
    prepareLight();

    vec3 pos = a_position;
    pos.x = ( pos.x * u_dx ) + u_x;
    pos.y = ( pos.y * u_dy ) + u_y;
    pos.z = ( pos.z * u_dz ) + u_z;
    
    gl_Position = mvp_matrix * vec4( pos, 1.0 );     
}
