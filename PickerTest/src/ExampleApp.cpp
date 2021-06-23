#include "GLCore.h"
#include "PickerTest.h"
#include "PointCloud.h"

using namespace GLCore;

class Example : public Application
{
public:
	Example()
		: Application("OpenGL Examples")
	{
		// PushLayer(new PickerTest());
		PushLayer(new PointCloud(GetWindow()));
	}
};

int main()
{
	std::unique_ptr<Example> app = std::make_unique<Example>();
	app->Run();
}