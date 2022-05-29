#include "save.h"

#include <stdio.h>                                                             

int save(const Vec3 position)
{
    FILE *fp;
    fp = fopen("savedata", "w");
    if (!fp) return -1;
    fwrite(position, 3, sizeof(float), fp);
    fclose(fp);
    return 0;
}

int load(Vec3 position)
{
    FILE *fp;
    fp = fopen("savedata", "r");
    if (!fp) return -1;
    fread(position, sizeof(float), 3, fp);
    fclose(fp);
    return 0;
}
