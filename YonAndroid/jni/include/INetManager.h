#ifndef _YON_NET_INETMANAGER_H_
#define _YON_NET_INETMANAGER_H_

#include "ISocket.h"

namespace yon{
namespace net{

	//TODO address families

	class INetManager : public virtual core::IReferencable{
	public:
		virtual ISocket* createSocket(ENUM_SOCKET_PROTOCOL protocol) = 0;
	};

	INetManager* createNetManager();
}
}
#endif