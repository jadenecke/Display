
#include  <def_graphics.h>
#include  <def_connect.h>
#include  <def_alloc.h>
#include  <def_queue.h>

#define  REGION_OF_INTEREST    1

public  Status  disconnect_components( volume, voxel_indices, axis,
                                       n_labels, labels,
                                       min_threshold, max_threshold )
    volume_struct   *volume;
    int             voxel_indices[3];
    int             axis[3];
    int             n_labels;
    label_struct    labels[];
    int             min_threshold;
    int             max_threshold;
{
    Status        status;
    int           conn_length, val, i;
    void          create_distance_transform();
    void          assign_region_flags();
    pixel_struct  **pixels;
    int           x, y, index[3], size[3];
    void          set_voxel_inactivity();
    void          get_volume_size();
    Status        label_components();

    get_volume_size( volume, &size[X_AXIS], &size[Y_AXIS], &size[Z_AXIS] );

    ALLOC2( status, pixels, size[axis[X_AXIS]], size[axis[Y_AXIS]],
            pixel_struct );

    index[axis[Z_AXIS]] = voxel_indices[axis[Z_AXIS]];
    for_less( x, 0, size[axis[X_AXIS]] )
    {
        index[axis[X_AXIS]] = x;
        for_less( y, 0, size[axis[Y_AXIS]] )
        {
            index[axis[Y_AXIS]] = y;

            val = GET_VOLUME_DATA( *volume, index[0], index[1], index[2] );

            pixels[x][y].inside = (val >= min_threshold && val <=max_threshold);
            pixels[x][y].label = 0;
        }
    }

    for_less( i, 0, n_labels )
    {
        if( labels[i].voxel_indices[axis[Z_AXIS]] == voxel_indices[Z_AXIS] )
        {
            pixels[labels[i].voxel_indices[X_AXIS]]
                  [labels[i].voxel_indices[Y_AXIS]].label = labels[i].id;
        }
    }

    status = label_components( size[X_AXIS], size[Y_AXIS], pixels,
                               REGION_OF_INTEREST );

    index[axis[Z_AXIS]] = voxel_indices[axis[Z_AXIS]];
    for_less( x, 0, size[axis[X_AXIS]] )
    {
        index[axis[X_AXIS]] = x;
        for_less( y, 0, size[axis[Y_AXIS]] )
        {
            index[axis[Y_AXIS]] = y;

            if( pixels[x][y].label != REGION_OF_INTEREST )
                set_voxel_inactivity( volume, index[0], index[1], index[2],
                                      TRUE );
            else
                set_voxel_inactivity( volume, index[0], index[1], index[2],
                                      FALSE );
        }
    }

    FREE2( status, pixels );

    return( status );
}