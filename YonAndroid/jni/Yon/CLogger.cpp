#include "CLogger.h"
#include "exception.h"
#include <memory.h>
#include <time.h>
#include <sys/timeb.h>

#ifdef YON_COMPILE_WITH_ANDROID
#include <android/log.h>
#endif

namespace yon{
	namespace debug{

#ifdef YON_COMPILE_WITH_ANDROID
//�ڱ�׼C��㲻��ʡ�Կɱ������������ȴ���Ը�������һ���յĲ�����Ϊ�˽��������⣬CPPʹ��һ������ġ�##��������
#define LOGD(fmt,...) __android_log_print(ANDROID_LOG_DEBUG,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#define LOGI(fmt,...) __android_log_print(ANDROID_LOG_INFO,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#define LOGW(fmt,...) __android_log_print(ANDROID_LOG_WARN,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#define LOGE(fmt,...) __android_log_print(ANDROID_LOG_ERROR,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#endif

		const core::stringc CLogger::LEVEL_NAME[ENUM_LOG_LEVEL_COUNT]={"DEBUG","INFO","WARN","ERROR"};
		CLogger::CLogger():
			m_path(""),m_name("log.txt"),m_pFile(NULL),
			m_format(MASK_FORMAT_DATE|MASK_FORMAT_TIME|MASK_FORMAT_MSEC|MASK_FORMAT_LEVEL|MASK_FORMAT_LOG),
			m_level(ENUM_LOG_LEVEL_DEBUG),
#ifdef YON_COMPILE_WITH_WIN32
			m_appender(MASK_APPENDER_FILE|MASK_APPENDER_VS)
#elif defined(YON_COMPILE_WITH_ANDROID)
			m_appender(MASK_APPENDER_CONSOLE)
#endif
		{

				#ifdef YON_COMPILE_WITH_WIN32
				InitializeCriticalSection(&m_mutex);
				#else
				pthread_mutex_init(&m_mutex,NULL);
				#endif
			}
		CLogger::~CLogger(){
			if(m_pFile)
				fclose(m_pFile);
			#ifdef YON_COMPILE_WITH_WIN32
			DeleteCriticalSection(&m_mutex);
			#else
			pthread_mutex_destroy(&m_mutex);
			#endif
		}

		void CLogger::setPath(const core::stringc& path){
			m_path=path;
		}
		void CLogger::setFileName(const core::stringc& name){
			m_name=name;
		}
		void CLogger::setFormat(s32 mask){
			m_format=mask;
		}
		void CLogger::setLevel(ENUM_LOG_LEVEL level){
			m_level=level;
		}
		void CLogger::setAppender(s32 mask){
			m_appender=mask;
		}
		void CLogger::lock(){
			#ifdef YON_COMPILE_WITH_WIN32
			EnterCriticalSection(&m_mutex);
			#else
			if(pthread_mutex_lock(&m_mutex) != 0 )
				throw core::exception("can not lock mutex!(pthread_mutex_lock return nonzero)");
			#endif
		}
		void CLogger::unlock(){
			#ifdef YON_COMPILE_WITH_WIN32
			LeaveCriticalSection(&m_mutex);
			#else
			if(pthread_mutex_unlock(&m_mutex) != 0 )
				throw core::exception("can not unlock mutex!(pthread_mutex_unlock return nonzero)");
			#endif
		}
		void CLogger::appendDateTime(int& index){
			if((m_format&MASK_FORMAT_DATE)||(m_format&MASK_FORMAT_TIME)){
#ifdef YON_COMPILE_WITH_WIN32
				static struct tm ttime;
				static struct _timeb timebuffer;
				_ftime64_s(&timebuffer); 
				localtime_s(&ttime,&timebuffer.time);
				struct tm* newtime=&ttime;
#elif defined(YON_COMPILE_WITH_ANDROID)
				static struct tm* newtime;
				static struct timeb timebuffer;
				ftime (&timebuffer);
				newtime=localtime(&timebuffer.time);
#endif
				sprintf_s(m_buffer+index,2,"[");
				++index;
				if(m_format&MASK_FORMAT_DATE){
					strftime(m_buffer+index,11,"%Y-%m-%d",newtime);
					index+=10;
				}
				if((m_format&MASK_FORMAT_DATE)&&(m_format&MASK_FORMAT_TIME)){
					sprintf_s(m_buffer+index,2," ");
					++index;
				}
				if(m_format&MASK_FORMAT_TIME){
					strftime(m_buffer+index,9,"%H:%M:%S",newtime);
					index+=8;
				}
				if((m_format&MASK_FORMAT_TIME)&&(m_format&MASK_FORMAT_MSEC)){
					sprintf_s(m_buffer+index,2,".");
					++index;
				}
				if(m_format&MASK_FORMAT_MSEC){
					sprintf_s(m_buffer+index,4,"%03d",timebuffer.millitm);
					index+=3;
				}
				/*static struct tm newtime;
				static time_t lt1;
				time(&lt1);
				localtime_s(&newtime,&lt1);
				sprintf_s(m_buffer+index,2,"[");
				++index;
				if(m_format&MASK_FORMAT_DATE){
					strftime(m_buffer+index,11,"%Y-%m-%d",&newtime);
					index+=10;
				}
				if((m_format&MASK_FORMAT_DATE)&&(m_format&MASK_FORMAT_TIME)){
					sprintf_s(m_buffer+index,2," ");
					++index;
				}
				if(m_format&MASK_FORMAT_TIME){
					strftime(m_buffer+index,9,"%H:%M:%S",&newtime);
					index+=8;
				}*/
				//int sprintf_s(char *buffer,size_t sizeOfBuffer,const char *format [,argument] ...);
				//sprintf_s()��sprintf()�İ�ȫ�汾��ͨ��ָ������������������sprintf()���ڵ��������
				sprintf_s(m_buffer+index,2,"]");
				++index;
			}
		}
		void CLogger::appendLevel(int& index,ENUM_LOG_LEVEL level){
			if(m_format&MASK_FORMAT_LEVEL){
				sprintf_s(m_buffer+index,8,"[%s]",LEVEL_NAME[level].c_str());
				index+=LEVEL_NAME[level].length()+2;
			}
		}
		void CLogger::output(ENUM_LOG_LEVEL level,const c8* pFmt,va_list args){
			if(level<m_level)
				return;
			if(m_pFile==NULL&&(m_appender&MASK_APPENDER_FILE)){
#ifdef YON_COMPILE_WITH_WIN32
				errno_t result=fopen_s(&m_pFile,(m_path+m_name).c_str(),"a+");
				if(result)
#elif defined(YON_COMPILE_WITH_ANDROID)
				m_pFile = fopen((m_path+m_name).c_str(),"ab");
				if(m_pFile)
#endif
				{
					core::stringc msg("open log file:");
					msg+=m_path+m_name;
					msg+=" failed!";
					throw core::exception(msg);
				}
			}
			lock();
			memset(m_buffer,0x0,BUFFER_SIZE);
			int index=0;
			appendDateTime(index);
			appendLevel(index,level);
			//_vsnprintf_s(m_buffer,BUFFER_SIZE,pFmt,args);
			//vsnprintf_s��_vsnprintf_sû�ж�������ֻ�Ǻ���ǰ�������
			//ʹ��_vsnprintf_sż���ᷢ���ڴ�й¶
			//��ʹ��vsnprintf_s�򲻻�
			//vsnprintf_s(m_buffer+index,BUFFER_SIZE-index,_TRUNCATE,pFmt,args);
			if(m_format&MASK_FORMAT_LOG){
				vsprintf_s(m_buffer+index,BUFFER_SIZE-index,pFmt,args);
			}else{
				sprintf_s(m_buffer+index,2,"\n");
			}
			unlock();
			if(m_appender&MASK_APPENDER_FILE){
				fprintf(m_pFile,"%s",m_buffer);
			}
			if(m_appender&MASK_APPENDER_CONSOLE){
#ifdef YON_COMPILE_WITH_WIN32
				printf("%s",m_buffer);
#elif defined(YON_COMPILE_WITH_ANDROID)
				switch(level)
				{
				case ENUM_LOG_LEVEL_DEBUG:
					LOGD("%s",m_buffer);
					break;
				case ENUM_LOG_LEVEL_INFO:
					LOGI("%s",m_buffer);
					break;
				case ENUM_LOG_LEVEL_WARN:
					LOGW("%s",m_buffer);
					break;
				case ENUM_LOG_LEVEL_ERROR:
					LOGE("%s",m_buffer);
					break;
				}
#endif//YON_COMPILE_WITH_WIN32
			}
			if(m_appender&MASK_APPENDER_VS){
				OutputDebugStringA(m_buffer);
			}
		}
		void CLogger::debug(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_DEBUG,pFmt,arg);
			va_end(arg);
		}
		void CLogger::info(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_INFO,pFmt,arg);
			va_end(arg);
		}
		void CLogger::warn(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_WARN,pFmt,arg);
			va_end(arg);
		}
		void CLogger::error(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_ERROR,pFmt,arg);
			va_end(arg);
		}
	}//debug
}//yon