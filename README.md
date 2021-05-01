# Pyubiomes, a Simple (wip) Python Wrapper For [Cubiomes by Cubitect](https://github.com/Cubitect/cubiomes)
## Introduction
Pyubiomes is a (relatively) easy to use, easy to understand wrapper for the Cubiomes library. This project is still a wip, so please mention bugs/improvements. 

## Installation


You can install Pyubiomes with pip using the following
```pip install Pyubiomes```

Manual Installation:
(wip) Will require cloning [Cubiomes](https://github.com/Cubitect/cubiomes)

## Example
This example is here to give ideas on how to use these functions together to make a seedfinder, as for beginners, it may not be obvious to them at first. 
```python
###############
#simple example program with mutithreading
#that can find biomes and structures
###############

import Pyubiomes
from Pyubiomes import Versions, Biomes, Structures, structureLower48, biomesInArea, isValidStructurePos, getSpawn

import concurrent.futures

wanted=[Biomes.dark_forest, Biomes.savanna] #There's a bug where if you only pass a list with 1 element, the program crashes.
structure=Structures.Mansion

seeds=[]

def finder(start, increment):
	seed=start
	while True:

			print(seed)

			p=structureLower48(structure, seed,-128, -128, 128, 128, Versions.MC_1_16) #check region for structure
			q=biomesInArea(wanted, seed, -64, -64, 64, 64, Versions.MC_1_16) #check region for list of biomes

			if p and q: #compare the 2
				r=isValidStructurePos (structure, seed, p[0], p[1], Versions.MC_1_16) #if there is a structure
				if r:
					spawn=getSpawn(seed, Versions.MC_1_16)
					print(f"found seed that met requirements: {seed} with structure at {p}, and a world spawn of {spawn}")
					global seeds
					seeds.append(seed)
				if len(seeds)>=1: #make sure all threads break out of the loop
					break
			seed+=1
				

def start(n): #ThreadPool
		with concurrent.futures.ThreadPoolExecutor(max_workers=n) as executor:
			jobs = {executor.submit(finder, *(i,n)) for i in range(n)}
		for fut in concurrent.futures.as_completed(jobs):
			print(fut.result())

start(4) #initialize

```
## Documentation:

### Biome-Searching Functions:

```python
biomeAtPos(biome: int, seed: long, xpos: int, zpos: int, version: int)
```
Returns ```True``` if there is a ```[biome]``` at ```(xpos, zpos)``` on the seed ```[seed]``` in minecraft version ```[version]```. Otherwise returns ```False```. 


```python
biomesInArea(biomes: list, seed: long, x1: int, z1: int, x2: int, z2: int, version: int)
```
Returns `True` if all of the given biomes, `[biomes]` are in the rectangular area between corners `(x1,z1)` and `(x2,z2)`. Otherwise returns `False`
**Note:** The x and z values of the first point must be strictly less than the corresponding values of the second point.

### Structure Finding:
```python 
structureLower48(structType: int, lower48: long, x1: int, z1: int, x2: int, z2: int, version: int)
```
Returns Structure Position as a tuple if the (lower 48 bits of) the seed, `[lower48]` has a structure of `[structureType]` in the rectangular region between points `(x1,z1)`, `(x2,z2)`. Otherwise returns `None`". **Note:** The x and z values of the first point must be strictly less than the corresponding values of the second point.


```python 
isValidStructurePos(structType: int, seed: long, structx: int, structz: int, version: int)
```
Returns `True` if the given structure type, `[structType]` has a valid spawning at the location `(structx, structz)`. Otherwise returns `False`.

## Get World Spawnpoint

```python
getSpawn(seed: long, version: int)
```
Returns a tuple containing the x and z coordinates of the worldspawn for `[seed]` in minecraft version `[version]`

## Constants
You've probably noticed that all of these functions ask for the biome, structure, etc. as an integer.

You do not need to memorize IDs to pass in as parameters, as there are keywords in a few classes.

To pass in a value for a biome into a function, you can pass in `Biomes.namespaced_id`, replacing "namespaced_id" with the namespaced id for the biome.  **Note:** for editions that use legacy names, you have to use the newer name of the biome


The same thing applies for structures, except the class you want to access is the `Structures` class, so it would be `Structures.namespaced_id`.

For the version, you can pass in `Versions.MC_1_xx`, or you can just pass in the final 2 digits of the version number
