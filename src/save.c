#include "save.h"

#include <stdio.h>                                                             

int save(const Transform *t)
{
    FILE *fp;
    fp = fopen("savedata", "w");
    if (!fp) return -1;
    fwrite(t, 1, sizeof(Transform), fp);
    fclose(fp);
    return 0;
}

int load(Transform *t)
{
    FILE *fp;
    fp = fopen("savedata", "r");
    if (!fp) return -1;
    fread(t, sizeof(Transform), 1, fp);
    fclose(fp);
    return 0;
}
