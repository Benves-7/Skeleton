//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file (Benjamin Vesterlund)
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "MathLib.h"
int
main(int argc, const char** argv)
{
	Example::ExampleApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
