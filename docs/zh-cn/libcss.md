# libcss

CSS 解析库。

## 参考资料

文档：

- https://developer.mozilla.org/zh-CN/docs/Web/API/CSS
- https://developer.mozilla.org/en-US/docs/Web/API/CSS_Object_Model
- https://developer.mozilla.org/en-US/docs/Web/API/CSS_Typed_OM_API

可供参考的代码片段：

```text
CSSKeywordValue
CSSUnitValue {value: 3, unit: "em"}

window.CSS.registerProperty({
name: '--my-color',
syntax: '<color>',
inherits: false,
initialValue: '#c0ffee',
});
CSS.supports(propertyName, value);

CSSFontFaceRule
CSSRule
CSSRuleList
CSSStylesheet
CSSStyleDeclaration


enum CSSRuleType {
STYLE_RULE,
IMPORT_RULE,
MEDIA_RULE,
FONT_FACE_RULE,
}

CSSRule {
CSSRuleType type
}

CSSStyleRule : CSSRule {
string selectorText
string cssText
CSSStyleSheet parentStyleSheet
CSSStyleDeclaration style
stylePropertyMap styleMap
}

CSSStyleSheet {
CSSRuleList cssRules
MediaList media
href
}

CSSStyleDeclaration {
getPropertyValue()
setProperty()
removeProperty()
}

CSSStyleDeclaration getComputedStyle();

```

MDN 并未描述 CSSStyleValue 内部的实现细节，对于多个值的情况，Chrome 浏览器中运行 `CSSStyleValue.parse('background-position', '0 0');` 后返回的结果是 CSSStyleValue 类型的对象，无法再展开该对象内部结构。

对于 CSSStyleValue 的实现，将它设计为类似于链表的结构是个合适的选择，虽然这样做需要多加一个字段来记录值的数量，会导致样式表内存占用增加，但现阶段不需要纠结这些问题。
