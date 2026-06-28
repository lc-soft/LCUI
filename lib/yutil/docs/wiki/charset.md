## 字符编码 charset

支持 UTF-8、ANSI 与 Unicode 间的字符编码转换，且中英文混用时不会有乱码。
下面举个例子：
```c
void charset(void)
{
	size_t len;
	char str[64];
	wchar_t wcs[64];

	// len 为返回实际转换字符的长度
	len = decode_utf8(wcs, "hello", 64);

	//将 ANSI 编码转换 Unicode 编码
	len = decode_string(wcs, "简体中文", 64, ENCODING_ANSI);

	//将 Unicode 编码转换 ANSI 编码
	len = encode_string(str, L"简体中文", 64, ENCODING_ANSI);

	//将 UTF-8 编码转换 Unicode 编码
	len = decode_utf8(wcs, "简体中文", 64);

	//将 Unicode 编码转换 UTF-8 编码
	len = encode_utf8(str, L"简体中文", 64);
}
```