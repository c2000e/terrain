
#include "chunk_manager.h"

#include <stdlib.h>
#include <math.h>

int ChunkManager_offsets(int radius, int* offsets)
{
    int count = 0;
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            for (int z = -radius; z <= radius; z++)
            {
                if (x * x + y * y + z * z <= radius * radius)
                {
                    if (offsets != NULL)
                    {
                        offsets[count] = x * 2 * CHUNK_RADIUS;
                        offsets[count + 1] = y * 2 * CHUNK_RADIUS;
                        offsets[count + 2] = z * 2 * CHUNK_RADIUS;
                    }
                    count += 3;
                }
            }
        }
    }
    return count;
}

ChunkManager ChunkManager_create(int radius, float player[3])
{
    ChunkManager chunk_manager;

    chunk_manager.radius = radius;
    
    int offset_count = ChunkManager_offsets(radius, NULL);
    chunk_manager.offsets = (int*)malloc(offset_count * sizeof(int));
    ChunkManager_offsets(radius, chunk_manager.offsets);
    
    chunk_manager.chunk_count = offset_count / 3;
    chunk_manager.chunks = (Chunk*)malloc(chunk_manager.chunk_count
           * sizeof(Chunk));
    
    chunk_manager.center[0] = 0;
    chunk_manager.center[1] = 0;
    chunk_manager.center[2] = 0;
    ChunkManager_update(&chunk_manager, player);

    return chunk_manager;
}

void ChunkManager_update(ChunkManager* chunk_manager, float player[3])
{
    int chunk_x = (int)floor(player[0] / (2 * CHUNK_RADIUS));
    int chunk_y = (int)floor(player[1] / (2 * CHUNK_RADIUS));
    int chunk_z = (int)floor(player[2] / (2 * CHUNK_RADIUS));
    chunk_x = chunk_x * 2 * CHUNK_RADIUS + CHUNK_RADIUS;
    chunk_y = chunk_y * 2 * CHUNK_RADIUS + CHUNK_RADIUS;
    chunk_z = chunk_z * 2 * CHUNK_RADIUS + CHUNK_RADIUS;
    if (chunk_x != chunk_manager->center[0] || chunk_y
            != chunk_manager->center[1] || chunk_z != chunk_manager->center[2])
    {
        chunk_manager->center[0] = chunk_x;
        chunk_manager->center[1] = chunk_y;
        chunk_manager->center[2] = chunk_z;
        for (int i = 0; i < chunk_manager->chunk_count; i++)
        {
            chunk_manager->chunks[i] = Chunk_create(
                    chunk_x + chunk_manager->offsets[i * 3],
                    chunk_y + chunk_manager->offsets[i * 3 + 1],
                    chunk_z + chunk_manager->offsets[i * 3 + 2]
            );
        }
    }
}
