
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
                        offsets[count] = x;
                        offsets[count + 1] = y;
                        offsets[count + 2] = z;
                    }
                    count += 3;
                }
            }
        }
    }
    return count;
}

void ChunkManager_worldToChunk(const float src[3], int dst[3])
{
    dst[0] = (int)floor(src[0] / CHUNK_WIDTH);
    dst[1] = (int)floor(src[1] / CHUNK_WIDTH);
    dst[2] = (int)floor(src[2] / CHUNK_WIDTH);
}

void ChunkManager_recenter(ChunkManager* chunk_manager, const int origin[3])
{
    chunk_manager->origin[0] = origin[0];
    chunk_manager->origin[1] = origin[1];
    chunk_manager->origin[2] = origin[2];
    for (int i = 0; i < chunk_manager->chunk_count; i++)
    {
        chunk_manager->chunks[i] = Chunk_create(
                origin[0] + chunk_manager->offsets[i * 3],
                origin[1] + chunk_manager->offsets[i * 3 + 1],
                origin[2] + chunk_manager->offsets[i * 3 + 2]
        );
    }
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
    
    ChunkManager_worldToChunk(player, chunk_manager.origin);
    ChunkManager_recenter(&chunk_manager, chunk_manager.origin);

    return chunk_manager;
}

void ChunkManager_update(ChunkManager* chunk_manager, float player[3])
{
    int new_origin[3];
    ChunkManager_worldToChunk(player, new_origin);
    if (new_origin[0] != chunk_manager->origin[0]
            || new_origin[1] != chunk_manager->origin[1]
            || new_origin[2] != chunk_manager->origin[2])
    {
        ChunkManager_recenter(chunk_manager, new_origin);
    }
}
