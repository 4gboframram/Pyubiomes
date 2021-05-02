# Pyubiomes, a Simple (wip) Python Wrapper For [Cubiomes by Cubitect](https://github.com/Cubitect/cubiomes)
## Introduction
Pyubiomes is a (relatively) easy to use, easy to understand wrapper for the Cubiomes library. This project is still a wip, so please mention bugs/improvements. 

## Installation

**NOTE: RIGHT NOW THE AUTO BUILD/INSTALL IS ONLY FOR LINUX. I AM TRYING TO FIX THAT RIGHT NOW, BUT I CAN'T GET SHIT TO COMPILE ON WINDOWS**

You can install Pyubiomes with pip using the following
```pip install Pyubiomes```

### Manual Build/Installation:
___
#### Requirements:

 Cubiomes and `Python.h`
 


#### Linux: 

- Clone this reposititory and [Cubiomes](https://github.com/Cubitect/cubiomes)

- Move the contents of the Cubiomes master branch into the `Pyubiomes-master/Pyubiomes/searches` directory
- Open the terminal and change the current working directory to the searches folder you just extracted the cubiomes repo into with `cd` 
- Run the following command `python setup.py build`
- Move the `.so` file from the folder that starts with `lib.linux` 2 folders up (to the Pyubiomes folder). 
- Rename that `.so` file to `searches.so`
- Run `cd ../../` to change the current directory to the main branch's folder
- Run `pip install .` to build and install the module

*Hopefully* nothing went wrong

___
### Windows:

I couldn't do it because I couldn't get the compiler to recognize `Python.h` or any of the files in the cubiomes library, but it is probably as follows.
 - Do the first 2 steps of the Linux  Installation

 - Instead of a `.so` file, you need to use setup.py to compile to a `.dll`
 - Perform the same actions to the `.dll` file as you would to the `.so` file
 - Modify line 4 of `pyubiomes-main/Pyubiomes/search.py` and replace `searches.so` with `searches.dll`
 - Modify line 4 of `pyubiomes-main/setup.py` and replace `searches.so` with `searches.dll`

___
### Installation Errors:

 If you get errors due to the absence of `Python.h`, here's how you might be able to fix them:


 **Linux:** 

Try to locate `Python.h` with `locate Python.h` If you can find the file located, most likely [this stackoverflow answer](stackoverflow.com/a/19344978/4954434) will work. It is likely a path issue



However, if you can't locate the file, use your package manager to install the header files and static libraries for python dev (courtesy of [this stackoverflow question](https://stackoverflowcom/questions/21530577/fatal-error-python-h-no-such-file-or-directory))

With `apt` (**Ubuntu, Debian...**)
:
`sudo apt-get install python3-dev`

For `yum` (**CentOS, RHEL...**)
:
`sudo yum install python3-devel`

For `dnf` (**Fedora...**)
:
`sudo dnf install python3-devel`

For `zypper` (**openSUSE...**)
:
`sudo zypper in python3-devel`

For `apk` (**Alpine...**)
:
`sudo apk add python3-dev `

For `apt-cyg` (**Cygwin...**)
:
`apt-cyg install python3-devel`

**Windows:**

Honestly if I knew the answer for Windows, I would've had an auto install/build for it by now. I belive `Python.h` comes with the standard Python installation, so the problem is with path variables or your compiler not checking the proper include paths. I've tried everything I could on Windows, but I couldn't get anything to work. Kind of ironic considering I use it more than Linux and know like 10 times more 

## Example Code
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
			seed+=increment
				

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
