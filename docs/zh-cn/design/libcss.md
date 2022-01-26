# libcss

CSS 解析库。

## 参考资料

- https://developer.mozilla.org/zh-CN/docs/Web/API/CSS
- https://developer.mozilla.org/en-US/docs/Web/API/CSS_Object_Model
- https://developer.mozilla.org/en-US/docs/Web/API/CSS_Typed_OM_API

片段：

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
