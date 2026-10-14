# Attachment 组件设计

## 目标

`attachment` 展示聊天消息或表单中的文件、图片及其处理状态，不负责传输协议。

## 结构与 API

```text
attachment
├── attachment-preview
├── attachment-details
│   ├── attachment-name
│   └── attachment-meta
└── attachment-action
```

提供 name、size、media type、state、remove setter 及子件工厂。

## 状态与边界

- 状态：idle、uploading、processing、error、done。
- action 只发出 preview/remove 事件，调用方管理上传、下载与重试。
- 不实现文件 I/O、网络传输、图片灯箱或横向附件组。

## 依赖与验证

复用 Image、Button、Progress。测试状态样式、事件和缺失预览；中文 demo 展示文件与图片。

## 文档示例

组件文档提供文件附件、图片附件、上传中、错误与移除操作四个示例。