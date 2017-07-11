#pragma once
namespace LCUI {

class LCUI_API Application {
public:
	Application();
	virtual void Load( Platform::String^ entryPoint );
	virtual void Run();
};

LCUI_API void Initialize();
LCUI_API void Run( Application &app );

};
