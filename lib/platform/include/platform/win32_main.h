#ifdef LIBPLATFORM_INCLUDE_MAIN_H
#define LIBPLATFORM_INCLUDE_MAIN_H

#include <errno.h>
#include <Windows.h>
#include <platform.h>
#include <yutil.h>

extern int main(int argc, char *argv[]);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
		     _In_opt_ HINSTANCE hPrevInstance,
		     _In_ LPSTR lpCmdLine,
		     _In_ int nCmdShow)
{
	char *cmdline;
	int ret, argc = 0;
	char **argv = NULL;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	app_set_instance(hInstance);
	cmdline = GetCommandLineA();
	argc = cmdsplit(cmdline, &argv);
	ret = main(argc, (char**)argv);
	while (argc-- > 0) {
		free(argv[argc]);
	}
	free(argv);
	return ret;
}

#endif
