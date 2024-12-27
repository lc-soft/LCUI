# lcui-router

([English](README.md)/**中文**)

## 介绍

LCUI Router 是 [LCUI](https://github.com/lc-soft/LCUI) 的官方路由管理器。它和 LCUI 的核心深度集成，提供与 [Vue Router](https://github.com/vuejs/vue-router) 类似的开发体验，能让构建多视图的 LCUI 应用变得易如反掌。包含的功能有：

- 嵌套的路由/视图表
- 模块化的、基于组件的路由配置
- 路由参数、查询、通配符
- 细粒度的导航控制
- 带有自动激活的 CSS class 的链接

## 快速上手

> **注意:** 我们将使用 [lcui-cli](https://github.com/lc-ui/lcui-cli) 工具来管理路由的配置和源代码。

创建一个 LCUI 应用程序项目：

``` bash
lcui create myapp
cd myapp
```

复制以下代码并覆盖到文件 `app/assets/views/app.xml`：

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<lcui-app>
  <resource type="text/css" src="assets/stylesheets/app.css"/>
  <ui>
    <w>
      <textview>Hello App!</textview>
      <w>
        <!-- 使用 router-link 部件用于导航 -->
        <!-- 通过 `to` 属性来指定链接 -->
        <router-link to="/foo">Go to Foo</router-link>
        <router-link to="/bar">Go to Bar</router-link>
      </w>
      <!-- 路由出口 -->
      <!-- 与路由匹配的部件将会渲染在这里 -->
      <router-view />
    </w>
  </ui>
</lcui-app>
```

生成两个部件：

```bash
lcui generate widget foo
lcui generate widget bar
```

保存以下代码到文件 `config/router.js`:

```js
module.exports = [
  { path: '/foo', component: 'foo' },
  { path: '/bar', component: 'bar' }
]
```

编译路由配置文件：

```bash
lcui compile router
```

运行应用程序：

``` bash
lcpkg run start
```

如需更多示例，可查看 [lcui-router-app](https://github.com/lc-ui/lcui-router-app) 项目。

## 文档

LCUI Router 的设计参考自 Vue Router，因此，你可以通过阅读 [Vue Router 的文档](https://router.vuejs.org/) 来学习 LCUI Router 的简单用法。

## 许可

代码基于 [MIT 许可协议](LICENSE) 发布。
