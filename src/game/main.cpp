#include <iostream>

#include "prism/Prism.h"
#include "prism/Examples/VoxelExample.h"
#include "prism/Entity/Registry.h"

int main()
{
	Prism::Application app(1280, 720, "Prism");
	
	app.CreateLayer<Prism::Examples::VoxelExample>("Voxel Example");
	
	app.Run();

	return 0;
}
