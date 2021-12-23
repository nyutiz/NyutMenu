#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include <iostream>
#include <stdlib.h>
#include<winsock2.h>
#include <stdio.h>
#include <Windows.h>


using namespace std;

#define PORT 80

const char szHost[] = "www.goolge.com";

#pragma comment(lib, "Ws2_32.lib")

SOCKET Socket, Sub;
WSADATA Winsock;
sockaddr_in Addr;
int Addrlen = sizeof(sockaddr_in);
char Buffer[256];
char* Str;
sockaddr_in IncomingAddress;
int AddressLen = sizeof(IncomingAddress);

int main()
{
    int choix = 0;
    system("Color 0A");
    SetConsoleTitleA("NyutMenu");

    std::cout << "==========================\n";
    std::cout << "Choisissez Votre Option:\n";
    std::cout << "==========================\n";
    std::cout << "[1] { Port Scanner }" << endl;
    std::cout << "[2] { Creer Server }" << endl;
    std::cout << "[3] { Creer Client }" << endl;
    std::cout << "[4] { Reload }" << endl;
    std::cout << "[5] { Quitter }" << endl;

    std::cout << "\nSelectionner: ";
    std::cin >> choix;
    std::cin.ignore();

    std::cout << "\n[ Recherche de l'option " << choix << "... ]\n" << std::endl;

    if (choix == 1) {
        WSADATA data;
        SOCKET sock;
        int err, i, startport, endport;
        char ip[20];
        struct sockaddr_in sock_addr;
        FILE* fp1;

        printf("ip: ");
        scanf("\n%s", ip);
        printf("start port: ");
        scanf("%d", &startport);
        printf("end port: ");
        scanf("%d", &endport);
        if ((WSAStartup(MAKEWORD(2, 0), &data) != 0))
        {
            printf("Erreur: Winsock ne peut pas etre initlaiser\n\n");
        }
        else
        {
            for (i = startport; i < endport; i++)
            {
                sock = socket(AF_INET, SOCK_STREAM, 0);
                sock_addr.sin_family = PF_INET;
                sock_addr.sin_port = htons(i);
                sock_addr.sin_addr.s_addr = inet_addr(ip);
                printf("Verification du port: \n", i);
                err = connect(sock, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr));
                if (err == 0)
                {
                    printf("Port Ouvert !\n\n\a");
                    fp1 = fopen("ports.txt", "a+");
                    fprintf(fp1, "Port Ouvert: %d\n\n", i);
                    closesocket(sock);
                    fclose(fp1);
                }
                else
                {
                    printf("Port Ferme...\n\n");
                }
            }
            WSACleanup();
            system("ports.txt");
        }
    }
    if (choix == 2) {
        WSAStartup(MAKEWORD(2, 2), &Winsock);    // Start Winsock

        if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
        {
            WSACleanup();
            return 0;
        }

        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        ZeroMemory(&Addr, sizeof(Addr));
        Addr.sin_family = AF_INET;
        Addr.sin_port = htons(6091);
        bind(Socket, (sockaddr*)&Addr, sizeof(Addr));

        if (listen(Socket, 1) == SOCKET_ERROR)
        {
            printf("listening error\n");
        }
        else
        {
            printf("listening ok\n");
        }

        if (Sub = accept(Socket, (sockaddr*)&IncomingAddress, &AddressLen))
        {
            char* ClientIP = inet_ntoa(IncomingAddress.sin_addr);
            int ClientPort = ntohs(IncomingAddress.sin_port);
            printf("Client conncted!\n");
            printf("IP: %s:%d\n", ClientIP, ClientPort);

            printf("Sending file .. \n");


            FILE* File;
            char* Buffer;
            unsigned long Size;

            File = fopen("C:\\Patate.txt", "rb");
            if (!File)
            {
                printf("Error while readaing the file\n");
                goto End;
            }

            fseek(File, 0, SEEK_END);
            Size = ftell(File);
            fseek(File, 0, SEEK_SET);

            Buffer = new char[Size];

            fread(Buffer, Size, 1, File);
            char cSize[MAX_PATH];
            sprintf(cSize, "%i", Size);

            fclose(File);

            send(Sub, cSize, MAX_PATH, 0); // File size
            send(Sub, Buffer, Size, 0); // File Binary
            free(Buffer);

        End:
            closesocket(Sub);
            closesocket(Socket);
            WSACleanup();
        }

        getchar();
        return 0;
    }
    if (choix == 3) {
        WSAStartup(MAKEWORD(2, 2), &Winsock);    // Start Winsock

        if (LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)    // Check version
        {
            WSACleanup();
            return 0;
        }

        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        ZeroMemory(&Addr, sizeof(Addr));    // clear the struct
        Addr.sin_family = AF_INET;    // set the address family
        Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        Addr.sin_port = htons(6091);    // set the port

        if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) < 0)
        {
            printf("Connection failed !\n");
            getchar();
            return 0;
        }

        printf("Connection successful !\n");

        printf("Receiving file .. \n");

        int Size;
        char* Filesize = new char[1024];

        if (recv(Socket, Filesize, 1024, 0)) // File size
        {
            Size = atoi((const char*)Filesize);
            printf("File size: %d\n", Size);
        }

        char* Buffer = new char[Size];

        if (recv(Socket, Buffer, Size, 0)) // File Binary
        {
            FILE* File;
            File = fopen("patate.txt", "wb");
            fwrite((const char*)Buffer, 1, Size, File);
            fclose(File);
        }

        getchar();
        closesocket(Socket);
        WSACleanup();
        return 0;
    }
    if (choix == 4) {
        system("cls");
        main();
    }

    if (choix == 5) {
        exit(0);
    }
    else
    {
        cout << "Veuillez Choisir une option correcte...\n\n";
        main();
    }
}

