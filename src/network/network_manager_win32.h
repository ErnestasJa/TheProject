#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <thread>

#define DEFAULT_BUFLEN 2048
#define DEFAULT_PORT "13337"

enum packet_type
{
    PACKET_CONNECT = 0x0,
    PACKET_DATA = 0x1,
    PACKET_DISCONNECT = 0x2,
    PACKET_ACCELEROMETER = 0x3,
    PACKET_HEIGHT_CHANGE = 0x4,
    PACKET_ROT_CHANGE = 0x5
};

class network_manager_win32
{
private:
    WSAData m_wsadata;
    SOCKET m_listen,m_client;

    std::thread m_thread;
    std::thread m_waiting_thread;

    char m_rcv_buff[DEFAULT_BUFLEN];
    int m_height_diff,m_rot_diff,m_result,m_rcv_buflen,m_send_result;
    glm::vec3 m_accelerometer_data;
    bool receive;
    void wait_for_connection();
public:
    network_manager_win32();
    ~network_manager_win32();
    bool init();
    void update();
    bool send_packet(){return true;} ///TODO
    void start_waiting_thread(){m_waiting_thread=std::thread(&network_manager_win32::wait_for_connection,this);}
    void start_thread(){m_thread=std::thread(&network_manager_win32::update,this);receive=true;}
    bool is_receiving(){return receive;}
    glm::vec3 get_accelerometer_data(){return m_accelerometer_data;}
    int get_height_diff(){int ret= m_height_diff;m_height_diff=0;return ret;}
    int get_rot_diff(){int ret= m_rot_diff;m_rot_diff=0;return ret;}
protected:
};
