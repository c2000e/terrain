#include "chunk.h"
#include "marching_cubes.h"

void Chunk_init(Chunk *chunk, IVec3 id)
{
    chunk->id[0] = id[0];
    chunk->id[1] = id[1];
    chunk->id[2] = id[2];

    chunk->vertex_count = 0;

    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);

    glGenBuffers(1, &chunk->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (GLvoid*)0);
    glEnableVertexAttribArray(0);
}

void Chunk_origin(const Chunk *chunk, Vec3 origin)
{
    origin[0] = chunk->id[0] * CHUNK_WIDTH;
    origin[1] = chunk->id[1] * CHUNK_WIDTH;
    origin[2] = chunk->id[2] * CHUNK_WIDTH;
}

static void Cube_corners(const Vec3 origin, float width, Vec3 corners[8])
{
    corners[0][0] = origin[0];
    corners[0][1] = origin[1];
    corners[0][2] = origin[2];

    corners[1][0] = origin[0] + width;
    corners[1][1] = origin[1];
    corners[1][2] = origin[2];
    
    corners[2][0] = origin[0] + width;
    corners[2][1] = origin[1];
    corners[2][2] = origin[2] + width;

    corners[3][0] = origin[0];
    corners[3][1] = origin[1];
    corners[3][2] = origin[2] + width;

    corners[4][0] = origin[0];
    corners[4][1] = origin[1] + width;
    corners[4][2] = origin[2];

    corners[5][0] = origin[0] + width;
    corners[5][1] = origin[1] + width;
    corners[5][2] = origin[2];

    corners[6][0] = origin[0] + width;
    corners[6][1] = origin[1] + width;
    corners[6][2] = origin[2] + width;

    corners[7][0] = origin[0];
    corners[7][1] = origin[1] + width;
    corners[7][2] = origin[2] + width;
}

void Chunk_meshify(Chunk *chunk, SDF f, float isolevel)
{
    chunk->vertex_count = 0;
    for (int i = 0; i < CHUNK_WIDTH; i++)
    {
        for (int j = 0; j < CHUNK_WIDTH; j++)
        {
            for (int k = 0; k < CHUNK_WIDTH; k++)
            {
                Vec3 chunk_origin;
                Chunk_origin(chunk, chunk_origin);

                Vec3 cell_origin = {
                    chunk_origin[0] + i,
                    chunk_origin[1] + j,
                    chunk_origin[2] + k
                };
                
                Vec3 cell_corners[8];
                Cube_corners(cell_origin, 1, cell_corners);

                chunk->vertex_count += MC_polygonize(cell_corners, f, isolevel,
                        &chunk->vertices[chunk->vertex_count]);
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * chunk->vertex_count,
            chunk->vertices, GL_STATIC_DRAW);
}

void Chunk_draw(const Chunk *chunk)
{
    glBindVertexArray(chunk->vao);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertex_count);
}
