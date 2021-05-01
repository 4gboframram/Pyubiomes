// check the biome at a block position

#include "searches.c"
#include <stdio.h>
#include <Python.h>

static PyObject* pyBiomeAtPos(PyObject* self, PyObject* args)
{
	int biome, xpos, zpos, version;
	long seed; 
    
    if(!PyArg_ParseTuple(args, "iliii", &biome, &seed, &xpos, &zpos, &version))
        return NULL;

    return Py_BuildValue("i", cBiomeAtPos(biome, seed, xpos, zpos, version));
}

static PyObject *pyBiomesInArea(PyObject *self, PyObject *args)
{
    PyObject* biomes;
    int biomes_array_length;
	int64_t seed; 
	int x1, z1, x2, z2, version;

	if (!PyArg_ParseTuple(args, "Oliiiii", &biomes, &seed, &x1, &z1, &x2, &z2, &version)) return NULL;

	biomes_array_length = (int)PyObject_Length(biomes);
	int biomes_array[biomes_array_length];
    
    if (biomes_array_length < 0)
        return NULL;
		
    if (biomes_array == NULL)
        return NULL;
    for (int index = 0; index < biomes_array_length; index++) {
        PyObject *item;
        item = PyList_GetItem(biomes, index);
        if (!PyLong_Check(item)){biomes_array[index] = 0;}

        biomes_array[index] = (int)PyLong_AsLong(item);
    }
    return Py_BuildValue("O", PyBool_FromLong((long)cBiomesInArea(biomes_array, seed, x1, z1, x2, z2, version)));
}

static PyObject* pyGetSpawn(PyObject* self, PyObject* args){
	int version;
	int64_t seed;

	if (!PyArg_ParseTuple(args, "li", &seed, &version)) return NULL;
	
	Pos p=cGetSpawn(seed, version);
	return Py_BuildValue("(ii)", p.x, p.z );
}

static PyObject* pyFindStructure(PyObject* self, PyObject* args)
{
	int structType, x1, z1, x2, z2, version;

    if(!PyArg_ParseTuple(args, "iiiiii", &structType, &x1, &z1, &x2, &z2, &version)) return NULL;


    return Py_BuildValue("l",findStructure(structType, x1, z1, x2, z2, version));
}

static PyObject* pyStructureLower48(PyObject* self, PyObject* args)
{
	int structType, x1, z1, x2, z2, version;
	int64_t lower48;
    if(!PyArg_ParseTuple(args, "iliiiii", &structType, &lower48, &x1, &z1, &x2, &z2, &version)) return NULL;

	
	Pos q=structureLower48(structType, lower48, x1, z1, x2, z2, version); 

	if(q.x==30000000) return Py_BuildValue("");
    return Py_BuildValue("(ii)", q.x, q.z);
}
static PyObject* pyIsValidStructurePos(PyObject* self, PyObject* args){
	int structType, structx, structz, version;
	int64_t seed; 
	if(!PyArg_ParseTuple(args, "iliii", &structType, &seed, &structx, &structz, &version)) return NULL;
	
	return Py_BuildValue("O", PyBool_FromLong(cIsViableStructurePos(structType, seed, structx, structz, version)));
}

//meth>methods 
static PyMethodDef methods[] = {
    { "biomeAtPos", pyBiomeAtPos, METH_VARARGS,"returns True if there is a [biome] at (xpos,zpos) on the [seed] in minecraft version [version]. Otherwise returns False"},
	{"biomesInArea", pyBiomesInArea, METH_VARARGS, "Returns True if all of the [biomes] are in the rectangular area of width [w] and height [h] with the most negative corner being at (x1,z1). Otherwise returns False"},
	{"getSpawn", pyGetSpawn, METH_VARARGS, "Returns a tuple containing the worldspawn for [seed] in minecraft version [version]"},
	{"findStructure", pyFindStructure, METH_VARARGS, "Returns tuple containing the structure's position if a structure of [structType] is in the rectangular area of width [w] and height [h] with the most negative corner being at (x1,z1) in the structure seed [lower48]. Otherwise returns None"},
	{"structureLower48", pyStructureLower48, METH_VARARGS,"Returns Structure Position as a tuple if the lower 48 bits of the seed has the structure in the given region. Otherwise returns None"},
	{"isValidStructurePos", pyIsValidStructurePos, METH_VARARGS,"Returns True if the given structure type could spawn at that location. Otherwise returns False"},
    { NULL, NULL, 0, NULL }
};



// Module Definition Shit
static struct PyModuleDef searches = {
    PyModuleDef_HEAD_INIT,
    "searches",
    "Search functions for Pyubiomes, a Python wrapper for cubiomes. ",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_searches(void)
{
    return PyModule_Create(&searches);
}


