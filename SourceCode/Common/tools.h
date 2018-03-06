/*
* Copyright (c) 2015, �����
* All rights reserved.
*
* �ļ����ƣ�tools.h
* �ļ���ʶ��GLOBAL
* ժ    Ҫ��С����ģ��(C++����)
*
* ��ǰ�汾��V 1.1.1.0
* ��    �ߣ������
* ��    �ڣ�2015-12-25
*
* �޸ļ�¼��
* 1 �޸����ݣ�
*   �޸����ڣ�
*   �� �� �ˣ�
*
* 2...
*/
#pragma once

////////////////////////////////////////
//�����ļ�
#include <string>
#include <memory>

//�������� (����� arraysize �ɼ������Ƿ�������)
//#define ZeroAry(array_ptr) tool::AryAssign(array_ptr, (decltype(array_ptr[0]+0))0, arraysize(array_ptr))
#define ZeroAry(array_ptr) memset(array_ptr, 0, arraysize(array_ptr))

//���鸳��ֵ
#define AryInit(array_ptr, data) tool::AryAssign(array_ptr, data, arraysize(array_ptr))

//���߿ռ�
namespace tool {
///////////////////////////////////////
//ģ��
//���鸴��(��ͬ����)
template<typename T1, typename T2> void AryCopy(T1 *pDest, const T2 *pSource, unsigned int count) {
	while (count-- > 0) *pDest++ = *pSource++; }

//����ֵ
template<typename T1> void AryAssign(T1 *pDest, T1 data, unsigned int count) {
	while (count-- > 0) *pDest++ = data; }

//������ͬ
template<typename T> bool SameAry(const T *p1, const T *p2, size_t n) {
	if (p1 == p2) return true;
	while (n-- > 0) if (*p1++ != *p2++) return false; return true; }

//�������
template<class Val_T> Val_T Sum(Val_T *pData, size_t cnt) {
	Val_T Res = (Val_T)0; while (cnt-- > 0) Res += *pData++; return Res; }


//����(p3�ռ��㹻��cnt1+cnt2),����p3��Ԫ�ظ���
template<typename T> size_t Union(const T *p1, size_t cnt1, const T *p2, size_t cnt2, T *p3) {
	T *p4 = p3;
	const T *_p1 = (cnt2 > cnt1 ? p2 : p1);	const T *_p2 = (cnt2 > cnt1 ? p1 : p2);
	size_t n1 = (cnt2 > cnt1 ? cnt2 : cnt1); size_t n2 = (cnt2 > cnt1 ? cnt1 : cnt2);
	//���
	size_t cnt = n1; while (cnt-- > 0) *p3++ = *_p1++;
	while (n2-- > 0) {
		cnt = 0; while (cnt < n1) {
			if (_p1[cnt] == _p2[n2]) break;	++cnt; }
		if (cnt == n1) *p3++ = _p2[n2]; }
	return (p3 - p4); }

//��������δд��
//Intersection

//�Ƿ��н���
template<typename T> bool Intersect(const T *p1, size_t cnt1, const T *p2, size_t cnt2) {
	if (cnt1 == 0 || cnt2 == 0) return false;
	while (cnt1-- > 0) {
		for (size_t i = 0; i < cnt2; ++i) if (*p1 == p2[i]) return true;
		++p1; }
	return false; }

//���ݰ���
template<typename T> bool Contain(const T *srcptr, size_t len, T data) {
	while (len-- > 0) if (*srcptr++ == data) return true;
	return false; }

//�������(P1 �Ƿ�ȫ���� P2)
template<typename T> bool AryContain(const T *p1, size_t n1, const T *p2, size_t n2) {
	if (n1 < n2) return false;
	while (n2-- > 0) { size_t i = 0; while (i < n1 && *p2 != p1[i]) ++i; if (i == n1) return false; else ++p2; }
	return true; }

//���ݲ��ظ�ƴ��(p1�ռ�����㹻��),���� P1 ����
template<typename T> size_t CatWithNoRepeat(T *p1, size_t cnt1, const T *p2, size_t cnt2) {
	T *p3 = p1 + cnt1;
	while (cnt2-- > 0) {
		size_t n = 0; while (n < cnt1 && p1[n] != *p2) ++n;
		if (n == cnt1) *p3++ = *p2;	++p2; }
	return (p3 - p1); }

///////////////////////////////////////////////////////////////////////////////////////////
//��������

//�������ܣ�����ת�����ַ�����ǧ��λ��ʾ
//��    ����number ��ת���֣�wsbuffer �ַ�������
void ltows_thousand(long long number, std::wstring &wsbuffer);

//�������ܣ���ȡ�ַ����е����֣����������ţ���ת��������
//��    ����wsbuffer �ַ���
//˵    ����Ϊ��߳���ȫ�ԣ���ʹ��wchar_t[]������Ϊ����
long long wstol_extract(const std::wstring &wstr);

//�������ܣ�����ת�����ַ�����������ʾ
//��    ����number ��ת���֣�wsbuffer �ַ�������; words ��д����
void ltows_cn(long long number, std::wstring &wsbuffer, bool convert_words = false);

}

//end file
//////////////////////////////////////////////////////////////////////