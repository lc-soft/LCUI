
#define assert(X) if(!(X)) {_DEBUG_MSG("nopass\n");return -1;}

int test_string( void );
int test_css_parser( void );
int test_char_render( void );
int test_string_render( void );
int test_widget_render( void );
