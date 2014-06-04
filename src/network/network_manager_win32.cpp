#include "precomp.h"
#include "network_manager_win32.h"

network_manager_win32::network_manager_win32()
{
    receive=false;
    m_height_diff=m_rot_diff=m_result=m_rcv_buflen=m_send_result=0;

    m_listen = INVALID_SOCKET;
    m_client = INVALID_SOCKET;

    m_rcv_buflen = DEFAULT_BUFLEN;

    m_accelerometer_data=glm::vec3(0);
}

network_manager_win32::~network_manager_win32()
{
    if(m_client!=INVALID_SOCKET)
    {
    m_result = shutdown(m_client, SD_SEND);
    if (m_result == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_client);
        WSACleanup();
        return;
    }
    // cleanup
    closesocket(m_client);
    }


    WSACleanup();

    if(m_thread.joinable())
        m_thread.join();
}

bool network_manager_win32::init()
{
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    m_result = WSAStartup(MAKEWORD(2,2), &m_wsadata);
    if (m_result != 0)
    {
        printf("WSAStartup failed with error: %d\n", m_result);
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    m_result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( m_result != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", m_result);
        WSACleanup();
        return false;
    }

    // Create a SOCKET for connecting to server
    m_listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listen == INVALID_SOCKET)
    {
        printf("socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    // Setup the TCP listening socket
    m_result = bind( m_listen, result->ai_addr, (int)result->ai_addrlen);
    if (m_result == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(m_listen);
        WSACleanup();
        return false;
    }

    freeaddrinfo(result);

    m_result = listen(m_listen, SOMAXCONN);
    if (m_result == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(m_listen);
        WSACleanup();
        return false;
    }

    start_waiting_thread();
    return true;
}

void network_manager_win32::deinit()
{
    if(m_waiting_thread.joinable())
        m_waiting_thread.join();

    if(m_thread.joinable())
        m_thread.join();

    if(m_client!=INVALID_SOCKET)
    {
        shutdown(m_client, SD_SEND);
        closesocket(m_client);
    }

    receive=false;
}

void network_manager_win32::wait_for_connection()
{
    while(m_client == INVALID_SOCKET)
    {
        // Accept a client socket
        m_client = accept(m_listen, NULL, NULL);
//        if (m_client == INVALID_SOCKET)
//        {
//            printf("accept failed with error: %d\n", WSAGetLastError());
//            break;
//            closesocket(m_listen);
//            WSACleanup();
//            return false;
//        }
    }

    // No longer need server socket
    closesocket(m_listen);
    start_thread();
}

void network_manager_win32::update()
{
    m_waiting_thread.join();
    while(receive)
    {
        int breakpoint=0;

        std::vector<std::string> subs;

        std::stringstream ss;
        std::string item;

        float *data=new float[3];

        m_result = recv(m_client, m_rcv_buff, m_rcv_buflen, 0);
        if (m_result > 0)
        {
            char isvalidpacket=m_rcv_buff[0];
            if(isvalidpacket=='P')
            {
                int code=atoi(&m_rcv_buff[1]);
                //printf("PACKET TYPE: %d\n",code);
                std::string pbuff="";
                switch(code)
                {
                case PACKET_CONNECT:
                    printf("Somebody did connect.\n");
                    break;
                case PACKET_DISCONNECT:
                    printf("Somebody did disconnect, terminating...\n");
                    receive=false;
                    break;
                case PACKET_DATA:
                    pbuff=std::string(m_rcv_buff);
                    pbuff.erase(0,3);
                    breakpoint=pbuff.find_first_of(';');
                    if(breakpoint>0)
                        pbuff.erase(breakpoint,pbuff.length());

                    printf("Client sent: %s\n", m_rcv_buff);
                    break;
                case PACKET_ACCELEROMETER:
                    pbuff=std::string(m_rcv_buff);
                    pbuff.erase(0,3);
                    breakpoint=pbuff.find_first_of(';');
                    if(breakpoint>0)
                        pbuff.erase(breakpoint,pbuff.length());

                    ss.str(pbuff);

                    subs.clear();
                    subs.resize(0);
                    while(std::getline(ss,item,','))
                    {
                        subs.push_back(item);
                    }

                    ss.clear();

                    for(int i=0; i<3; i++)
                    {
                        data[i]=atof(subs[i].c_str());
                    }
                    m_accelerometer_data=glm::vec3(data[0],data[1],data[2]);
                    //printf("Accelerometer data: %f %f %f\n",data[0],data[1],data[2]);
                    break;
                case PACKET_HEIGHT_CHANGE:
                    pbuff=std::string(m_rcv_buff);
                    pbuff.erase(0,3);
                    breakpoint=pbuff.find_first_of(';');
                    if(breakpoint>0)
                        pbuff.erase(breakpoint,pbuff.length());

                    m_height_diff=atoi(pbuff.c_str());
                    //printf("Height data:%d\n",height);
                    break;

                case PACKET_ROT_CHANGE:
                    pbuff=std::string(m_rcv_buff);
                    pbuff.erase(0,3);
                    breakpoint=pbuff.find_first_of(';');
                    if(breakpoint>0)
                        pbuff.erase(breakpoint,pbuff.length());

                    m_rot_diff=atoi(pbuff.c_str());
                    //printf("Rotation data:%d\n",height);
                    break;
                default:
                    break;
                }


            }

            // Echo the buffer back to the sender
            //iSendResult = send( ClientSocket, m_rcv_buff, iResult, 0 );
            //if (iSendResult == SOCKET_ERROR) {
            //    printf("send failed with error: %d\n", WSAGetLastError());
            //    closesocket(ClientSocket);
            //    WSACleanup();
            //    return 1;
            //}
            //printf("Bytes sent: %d\n", iSendResult);
        }
        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(m_client);
            WSACleanup();
            return;
        }
    }
}

