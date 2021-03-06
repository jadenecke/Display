
#include  <display.h>

#define  REGION_OF_INTEREST    1

public  void  disconnect_components(
    Volume          volume,
    int             voxel_indices[],
    int             axis[],
    int             n_labels,
    label_struct    labels[],
    Real            min_threshold,
    Real            max_threshold )
{
    Real          val;
    int           i;
    pixel_struct  **pixels;
    int           x, y, index[N_DIMENSIONS], size[N_DIMENSIONS];
    BOOLEAN       inside;

    get_volume_sizes( volume, size );

    ALLOC2D( pixels, size[axis[X]], size[axis[Y]] );

    index[axis[Z]] = voxel_indices[axis[Z]];
    for_less( x, 0, size[axis[X]] )
    {
        index[axis[X]] = x;
        for_less( y, 0, size[axis[Y]] )
        {
            index[axis[Y]] = y;

            if( get_voxel_activity_flag( volume, index ) )
            {
                GET_VALUE_3D( val, volume, index[0], index[1], index[2] );

                inside = (val >= min_threshold && val <= max_threshold);
            }
            else
                inside = FALSE;

            pixels[x][y].inside = inside;
            pixels[x][y].label = 0;
        }
    }

    for_less( i, 0, n_labels )
    {
        if( labels[i].voxel_indices[axis[Z]] == voxel_indices[Z] )
        {
            pixels[labels[i].voxel_indices[axis[X]]]
                  [labels[i].voxel_indices[axis[Y]]].label = labels[i].id;
        }
    }

    label_components( size[axis[X]], size[axis[Y]], pixels,
                      REGION_OF_INTEREST );

    index[axis[Z]] = voxel_indices[axis[Z]];
    for_less( x, 0, size[axis[X]] )
    {
        index[axis[X]] = x;
        for_less( y, 0, size[axis[Y]] )
        {
            index[axis[Y]] = y;

            if( pixels[x][y].inside )
            {
                if( pixels[x][y].label != REGION_OF_INTEREST )
                    set_voxel_activity_flag( volume, index, FALSE );
                else
                    set_voxel_activity_flag( volume, index, TRUE );
            }
        }
    }

    FREE2D( pixels );
}
