// check the biome at a block position
#include "finders.c"
#include "generator.c"
#include "layers.c"
#include "util.c"
#include <stdio.h>
 //set a reserved value, that its block position in inaccesible to be the equivelent of returning nothing, to make sure no structure generation accuracy is lost

int posToRegionx(int x1,int z1){
	if (x1!=0){
	int result=(int)((abs(x1)/x1)*(floor(abs(x1/512))));
	return result;
	} //Avoid dividing by zero
	return 0;
}
int posToRegionz(int x1, int z1){
	if (z1!=0){
		int result=(int)((abs(z1)/z1)*(floor(abs(z1/512))));
		return result;
	}
	return 0; //Avoid dividing by zero
}
int cBiomeAtPos(int biome, int64_t seed, int xpos, int zpos, int version)
{
    LayerStack g;
    setupGenerator(&g, version);
    Pos pos = {xpos,zpos}; // block position to be checked


        // Go through the layers in the layer stack and initialize the seed
        // dependent aspects of the generator.
    applySeed(&g, seed);

        // To get the biome at single block position we can use getBiomeAtPos().
    int biomeID = getBiomeAtPos(&g, pos);
    if (biomeID == biome)
        return 1;
	else return 0;

}
int cBiomesInArea(int* biomes, int64_t seed, int x1, int z1, int x2, int z2, int version)
{
    LayerStack g;
    BiomeFilter filter;

    setupGenerator(&g, version);

    // Define the required biomes.
    int* wanted=biomes;

    filter = setupBiomeFilter(wanted, sizeof(wanted) / sizeof(int));


    int x = x1, z = z1, w=x2-x1, h=z2-z1;
    int entry = L_VORONOI_ZOOM_1;
    int *area = allocCache(&g.layers[entry], w, h);

    if (checkForBiomes(&g, entry, area, seed, x, z, w, h, filter, 1) > 0){
		return 1;
	}
    free(area);
    return 0;
}

Pos cGetSpawn(int64_t seed, int version){
	LayerStack g;
    setupGenerator(&g, version);
    applySeed(&g, seed);
	Pos pos;
	pos=getSpawn(version, &g, NULL, seed);
	return pos;
}
long findStructure(int structType, int x1, int z1, int x2, int z2, int version)
{

    LayerStack g;
    setupGenerator(&g, version);

    int64_t lower48;
    for (lower48 = 0; ; lower48++)
    {
        // The structure position depends only on the region coordinates and
        // the lower 48-bits of the world seed.
        Pos p;
        if (!getStructurePos(structType, version, lower48, 0, 0, &p))
            continue;

        // Look for a seed with the structure at the origin chunk.
        if (p.x >= x2 || p.z >= z2 || p.x<=x1 ||p.z<=z1)
            continue;

        // Look for a full 64-bit seed with viable biomes.
        int64_t upper16;
        for (upper16 = 0; upper16 < 0x10000; upper16++)
        {
            int64_t seed = lower48 | (upper16 << 48);
            if (isViableStructurePos(structType, version, &g, seed, p.x, p.z))
                return seed;
            }
        }
    }


Pos bruh;
Pos structureLower48(int structType, long lower48, int x1, int z1, int x2, int z2, int version){

	LayerStack g;
    setupGenerator(&g, version);
	
	
	for(int regx=posToRegionx(x1,z1); regx<=posToRegionx(x2,z2); regx++){
		for(int regz=posToRegionz(x1,z1); regz<=posToRegionz(x2,z2); regz++){
			if (!getStructurePos(structType, version, lower48, regx, regz, &bruh)) break;

			if (bruh.x >= x2 || bruh.z >= z2 || bruh.x<=x1 ||bruh.z<=z1) break;
	
			return bruh;
			}
	}
bruh.x=30000000;
bruh.z=30000000;
return bruh;

}
long cIsViableStructurePos(int structType, int64_t seed, int structx, int structz, int version){
	LayerStack g;
    setupGenerator(&g, version);

	return (long)isViableStructurePos(structType, version, &g, seed, structx, structz);

		
	}

