
#ifndef  DEF_SLICE
#define  DEF_SLICE

#include  <def_bitlist.h>
#include  <def_objects.h>
#include  <def_graphics_types.h>

typedef  struct
{
    int       axis_index;
    int       axis_index1;
    Boolean   flip1;
    int       axis_index2;
    Boolean   flip2;
    int       x_offset, y_offset;
    Real      x_scale, y_scale;
    Boolean   update_flag;
} slice_view_struct;


typedef  struct
{
    volume_struct          *volume;

    Boolean                fast_lookup_present;
    Pixel_colour           *fast_lookup;
    colour_coding_struct   colour_coding;

    int                    x_split, y_split;

    int                    slice_index[N_DIMENSIONS];
    slice_view_struct      slice_views[3];

    int                    *temporary_indices;
    int                    temporary_indices_alloced;

} slice_window_struct;


#endif
