
#include  <def_graphics.h>
#include  <def_math.h>

private  Boolean  get_current_volume( graphics, volume )
    graphics_struct   *graphics;
    volume_struct     **volume;
{
    Status          status;
    Boolean         found;
    object_struct   *current_object;
    Boolean         get_current_object();

    found = FALSE;

    if( get_current_object( graphics, &current_object ) )
    {
        BEGIN_TRAVERSE_OBJECT( status, current_object )

            if( !found && OBJECT->object_type == VOLUME )
            {
                found = TRUE;
                *volume = OBJECT->ptr.volume;
            }

        END_TRAVERSE_OBJECT
    }

    return( found );
}

public  DEF_MENU_FUNCTION( advance_slice )   /* ARGSUSED */
{
    volume_struct   *volume;
    void            rebuild_selected_list();

    if( get_current_volume(graphics,&volume) )
    {
        volume->slice_visibilities[volume->current_slice] = OFF;
        ++volume->current_slice;
        if( volume->current_slice >= volume->nz )
        {
            volume->current_slice = 0;
        }
        volume->slice_visibilities[volume->current_slice] = ON;

        rebuild_selected_list( graphics, menu_window );
    }

    graphics->update_required = TRUE;

    return( OK );
}

public  DEF_MENU_UPDATE(advance_slice )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION( retreat_slice )   /* ARGSUSED */
{
    volume_struct   *volume;
    void            rebuild_selected_list();

    if( get_current_volume(graphics,&volume) )
    {
        volume->slice_visibilities[volume->current_slice] = OFF;
        --volume->current_slice;
        if( volume->current_slice < 0 )
        {
            volume->current_slice = volume->nz-1;
        }
        volume->slice_visibilities[volume->current_slice] = ON;

        rebuild_selected_list( graphics, menu_window );
    }

    graphics->update_required = TRUE;

    return( OK );
}

public  DEF_MENU_UPDATE(retreat_slice )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION(set_slice_transform )   /* ARGSUSED */
{
    volume_struct   *volume;
    Real            degrees, x_offset, y_offset;
    void            create_2d_transform();

    if( get_current_volume(graphics,&volume) )
    {
        PRINT( "Enter degrees, x_off, y_off: " );
        if( scanf( "%f %f %f", &degrees, &x_offset, &y_offset ) == 3 )
        {
            create_2d_transform( &volume->slice_transforms[
                                 volume->current_slice],
                                 degrees, x_offset, y_offset );
        }
    }

    graphics->update_required = TRUE;

    return( OK );
}

public  DEF_MENU_UPDATE(set_slice_transform )   /* ARGSUSED */
{
    return( OK );
}

private  void  create_2d_transform( transform, degrees, x_offset, y_offset )
    Transform   *transform;
    Real        degrees;
    Real        x_offset;
    Real        y_offset;
{
    Real   c, s;
    void   make_identity_transform();

    make_identity_transform( transform );

    c = cos( (double) degrees * DEG_TO_RAD );
    s = sin( (double) degrees * DEG_TO_RAD );

    Transform_elem(*transform,0,0) = c;
    Transform_elem(*transform,0,1) = s;
    Transform_elem(*transform,1,0) = -s;
    Transform_elem(*transform,1,1) = c;

    Transform_elem(*transform,0,3) = x_offset;
    Transform_elem(*transform,1,3) = y_offset;
}
