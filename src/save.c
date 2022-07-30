#include "save.h"

#include <stdio.h>                                                             

b8 save(const Transform *t)
{
    FILE *fp;
    fp = fopen("savedata", "w");
    if (!fp) return false;
    fwrite(t, 1, sizeof(Transform), fp);
    fclose(fp);
    return true;
}

b8 load(Transform *t)
{
    FILE *fp;
    fp = fopen("savedata", "r");
    if (!fp) return false;
    fread(t, sizeof(Transform), 1, fp);
    fclose(fp);
    return true;
}

