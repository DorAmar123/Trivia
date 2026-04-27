#pragma once
#include <vector>
#include <ctime>
#include "Requests.h"

struct RequestInfo
{
    int id;
    time_t receivalTime;
    std::vector<unsigned char> buffer;
};

class JsonRequestPacketDeserializer
{
public:
    static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
    static SignupRequest deserializeSignupRequest(const std::vector<unsigned char>& buffer);
};
