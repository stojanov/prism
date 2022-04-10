#include <iostream>

#include "prism/Prism.h"
#include "prism/Examples/VoxelExample.h"
#include "procgen-terrain/ProcGenTerrainVertex.h"

int main()
{
	Prism::Application app(1280, 720, "Prism");
	
	app.CreateLayer<ProcGenTerrainVertex>("Voxel Example");
	
	app.Run();

	return 0;
}
