#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <poll.h>
#include <unordered_set>
#include <signal.h>
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <thread>
#include <ctime>
#include <vector>
#include <algorithm>
#include <mutex>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

bool bonus;
int servFd;
int stanGry;
vector<int> kolejnosc;
vector<bool> watki;
vector<vector<int>> punktacja;
vector<vector<string>> Odpowiedzi;
vector<vector<string>> Baza;
char lit[1];
mutex mtx2;

// data for poll
int descrCapacity = 16;
int descrCount = 1;
pollfd * descr;

// handles SIGINT
void ctrl_c(int);

// sends data to clientFds excluding fd
void sendToAllBut(int fd, char * buffer, int count);

void sendToAll(char * buffer, int count);

void sendTime(char * buffer, int count);

// converts cstring to port
uint16_t readPort(char * txt);

// sets SO_REUSEADDR
void setReuseAddr(int sock);

int zegar = 0;
bool Czas(int c){
    zegar = 0;
    while (zegar<c){
        cout<<"Czas: "<<c-zegar<<" | ";
        char buf[2];
        strcpy(buf,to_string(c-zegar).c_str());
        if(descrCount>0){
            sendTime(buf,strlen(buf));
        }
            zegar++;
        sleep(1);
        if(stanGry==0){
            return false;
        }
    }
        return true;
}

void eventOnServFd(int revents) {
    // Wszystko co nie jest POLLIN na gnieździe nasłuchującym jest traktowane
    // jako błąd wyłączający aplikację
    if(revents & ~POLLIN){
        error(0, errno, "Event %x on server socket", revents);
        ctrl_c(SIGINT);
    }

    if(revents & POLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);

        auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1) error(1, errno, "accept failed");

        if(descrCount == descrCapacity){
            // Skończyło się miejsce w descr - podwój pojemność
            descrCapacity<<=1;
            descr = (pollfd*) realloc(descr, sizeof(pollfd)*descrCapacity);
        }

        descr[descrCount].fd = clientFd;
        descr[descrCount].events = POLLIN|POLLRDHUP;
        descrCount++;
        watki.push_back(false);
        punktacja.push_back(vector<int>());
        punktacja[punktacja.size()-1].push_back(clientFd-4);
        punktacja[punktacja.size()-1].push_back(0);

        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
    }
}

void Wyniki(){
    vector<int> punkty;
    for(int i = 0; i < (int)Odpowiedzi.size();i++){
    int sum = 0;
        for(int j = 0; j < (int)Odpowiedzi[i].size();j++){
            int w = 15;
            for(int x = 0;x<(int)Odpowiedzi.size();x++){
                if(x==i){
                continue;}
                for(int y = 0; y<(int)Odpowiedzi[x].size();y++){
                    if(Odpowiedzi[i][j]==Odpowiedzi[x][y]){
                        if(w >= 10){
                            w-=5;
                        }
                        break;
                    }
                }
            }
            sum += w;
        }
    punkty.push_back(sum);
    }
    cout<<"Punkty: ";
    for(int c = 0; c < (int)punkty.size(); c++){
    cout<<punkty[c]<<" - ";}
    cout<<endl;
    Odpowiedzi.clear();
    for(int i = 0;i<(int)kolejnosc.size();i++){
        for(int j = 0;j<(int)punktacja.size(); j++){
            if(kolejnosc[i] == punktacja[j][0]){
                punktacja[j][1]+=punkty[i];
            }
        }
    }
    punkty.clear();
    kolejnosc.clear();
    string s;
    for(int i = 0; i<(int)punktacja.size(); i++){
    s = s + "Gracz "+ to_string(punktacja[i][0]) + ": " + to_string(punktacja[i][1]) + "\n";
    }
    cout<<s<<endl;
    char wyn[s.length()];
    strcpy(wyn,s.c_str());
    sendToAll(wyn,s.length());
}

void OtrzOdp(char * buff, int count, auto clientFD){
    string panstwa_miasta[5];
    int j = 0;
    panstwa_miasta[j] += buff[0];
    for(int i = 1; i<count; i++){
        if(isupper(buff[i])>0){
        j++;
        }
        panstwa_miasta[j] += buff[i];
    }
    cout<<"Panstwo:  "<<panstwa_miasta[0]<<endl;
    cout<<"Miasto:   "<<panstwa_miasta[1]<<endl;
    cout<<"Imie:     "<<panstwa_miasta[2]<<endl;
    cout<<"Zwierze:  "<<panstwa_miasta[3]<<endl;
    cout<<"Zawod:    "<<panstwa_miasta[4]<<endl;
    int n=0;
    int b=0;
    while(toupper(lit[0])!=Baza[n][0].at(0)){
    n+=5;
    }
    if(panstwa_miasta[0].at(0)==toupper(lit[0])&&Baza[n][0].at(0)==toupper(lit[0])){
        Odpowiedzi.push_back(vector<string>());
        for(int i = 0; i < 5; i++){
            for(int j = 0; j<(int)Baza[n+i].size(); j++){
                //cout<<Baza[n+i][j]<<" - "<<panstwa_miasta[i]<<endl;
                if(Baza[n+i][j] == panstwa_miasta[i]){
                    b++;
                    if(b==5 && bonus == true){
                        bonus=false;
                        for(int x = 0; x<(int)punktacja.size();x++){
                            if(punktacja[x][0]==clientFD-4){
                                punktacja[x][1]+=10;
                            }
                        }
                    }
                    cout<<"Dobra odpowiedź +15 | ";
                    Odpowiedzi[(int)Odpowiedzi.size()-1].push_back(Baza[n+i][j]);
                    cout<<(int)Odpowiedzi.size()-1<<endl;
                    break;
                }
            }
        }
    }
}

void gra(){
    //mtx.lock();
    cout<<"Ilosc graczy: "<<descrCount-1<<endl;
    for(int i =0;i<(int)watki.size();i++){
        cout<<watki[i]<<" - ";
        }
    cout<<endl;
    mtx2.lock();
    if(all_of(watki.begin(),watki.end(),[](bool b){return b==true;})&&(stanGry==0||stanGry==1)&&watki.empty()==false){
        stanGry = 1;
        bool b = Czas(5);
        if(b==true&&stanGry==1&&all_of(watki.begin(),watki.end(),[](bool b){return b==true;})){
            char lul[] = "abcdefghijklmnoprstuwz";
                mtx2.unlock();
            for(int l = 0;l<5;l++){
                bonus = true;
                lit[0]=lul[rand()%22];
                stanGry=2;
                sendToAll(lit,1);
                if(Czas(60)==false){break;}
                stanGry = 3;
                Wyniki();
                if(Czas(10)==false){break;}
                }
            stanGry=0;
            char liti[]="N";//N - nowa gra
            sendToAll(liti,1);
            for(int i = 0; i<(int)watki.size();i++){
                watki[i]=false;
                punktacja[i][1]=0;
            }
        }
        else{
        stanGry=0;
        char Stop[] = "Stop";
        sendToAll(Stop,4);
        mtx2.unlock();
        }

    }
    else{mtx2.unlock();}

}

void eventOnClientFd(int indexInDescr) {
    auto clientFd = descr[indexInDescr].fd;
    auto revents = descr[indexInDescr].revents;

    if(revents & POLLIN){

        char buffer[255];
        int count = read(clientFd, buffer, 255);
        if(count < 1)
            revents |= POLLERR;
        else{
            if(stanGry==0&&buffer[0]=='0'){
                    watki[clientFd-4]=true;
                    thread(gra).detach();

                }
            else if(stanGry==1||stanGry==0){
                if(buffer[0]=='1'){
                cout<<"Gracz "<<clientFd-4<<" nie jest gotów!"<<endl;
                watki[clientFd-4]=false;
                for(int i =0;i<(int)watki.size();i++){
                cout<<watki[i]<<" - ";}
                cout<<endl;
                stanGry=0;

                }
            }
            else if(stanGry == 2 && zegar<45){
                zegar = 45;
            }
            if(stanGry == 2){
                kolejnosc.push_back(clientFd-4);
                OtrzOdp(buffer,count,clientFd);
            }
        }
    }

    if(revents & ~POLLIN){
        printf("removing (in evendtf) %d\n", clientFd);

        // remove from description of watched files for poll
        watki.erase(watki.begin() + clientFd-4);
        for(int i = 0;i<(int)punktacja.size();i++){
            if(punktacja[i][0]==clientFd-4){
                punktacja.erase(punktacja.begin() + i);
            }
        }
        descr[indexInDescr] = descr[descrCount-1];
        descrCount--;

        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
        if(watki.empty()==true){
            stanGry=0;
        }
        else if(stanGry==0){
        thread(gra).detach();}

    }
}

int main(int argc, char ** argv){
    // get and validate port number
    if(argc != 2) error(1, 0, "Need 1 arg (port)");
    auto port = readPort(argv[1]);

    // create socket
    servFd = socket(AF_INET, SOCK_STREAM, 0);
    if(servFd == -1) error(1, errno, "socket failed");

    // graceful ctrl+c exit
    signal(SIGINT, ctrl_c);
    // prevent dead sockets from throwing pipe errors on write
    signal(SIGPIPE, SIG_IGN);

    setReuseAddr(servFd);

    // bind to any address and port provided in arguments
    sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
    if(res) error(1, errno, "bind failed");

    // enter listening mode
    res = listen(servFd, 1);
    if(res) error(1, errno, "listen failed");

    descr = (pollfd*) malloc(sizeof(pollfd)*descrCapacity);

    descr[0].fd = servFd;
    descr[0].events = POLLIN;
    try{
    ifstream file("Panstwa-miasta.txt",ios::in);
        if (file.good()){
            string str;
            int n = 0;
            while(getline(file, str))
            {
                Baza.push_back(vector<string>());
                istringstream ss(str);
                while( ss.good() ){
                    string substr;
                    getline( ss, substr, ',' );
                    Baza[n].push_back( substr );
                }
                n++;
            }
        }
        file.close();
    }catch(std::exception const& e){
     cout << "There was an error: " << e.what() << endl;
  }
    stanGry = 0;
    srand (time(NULL));
    cout<<"Server uruchomiony!"<<endl;
    while(true){

        int ready = poll(descr, descrCount, -1);
        if(ready == -1){
            error(0, errno, "poll failed");
            ctrl_c(SIGINT);
        }
        for(int i = 0 ; i < descrCount && ready > 0 ; ++i){
            if(descr[i].revents){
                if(descr[i].fd == servFd)
                    eventOnServFd(descr[i].revents);
                else{
                    printf("Event\n");
                    eventOnClientFd(i);
                }

                ready--;
            }
        }
    }
}

uint16_t readPort(char * txt){
    char * ptr;
    auto port = strtol(txt, &ptr, 10);
    if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"illegal argument %s", txt);
    return port;
}

void setReuseAddr(int sock){
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res) error(1,errno, "setsockopt failed");
}

void ctrl_c(int){
    for(int i = 1 ; i < descrCount; ++i){
        shutdown(descr[i].fd, SHUT_RDWR);
        close(descr[i].fd);
    }
    close(servFd);
    printf("Closing server\n");
    exit(0);
}


void sendToAllBut(int fd, char * buffer, int count){
    int i = 1;
    while(i < descrCount){
        int clientFd = descr[i].fd;
        if(clientFd == fd) {
            i++;
            continue;
        }
        int counted = count + 3;
        string str(buffer,count);
        string stri = to_string(fd) +": "+str;
        char buff[counted];
        strcpy(buff, stri.c_str());

        int res = write(clientFd, buff, counted);
        if(res!=counted){
            printf("removing (in sendTof) %d\n", clientFd);
            watki.erase(watki.begin() + clientFd-4);
            shutdown(clientFd, SHUT_RDWR);
            close(clientFd);
            descr[i] = descr[descrCount-1];
            descrCount--;
            continue;

        }
        i++;
    }
}


void sendToAll(char * buffer, int count){
    int i = 1;
    while(i < descrCount){
        int clientFd = descr[i].fd;

        string str(buffer,count);
        strcpy(buffer, str.c_str());
        write(clientFd, buffer, count);
        i++;
    }
}
void sendTime(char * buffer, int count){
    int i = 1;
    cout<<"wysylam do: ";
    while(i < descrCount){
        cout<<descr[i].fd<<", ";
        int clientFd = descr[i].fd;
        int counted = count + 6;
        string str(buffer,count);
        string stri = "Czas: "+str;
        char buff[counted];
        strcpy(buff, stri.c_str());
        /*int res = */write(clientFd, buff, counted);
        /*if(res!=counted){

            cout<<"usuwam: "<<clientFd<<endl;
            printf("removing %d\n", clientFd);
            watki.erase(watki.begin() + clientFd-4);
            shutdown(clientFd, SHUT_RDWR);
            close(clientFd);
            descr[i] = descr[descrCount-1];
            descrCount--;

            continue;
        }*/
        i++;
    }
    cout<<endl;
}

