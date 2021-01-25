#include <iostream>

#include "prism/Prism.h"
#include "prism/Examples/BaseExample.h"

int main()
{
	Prism::Application app(980, 750, "Prism");
	
	app.CreateLayer<Prism::Examples::Base>("Base Example");
	
	app.Run();
	
	return 0;
}
