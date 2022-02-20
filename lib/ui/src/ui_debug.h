void ui_widget_to_string(ui_widget_t *w, char *str);
void ui_widget_size_to_string(ui_widget_t *w, char str[32]);

#define UI_WIDGET_STR(W, STR) \
	char STR[256];              \
	ui_widget_to_string(W, STR);

#define UI_WIDGET_SIZE_STR(W, STR) \
	char STR[32];              \
	ui_widget_size_to_string(W, STR);

#ifdef UI_DEBUG_ENABLED
extern int ui_debug_msg_indent;

#define UI_DEBUG_MSG(FMT, ...)                                               \
	logger_log(LOGGER_LEVEL_DEBUG, "%*s" FMT "\n", ui_debug_msg_indent * 4, \
		   "", ##__VA_ARGS__)

#endif
