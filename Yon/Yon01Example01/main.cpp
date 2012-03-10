#include "stdio.h"

#include <crtdbg.h>
#include <typeinfo>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#include "yon.h"
using namespace yon;
using namespace yon::core;

#pragma comment(lib, "Yon.lib")

void print(const matrix4f& m){
	for(int i=0;i<4;++i)
		printf("%.2f\t%.2f\t%.2f\t%.2f\n",m.m[0][i],m.m[1][i],m.m[2][i],m.m[3][i]);
	printf("\n");
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//SYonEngineParameters params;
	//IYonEngine* pYE=CreateEngine(params);

	
#if 0
	//setlocale(LC_ALL,"zh_CN.utf8");
	setlocale(LC_ALL,"chs");

	core::stringc str;
	printf("%d,%s\n",str.length(),str);

	str+="test";
	printf("%d,%s\n",str.length(),str);

	core::stringc str1=str+"test";
	printf("%d,%s\n",str1.length(),str1);

	core::stringw wstr;
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	wstr+=L"中华人民共和国";
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	core::stringw wstr1=wstr+L"test";
	wprintf(L"%d,%s\n",wstr1.length(),wstr1);

	core::stringc tstr("你好");
	printf("%s\n\n\n",tstr);
#endif

	matrix4f m;
	m.makeIdentity();
	print(m);

	vector3df v(3,2,3);
	m.setTranslation(v);
	print(m);

	matrix4f n;
	n.makeIdentity();
	n.translate(1,2,3);
	print(n);

	matrix4f r;
	r=m*n;
	print(r);

	f32 temp[]={2,0,0,1,0,1,0,2,0,0,1,3,0,0,0,1};
	matrix4f l(temp);
	r=m*l;
	printf("==>");
	print(r);

	r.rotate(60,1,1,1);
	print(r);

	f32 temp2[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	matrix4f k(temp2);
	print(k);
	k.makeTranspose();
	print(k);

	


	getchar();
	return 0;
}