#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_STYLE_LIBRARY_H

char style_string[1024] = {
	".test_class { "
	"  width:   240; "
	"  height:   320; "
	"}"
};

int main()
{
	LCUI_StyleLibrary lib;
	LCUI_StyleClass *my_class;
	char buff[256];
	
	StyleLib_Init( &lib );
	StyleLib_AddStyleFromString( &lib, style_string );
	StyleLib_AddStyleClass( &lib, "test_class" );
	my_class = StyleLib_GetStyleClass( &lib, "test_class" );
	StyleClass_GetStyleAttrValue( my_class, NULL, "width", buff );
	printf( "width: %s\n", buff );
	StyleClass_GetStyleAttrValue( my_class, NULL, "height", buff );
	printf( "height: %s\n", buff );
	return 0;
}
