# ZeusOps SQM Utilities
A quick-and-dirty utility for checking, modifying and generating (parts of) ARMA3 sqm files.

## License
These utilities are governed by the GNU GPL v2.0 license, but includes works from different people, licensed under different terms. See [`LICENSE`](LICENSE) for more information.

## Requirements
 - [CMake](https://cmake.org/)
 - [Qt 5](https://www.qt.io/)

## What can this utility do?
 - It can read and write both binarized and non-binarized SQM files as well as PBOs.
 - Extracting a mission.sqm from a PBO to a non-binarized SQM: `zeusSqm input.pbo mission.sqm`
 - Extracting a mission.sqm from a PBO to a binarized SQM: `zeusSqm --saveBinarized input.pbo mission.sqm`
 - Converting a binarized SQM to a non-binarized SQM: `zeusSqm binarized.sqm non-binarized.sqm`
 - Converting a non-binarized SQM to a binarized SQM: `zeusSqm --saveBinarized non-binarized.sqm binarized.sqm`

## Commandline options
 - `--inplace`: Write output to input file. Use with care.
 - `--extractFromPbo <name>`: Extract the mission.sqm from PBO file and save it to name, untouched. This is not the option you are looking for, if you just want the file, this is meant for debugging differences between the original input file and the internally produced output.
 - `--stripComments`: Remove comments (lines starting in // and #define) from SQMs, even from the 'unchanged' extract from PBO.
 - `--saveBinarized`: Sets the output SQM file to be binarized. Default is non-binarized.
 - `--saveBinarizedIfBinarizedInput`: Sets the output SQM file to be binarized, if the input was binarized.

Say you want your NATO markers to always be on the center of the 100m grid, but doing this in the editor sucks. Well, you are in luck:
 - `-m` or `--markers`: Check marker placements for grid alignment.
 - `--markerMaxDistance`: Maximum distance of marker from grid center to be considered for correction (to leave deliberately placed markers where they are). Default: 25.
 - `--markerAskConfirmation`: Will ask for each marker whether it should be moved. Default: no.
 - `--markerGrid`: What grid the markers should snap to. Default: 100.
 
Did you forget to set the side relationships and OP is about to start?
 - `--setBluforVsIndependent <new relation>`: Overwrite side relations between BLUFOR and independent. New relation can be either `war` or `peace`.
 - `--setOpforVsIndependent <new relation>`: Overwrite side relations between OPFOR and independent. New relation can be either `war` or `peace`.

Did you ever want to translate a 2D picture into a structure in ARMA3?
![2D input for object generation](/tree.png?raw=true "Input picture for the object generation.")
![Generated structure in ARMA3](/treeInGame.jpg?raw=true "The generated structure in ARMA3.")
You have an nice picture like the one shown above and want a festive tree in your mission? No problem, `zeusSqm` got you covered:
 - `--buildFromImage <image>`: Build a 2D structure from an image, where a non-transparent pixel is translated to a block and to air otherwise.
 - `--buildMinAlpha <1 - 255>`: Minimal alpha channel value for a pixel not to be considered transparent.
 - `--buildStartingPoint x,y,z`: Coordinates of where to start building the structure, e.g. `1000,1000,10`.
 

## How to build
1. Clone the repository `https://github.com/blizzard4591/sqmTools.git` to a folder of your choice, we will call this folder `D:\sqmTools`.
	Be sure to include submodules: `git clone --recurse-submodules`!
2. (Optional) Fix the path to your Qt5 installation in `CMakeLists.txt` in the variable `CMAKE_PREFIX_PATH`.
3. Execute CMake on the sources, if possible, do an out-of-tree build. 
	On Window, in CMake, the source directory would be `D:\sqmTools` and `Where to build the binaries` would be `D:\sqmTools\build`. Then click `Configure` and `Generate`.
	On Linux, create a build dir (`mkdir build` in a location of your choice) and call CMake accordingly: `cmake ../path/to/the/root/of/sqmTools.git`.
4. On Windows, open the created project file `ZeusDebug.sln` in Visual Studio. Build the solution in either `Debug` or `Release` configuration, depending on your needs (if unsure, choose Release).
On Linux, execute `make`.
