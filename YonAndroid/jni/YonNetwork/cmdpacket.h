#ifndef _CMDPACKET_H_
#define _CMDPACKET_H_

#include "netconfig.h"

/* define default packet buffer size */
const int DEFAULT_CMD_PACKET_SIZE = 1024;
/* define max packet buffer size */
const int MAX_CMD_PACKET_SIZE	  = 1024 * 16;

/* define some length used in packet class */
const int BYTE_SIZE  = 1;
const int LONG_SIZE  = 4;
const int SHORT_SIZE = 2;
const int FLOAT_SIZE = 4;
const int LONG_LONG_SIZE  = 8;
const int WCHAR_SIZE = 2;		// !! Attention, sizeof(wchar_t) on android = 4

#include "CPacketMgr.h"
#include "basenetwork.h"

//�����
//����������������/д���ķ�װ
//NOTE: ��/д����ע���ֽ����С��
class CCmdPacket
{
private:
	SPacketBuffer* m_pBuffer;
	static io::IFileSystem* s_pFileSystem;
	io::IReadStream* m_pReadStream;
	core::array<core::stringc> inPlaces;

	bool ReadData(void *data,int len);
	bool WriteData(void *data,int len);
public:
	CCmdPacket();
	virtual ~CCmdPacket();

	static void setFileSystem(io::IFileSystem* fs);
	SPacketBuffer* getBuffer(){return m_pBuffer;}

	// �ַ�������UTF-8���봫��
	// ��ʽ�� | short | UTF8 string | '\0' | 
	//    ����һ��short��ʾ�������ݵĳ���+�ַ�������+0��β
	// Ϊ��Ч�ʿ��ǣ��ṩ��һ���ȡ��������ע��ʹ��˵����
	//
	// ReadString(char* pstr) 
	// - �û��Լ��ṩ�ַ������棬ע��Ҫ�ṩ�㹻��Ļ��棬����Խ��
	// - һ�������û�֪�����ַ�������󳤶ȣ������˺ţ������16���ַ��� 
	//
	// ReadString(char** ppstr) 
	// - �û����ṩ�ַ������棬�ɱ������ڲ����룬ʹ����Ϻ���Ҫ�û��ֶ�delete��
	// - һ�����ڲ�֪�����ַ�����ȷ�г��ȣ������ǱȽϳ����ַ�����
	//
	// ReadStringInPlace(char** ppstr) 
	// - �û����ṩ�ַ������棬�������ڲ�ֻ���޸ĵ�ַ��Ҳ����ֱ��ʹ������ģ���ڲ��Ļ��棬
	// - ȷ����ҪԽ������޸ģ�������ܵ�������ģ�������
	// - ��Ҫ�� ��ָ��ֻ�ڵ��ú�������Ч���˳����ú�����ʧЧ��
	// - �ú���Ч����ߣ����漰�κ�����ʱ��̬�ڴ���䣬Ψһ��Ҫ���ǽ���ʹ�ã�

	bool ReadString(char* pstr);
	//bool ReadString(char** ppstr);//����û�õ�
	bool ReadStringInPlace(char** ppstr);

	bool ReadFloat(float *f);
	bool ReadInt32(int *l);
	bool ReadLongLong(long long *ll);
	bool ReadShort(short *s);
	bool ReadByte(unsigned char *c);

	void BeginRead(char *p,int len);	//��ʼ����״̬
	//void BeginRead();//only for emulate Server

	bool ReadBinary(char **data,int *len);

	void BeginWrite();

	bool WriteBinary(char *data,int len);
	// д��0��β��c�ַ���
	// ��ʽ�ο�ReadString
	// ע�⣺��β0�ֽڲ�д��������
	bool WriteString(const char *str);

	bool WriteFloat(float f);
	bool WriteInt32(int l);
	bool WriteShort(short s);
	bool WriteByte(char c);

	char *GetData();
	int   GetDataSize();

	bool  SetSize(int len);
	//bool  CloneFrom(CCmdPacket *packet);//û���õ�
};

#endif