
#include  <def_graphics.h>

public  Status  convert_lines_to_tubes( graphics, lines, n_around, radius )
    graphics_struct   *graphics;
    lines_struct      *lines;
    int               n_around;
    Real              radius;
{
    int              i, l, line_size;
    object_struct    *object;
    quadmesh_struct  *quadmesh;
    Point            *points;
    Status           status;
    Status           create_object();
    model_struct     *get_current_model();
    Status           add_object_to_model();
    void             get_default_surfprop();
    void             generate_tube();

    status = OK;

    for_less( l, 0, lines->n_items )
    {
        status = create_object( &object, QUADMESH );

        if( status == OK )
        {
            quadmesh = object->ptr.quadmesh;

            quadmesh->colour_flag = ONE_COLOUR;

            ALLOC( status, quadmesh->colours, 1 );
            quadmesh->colours[0] = lines->colours[0];
            get_default_surfprop( &quadmesh->surfprop );

            line_size = GET_OBJECT_SIZE( *lines, l );

            quadmesh->m = line_size;
            quadmesh->m_closed = FALSE;
            quadmesh->n = n_around;
            quadmesh->n_closed = TRUE;

            ALLOC( status, quadmesh->points, quadmesh->m * quadmesh->n );
        }

        if( status == OK )
            ALLOC( status, quadmesh->normals, quadmesh->m * quadmesh->n );

        if( status == OK )
            ALLOC( status, points, line_size );

        if( status == OK )
        {
            for_less( i, 0, line_size )
            {
                points[i] = lines->points[lines->indices[
                               POINT_INDEX(lines->end_indices,l,i)]];
            }

            generate_tube( line_size, points, n_around, radius,
                           quadmesh->points, quadmesh->normals );
 
            FREE( status, points );
        }

        if( status == OK )
            status = add_object_to_model( get_current_model(graphics), object );

        if( status != OK )
            break;
    }

    return( status );
}
