
#include  <def_graphics.h>
#include  <def_globals.h>

public  void  initialize_slice_window( graphics )
    graphics_struct   *graphics;
{
    void                 initialize_slice_window_events();
    int                  c;

    graphics->slice.volume = (volume_struct *) 0;

    initialize_slice_window_events( graphics );

    for_less( c, 0, N_DIMENSIONS )
    {
        graphics->slice.slice_views[c].slice_index = 0;
        graphics->slice.slice_views[c].x_offset = 0;
        graphics->slice.slice_views[c].y_offset = 0;
        graphics->slice.slice_views[c].x_scale = 1.0;
        graphics->slice.slice_views[c].y_scale = 1.0;
    }
}

public  void  update_slice_window( graphics )
    graphics_struct  *graphics;
{
    int   axis_index;
    int   voxel_indices[N_DIMENSIONS];
    int   x_pixel_start, x_pixel_end, y_pixel_start, y_pixel_end;
    void  draw_2d_line();
    void  draw_slice();
    void  get_slice_view();

    draw_2d_line( graphics, PIXEL_VIEW, &Slice_split_colour,
                  (Real) graphics->slice.x_split, 0.0,
                  (Real) graphics->slice.x_split,
                  (Real) (graphics->window.y_size-1) );

    draw_2d_line( graphics, PIXEL_VIEW, &Slice_split_colour,
                  0.0, (Real) graphics->slice.y_split,
                  (Real) (graphics->window.x_size-1),
                  (Real) graphics->slice.y_split );

    for_less( axis_index, 0, N_DIMENSIONS )
    {
        get_slice_view( graphics, axis_index, 
                        &x_pixel_start, &y_pixel_start,
                        &x_pixel_end, &y_pixel_end,
                        voxel_indices );

        draw_slice( graphics, axis_index, x_pixel_start, y_pixel_start,
                    x_pixel_end, y_pixel_end, voxel_indices );
    }
}

public  Status  delete_slice_window_info( slice_window )
    slice_window_struct   *slice_window;
{
    return( OK );
}

public  Boolean  find_slice_view_mouse_is_in( graphics, x_pixel, y_pixel,
                                              axis_index )
    graphics_struct   *graphics;
    int               x_pixel, y_pixel;
    int               *axis_index;
{
    Boolean  found;
    int      c;
    int      x_min, x_max, y_min, y_max;
    void     get_slice_viewport();

    found = FALSE;

    for_less( c, 0, N_DIMENSIONS )
    {
        get_slice_viewport( graphics, c, &x_min, &x_max, &y_min, &y_max );

        if( x_pixel >= x_min && x_pixel <= x_max &&
            y_pixel >= y_min && y_pixel <= y_max )
        {
            *axis_index = c;
            found = TRUE;

            break;
        }
    }

    return( found );
}

public  Boolean  convert_pixel_to_voxel( graphics, x_pixel, y_pixel, x, y, z )
    graphics_struct   *graphics;
    int               x_pixel, y_pixel;
    int               *x, *y, *z;
{
    Boolean  found;
    Real     x_scale, y_scale;
    int      axis_index, x_index, y_index;
    int      start_indices[N_DIMENSIONS];
    int      voxel_indices[N_DIMENSIONS];
    int      x_pixel_start, x_pixel_end, y_pixel_start, y_pixel_end;
    void     get_slice_view();
    void     get_2d_slice_axes();

    found = FALSE;

    if( find_slice_view_mouse_is_in( graphics, x_pixel, y_pixel,
                                         &axis_index ) )
    {
        get_slice_view( graphics, axis_index, 
                        &x_pixel_start, &y_pixel_start,
                        &x_pixel_end, &y_pixel_end,
                        start_indices );

        if( x_pixel >= x_pixel_start && x_pixel <= x_pixel_end &&
            y_pixel >= y_pixel_start && y_pixel <= y_pixel_end )
        {
            get_2d_slice_axes( axis_index, &x_index, &y_index );

            x_scale = graphics->slice.slice_views[axis_index].x_scale;
            y_scale = graphics->slice.slice_views[axis_index].y_scale;

            voxel_indices[axis_index] = start_indices[axis_index];

            voxel_indices[x_index] = start_indices[x_index] +
                                        (x_pixel - x_pixel_start) * x_scale;
            voxel_indices[y_index] = start_indices[y_index] +
                                        (y_pixel - y_pixel_start) * y_scale;

            *x = voxel_indices[X_AXIS];
            *y = voxel_indices[Y_AXIS];
            *z = voxel_indices[Z_AXIS];

            found = TRUE;
        }
    }

    return( found );
}

private  void  get_slice_viewport( graphics, axis_index,
                                   x_min, x_max, y_min, y_max )
    graphics_struct   *graphics;
    int               axis_index;
    int               *x_min, *x_max, *y_min, *y_max;
{
    switch( axis_index )
    {
    case X_AXIS:
        *x_min = Slice_split_border;
        *x_max = graphics->slice.x_split-1-Slice_split_border;
        *y_min = graphics->slice.y_split+1+Slice_split_border;
        *y_max = graphics->window.y_size-Slice_split_border;
        break;

    case Y_AXIS:
        *x_min = graphics->slice.x_split+1+Slice_split_border;
        *x_max = graphics->window.x_size-Slice_split_border;
        *y_min = graphics->slice.y_split+1+Slice_split_border;
        *y_max = graphics->window.y_size-Slice_split_border;
        break;

    case Z_AXIS:
        *x_min = Slice_split_border;
        *x_max = graphics->slice.x_split-1-Slice_split_border;
        *y_min = Slice_split_border;
        *y_max = graphics->slice.y_split-1-Slice_split_border;
        break;
    }
}

private  void  get_slice_view( graphics, axis_index, 
                               x_pixel, y_pixel, x_pixel_end, y_pixel_end,
                               indices )
    graphics_struct  *graphics;
    int              axis_index;
    int              *x_pixel, *y_pixel;
    int              *x_pixel_end, *y_pixel_end;
    int              indices[N_DIMENSIONS];
{
    int   x_axis_index, y_axis_index;
    int   x_offset, y_offset;
    int   x_size, y_size;
    int   x_min, x_max, y_min, y_max;
    Real  x_scale, y_scale;
    void  get_slice_viewport();
    void  get_2d_slice_axes();

    indices[axis_index] = graphics->slice.slice_views[axis_index].slice_index;

    x_offset = graphics->slice.slice_views[axis_index].x_offset;
    y_offset = graphics->slice.slice_views[axis_index].y_offset;
    x_scale = graphics->slice.slice_views[axis_index].x_scale;
    y_scale = graphics->slice.slice_views[axis_index].y_scale;

    get_2d_slice_axes( axis_index, &x_axis_index, &y_axis_index );

    x_size = graphics->slice.volume->size[x_axis_index];
    y_size = graphics->slice.volume->size[y_axis_index];

    get_slice_viewport( graphics, axis_index, &x_min, &x_max, &y_min, &y_max );

    *x_pixel = x_min + x_offset;
    indices[x_axis_index] = 0;

    if( *x_pixel < x_min )
    {
        *x_pixel = x_min;
        indices[x_axis_index] = ROUND( - (Real) x_offset / x_scale );

        if( indices[x_axis_index] >= x_size )
        {
            *x_pixel = x_max + 1;
        }
    }

    *x_pixel_end = MIN( ROUND(x_min + x_offset + (Real) (x_size - 1) / x_scale),
                        x_max );

    *y_pixel = y_min + y_offset;
    indices[y_axis_index] = 0;

    if( *y_pixel < y_min )
    {
        *y_pixel = y_min;
        indices[y_axis_index] = ROUND( - (Real) y_offset / y_scale );

        if( indices[y_axis_index] >= y_size )
        {
            *y_pixel = y_max + 1;
        }
    }

    *y_pixel_end = MIN( ROUND(y_min + y_offset + (Real) (y_size - 1) / y_scale),
                        y_max );
}

public   void     get_2d_slice_axes( axis_index, x_index, y_index )
    int   axis_index;
    int   *x_index;
    int   *y_index;
{
    switch( axis_index )
    {
    case X_AXIS:
        *x_index = Z_AXIS;
        *y_index = Y_AXIS;
        break;

    case Y_AXIS:
        *x_index = X_AXIS;
        *y_index = Z_AXIS;
        break;

    case Z_AXIS:
        *x_index = X_AXIS;
        *y_index = Y_AXIS;
        break;
    }
}
