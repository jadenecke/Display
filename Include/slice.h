
#ifndef  DEF_SLICE
#define  DEF_SLICE

#include  <volume_io.h>
#include  <atlas.h>

#define   N_SLICE_VIEWS   4

#define   OBLIQUE_VIEW_INDEX    (N_SLICE_VIEWS-1)

typedef  struct
{
    BOOLEAN       visibility;
    Real          x_axis[N_DIMENSIONS];
    Real          y_axis[N_DIMENSIONS];
    Real          x_trans, y_trans;
    Real          x_scaling, y_scaling;
    int           prev_viewport_x_size;
    int           prev_viewport_y_size;
    int           used_viewport_x_size;
    int           used_viewport_y_size;
    BOOLEAN       update_flag;
    Filter_types  filter_type;
    Real          filter_width;
} slice_view_struct;

typedef  struct
{
    Real              min_threshold;
    Real              max_threshold;
    Neighbour_types   connectivity;
} segmenting_struct;

typedef struct
{
    Real             top_offset;
    Real             bottom_offset;
    Real             left_offset;
    Real             bar_width;
    Real             tick_width;
    int              desired_n_intervals;
} colour_bar_struct;

typedef  struct
{
    int              axis_index;
    int              slice_index;
    int              **saved_labels;
} slice_undo_struct;

typedef  struct
{
    Volume                 original_volume;
    Volume                 labels;

    Volume                 volume;

    int                    n_labels;
    int                    n_distinct_colour_tables;
    int                    offset;
    Colour                 **colour_tables;
    Colour                 *label_colours;
    Real                   label_colour_ratio;
    colour_coding_struct   colour_coding;
    colour_bar_struct      colour_bar;
    BOOLEAN                display_labels;

    Real                   x_split, y_split;

    Real                   current_voxel[N_DIMENSIONS];
    slice_view_struct      slice_views[N_SLICE_VIEWS];
    int                    next_to_update;
    void                   *render_storage;

    segmenting_struct      segmenting;
    atlas_struct           atlas;

    Real                   x_brush_radius, y_brush_radius, z_brush_radius;
    int                    current_paint_label;
    object_struct          *brush_outline;
    slice_undo_struct      undo;

    lines_struct           unscaled_histogram_lines;
    object_struct          *histogram_object;

    int                    cross_section_index;
    BOOLEAN                cross_section_visibility;
    BOOLEAN                cross_section_vector_present;
    Real                   cross_section_vector[MAX_DIMENSIONS];

} slice_window_struct;


#endif
