# Cairo Example

([English](./README.md)/**中文**)

PandaGL 和 Cairo 图形库混用的例子。大致流程是先用 PandaGL 创建画布，然后基于画布创建一个 Surface 供 Cairo 绘制复杂图形，之后用 PandaGL 填充简单的图形，最后用 Cairo 将绘制结果输出为 png 图片文件。

```bash
# 构建 PandaGL 库
xmake build pandagl

# 打包 PandaGL 库
xmake package pandagl

# 构建当前目录下的示例
xmake -P .

# 运行当前目录下的示例项目
xmake run -P . -w .
```

输出的 output.png 图片内容如下：

![output](./output.png)
