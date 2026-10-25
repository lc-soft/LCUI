#include <LCUI.h>

static const char *collapsible_file_tree_css = "\
.collapsible-file-tree-demo {\
  width: 320px;\
}\
.collapsible-file-tree-demo card {\
  width: 100%;\
}\
.collapsible-file-tree-demo .file-tree {\
  display: flex;\
  flex-direction: column;\
  gap: 2px;\
  padding-top: 8px;\
}\
.collapsible-file-tree-demo .file-row {\
  width: 100%;\
  justify-content: flex-start;\
  border: 0;\
  background: transparent;\
}\
.collapsible-file-tree-demo .file-children {\
  display: flex;\
  flex-direction: column;\
  gap: 2px;\
  padding-left: 20px;\
}\
";

static ui_widget_t *create_file_row(const char *name)
{
        ui_widget_t *button = ui_create_button();
        ui_widget_t *icon = ui_create_fluent_icon("document-text");
        ui_widget_t *label = ui_create_widget("text");

        ui_button_set_variant(button, UI_BUTTON_VARIANT_LINK);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_widget_add_class(button, "file-row");
        ui_text_set_content(label, name);
        ui_widget_append(button, icon);
        ui_widget_append(button, label);
        return button;
}

static ui_widget_t *create_folder_row(const char *name, bool open,
                                      ui_widget_t **children)
{
        ui_widget_t *collapsible = ui_create_collapsible();
        ui_widget_t *trigger = ui_create_collapsible_trigger();
        ui_widget_t *button = ui_create_button();
        ui_widget_t *chevron = ui_create_fluent_icon("chevron-right");
        ui_widget_t *folder = ui_create_fluent_icon("folder-open");
        ui_widget_t *label = ui_create_widget("text");
        ui_widget_t *content = ui_create_collapsible_content();

        ui_button_set_variant(button, UI_BUTTON_VARIANT_GHOST);
        ui_button_set_size(button, UI_BUTTON_SIZE_SM);
        ui_widget_add_class(button, "file-row");
        ui_text_set_content(label, name);
        ui_widget_append(button, chevron);
        ui_widget_append(button, folder);
        ui_widget_append(button, label);
        ui_widget_append(trigger, button);
        ui_widget_add_class(content, "file-children");
        ui_widget_append(collapsible, trigger);
        ui_widget_append(collapsible, content);
        if (open) {
                ui_collapsible_set_open(collapsible, true);
        }
        *children = content;
        return collapsible;
}

void collapsible_file_tree_init(ui_widget_t *parent)
{
        ui_widget_t *box = ui_create_widget(NULL);
        ui_widget_t *card = ui_create_card();
        ui_widget_t *header = ui_create_card_header();
        ui_widget_t *title = ui_create_card_title();
        ui_widget_t *content = ui_create_card_content();
        ui_widget_t *tree = ui_create_widget(NULL);
        ui_widget_t *components;
        ui_widget_t *components_children;
        ui_widget_t *ui_folder;
        ui_widget_t *ui_children;
        ui_widget_t *lib;
        ui_widget_t *lib_children;

        ui_load_css_string(collapsible_file_tree_css,
                           "collapsible-file-tree-demo");
        ui_widget_add_class(box, "collapsible-file-tree-demo");
        ui_card_set_size(card, UI_CARD_SIZE_SM);
        ui_text_set_content(title, "Project files");
        ui_widget_add_class(tree, "file-tree");

        components =
            create_folder_row("components", true, &components_children);
        ui_folder = create_folder_row("ui", true, &ui_children);
        ui_widget_append(ui_children, create_file_row("button.tsx"));
        ui_widget_append(ui_children, create_file_row("card.tsx"));
        ui_widget_append(ui_children, create_file_row("dialog.tsx"));
        ui_widget_append(components_children, ui_folder);
        ui_widget_append(components_children,
                         create_file_row("login-form.tsx"));
        ui_widget_append(components_children,
                         create_file_row("register-form.tsx"));
        ui_widget_append(tree, components);

        lib = create_folder_row("lib", false, &lib_children);
        ui_widget_append(lib_children, create_file_row("utils.ts"));
        ui_widget_append(lib_children, create_file_row("api.ts"));
        ui_widget_append(tree, lib);

        ui_widget_append(tree, create_file_row("app.tsx"));
        ui_widget_append(tree, create_file_row("package.json"));
        ui_widget_append(tree, create_file_row("README.md"));

        ui_widget_append(header, title);
        ui_widget_append(content, tree);
        ui_widget_append(card, header);
        ui_widget_append(card, content);
        ui_widget_append(box, card);
        ui_widget_append(parent, box);
}
