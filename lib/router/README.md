# lcui-router

(**English**/[中文](README.zh-cn.md))

## Introduction

LCUI Router is the official router for [LCUI](https://github.com/lc-soft/LCUI). It provides a similar development experience to the [Vue Router](https://github.com/vuejs/vue-router) and make building multiple views applications with LCUI a breeze. Features include:

- Nested route/view mapping
- Modular, component-based router configuration
- Route params, query, wildcards
- Fine-grained navigation control
- Links with automatic active CSS classes

## Getting Started

> **Note:** We will using [lcui-cli](https://github.com/lc-ui/lcui-cli) tool to manage the configuration and source code for the router.

Create an LCUI application project:

``` bash
lcui create myapp
cd myapp
```

Overwrite the following code to the file `app/assets/views/app.xml`:

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<lcui-app>
  <resource type="text/css" src="assets/stylesheets/app.css"/>
  <ui>
    <w>
      <textview>Hello App!</textview>
      <w>
        <!-- use router-link component for navigation. -->
        <!-- specify the link by passing the `to` prop. -->
        <router-link to="/foo">Go to Foo</router-link>
        <router-link to="/bar">Go to Bar</router-link>
      </w>
      <!-- route outlet -->
      <!-- component matched by the route will render here -->
      <router-view />
    </w>
  </ui>
</lcui-app>
```

Generate two widgets:

```bash
lcui generate widget foo
lcui generate widget bar
```

Save following code as file `config/router.js`:

```js
module.exports = [
  { path: '/foo', component: 'foo' },
  { path: '/bar', component: 'bar' }
]
```

Compile config file for router:

```bash
lcui compile router
```

Run app:

``` bash
lcpkg run start
```

For more examples, please see the [lcui-router-app](https://github.com/lc-ui/lcui-router-app) project.

## Documentation

LCUI Router is referenced from Vue Router, so you can read the [Vue Router documentation](https://router.vuejs.org/) to learn about the basic usage of LCUI Router.

## License

[MIT licensed](LICENSE).
