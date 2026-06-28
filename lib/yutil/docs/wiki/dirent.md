## dirent

提供 linux 和 windows 通用的目录操作相关函数。

下面举个例子：
```c
#define PATH_LEN 2048

static dir_t* dir;

static void test_dirent()
{
	dir_entry_t* entry;
	size_t dir_len = 0;
	char* name;
	char path[PATH_LEN] = { 0 };

#if defined(_WIN32)
	wchar_t path_w[PATH_LEN] = { 0 };
	GetCurrentDirectoryW(PATH_LEN, path_w);
#else
	if (!getcwd(path, PATH_LEN - 1)) {
		return;
	}
#endif

	//成功打开返回 0
	dir_open_a(path, dir);

	//遍历目录
	while ((entry = dir_read_a(dir)) != NULL) {
		dir_len++;
		//获取目录名
		name = dir_get_file_name_a(entry);
		if (name[0] == '.') {
			continue;
		} else {
			it_b("dir_get_file_name_a() should work", name != NULL,
			     TRUE);
			break;
		}
	}
}

void test_dirent(void)
{
	//初始化 *dir
	dir = dir_create() test_dirent_a();
	test_dirent_w();
	//销毁 *dir
	dir_destroy(dir);
}
```