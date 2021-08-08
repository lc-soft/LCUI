# liblayout

布局引擎。

## 参考对象

暂无参考对象。

## 设计

```c

typedef struct layout_node_t layout_node_t;
typedef struct layout_box_t layout_box_t;
typedef struct flow_params_t flow_params_t;
typedef struct flexbox_params_t flexbox_params_t;
typedef struct layout_length_t layout_length_t;
typedef enum layout_unit_t layout_unit_t;
typedef float layout_number_t;

enum layout_unit_t {
    LAYOUT_UNIT_NONE,
    LAYOUT_UNIT_PX,
    LAYOUT_UNIT_PT
};

struct layout_length_t {
    layout_number_t value;
    layout_unit_t unit;
};

typedef enum layout_mode_t {
    LAYOUT_MODE_FLOW,
    LAYOUT_MODE_FLEXBOX,
} layout_mode_t;

struct layout_box_t {
    float top;
    float left;
    float width;
    float height;
};

struct flow_params_t {

};

struct flexbox_params_t {

};

struct layout_node_t {
    layout_mode_t mode;
    layout_box_t box;
    union {
        flow_params_t flow;
        flexbox_params_t flexbox;
    };
    layout_node_t *prev;
    layout_node_t *next;
};

void flow_reflow(layout_node_t *node);
void flexbox_reflow(layout_node_t *node);
```

## 问题

**如何将布局对象与 UI 元素绑定？**

有两种方案：

1. **将布局对象嵌入到 UI 元素里。** 和 LCUI 现在的做法一样，`LCUI_Widget` 结构体同时定义了界面元素和布局对象，界面元素即是布局对象，它们的数据和逻辑代码高度耦合。

1. **单独维护一个布局树。** 参考浏览器的工作模式，用 DOM 树记录元素的结构，在渲染前根据 DOM 树构建成渲染树，交由布局引擎计算。

采用方案 2 需要考虑以下问题：

- **渲染树如何与 DOM 树同步？** 如果持续维护渲染树的话，每当 DOM 树因增删改操作而变化时都要操作渲染树，这两块代码容易耦合在一起，而且要暴露一些接口供 DOM 树的各种操作代码调用，增加了使用成本。如果只在布局时临时创建的话，需要考虑这些问题：渲染树的创建性能如何？创建包含 10 万个布局对象的树耗时多长？计算 10 万个布局对象的布局耗时多长？

**是否应该将样式计算和布局逻辑分开？**

- 不分开：
- 分开：

**布局结果是否需要存储到 UI 元素中？**

出于内存占用方面的考虑，。。。。

**是否需要简化盒模型数据？**

- 只给 margin-box：
- 提供全部盒模型数据：

## 知识点

样式计算阶段和布局阶段都有差异对比功能来避免触发无意义的布局，这样能增加布局树方案的性能预算和可行性。

布局对象中的布局参数可以定义为指针，由 UI 元素记录布局参数实体并在样式计算阶段更新，在布局阶段时，只需对构建的每个布局对象的布局参数指针赋值即可，这样能降低数据复制成本。
