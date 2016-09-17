/* ***************************************************************************
 * memdebug.c -- memory debug module, used for memory usage statistics.
 *
 * Copyright (C) 2016 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 *
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * memdebug.c -- 内存调试模块，用于统计各个内存块的使用情况，方便调试。
 *
 * 版权所有 (C) 2016 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#define ENABLE_MEMDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/thread.h>
#include <LCUI/util/memdebug.h>

#define HEAD_SIGN 123456
#define TAIL_SIGN 654321

/** 内存块组 */
typedef struct MemBlockGroupRec_ {
	int id;			/**< 组的标识号 */
	char *name;		/**< 组的名称 */
	size_t size;		/**< 当前组内内存块的总占用大小 */
	size_t count;		/**< 内存块总数 */
} MemBlockGroupRec, *MemBlockGroup;

/** 内存块 */
typedef struct MemBlockRec_ {
	int head_sign;		/**< 头部标识 */
	size_t size;		/**< 内存块大小 */
	MemBlockGroup group;	/**< 内存块所属组 */
	int tail_sign;		/**< 尾部标识 */
} MemBlockRec, *MemBlock;

static struct {
	int id_count;
	Dict *groups;
	LCUI_Mutex mutex;
	DictType dtype;
} self;

void LCUI_InitMemDebug( void )
{
	self.dtype = DictType_StringCopyKey;
	self.dtype.keyDup = NULL;
	self.dtype.keyDestructor = NULL;
	self.id_count = 1;
	self.groups = Dict_Create( &self.dtype, NULL );
	LCUIMutex_Init( &self.mutex );
}

void LCUI_ExitMemDebug( void )
{

}

void LCUI_PrintMemStats( void )
{
	DictEntry *entry;
	DictIterator *iter;
	printf("mem stats list start\n");
	LCUIMutex_Lock( &self.mutex );
	iter = Dict_GetIterator( self.groups );
	while( (entry = Dict_Next(iter)) ) {
		MemBlockGroup group = DictEntry_GetVal( entry );
		printf( "[%40s] size: %.02f KB, count: %lu\n", group->name, 
			1.0 * group->size / 1024., group->count );
	}
	LCUIMutex_Unlock( &self.mutex );
	printf("mem stats list end\n");
}

static void SetMemBlock( MemBlock blk, size_t size, const char *name )
{
	MemBlockGroup group;
	LCUIMutex_Lock( &self.mutex );
	group = Dict_FetchValue( self.groups, name );
	if( !group ) {
		group = malloc( sizeof(MemBlockGroupRec) );
		group->id = self.id_count++;
		group->name = strdup( name );
		group->size = 0;
		group->count = 0;
		Dict_Add( self.groups, group->name, group );
	}
	group->count += 1;
	group->size += size;
	LCUIMutex_Unlock( &self.mutex );
	blk->head_sign = HEAD_SIGN;
	blk->tail_sign = TAIL_SIGN;
	blk->size = size;
	blk->group = group;
}

void LCUI_SetMemBlockName( void *ptr, const char *name )
{
	MemBlock blk = (MemBlock)((char*)ptr - sizeof( MemBlockRec ));
	if( blk->head_sign != HEAD_SIGN || blk->tail_sign != TAIL_SIGN ) {
		abort();
	}
	LCUIMutex_Lock( &self.mutex );
	blk->group->size -= blk->size;
	blk->group->count -= 1;
	LCUIMutex_Unlock( &self.mutex );
	SetMemBlock( blk, blk->size, name );
}

void *LCUI_Calloc( size_t count, size_t size, const char *name )
{
	MemBlock blk;
	size_t totalsize = size * count + sizeof( MemBlockRec );
	blk = calloc( count, (size_t)(1.0 * totalsize / count + 0.5) );
	SetMemBlock( blk, size * count, name );
	return ((char*)blk) + sizeof( MemBlockRec );
}

void *LCUI_Malloc( size_t size, const char *name )
{
	MemBlock blk = malloc( size + sizeof( MemBlockRec ) );
	SetMemBlock( blk, size, name );
	return ((char*)blk) + sizeof( MemBlockRec );
}

void *LCUI_Realloc( void *ptr, size_t size )
{
	MemBlock newblk;
	MemBlock blk = (MemBlock)((char*)ptr - sizeof( MemBlockRec ));
	if( blk->head_sign != HEAD_SIGN || blk->tail_sign != TAIL_SIGN ) {
		abort();
	}
	newblk = realloc( blk, size + sizeof( MemBlockRec ) );
	if( newblk ) {
		LCUIMutex_Lock( &self.mutex );
		newblk->group->size += size - newblk->size;
		newblk->size = size;
		LCUIMutex_Unlock( &self.mutex );
		return ((char*)blk) + sizeof( MemBlockRec );
	}
	return NULL;
}

void LCUI_Free( void *ptr )
{
	MemBlock blk = (MemBlock)((char*)ptr - sizeof( MemBlockRec ));
	if( blk->head_sign != HEAD_SIGN || blk->tail_sign != TAIL_SIGN ) {
		abort();
	}
	LCUIMutex_Lock( &self.mutex );
	blk->group->size -= blk->size;
	blk->group->count -= 1;
	LCUIMutex_Unlock( &self.mutex );
	free( blk );
}
