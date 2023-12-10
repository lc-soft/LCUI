# UI 变更观察器

- 开始日期：2023-04-02
- 目标主要版本：3.x
- 参考问题：无
- 实现 PR：无

## 概括

UIMutationObserver 提供监听部件的属性、位置、尺寸、子树等变化的能力。

## 基本示例

```c
void on_widget_mutation(ui_mutation_list_t *mutation_list,
                        ui_mutation_observer_t *observer,
                        void *arg)
{

        list_node_t *node;
        ui_mutation_record_t *mutation;

        for (list_each(node, mutation_list)) {
                mutation = node->data;
                switch (mutation->type) {
                case UI_MUTATION_RECORD_TYPE_PROPERTIES:
                        printf("property: %ls\n", mutation->property_name);
                        break;
                default:
                        break;
                }
        }
}

void example(void)
{
        ui_widget_t *widget;
        ui_mutation_observer_t *observer;
        ui_mutation_observer_init_t options = { 0 };

        ...

        // 创建一个观察器
        observer = ui_mutation_observer_create(on_widget_mutation, NULL);

        ...

        // 监听属性变化
        options.properties = TRUE;
        // 监听子组件的变化
        options.child_list = TRUE;
        // 监听子树的变化
        options.subtree = TRUE;
        // 监听组件
        ui_mutation_observer_observe(observer, widget, options);

        ...

        // 销毁观察器
        ui_mutation_observer_destroy(observer);
}
```

## 动机

为了让部件与系统窗口能够同步数据，2.x 版本中的 UI 模块包含了对 Surface 的通信代码。然而，Surface 属于平台库的接口，而平台库与 UI 库在 3.x 版本中是需要解耦的。因此，需要一种全新且通用的机制来代替旧的通信方式。

## 详细设计

设计参考自：[MutationObserver - Web API 接口参考 | MDN](https://developer.mozilla.org/zh-CN/docs/Web/API/MutationObserver)

主要实现方式是，在部件结构体中添加观察器的连接列表，利用已有的样式和布局差异比较机制，在检测出差异时将变更记录添加到已连接该部件的各个观察器中。等部件树更新完毕后，再批量处理所有变更记录，也就是调用所有观察器的回调函数，将变更记录列表传给它们。

连接是双向的，当部件被销毁时会解除所有与之相连的观察器的连接，当观察器被销毁时它所观察的部件中的连接记录也会随之移除。

API 设计方面，添加以下类型：

- `ui_mutation_observer_t`：变更观察器。用于存储观察器的连接、变更记录、回掉函数等数据。为减少数据结构依赖，此类型的定义不在公共头文件公开。
- `ui_mutation_record_t`: 变更记录。
- `ui_mutation_record_type_t` 变更类型。
- `ui_mutation_connection_t` 连接记录，包含已建立连接的部件、观察器和配置。该类型仅供内部使用。

观察器的主要操作函数有：

- `ui_mutation_observer_observe()`: 设置观察对象和相关参数。
- `ui_mutation_observer_disconnect()`: 解除与观察对象的连接。
- `ui_process_mutation_observers()`: 处理所有观察器的变更记录，这时观察器中注册的回调函数会被调用。

`ui_mutation_record_t` 的具体定义如下：

```c
typedef struct ui_mutation_record_t {
        ui_mutation_record_type_t type;
        ui_widget_t *target;
        list_t added_widgets;
        list_t removed_widgets;
        char *attribute_name;
        char *property_name;
} ui_mutation_record_t;
```

`ui_mutation_record_type_t` 的具体定义如下：

```c
typedef enum ui_mutation_record_type_t {
        UI_MUTATION_RECORD_TYPE_NONE,
        UI_MUTATION_RECORD_TYPE_ATTRIBUTES,
        UI_MUTATION_RECORD_TYPE_PROPERTIES,
        UI_MUTATION_RECORD_TYPE_CHILD_LIST,
} ui_mutation_record_type_t;
```

`UI_MUTATION_RECORD_TYPE_ATTRIBUTES` 表示部件的 attributes 成员变更，考虑到实现成本和实际需求，暂不实现这类变更检测。

`UI_MUTATION_RECORD_TYPE_PROPERTIES` 表示部件自身属性的变更，例如：x、y、width、height。考虑到实现成本和实际需求，暂只实现 x、y、width、height 的变更检测，这些属性是比较常用的，而且 UIServer 也需要它们。

`UI_MUTATION_RECORD_TYPE_CHILD_LIST` 表示部件的子部件列表的变更，当处理到该类型的变更时，可访问 `ui_mutation_record_t` 对象中的 added_widgets 和 removed_widgets 成员来获取新增和删除的子部件。

## 缺点

`ui_mutation_observer_*` 系列操作函数的名称太长。

## 备选方案

无。

## 采用策略

这是个新功能，对现有代码无破坏性改动。
