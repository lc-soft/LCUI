# Resource 资源加载模块设计

## 目标

`ui-resource` 负责将本地路径和远程 URI 解析为可供 UI 消费的资源，并提供共享下载、磁盘缓存、取消订阅和 UI 线程事件分发。Avatar 等部件只订阅资源状态，不自行发起 HTTP 请求或管理线程。

## 范围与 API

首期聚焦图像资源，提供不透明的 request/subscription API：以规范化 URI、尺寸提示和缓存策略形成请求 key；状态为 pending、loading、complete、error；监听 load/progress/error。请求完成后向 UI 提供受控本地缓存路径，复用现有 `ui_image_create(path)` 的异步解码和引用计数。

请求共享：多个 Avatar 订阅相同 key 时只下载一次；销毁订阅只取消自身观察，最后一个订阅取消后才允许终止尚未完成的请求。

## 安全与边界

- 首期支持本地文件路径和 HTTPS URI；HTTP、任意 scheme、认证、上传和任意内存 buffer 不在范围。
- 限制重定向次数、最大下载字节、最大解码像素与磁盘缓存总量；缓存文件由模块拥有。
- 网络、缓存和下载失败统一投递 error，Avatar 显示 fallback。
- 不在 Avatar 内实现网络栈；也不把下载得到的 `pd_canvas_t` 或后端像素公开给调用方。

## 演进

后续可让 `ui_image` 支持 provider/stream，直接从资源流解码以消除磁盘中转；该演进不改变 Avatar 的 URI API。

## 文档与验证

资源模块不是 widget 页面。其开发文档将提供本地路径、共享远程 URI、订阅取消、缓存命中、超限和错误回退的 C 测试/示例；Avatar 文档负责展示本地头像、远程头像、加载占位和 fallback。