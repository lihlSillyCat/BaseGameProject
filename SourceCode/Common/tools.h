/*
* Copyright (c) 2015, 李辉龙
* All rights reserved.
*
* 文件名称：tools.h
* 文件标识：GLOBAL
* 摘    要：小功能模板(C++代码)
*
* 当前版本：V 1.1.1.0
* 作    者：李辉龙
* 日    期：2015-12-25
*
* 修改记录：
* 1 修改内容：
*   修改日期：
*   修 改 人：
*
* 2...
*/
#pragma once

////////////////////////////////////////
//包含文件
#include <string>
#include <memory>

//数组清零 (后面的 arraysize 可检测参数是否是数组)
//#define ZeroAry(array_ptr) tool::AryAssign(array_ptr, (decltype(array_ptr[0]+0))0, arraysize(array_ptr))
#define ZeroAry(array_ptr) memset(array_ptr, 0, arraysize(array_ptr))

//数组赋初值
#define AryInit(array_ptr, data) tool::AryAssign(array_ptr, data, arraysize(array_ptr))

//工具空间
namespace tool {
///////////////////////////////////////
//模板
//数组复制(不同类型)
template<typename T1, typename T2> void AryCopy(T1 *pDest, const T2 *pSource, unsigned int count) {
	while (count-- > 0) *pDest++ = *pSource++; }

//赋初值
template<typename T1> void AryAssign(T1 *pDest, T1 data, unsigned int count) {
	while (count-- > 0) *pDest++ = data; }

//数组相同
template<typename T> bool SameAry(const T *p1, const T *p2, size_t n) {
	if (p1 == p2) return true;
	while (n-- > 0) if (*p1++ != *p2++) return false; return true; }

//数据求和
template<class Val_T> Val_T Sum(Val_T *pData, size_t cnt) {
	Val_T Res = (Val_T)0; while (cnt-- > 0) Res += *pData++; return Res; }


//并集(p3空间足够大cnt1+cnt2),返回p3的元素个数
template<typename T> size_t Union(const T *p1, size_t cnt1, const T *p2, size_t cnt2, T *p3) {
	T *p4 = p3;
	const T *_p1 = (cnt2 > cnt1 ? p2 : p1);	const T *_p2 = (cnt2 > cnt1 ? p1 : p2);
	size_t n1 = (cnt2 > cnt1 ? cnt2 : cnt1); size_t n2 = (cnt2 > cnt1 ? cnt1 : cnt2);
	//填充
	size_t cnt = n1; while (cnt-- > 0) *p3++ = *_p1++;
	while (n2-- > 0) {
		cnt = 0; while (cnt < n1) {
			if (_p1[cnt] == _p2[n2]) break;	++cnt; }
		if (cnt == n1) *p3++ = _p2[n2]; }
	return (p3 - p4); }

//交集（还未写）
//Intersection

//是否有交集
template<typename T> bool Intersect(const T *p1, size_t cnt1, const T *p2, size_t cnt2) {
	if (cnt1 == 0 || cnt2 == 0) return false;
	while (cnt1-- > 0) {
		for (size_t i = 0; i < cnt2; ++i) if (*p1 == p2[i]) return true;
		++p1; }
	return false; }

//数据包含
template<typename T> bool Contain(const T *srcptr, size_t len, T data) {
	while (len-- > 0) if (*srcptr++ == data) return true;
	return false; }

//数组包含(P1 是否全包含 P2)
template<typename T> bool AryContain(const T *p1, size_t n1, const T *p2, size_t n2) {
	if (n1 < n2) return false;
	while (n2-- > 0) { size_t i = 0; while (i < n1 && *p2 != p1[i]) ++i; if (i == n1) return false; else ++p2; }
	return true; }

//数据不重复拼接(p1空间必须足够大),返回 P1 长度
template<typename T> size_t CatWithNoRepeat(T *p1, size_t cnt1, const T *p2, size_t cnt2) {
	T *p3 = p1 + cnt1;
	while (cnt2-- > 0) {
		size_t n = 0; while (n < cnt1 && p1[n] != *p2) ++n;
		if (n == cnt1) *p3++ = *p2;	++p2; }
	return (p3 - p1); }

///////////////////////////////////////////////////////////////////////////////////////////
//函数工具

//函数功能：数字转换宽字符串，千分位显示
//参    数：number 待转数字；wsbuffer 字符串缓存
void ltows_thousand(long long number, std::wstring &wsbuffer);

//函数功能：提取字符串中的数字（包括正负号），转换成整数
//参    数：wsbuffer 字符串
//说    明：为提高程序安全性，不使用wchar_t[]数组作为参数
long long wstol_extract(const std::wstring &wstr);

//函数功能：数字转换宽字符串，中文显示
//参    数：number 待转数字；wsbuffer 字符串缓存; words 大写汉字
void ltows_cn(long long number, std::wstring &wsbuffer, bool convert_words = false);

}

//end file
//////////////////////////////////////////////////////////////////////