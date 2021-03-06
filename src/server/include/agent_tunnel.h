/* 
** NetXMS - Network Management System
** Server Core
** Copyright (C) 2003-2020 Victor Kirhenshtein
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** File: nxcore.h
**
**/

#ifndef _agent_tunnel_h_
#define _agent_tunnel_h_

#include <nms_util.h>

#define UNBOUND_TUNNEL_PROCESSOR_TASK_ID _T("AgentTunnels.ProcessUnbound")
#define RENEW_AGENT_CERTIFICATES_TASK_ID _T("AgentTunnels.RenewCertificates")

class AgentTunnel;

/**
 * Agent tunnel communication channel
 */
class AgentTunnelCommChannel : public AbstractCommChannel
{
private:
   AgentTunnel *m_tunnel;
   UINT32 m_id;
   bool m_active;
   RingBuffer m_buffer;
#ifdef _WIN32
   CRITICAL_SECTION m_bufferLock;
   CONDITION_VARIABLE m_dataCondition;
#else
   pthread_mutex_t m_bufferLock;
   pthread_cond_t m_dataCondition;
#endif

protected:
   virtual ~AgentTunnelCommChannel();

public:
   AgentTunnelCommChannel(AgentTunnel *tunnel, UINT32 id);

   virtual ssize_t send(const void *data, size_t size, MUTEX mutex = INVALID_MUTEX_HANDLE) override;
   virtual ssize_t recv(void *buffer, size_t size, UINT32 timeout = INFINITE) override;
   virtual int poll(UINT32 timeout, bool write = false) override;
   virtual int shutdown() override;
   virtual void close() override;

   UINT32 getId() const { return m_id; }

   void putData(const BYTE *data, size_t size);
};

/**
 * Tunnel state
 */
enum AgentTunnelState
{
   AGENT_TUNNEL_INIT = 0,
   AGENT_TUNNEL_UNBOUND = 1,
   AGENT_TUNNEL_BOUND = 2,
   AGENT_TUNNEL_SHUTDOWN = 3
};

/**
 * Agent tunnel
 */
class AgentTunnel : public RefCountObject
{
protected:
   uint32_t m_id;
   uuid m_guid;
   InetAddress m_address;
   SOCKET m_socket;
   SSL_CTX *m_context;
   SSL *m_ssl;
   MUTEX m_sslLock;
   MUTEX m_writeLock;
   MsgWaitQueue m_queue;
   VolatileCounter m_requestId;
   uint32_t m_nodeId;
   int32_t m_zoneUIN;
   time_t m_certificateExpirationTime;
   AgentTunnelState m_state;
   time_t m_startTime;
   NodeHardwareId m_hardwareId;
   TCHAR *m_systemName;
   TCHAR m_hostname[MAX_DNS_NAME];
   TCHAR *m_platformName;
   TCHAR *m_systemInfo;
   TCHAR *m_agentVersion;
   TCHAR *m_agentBuildTag;
   uuid m_agentId;
   UINT32 m_bindRequestId;
   uuid m_bindGuid;
   uint32_t m_bindUserId;
   bool m_userAgentInstalled;
   bool m_agentProxy;
   bool m_snmpProxy;
   bool m_snmpTrapProxy;
   RefCountHashMap<UINT32, AgentTunnelCommChannel> m_channels;
   MUTEX m_channelLock;
   
   virtual ~AgentTunnel();

   void recvThread();
   static THREAD_RESULT THREAD_CALL recvThreadStarter(void *arg);
   
   int sslWrite(const void *data, size_t size);
   bool sendMessage(NXCPMessage *msg);
   NXCPMessage *waitForMessage(UINT16 code, UINT32 id) { return m_queue.waitForMessage(code, id, g_agentCommandTimeout); }

   void processCertificateRequest(NXCPMessage *request);
   void processChannelClose(UINT32 channelId);

   void setup(const NXCPMessage *request);
   uint32_t initiateCertificateRequest(const uuid& nodeGuid, uint32_t userId);

public:
   AgentTunnel(SSL_CTX *context, SSL *ssl, SOCKET sock, const InetAddress& addr, uint32_t nodeId, int32_t zoneUIN, time_t certificateExpirationTime);
   
   void start();
   void shutdown();
   uint32_t bind(uint32_t nodeId, uint32_t userId);
   uint32_t renewCertificate();
   AgentTunnelCommChannel *createChannel();
   void closeChannel(AgentTunnelCommChannel *channel);
   ssize_t sendChannelData(uint32_t id, const void *data, size_t len);
   void resetStartTime() { m_startTime = time(NULL); }

   uint32_t getId() const { return m_id; }
   const InetAddress& getAddress() const { return m_address; }
   const NodeHardwareId& getHardwareId() const { return m_hardwareId; }
   const TCHAR *getSystemName() const { return CHECK_NULL_EX(m_systemName); }
   const TCHAR *getHostname() const { return m_hostname; }
   const TCHAR *getDisplayName() const { return (m_hostname[0] != 0) ? m_hostname : CHECK_NULL_EX(m_systemName); }
   const TCHAR *getSystemInfo() const { return CHECK_NULL_EX(m_systemInfo); }
   const TCHAR *getPlatformName() const { return CHECK_NULL_EX(m_platformName); }
   const TCHAR *getAgentVersion() const { return CHECK_NULL_EX(m_agentVersion); }
   const TCHAR *getAgentBuildTag() const { return CHECK_NULL_EX(m_agentBuildTag); }
   const uuid& getAgentId() const { return m_agentId; }
   int32_t getZoneUIN() const { return m_zoneUIN; }
   bool isBound() const { return m_nodeId != 0; }
   uint32_t getNodeId() const { return m_nodeId; }
   time_t getCertificateExpirationTime() const { return m_certificateExpirationTime; }
   AgentTunnelState getState() const { return m_state; }
   time_t getStartTime() const { return m_startTime; }

   void fillMessage(NXCPMessage *msg, UINT32 baseId) const;

   void debugPrintf(int level, const TCHAR *format, ...);
};

/**
 * Setup server side TLS context
 */
bool SetupServerTlsContext(SSL_CTX *context);

/**
 * Get tunnel for node
 */
AgentTunnel *GetTunnelForNode(uint32_t nodeId);

#endif
