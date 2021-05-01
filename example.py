###############
#simple example program with mutithreading
#that can find biomes and structures
import Pyubiomes

from Pyubiomes import Versions, Biomes, Structures

import concurrent.futures
from Pyubiomes import structureLower48, biomesInArea, isValidStructurePos, getSpawn
wanted=[Biomes.ice_spikes, Biomes.ice_spikes] #program does not like it if there's only 1 argument
structure=Structures.Outpost

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
				

def start(n):
		with concurrent.futures.ThreadPoolExecutor(max_workers=n) as executor:
			jobs = {executor.submit(finder, *(i,n)) for i in range(n)}
		for fut in concurrent.futures.as_completed(jobs):
			print(fut.result())

start(4)




