# The pCT simulator in TOPAS
This project is an implementation of the pCT aparatus in [TOPAS](http://www.topasmc.org/) . As such, you will need a TOPAS license. In addition, you will also need either [MATLAB](https://www.mathworks.com/products/matlab.html) or [GNU Octave](https://www.gnu.org/software/octave/). The apparatus is very simplified. Instead of the calorimeter, it uses the energy values of the phase space outputs at the 4 T-V detectors to determine the energy loss of each proton as it passes through the phantom. For those who require a more realistic simulator, please use the stripified branch of [Geant4](https://github.com/pCT-collaboration/Geant4).
## Directories
- original_pCT This directory contains the original TOPAS simulator code supplied by Jose Ramos-Méndez and Pierluigi Piersimoni. It is included to preserve this code.
- pCTextensions This directory contains a TOPAS scorer extension for an earlier version of TOPAS. It was supplied by ? and is included here to preserve this code.
- pCT This directory contains the working simulator, calibration and preprocessing tools.
## Running the simulator
	Usage: ./run.sh
	    --phantom=[WaterPhantom|CTP404]
	    --dir=<output directory>
	    [--preparer=<name of preparer>]
	    [--width=<width of detector in mm>]
	    [--height=<height of detector in mm>]
	    [--pdensity=<number of protons / mm^2]
	    [--nproj=<number of projections>]
	    [--start=<start angle>]
	    [--incr=<angle increment>]
The run.sh script will generate projection data for the chosen phantom (currently only the water and CTP404 phantoms are available) and put the outputs under the named output directory. The other parameters are optional and have defaults. If preparer is not specified, it is extracted from the /etc/passwd file. width and height default to 349 mm and 86.2 mm, respectively. pdensity is the number of protons / mm^2.  The total number of protons per projection is the product of width * height * pdensity truncated to an integer. nproj is the number of projections to generate starting and the start angle and incrementing by incr. nproj defaults to 1, start defaults to 0, and incr defaults to 4.
The output of each projection is generated in &lt;output directory&gt;/&lt;angle&gt;. 16 pairs of phase space files are generated for each projection. These need to be converted into the format required by the reconstruction program by the preprocessing step described below.
An example run to generate 90 projections of the CTP404 phantom at 4 degree increments with 200 protons / mm^2 is:

	./run.sh --phantom=CTP404 --dir=/home/Reconstructions/topas --pdensity=200 --nproj=90
## Visualization
TDB
## Calibration
Each proton event in a phase space file contains the energy of the proton when is passed through on of the T-V detectors. Subtracting the energies between detectors 2 and 3 we get the energy lost as the proton passed through the phantom. We need to convert the energy lost to the water equivalent path length (WEPL). To do that, we do 5 calibration runs with a known phantom. The first run uses a wedge consisting of water. (Not very realistic, but Geant4 does not require that water has a container.)) Each successive run adds a water brick to the phantom. For each proton in the phase space outputs we have the T-U-V coordinates of the proton as it passes through the first and second detectors. Projecting that line to the phantom we get the coordinates where the proton entered the phantom. Similarly, projecting the line connecting T-U-V coordinates of the proton as it passes through the third and fourth detectors we get the coordinates of where the proton exits the phantom. The straight line distance (not MLP) between these two points is the water equivalent path length of the proton and the difference of the proton's energy between the second and third detectors is the energy loss. We record these values for a "large" number of protons, say 500 protons for each of the 5 calibration runs for a total of 2500 protons. We then do a least squares fit with a 7th degree poynomial. Figure 1 is an example of this output. Figure 2 is the same plot except the outliers have been removed.The output of calibratin is three files (the output of polyfit), P.mat, S.mat, and Mu.mat.  These are required for preprocessing.
![ Figure 1](Figure1.png  "Scatter plot of energy loss vs. WEPL") 
![ Figure 2](Figure2.png  "Figure 2. Scatter plot with outliers removed.") 
## Preprocessing
TBD
## Reconstruction
TBD