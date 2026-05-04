#include "Communicator.h"
#include <iostream>
#include <thread>
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

void Communicator::startHandleRequests()
{
    bindAndListen();
}

void Communicator::bindAndListen()
{
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
}
