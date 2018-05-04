#ifndef POOL_MINING_MANAGER_H
#define POOL_MINING_MANAGER_H

#include <string>
#include "common/rpc_client.h"

typedef struct _MINING_CONFIG {
    std::string mining_algorithm;
    std::string pool_server_address;
    unsigned short pool_port;
    std::string wallet_address;
    std::string wallet_password;
    unsigned int  threads_count;
}MINING_CONFIG,*PMINING_CONFIG;

class PoolMiningManager {
private:
    tools::t_rpc_client* m_rpc_client;
    bool m_is_rpc;
public:
    PoolMiningManager();
    ~PoolMiningManager();
    void setMiningRpcAddress(const std::string &ip, int port);
    bool isMining() const;
    bool startMining(MINING_CONFIG& config);
    bool stopMining();
    double miningHashRate() const;
private:
    std::string m_mining_rpc_address;
    bool  m_isMining;
};


#endif // POOL_MINING_MANAGER_H
