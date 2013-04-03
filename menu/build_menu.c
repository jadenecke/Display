/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef lint

#endif


#include  <display.h>

typedef  struct
{
    int       key;
    VIO_STR    key_name;
    VIO_Real      x_pos, y_pos, length;
    VIO_BOOL   in_slanted_part_of_keyboard;
} position_struct;

static   position_struct   positions[] = {
                           {(int) LEFT_ARROW_KEY, "<", 6.5, 0.0, 0.5, FALSE},
                           {(int) DOWN_ARROW_KEY, "v", 7.0, 0.0, 0.5, FALSE},
                           {(int) RIGHT_ARROW_KEY, ">", 7.5, 0.0, 0.5, FALSE},
                           {(int) UP_ARROW_KEY, "^", 7.0, 1.0, 0.5, FALSE},

                           {' ', "space", 1.0, 0.0, 3.0, FALSE },

                           {'z', "Z", 0.0, 1.0, 1.0, TRUE },
                           {'x', "X", 1.0, 1.0, 1.0, TRUE },
                           {'c', "C", 2.0, 1.0, 1.0, TRUE },
                           {'v', "V", 3.0, 1.0, 1.0, TRUE },
                           {'b', "B", 4.0, 1.0, 1.0, TRUE },
                           {'n', "N", 5.0, 1.0, 1.0, TRUE },
                           {'m', "M", 6.0, 1.0, 1.0, TRUE },

                           {'a', "A", 0.0, 2.0, 1.0, TRUE },
                           {'s', "S", 1.0, 2.0, 1.0, TRUE },
                           {'d', "D", 2.0, 2.0, 1.0, TRUE },
                           {'f', "F", 3.0, 2.0, 1.0, TRUE },
                           {'g', "G", 4.0, 2.0, 1.0, TRUE },
                           {'h', "H", 5.0, 2.0, 1.0, TRUE },
                           {'j', "J", 6.0, 2.0, 1.0, TRUE },

                           {'q', "Q", 0.0, 3.0, 1.0, TRUE },
                           {'w', "W", 1.0, 3.0, 1.0, TRUE },
                           {'e', "E", 2.0, 3.0, 1.0, TRUE },
                           {'r', "R", 3.0, 3.0, 1.0, TRUE },
                           {'t', "T", 4.0, 3.0, 1.0, TRUE },
                           {'y', "Y", 5.0, 3.0, 1.0, TRUE },
                           {'u', "U", 6.0, 3.0, 1.0, TRUE },

                           {'1', "1", 0.0, 4.0, 1.0, TRUE },
                           {'2', "2", 1.0, 4.0, 1.0, TRUE },
                           {'3', "3", 2.0, 4.0, 1.0, TRUE },
                           {'4', "4", 3.0, 4.0, 1.0, TRUE },
                           {'5', "5", 4.0, 4.0, 1.0, TRUE },
                           {'6', "6", 5.0, 4.0, 1.0, TRUE },
                           {'7', "7", 6.0, 4.0, 1.0, TRUE },

                           {'+', "+", 7.2, 2.0, 0.4, TRUE },
                           {'=', "=", 0.0, 0.0, 0.0, TRUE },
                           {'-', "-", 7.7, 2.0, 0.4, TRUE }
                         };

static  void   create_menu_text(
    display_struct    *menu_window,
    menu_entry_struct *menu_entry );
static  VIO_BOOL  lookup_key(
    int              key,
    position_struct  **desc );
static  void   compute_key_position(
    menu_window_struct   *menu,
    int                  key,
    VIO_Real                 *x1,
    VIO_Real                 *y1,
    VIO_Real                 *x2,
    VIO_Real                 *y2,
    VIO_Real                 *length );
static  VIO_STR  get_key_string(
    int     key );
static  object_struct   *create_menu_box(
    display_struct    *menu_window,
    int               key );
static  object_struct   *create_menu_character(
    display_struct    *menu_window,
    int               key );
static  VIO_BOOL   point_within_menu_key_entry(
    display_struct   *menu_window,
    int              key,
    VIO_Real             x,
    VIO_Real             y );
static  void   set_menu_text_position(
    display_struct    *menu_window,
    menu_entry_struct *menu_entry );
static  void   set_menu_box_position(
    display_struct    *menu_window,
    int               key,
    object_struct     *box_object );
static  void   set_menu_character_position(
    display_struct    *menu_window,
    int               key,
    object_struct     *text_object );

/* -------------------------------------------------------------------- */

  void  build_menu(
    display_struct    *menu_window )
{
    int      i, key;

    for_less( i, 0, VIO_SIZEOF_STATIC_ARRAY(positions) )
    {
        if( positions[i].length > 0.0 )
        {
            key = positions[i].key;
            menu_window->menu.box_objects[key] =
                                   create_menu_box( menu_window, key );
            menu_window->menu.text_objects[key] =
                                   create_menu_character( menu_window, key );
        }
    }

    for_less( i, 1, menu_window->menu.n_entries )
        create_menu_text( menu_window, &menu_window->menu.entries[i] );

    rebuild_menu( menu_window );
}

  void  rebuild_menu(
    display_struct    *menu_window )
{
    int      i, key;

    for_less( i, 0, VIO_SIZEOF_STATIC_ARRAY(positions) )
    {
        if( positions[i].length > 0.0 )
        {
            key = positions[i].key;
            set_menu_box_position( menu_window, key,
                                   menu_window->menu.box_objects[key] );
            set_menu_character_position( menu_window, key,
                                     menu_window->menu.text_objects[key] );
        }
    }

    for_less( i, 1, menu_window->menu.n_entries )
        set_menu_text_position( menu_window, &menu_window->menu.entries[i] );
}


static  void   create_menu_text(
    display_struct    *menu_window,
    menu_entry_struct *menu_entry )
{
    int             i;
    VIO_Point           origin;
    text_struct     *text;
    model_struct    *model;

    ALLOC( menu_entry->text_list, menu_window->menu.n_lines_in_entry );

    for_less( i, 0, menu_window->menu.n_lines_in_entry )
    {
        menu_entry->text_list[i] = create_object( TEXT );

        menu_entry->text_list[i]->visibility = FALSE;

        model = get_graphics_model( menu_window, MENU_BUTTONS_MODEL );

        add_object_to_model( model, menu_entry->text_list[i] );

        text = get_text_ptr( menu_entry->text_list[i] );

        fill_Point( origin, 0.0, 0.0, 0.0 );

        initialize_text( text, &origin, Menu_character_colour,
                         (Font_types) Menu_window_font,
                         menu_window->menu.font_size );
    }

    set_menu_text_position( menu_window, menu_entry );

    set_menu_text( menu_window, menu_entry, menu_entry->label );

    update_menu_text( menu_window, menu_entry );
}

static  void   set_menu_text_position(
    display_struct    *menu_window,
    menu_entry_struct *menu_entry )
{
    VIO_Real            x, y, length;
    int             i;
    text_struct     *text;
    VIO_STR          key_string;

    for_less( i, 0, menu_window->menu.n_lines_in_entry )
    {
        text = get_text_ptr( menu_entry->text_list[i] );

        compute_key_position( &menu_window->menu, menu_entry->key, &x, &y,
                              NULL, NULL, &length );

        x += menu_window->menu.x_menu_text_offset;
        y += (VIO_Real) (menu_window->menu.n_lines_in_entry - i) *
             menu_window->menu.character_height -
             menu_window->menu.y_menu_text_offset;

        if( i == 0 )
        {
            key_string = get_key_string( menu_entry->key );
            x += G_get_text_length( key_string, (Font_types) Menu_window_font,
                                    menu_window->menu.font_size );
            delete_string( key_string );
        }

        fill_Point( text->origin, x, y, 0.0 );
        text->size = menu_window->menu.font_size;
    }

    menu_entry->n_chars_across = (int)
                 (length * (VIO_Real) menu_window->menu.n_chars_per_unit_across);
}

static  VIO_BOOL  lookup_key(
    int              key,
    position_struct  **desc )
{
    int      i;
    VIO_BOOL  found;

    found = FALSE;

    for_less( i, 0, VIO_SIZEOF_STATIC_ARRAY(positions) )
    {
        if( positions[i].key == key )
        {
            *desc = &positions[i];
            found = TRUE;
            break;
        }
    }

    return( found );
}

static  void   compute_key_position(
    menu_window_struct   *menu,
    int                  key,
    VIO_Real                 *x1,
    VIO_Real                 *y1,
    VIO_Real                 *x2,
    VIO_Real                 *y2,
    VIO_Real                 *length )
{
    VIO_BOOL          found;
    position_struct  *desc;
    VIO_Real             x_dx, y_dy;

    found = lookup_key( key, &desc );

    if( !found )
    {
        print( "Character %c\n", key );
        HANDLE_INTERNAL_ERROR( "Unrecognized menu key\n" );
    }
    else if( desc->length <= 0.0 )
    {
        *x1 = 0.0;
        if( x2 != NULL )
            *x2 = 0.0;
        *y1 = 0.0;
        if( y2 != NULL )
            *y2 = 0.0;
        *length = 0.0;
    }
    else
    {
        x_dx = menu->x_dx + (VIO_Real) menu->n_chars_per_unit_across *
               menu->character_width;
        y_dy = menu->y_dy + (VIO_Real) menu->n_lines_in_entry *
               menu->character_height;

        *x1 = menu->x_menu_origin + (VIO_Real) desc->x_pos * x_dx;
        *y1 = menu->y_menu_origin + (VIO_Real) desc->y_pos * y_dy;

        if( desc->in_slanted_part_of_keyboard )
        {
            *x1 += (VIO_Real) desc->y_pos * menu->y_dx;
            *y1 += (VIO_Real) desc->x_pos * menu->x_dy;
        }

        *length = desc->length;

        if( x2 != NULL )
        {
            *x2 = *x1 + *length * (VIO_Real) menu->n_chars_per_unit_across *
                        menu->character_width;
        }

        if( y2 != NULL )
        {
            *y2 = *y1 + (VIO_Real) menu->n_lines_in_entry * menu->character_height;
        }
    }
}

static  VIO_STR  get_key_string(
    int      key )
{
    char              buffer[VIO_EXTREMELY_LARGE_STRING_SIZE];
    VIO_STR            string;
    position_struct   *desc;

    if( lookup_key( key, &desc ) )
    {
        (void) sprintf( buffer, "%s ", desc->key_name );
        string = create_string( buffer );
    }
    else
        string = create_string( NULL );

    return( string );
}

  VIO_Real  get_size_of_menu_text_area(
    display_struct   *menu_window,
    int              key,
    int              line_number )
{
    VIO_Real          size, x1, y1, x2, y2, length;
    text_struct   *text;

    compute_key_position( &menu_window->menu, key, &x1, &y1, &x2, &y2, &length);

    text = get_text_ptr(
                menu_window->menu.key_menus[key]->text_list[line_number] );

    size = x2 - (VIO_Real) Point_x(text->origin);

    return( size );
}

static  object_struct   *create_menu_box(
    display_struct    *menu_window,
    int               key )
{
    object_struct   *object;
    lines_struct    *lines;
    model_struct    *model;

    object = create_object( LINES );

    model = get_graphics_model( menu_window, MENU_BUTTONS_MODEL );

    add_object_to_model( model, object );

    lines = get_lines_ptr( object );
    initialize_lines( lines, Menu_box_colour );

    lines->line_thickness = 1.0f;
    lines->n_points = 4;
    lines->n_items = 1;

    ALLOC( lines->points, lines->n_points );
    ALLOC( lines->end_indices, lines->n_items );

    lines->end_indices[0] = 5;

    ALLOC( lines->indices, lines->end_indices[0] );

    lines->indices[0] = 0;
    lines->indices[1] = 1;
    lines->indices[2] = 2;
    lines->indices[3] = 3;
    lines->indices[4] = 0;

    return( object );
}

static  void   set_menu_box_position(
    display_struct    *menu_window,
    int               key,
    object_struct     *box_object )
{
    lines_struct    *lines;
    VIO_Real            x1, y1, x2, y2, length;

    lines = get_lines_ptr( box_object );

    compute_key_position( &menu_window->menu, key, &x1, &y1, &x2, &y2, &length);

    fill_Point( lines->points[0], x1, y1, 0.0 );
    fill_Point( lines->points[1], x2, y1, 0.0 );
    fill_Point( lines->points[2], x2, y2, 0.0 );
    fill_Point( lines->points[3], x1, y2, 0.0 );
}

static  object_struct   *create_menu_character(
    display_struct    *menu_window,
    int               key )
{
    VIO_Real            x, y, length;
    VIO_Point           origin;
    object_struct   *object;
    text_struct     *text;
    model_struct    *model;

    object = create_object( TEXT );

    compute_key_position( &menu_window->menu, key, &x, &y, NULL, NULL, &length);

    fill_Point( origin,
                x + menu_window->menu.x_menu_text_offset,
                y + (VIO_Real) menu_window->menu.n_lines_in_entry *
                    menu_window->menu.character_height -
                    menu_window->menu.y_menu_text_offset,
                0.0 );

    text = get_text_ptr( object );

    initialize_text( text, &origin, Menu_key_colour,
                     (Font_types) Menu_window_font,
                     menu_window->menu.font_size );

    replace_string( &text->string, get_key_string( key ) );

    model = get_graphics_model( menu_window, MENU_BUTTONS_MODEL );

    add_object_to_model( model, object );

    return( object );
}

static  void   set_menu_character_position(
    display_struct    *menu_window,
    int               key,
    object_struct     *text_object )
{
    VIO_Real            x, y, length;
    text_struct     *text;

    text = get_text_ptr( text_object );

    compute_key_position( &menu_window->menu, key, &x, &y, NULL, NULL, &length);

    fill_Point( text->origin,
                x + menu_window->menu.x_menu_text_offset,
                y + (VIO_Real) menu_window->menu.n_lines_in_entry *
                    menu_window->menu.character_height -
                    menu_window->menu.y_menu_text_offset,
                0.0 );

    text->size = menu_window->menu.font_size;
}

static  VIO_BOOL   point_within_menu_key_entry(
    display_struct   *menu_window,
    int              key,
    VIO_Real             x,
    VIO_Real             y )
{
    VIO_Real      x1, y1, x2, y2, length;

    compute_key_position( &menu_window->menu, key, &x1, &y1, &x2, &y2, &length);

    if( length <= 0.0 )
        return( FALSE );
    else
        return( x >= x1 && x <= x2 && y >= y1 && y <= y2 );
}

  VIO_BOOL   lookup_key_for_mouse_position(
    display_struct   *menu_window,
    VIO_Real             x,
    VIO_Real             y,
    int              *key )
{
    int      i;
    VIO_BOOL  found;

    found = FALSE;

    for_less( i, 0, VIO_SIZEOF_STATIC_ARRAY(positions) )
    {
        if( point_within_menu_key_entry( menu_window, positions[i].key, x, y ) )
        {
            *key = positions[i].key;
            found = TRUE;
            break;
        }
    }

    return( found );
}
