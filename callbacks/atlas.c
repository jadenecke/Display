
#include  <def_display.h>

public  DEF_MENU_FUNCTION( set_atlas_on_or_off )   /* ARGSUSED */
{
    Boolean          state;
    display_struct   *slice_window;

    if( get_slice_window( display, &slice_window ) )
    {
        state = !slice_window->slice.atlas.enabled;
        set_atlas_state( slice_window, state );
        set_slice_window_update( slice_window, 0 );
        set_slice_window_update( slice_window, 1 );
        set_slice_window_update( slice_window, 2 );
    }

    return( OK );
}

public  DEF_MENU_UPDATE(set_atlas_on_or_off )   /* ARGSUSED */
{
    String           text;
    display_struct   *slice_window;

    if( get_slice_window( display, &slice_window ) )
        set_text_on_off( label, text, slice_window->slice.atlas.enabled );
    else
        (void) sprintf( text, label, "none" );

    set_menu_text( menu_window, menu_entry, text );

    return( OK );
}

public  DEF_MENU_FUNCTION( set_atlas_opacity )   /* ARGSUSED */
{
    Real             opacity;
    display_struct   *slice_window;

    if( get_slice_window( display, &slice_window ) )
    {
        print( "Enter atlas opacity: " );
        if( input_real( stdin, &opacity ) == OK && opacity >= 0.0 &&
            opacity <= 1.0 )
        {
            slice_window->slice.atlas.opacity = opacity;
            set_slice_window_update( slice_window, 0 );
            set_slice_window_update( slice_window, 1 );
            set_slice_window_update( slice_window, 2 );
        }
        (void) input_newline( stdin );
    }

    return( OK );
}

public  DEF_MENU_UPDATE(set_atlas_opacity )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION( set_atlas_transparent_threshold )   /* ARGSUSED */
{
    int              threshold;
    display_struct   *slice_window;

    if( get_slice_window( display, &slice_window ) )
    {
        print( "Enter value above which atlas is transparent (e.g. 220): " );
        if( input_int( stdin, &threshold ) == OK && threshold >= 0 )
        {
            slice_window->slice.atlas.transparent_threshold = threshold;
            set_slice_window_update( slice_window, 0 );
            set_slice_window_update( slice_window, 1 );
            set_slice_window_update( slice_window, 2 );
        }
        (void) input_newline( stdin );
    }

    return( OK );
}

public  DEF_MENU_UPDATE(set_atlas_transparent_threshold )   /* ARGSUSED */
{
    return( OK );
}

private  void  flip_atlas_on_an_axis(
    display_struct    *display,
    int               axis )
{
    display_struct   *slice_window;

    if( get_slice_window( display, &slice_window ) )
    {
        slice_window->slice.atlas.flipped[axis] =
                             !slice_window->slice.atlas.flipped[axis];
        set_slice_window_update( slice_window, 0 );
        set_slice_window_update( slice_window, 1 );
        set_slice_window_update( slice_window, 2 );
    }
}

public  DEF_MENU_FUNCTION( flip_atlas_x )   /* ARGSUSED */
{
    flip_atlas_on_an_axis( display, X );
    return( OK );
}

public  DEF_MENU_UPDATE(flip_atlas_x )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION( flip_atlas_y )   /* ARGSUSED */
{
    flip_atlas_on_an_axis( display, Y );
    return( OK );
}

public  DEF_MENU_UPDATE(flip_atlas_y )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION( flip_atlas_z )   /* ARGSUSED */
{
    flip_atlas_on_an_axis( display, Z );
    return( OK );
}

public  DEF_MENU_UPDATE(flip_atlas_z )   /* ARGSUSED */
{
    return( OK );
}

private  void  set_atlas_tolerance(
    display_struct    *display,
    int               axis )
{
    Real             distance_tolerance;
    display_struct   *slice_window;

    if( get_slice_window( display, &slice_window ) )
    {
        print( "Enter new tolerance: " );

        if( input_real( stdin, &distance_tolerance ) == OK )
        {
            slice_window->slice.atlas.slice_tolerance[axis] =
                             distance_tolerance;
            regenerate_atlas_lookup( slice_window );
            set_slice_window_update( slice_window, 0 );
            set_slice_window_update( slice_window, 1 );
            set_slice_window_update( slice_window, 2 );
        }
        (void) input_newline( stdin );
    }
}

public  DEF_MENU_FUNCTION( set_atlas_tolerance_x )   /* ARGSUSED */
{
    set_atlas_tolerance( display, X );
    return( OK );
}

public  DEF_MENU_UPDATE(set_atlas_tolerance_x )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION( set_atlas_tolerance_y )   /* ARGSUSED */
{
    set_atlas_tolerance( display, Y );
    return( OK );
}

public  DEF_MENU_UPDATE(set_atlas_tolerance_y )   /* ARGSUSED */
{
    return( OK );
}

public  DEF_MENU_FUNCTION( set_atlas_tolerance_z )   /* ARGSUSED */
{
    set_atlas_tolerance( display, Z );
    return( OK );
}

public  DEF_MENU_UPDATE(set_atlas_tolerance_z )   /* ARGSUSED */
{
    return( OK );
}