#include <LCUI.h>
#include <ctest-custom.h>

static void should_create_button_with_default_state(void)
{
        ui_widget_t *button = ui_create_button();

        ctest_equal_bool("factory should create a button widget",
                         ui_check_widget_type(button, "button"), true);
        ctest_equal_int("variant should default to outline",
                        ui_button_get_variant(button),
                        UI_BUTTON_VARIANT_OUTLINE);
        ctest_equal_int("size should default to default",
                        ui_button_get_size(button), UI_BUTTON_SIZE_DEFAULT);
        ui_widget_destroy(button);
}

static void should_switch_variant_status(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_OUTLINE);
        ctest_equal_bool("outline variant should add outline status",
                         ui_widget_has_status(button, "outline"), true);
        ui_button_set_variant(button, UI_BUTTON_VARIANT_PRIMARY);
        ctest_equal_int("variant getter should return primary",
                        ui_button_get_variant(button),
                        UI_BUTTON_VARIANT_PRIMARY);
        ctest_equal_bool("switching variant should remove outline status",
                         ui_widget_has_status(button, "outline"), false);
        ctest_equal_bool("switching variant should add primary status",
                         ui_widget_has_status(button, "primary"), true);
        ui_widget_destroy(button);
}

static void should_keep_size_independent_from_variant(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ctest_equal_int("icon size should be readable",
                        ui_button_get_size(button), UI_BUTTON_SIZE_ICON);
        ctest_equal_bool("icon size should add icon status",
                         ui_widget_has_status(button, "icon"), true);
        ctest_equal_int("icon size should preserve ghost variant",
                        ui_button_get_variant(button), UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_DEFAULT);
        ctest_equal_bool("default size should remove icon status",
                         ui_widget_has_status(button, "icon"), false);
        ctest_equal_bool("size changes should preserve ghost status",
                         ui_widget_has_status(button, "ghost"), true);
        ui_widget_destroy(button);
}

static void should_fall_back_for_invalid_enum_values(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_variant(button, (ui_button_variant_t)99);
        ctest_equal_int("invalid variant should fall back to outline",
                        ui_button_get_variant(button),
                        UI_BUTTON_VARIANT_OUTLINE);
        ui_button_set_size(button, (ui_button_size_t)99);
        ctest_equal_int("invalid size should fall back to default",
                        ui_button_get_size(button), UI_BUTTON_SIZE_DEFAULT);
        ui_widget_destroy(button);
}

static void should_set_size_presets(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ctest_equal_int("size getter should return xs",
                        ui_button_get_size(button), UI_BUTTON_SIZE_XS);
        ctest_equal_bool("xs size should add xs status",
                         ui_widget_has_status(button, "xs"), true);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_LG);
        ctest_equal_bool("previous size status should be removed",
                         ui_widget_has_status(button, "xs"), false);
        ctest_equal_int("size getter should return icon-lg",
                        ui_button_get_size(button), UI_BUTTON_SIZE_ICON_LG);
        ctest_equal_bool("icon-lg size should add icon-lg status",
                         ui_widget_has_status(button, "icon-lg"), true);
        ui_button_set_size(button, UI_BUTTON_SIZE_DEFAULT);
        ctest_equal_bool("default size should remove icon-lg status",
                         ui_widget_has_status(button, "icon-lg"), false);
        ui_widget_destroy(button);
}

static void should_sync_variant_and_size_attributes(void)
{
        ui_widget_t *button = ui_create_button();

        ui_widget_set_attr(button, "variant", "link");
        ui_widget_set_attr(button, "size", "icon");
        ctest_equal_int("variant attribute should select link",
                        ui_button_get_variant(button), UI_BUTTON_VARIANT_LINK);
        ctest_equal_int("size attribute should select icon",
                        ui_button_get_size(button), UI_BUTTON_SIZE_ICON);
        ctest_equal_bool("link attribute should add link status",
                         ui_widget_has_status(button, "link"), true);

        ui_widget_set_attr(button, "variant", "unknown");
        ui_widget_set_attr(button, "size", "unknown");
        ctest_equal_int("unknown variant should read as outline",
                        ui_button_get_variant(button),
                        UI_BUTTON_VARIANT_OUTLINE);
        ctest_equal_int("unknown size should read as default",
                        ui_button_get_size(button), UI_BUTTON_SIZE_DEFAULT);
        ctest_equal_bool("unknown variant should remove link status",
                         ui_widget_has_status(button, "link"), false);
        ctest_equal_bool("unknown size should remove icon status",
                         ui_widget_has_status(button, "icon"), false);
        ui_widget_destroy(button);
}

static void should_retain_text_widget_behavior(void)
{
        ui_widget_t *button = ui_create_button();

        ui_button_set_text(button, "Continue");
        ctest_equal_bool("button should inherit text widget behavior",
                         ui_check_widget_type(button, "text"), true);
        ui_widget_destroy(button);
}

static void should_forward_text_attributes_to_the_text_prototype(void)
{
        ui_widget_t *button = ui_create_button();
        float width_before;

        ui_widget_append(ui_root(), button);
        ui_button_set_text(button, "  Save  ");
        ui_update();
        width_before = button->border_box.width;
        ui_widget_set_attr(button, "trimming", "false");
        ui_update();
        ctest_equal_bool("trimming false should keep surrounding whitespace",
                         button->border_box.width > width_before, true);
        ui_widget_set_attr(button, "trimming", "true");
        ui_update();
        ctest_equal_float("trimming true should restore the trimmed width",
                          button->border_box.width, width_before);
        ui_widget_destroy(button);
}

static void should_underline_link_variant_only_on_hover(void)
{
        ui_widget_t *button = ui_create_button();

        ui_widget_append(ui_root(), button);
        ui_button_set_variant(button, UI_BUTTON_VARIANT_LINK);
        ui_update();
        ctest_equal_int("link should not be underlined normally",
                        button->computed_style.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_NONE);
        ui_widget_add_status(button, "hover");
        ui_update();
        ctest_equal_int("link hover should be underlined",
                        button->computed_style.type_bits.text_decoration,
                        CSS_TEXT_DECORATION_UNDERLINE);
        ui_widget_destroy(button);
}

static void should_size_icons_inside_button_presets(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();
        float icon_xs;
        float icon_sm;
        float icon_default;

        ui_icon_set_name(icon, "arrow-up-right");
        ui_widget_append(button, icon);
        ui_widget_append(ui_root(), button);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_XS);
        ui_update();
        icon_xs = icon->computed_style.font_size;
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_SM);
        ui_update();
        icon_sm = icon->computed_style.font_size;
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_update();
        icon_default = icon->computed_style.font_size;
        ctest_equal_bool("icon sizes should grow with the icon presets",
                         icon_xs < icon_sm && icon_sm < icon_default, true);
        ui_widget_destroy(button);
}

static void should_apply_icon_size_independently_of_variant(void)
{
        ui_widget_t *button = ui_create_button();

        ui_widget_append(ui_root(), button);
        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_ICON);
        ui_update();
        ctest_equal_float("icon size should make the button square",
                          button->border_box.width, button->border_box.height);
        ctest_equal_int("icon size should preserve ghost variant",
                        ui_button_get_variant(button), UI_BUTTON_VARIANT_GHOST);
        ui_widget_destroy(button);
}

static void should_scale_text_and_icon_size_presets(void)
{
        ui_widget_t *button = ui_create_button();
        float height_xs, height_default, height_lg;

        ui_widget_append(ui_root(), button);
        ui_button_set_text(button, "Size");
        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ui_update();
        height_xs = button->border_box.height;

        ui_button_set_size(button, UI_BUTTON_SIZE_DEFAULT);
        ui_update();
        height_default = button->border_box.height;

        ui_button_set_size(button, UI_BUTTON_SIZE_LG);
        ui_update();
        height_lg = button->border_box.height;
        ctest_equal_bool(
            "sizes should grow from xs to default to lg",
            height_xs < height_default && height_default < height_lg, true);

        ui_button_set_size(button, UI_BUTTON_SIZE_ICON_SM);
        ui_update();
        ctest_equal_float("icon-sm should make the button square",
                          button->border_box.width, button->border_box.height);
        ui_widget_destroy(button);
}

static void should_keep_disabled_style_while_hovered(void)
{
        ui_widget_t *button = ui_create_button();
        float opacity;

        ui_widget_append(ui_root(), button);
        ui_widget_set_disabled(button, true);
        ui_update();
        opacity = button->computed_style.opacity;

        ui_widget_add_status(button, "hover");
        ui_update();
        ctest_equal_float("disabled hover should preserve the disabled opacity",
                          button->computed_style.opacity, opacity);
        ui_widget_destroy(button);
}

static void should_size_text_children_inside_buttons(void)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_icon();
        ui_widget_t *label = ui_create_widget("text");

        ui_icon_set_name(icon, "checkmark");
        ui_text_set_content(label, "New Branch");
        ui_widget_append(button, icon);
        ui_widget_append(button, label);
        ui_widget_append(ui_root(), button);
        ui_update();
        ctest_equal_float("button text should inherit the default font size",
                          label->computed_style.font_size,
                          button->computed_style.font_size);
        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ui_update();
        ctest_equal_float("xs button text should inherit its font size",
                          label->computed_style.font_size,
                          button->computed_style.font_size);
        ui_button_set_size(button, UI_BUTTON_SIZE_LG);
        ui_update();
        ctest_equal_float("lg button text should inherit its font size",
                          label->computed_style.font_size,
                          button->computed_style.font_size);
        ui_widget_destroy(button);
}

static void should_match_heights_of_text_and_icon_buttons(void)
{
        ui_widget_t *text_btn = ui_create_button();
        ui_widget_t *icon_btn = ui_create_button();
        ui_widget_t *icon = ui_create_icon();

        ui_icon_set_name(icon, "arrow-up-right");
        ui_widget_append(icon_btn, icon);
        ui_widget_append(ui_root(), text_btn);
        ui_widget_append(ui_root(), icon_btn);
        ui_button_set_text(text_btn, "Size");

        ui_button_set_size(text_btn, UI_BUTTON_SIZE_XS);
        ui_button_set_size(icon_btn, UI_BUTTON_SIZE_ICON_XS);
        ui_update();
        ctest_equal_float("xs and icon-xs buttons should have the same height",
                          text_btn->border_box.height,
                          icon_btn->border_box.height);

        ui_button_set_size(text_btn, UI_BUTTON_SIZE_SM);
        ui_button_set_size(icon_btn, UI_BUTTON_SIZE_ICON_SM);
        ui_update();
        ctest_equal_float("sm and icon-sm buttons should have the same height",
                          text_btn->border_box.height,
                          icon_btn->border_box.height);

        ui_button_set_size(text_btn, UI_BUTTON_SIZE_DEFAULT);
        ui_button_set_size(icon_btn, UI_BUTTON_SIZE_ICON);
        ui_update();
        ctest_equal_float(
            "default and icon buttons should have the same height",
            text_btn->border_box.height, icon_btn->border_box.height);

        ui_button_set_size(text_btn, UI_BUTTON_SIZE_LG);
        ui_button_set_size(icon_btn, UI_BUTTON_SIZE_ICON_LG);
        ui_update();
        ctest_equal_float("lg and icon-lg buttons should have the same height",
                          text_btn->border_box.height,
                          icon_btn->border_box.height);
        ui_widget_destroy(text_btn);
        ui_widget_destroy(icon_btn);
}

static void should_fill_button_content_box_with_its_text_line(void)
{
        ui_widget_t *button = ui_create_button();

        ui_widget_append(ui_root(), button);
        ui_button_set_text(button, "Label");
        ui_button_set_size(button, UI_BUTTON_SIZE_XS);
        ui_update();
        ctest_equal_float("xs button content height should fit its text line",
                          button->content_box.height,
                          button->computed_style.line_height);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_update();
        ctest_equal_float("sm button content height should fit its text line",
                          button->content_box.height,
                          button->computed_style.line_height);
        ui_button_set_size(button, UI_BUTTON_SIZE_DEFAULT);
        ui_update();
        ctest_equal_float(
            "default button content height should fit its text line",
            button->content_box.height, button->computed_style.line_height);
        ui_button_set_size(button, UI_BUTTON_SIZE_LG);
        ui_update();
        ctest_equal_float("lg button content height should fit its text line",
                          button->content_box.height,
                          button->computed_style.line_height);
        ui_widget_destroy(button);
}

static void should_not_respond_to_mouse_when_disabled(void)
{
        ui_widget_t *button = ui_create_button();
        ui_event_t e = { 0 };

        ui_button_set_text(button, "Button");
        ui_widget_set_disabled(button, true);
        ui_widget_append(ui_root(), button);
        ui_widget_resize(ui_root(), 200, 200);
        ui_update();

        e.type = UI_EVENT_MOUSEDOWN;
        e.mouse.button = MOUSE_BUTTON_LEFT;
        ui_dispatch_event(&e);
        ui_update();
        ctest_equal_float("disabled button should not shift on press",
                          button->computed_style.top, 0.f);
        ui_widget_destroy(button);
}

void test_button(void)
{
        lcui_init();
        ctest_describe("button factory",
                       should_create_button_with_default_state);
        ctest_describe("button variant state", should_switch_variant_status);
        ctest_describe("button size state",
                       should_keep_size_independent_from_variant);
        ctest_describe("button invalid state",
                       should_fall_back_for_invalid_enum_values);
        ctest_describe("button size presets", should_set_size_presets);
        ctest_describe("button attributes",
                       should_sync_variant_and_size_attributes);
        ctest_describe("button text behavior",
                       should_retain_text_widget_behavior);
        ctest_describe("button text attribute inheritance",
                       should_forward_text_attributes_to_the_text_prototype);
        ctest_describe("button link hover style",
                       should_underline_link_variant_only_on_hover);
        ctest_describe("button icon preset style",
                       should_size_icons_inside_button_presets);
        ctest_describe("button text child style",
                       should_size_text_children_inside_buttons);
        ctest_describe("button height parity",
                       should_match_heights_of_text_and_icon_buttons);
        ctest_describe("button text vertical fit",
                       should_fill_button_content_box_with_its_text_line);
        ctest_describe("button icon size style",
                       should_apply_icon_size_independently_of_variant);
        ctest_describe("button size scale style",
                       should_scale_text_and_icon_size_presets);
        ctest_describe("button disabled hover style",
                       should_keep_disabled_style_while_hovered);
        ctest_describe("button disabled pointer events",
                       should_not_respond_to_mouse_when_disabled);
        lcui_destroy();
}
