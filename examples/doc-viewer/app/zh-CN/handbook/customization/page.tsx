import { Text, Widget } from "@lcui/react";
import CodeBlockCopy from "../../../components/code-block-copy";

export default function CustomizationDocPage() {
  return (
    <Widget className="doc-page">
      <h1>自定义</h1>
      <p className="doc-page-desc">{`你可以创建全新的部件类型，也可以在现有部件基础上进行扩展。`}</p>
      <Widget className="doc-section">
        <h2>创建自定义部件</h2>
        <p>{`使用 [bgcolor=#eee] ui_create_widget_prototype() [/bgcolor] 注册新的部件类型：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/customization/0">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/counter.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#6e7781]#[color=#cf222e]include[/color] [color=#0a3069]<LCUI.h>[/color][/color]`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#cf222e]typedef[/color] [color=#cf222e]struct[/color] {`}</Text>
            <Text className="code-line">{`        [color=#953800]int[/color] value;`}</Text>
            <Text className="code-line">{`} [color=#953800]counter_data_t[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_prototype_t[/color] *counter_proto;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]counter_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]counter_data_t[/color] *data;`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *text;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        data = ui_widget_add_data(w, counter_proto, [color=#cf222e]sizeof[/color]([color=#953800]counter_data_t[/color]));`}</Text>
            <Text className="code-line">{`        data->value = [color=#0550ae]0[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        text = ui_create_widget([color=#0a3069]"text"[/color]);`}</Text>
            <Text className="code-line">{`        ui_text_set_content(text, [color=#0a3069]"0"[/color]);`}</Text>
            <Text className="code-line">{`        ui_widget_append(w, text);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]counter_destroy[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#6e7781]/* 释放自有资源（如有） */[/color]`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]register_counter_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        counter_proto = ui_create_widget_prototype([color=#0a3069]"counter"[/color], [color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`        counter_proto->init = counter_init;`}</Text>
            <Text className="code-line">{`        counter_proto->destroy = counter_destroy;`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <p>{`使用这个自定义部件：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/customization/1">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/main.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#cf222e]extern[/color] [color=#953800]void[/color] [color=#8250df]register_counter_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color];`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]int[/color] [color=#8250df]main[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_t[/color] *counter;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        lcui_init();`}</Text>
            <Text className="code-line">{`        register_counter_widget();`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        counter = ui_create_widget([color=#0a3069]"counter"[/color]);`}</Text>
            <Text className="code-line">{`        ui_widget_append(ui_root(), counter);`}</Text>
            <Text className="code-line">{`        [color=#cf222e]return[/color] lcui_main();`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>扩展现有部件</h2>
        <p>{`把父类型名作为 [bgcolor=#eee] ui_create_widget_prototype() [/bgcolor] 的第二个参数传入：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/customization/2">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/icon_button.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]ui_widget_prototype_t[/color] *icon_button_proto;`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]icon_button_init[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#6e7781]/* 调用父类（button）的初始化逻辑 */[/color]`}</Text>
            <Text className="code-line">{`        icon_button_proto->proto->init(w);`}</Text>
            <Text className="code-line">{`        ui_widget_add_class(w, [color=#0a3069]"icon-button"[/color]);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]void[/color] [color=#8250df]register_icon_button[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        icon_button_proto =`}</Text>
            <Text className="code-line">{`            ui_create_widget_prototype([color=#0a3069]"icon-button"[/color], [color=#0a3069]"button"[/color]);`}</Text>
            <Text className="code-line">{`        icon_button_proto->init = icon_button_init;`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]父类的 [bgcolor=#eee] init [/bgcolor] 必须手动调用。[/b] LCUI 在创建子类型部件时不会自动调用父类型的 [bgcolor=#eee] init [/bgcolor]。如果你忘记调用 [bgcolor=#eee] proto->proto->init(w) [/bgcolor]，部件会缺少父类型的基础行为（例如 [bgcolor=#eee] button [/bgcolor] 不会注册点击事件、不会设置默认样式）。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>部件生命周期</h2>
        <p>{`[bgcolor=#eee] ui_widget_prototype_t [/bgcolor] 上有多个回调钩子，覆盖部件的各个生命周期阶段：`}</p>
        <ul>
          <li><Text>{`[b][bgcolor=#eee] init [/bgcolor][/b] — 部件创建时。负责初始化自有数据、子部件、事件绑定。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] destroy [/bgcolor][/b] — 部件销毁时。负责释放自有内存、解绑事件。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] update [/bgcolor][/b] — 部件需要更新时，接收 [bgcolor=#eee] ui_task_type_t [/bgcolor] 参数标识更新类型（样式更新、属性更新等）。LCUI 内部在每帧更新循环中调用此钩子。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] setattr [/bgcolor][/b] — 设置 XML 属性时（用于响应属性变更）。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] settext [/bgcolor][/b] — 设置文本内容时。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] sizehint [/bgcolor][/b] — 估算部件在无外部约束时的自然尺寸约束（最小/最大内容尺寸）。布局引擎在计算子部件尺寸时会参考此结果。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] resize [/bgcolor][/b] — 部件尺寸变化时。接收新的内容区域宽高作为参数。`}</Text></li>
          <li><Text>{`[b][bgcolor=#eee] paint [/bgcolor][/b] — 部件需要重绘时。用于自定义绘制逻辑。`}</Text></li>
        </ul>
      </Widget>
      <Widget className="doc-section">
        <h2>自定义绘制</h2>
        <p>{`可以重写 [bgcolor=#eee] paint [/bgcolor] 回调，在部件上绘制自定义图形：`}</p>
        <Widget className="code-block" data-language="c" data-source="zh-CN/handbook/customization/3">
          <Widget className="code-block-header">
            <Text className="code-block-title">src/my_widget.c</Text>
          </Widget>
          <Widget className="code-block-body">
            <CodeBlockCopy />
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]my_widget_paint[/color][color=#0550ae]([color=#953800]ui_widget_t[/color] *w, [color=#953800]pd_context_t[/color] *paint_ctx,[/color]`}</Text>
            <Text className="code-line">{`[color=#0550ae]                              [color=#953800]ui_widget_actual_style_t[/color] *style)[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]pd_canvas_t[/color] *canvas = paint_ctx->canvas;`}</Text>
            <Text className="code-line">{`        [color=#953800]pd_color_t[/color] color = pd_color_from_rgb([color=#0550ae]255[/color], [color=#0550ae]0[/color], [color=#0550ae]0[/color]);`}</Text>
            <Text className="code-line">{`        [color=#953800]pd_rect_t[/color] rect = { style->left, style->top, w->width, w->height };`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`        pd_canvas_fill_rect(canvas, color, rect);`}</Text>
            <Text className="code-line">{`}`}</Text>
            <Text className="code-line">{` `}</Text>
            <Text className="code-line">{`[color=#953800]static[/color] [color=#953800]void[/color] [color=#8250df]register_my_widget[/color][color=#0550ae]([color=#953800]void[/color])[/color]`}</Text>
            <Text className="code-line">{`{`}</Text>
            <Text className="code-line">{`        [color=#953800]ui_widget_prototype_t[/color] *proto =`}</Text>
            <Text className="code-line">{`            ui_create_widget_prototype([color=#0a3069]"my-widget"[/color], [color=#0550ae]NULL[/color]);`}</Text>
            <Text className="code-line">{`        proto->paint = my_widget_paint;`}</Text>
            <Text className="code-line">{`}`}</Text>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] paint [/bgcolor] 只负责绘制。[/b] 部件尺寸估算由 [bgcolor=#eee] sizehint [/bgcolor] 处理；不要在 [bgcolor=#eee] paint [/bgcolor] 里修改部件尺寸或子部件树，否则可能触发无限循环。`}</p>
          </Widget>
        </Widget>
      </Widget>
      <Widget className="doc-section">
        <h2>注意事项</h2>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b][bgcolor=#eee] destroy [/bgcolor] 回调要释放自有资源。[/b] 如果自定义部件在 [bgcolor=#eee] init [/bgcolor] 里分配了内存、打开了文件、订阅了系统事件（如 [bgcolor=#eee] lcui_settings_on_deserialize [/bgcolor]），必须在 [bgcolor=#eee] destroy [/bgcolor] 里做对称释放。`}</p>
          </Widget>
        </Widget>
        <Widget className="admonition admonition-warning">
          <Text className="admonition-title">注意</Text>
          <Widget className="admonition-content">
            <p>{`[b]自定义部件必须在 [bgcolor=#eee] lcui_init() [/bgcolor] 之后、首次实例化之前注册。[/b] 在 [bgcolor=#eee] main() [/bgcolor] 开头立即调用你的 [bgcolor=#eee] register_xxx() [/bgcolor] 函数是最稳妥的位置，可以确保所有使用方（XML 加载、路由实例化、TSX 编译结果）都能看到原型。`}</p>
          </Widget>
        </Widget>
      </Widget>
    </Widget>
  );
}
