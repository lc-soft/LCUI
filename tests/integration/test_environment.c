#include <stdio.h>
#include <stdlib.h>
#include <LCUI.h>
#include <ctest-custom.h>

void test_environment(void)
{
        char *mono, *sans, *serif;
        const char *display = getenv("DISPLAY");
        const char *wayland = getenv("WAYLAND_DISPLAY");

        lcui_init();
        mono = pd_font_find_path("monospace");
        sans = pd_font_find_path("sans-serif");
        serif = pd_font_find_path("serif");

        printf("[env] version: %s\n", lcui_get_version());
        printf("[env] display: DISPLAY=%s WAYLAND_DISPLAY=%s\n",
               display ? display : "(unset)", wayland ? wayland : "(unset)");
        printf("[env] metrics: dpi=%.2f scale=%.2f density=%.2f "
               "scaled_density=%.2f\n",
               ui_metrics.dpi, ui_metrics.scale, ui_metrics.density,
               ui_metrics.scaled_density);
        printf("[env] screen: %dx%d\n", ptk_screen_get_width(),
               ptk_screen_get_height());
        printf("[env] font monospace: %s\n", mono ? mono : "(none)");
        printf("[env] font sans-serif: %s\n", sans ? sans : "(none)");
        printf("[env] font serif: %s\n", serif ? serif : "(none)");
        printf("[env] font ids: monospace=%d sans-serif=%d serif=%d\n",
               pd_font_get_id("monospace", PD_FONT_STYLE_NORMAL,
                              PD_FONT_WEIGHT_NORMAL),
               pd_font_get_id("sans-serif", PD_FONT_STYLE_NORMAL,
                              PD_FONT_WEIGHT_NORMAL),
               pd_font_get_id("serif", PD_FONT_STYLE_NORMAL,
                              PD_FONT_WEIGHT_NORMAL));

        free(mono);
        free(sans);
        free(serif);
        lcui_destroy();
}
