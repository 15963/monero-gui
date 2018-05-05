#include "net/http_client.h"
#include "pool_mining_manager.h"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "xmrig_rpc_server_commands_defs.h"

namespace {
    template<typename Request, typename Response>
    bool connect_and_invoke(const std::string& address, const std::string& path, const Request& request, Response& response)
    {
        epee::net_utils::http::http_simple_client client{};
        return client.set_server(address, boost::none) && epee::net_utils::invoke_http_json(path, request, response, client);
    }
}

PoolMiningManager::PoolMiningManager():
      m_isMining(false),
      m_rpc_client(NULL) {

}

PoolMiningManager::~PoolMiningManager() {
    if (m_rpc_client != NULL)
    {
        delete m_rpc_client;
    }
}

void PoolMiningManager::setMiningRpcAddress(const std::string &ip, int port) {
    uint32_t nip = 0;
    epee::string_tools::get_ip_int32_from_string(nip,ip);
    boost::optional<epee::net_utils::http::login> http_login{};
    http_login.emplace("xmrig", "");
    m_rpc_client = new tools::t_rpc_client(nip, (uint16_t)port, std::move(http_login));
}

bool PoolMiningManager::isMining() const {
    return m_isMining;
}

bool PoolMiningManager::startMining(MINING_CONFIG& config) {
    tools::xmrig_rpc::COMMAND_RPC_START_MINING::request   mreq;
    tools::xmrig_rpc::COMMAND_RPC_START_MINING::response  mres;
    mreq.mining_algorithm = config.mining_algorithm;
    mreq.pool_server_address = config.pool_server_address;
    mreq.pool_port = (unsigned short)config.pool_port;
    mreq.wallet_address = config.wallet_address;
    mreq.wallet_password = config.wallet_password;
    mreq.threads_count = config.threads_count;

    std::string failure_message = "Couldn't not start mining";

    if (!m_rpc_client->rpc_request(mreq, mres, "/start", failure_message.c_str()))
    {
          m_isMining = false;
          return false;
    }
    m_isMining = true;

    return true;
}

bool PoolMiningManager::stopMining() {
    tools::xmrig_rpc::COMMAND_RPC_STOP_MINING::request   mreq;
    tools::xmrig_rpc::COMMAND_RPC_STOP_MINING::response  mres;

    std::string failure_message = "Couldn't not stop mining";

    if (!m_rpc_client->rpc_request(mreq, mres, "/stop", failure_message.c_str()))
    {
          m_isMining = false;
          return false;
    }
    m_isMining = false;

    return true;
}

double PoolMiningManager::miningHashRate() const {
    tools::xmrig_rpc::COMMAND_RPC_GET_HASHRATE::request   mreq;
    tools::xmrig_rpc::COMMAND_RPC_GET_HASHRATE::response  mres;

    std::string failure_message = "Couldn't not get mining hash rate";

    if (!m_rpc_client->rpc_request(mreq, mres, "/get_hash_rate", failure_message.c_str()))
    {
         return 0;
    }

    return ::atof(mres.hashrate.c_str());
}

  bool PoolMiningManager::isRuning() const {
    
    tools::xmrig_rpc::COMMAND_RPC_STATUS_MINING::request   mreq;
    tools::xmrig_rpc::COMMAND_RPC_STATUS_MINING::response  mres;

    std::string failure_message = "Couldn't not get mining run status";

    if (!m_rpc_client->rpc_request(mreq, mres, "/status", failure_message.c_str()))
    {
         return false;
    }

    if (mres.status == "BUSY") {
        return true; 
    }

    return false; 

  }