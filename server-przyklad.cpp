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
int iloscwatkow;
vector<int> kolejnosc;
int watki;
vector<vector<int>> aktywnosc;
vector<vector<int>> punktacja_zapisana;
vector<vector<string>> Odpowiedzi;
vector<vector<string>> Baza;

char lit[1];
mutex mtx2;

int descrCapacity = 16;
int descrCount = 1;
pollfd * descr;

void ctrl_c(int);

void sendToAll(char * buffer, int count);

void gotowosc();

void sendTime(char * buffer, int count, bool poczekalnia);

uint16_t readPort(char * txt);

void setReuseAddr(int sock);

int zegar = 0;
bool Czas(int c, bool poczekalnia){
    zegar = 0;
    while (zegar<c){
        cout<<"Czas: "<<c-zegar<<" | ";
        char buf[2];
        strcpy(buf,to_string(c-zegar).c_str());
        if(descrCount>0){
            sendTime(buf,strlen(buf),poczekalnia);
        }
            zegar++;
        sleep(1);
        if(stanGry==0){
            return false;
        }

        if(iloscwatkow<=(int)kolejnosc.size()&&stanGry==2)
        {
            return true;
        }
    }
        return true;
}



void eventOnServFd(int revents) {
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
            descrCapacity<<=1;
            descr = (pollfd*) realloc(descr, sizeof(pollfd)*descrCapacity);
        }

        descr[descrCount].fd = clientFd;
        descr[descrCount].events = POLLIN|POLLRDHUP;
        descrCount++;
        aktywnosc.push_back(vector<int>());
        aktywnosc[aktywnosc.size()-1].push_back(clientFd);
        aktywnosc[aktywnosc.size()-1].push_back(0);
        aktywnosc[aktywnosc.size()-1].push_back(0);
        int score = 0;
        bool czydodac = true;
        for(int i = 0; i<(int)punktacja_zapisana.size();i++){
            if(punktacja_zapisana[i][0]==static_cast<int>(clientAddr.sin_addr.s_addr)){
                score+=punktacja_zapisana[i][1];
                punktacja_zapisana[i][2]=clientFd-4;
                czydodac=false;
                break;
            }
        }
        if(czydodac==true){
            punktacja_zapisana.push_back(vector<int>());
            punktacja_zapisana[punktacja_zapisana.size()-1].push_back(static_cast<int>(clientAddr.sin_addr.s_addr));
            punktacja_zapisana[punktacja_zapisana.size()-1].push_back(0);
            punktacja_zapisana[punktacja_zapisana.size()-1].push_back(clientFd-4);
        }
        if(stanGry==0 ||stanGry==1){
        char liti[]="N";//N - nowa gra
        write(clientFd, liti, 1);
        }else if(stanGry == 2){
        watki++;
        aktywnosc[aktywnosc.size()-1][2]=1;
        char liti[]="Trwa rozgrywka, prosze czekac do konca tury.";
        write(clientFd, liti, 44);
        }
        gotowosc();
        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
    }
}

void Wyniki(int sl){
    sleep(sl);
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
    Odpowiedzi.clear();
    for(int i = 0;i<(int)kolejnosc.size();i++){
        for(int j = 0;j<(int)punktacja_zapisana.size(); j++){
            if(kolejnosc[i] == punktacja_zapisana[j][0]){
                punktacja_zapisana[j][1]+=punkty[i];
                cout<<punktacja_zapisana[j][1]<<" gracz - "<<j<<endl;
            }
        }
    }
    punkty.clear();
    kolejnosc.clear();
    string s;
    for(int i = 0; i<(int)punktacja_zapisana.size(); i++){
    s = s + "Gracz "+ to_string(punktacja_zapisana[i][2]) + ": " + to_string(punktacja_zapisana[i][1]) + "\n";
    }
    char wyn[s.length()];
    strcpy(wyn,s.c_str());
    sendToAll(wyn,s.length());
}

void OtrzOdp(char * buff, int count, int adres, int clientFd){
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
        kolejnosc.push_back(adres);
        if(zegar<45){zegar=45;}
        Odpowiedzi.push_back(vector<string>());
        for(int i = 0; i < 5; i++){
            for(int j = 0; j<(int)Baza[n+i].size(); j++){
                if(Baza[n+i][j] == panstwa_miasta[i]){
                    b++;
                    if(b==5 && bonus == true){
                        bonus=false;
                        for(int x = 0; x<(int)punktacja_zapisana.size();x++){
                            if(punktacja_zapisana[x][0]==adres){
                                punktacja_zapisana[x][1]+=10;
                            }
                        }
                    }
                    cout<<"Dobra odpowiedź u gracza ";
                    Odpowiedzi[(int)Odpowiedzi.size()-1].push_back(Baza[n+i][j]);
                    cout<<(int)Odpowiedzi.size()-1<<endl;
                    break;
                }
            }
        }
    }else{
    write(clientFd,lit,1);
    }
}

void gra(){
    cout<<"Ilosc graczy: "<<descrCount-1<<endl;
    mtx2.lock();
    if(watki==(int)aktywnosc.size()&&(stanGry==0||stanGry==1)&&watki>0){
        stanGry = 1;
        bool b = Czas(5,false);
        if(b==true&&stanGry==1&&watki==(int)aktywnosc.size()){
            char lul[] = "abcdefghijklmnoprstuwz";
                mtx2.unlock();
            for(int l = 0;l<5;l++){
                bonus = true;
                lit[0]=lul[rand()%22];
                stanGry=2;
                iloscwatkow=watki;
                sendToAll(lit,1);
                if(Czas(60,false)==false){break;}
                stanGry = 3;
                char liti[]="Gromadzenie i obliczanie wynikow...";
                sendToAll(liti,35);
                thread(Wyniki,5).detach();
                if(Czas(10,true)==false){break;}
                }
                watki=0;
                for(int i = 0; i<(int)aktywnosc.size();i++){
                    aktywnosc[i][2]=0;
                }
            stanGry=0;
            char liti[]="N";//N - nowa gra
            sendToAll(liti,1);
            for(int i = 0; i<(int)punktacja_zapisana.size();i++){
                punktacja_zapisana[i][1]=0;
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

void gotowosc(){
    if(stanGry==0||stanGry==1){
        string str = "Gotowi: "+to_string(watki)+"/"+to_string((int)aktywnosc.size());
        char buff[13];
        strcpy(buff, str.c_str());
        sendToAll(buff,str.length());
    }
}

void usun(int clientFd){
        printf("removing (in evendtf) %d\n", clientFd);

        for(int i = 0;i<(int)aktywnosc.size();i++){
            if(aktywnosc[i][0]==clientFd){
                if(aktywnosc[i][2]==1){
                    watki--;
                    if(stanGry==2){
                        iloscwatkow--;
                    }
                }
                aktywnosc.erase(aktywnosc.begin() + i);
            }
        }

        int indexInDescr;
        for(int x = 0; x<descrCount;x++){
            if(clientFd == descr[x].fd){
                indexInDescr = x;
                break;
            }
        }
        descr[indexInDescr] = descr[descrCount-1];
        descrCount--;

        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
        gotowosc();
        if(watki==0){
            stanGry=0;
        }
        else if(stanGry==0){
        thread(gra).detach();}

}

void czyUsunacKlienta(){
    while(true){
        sleep(1);

        int i = 1;
        if(stanGry==0){
            while(i < descrCount){
                int clientFd = descr[i].fd;
                for(int j = 0;j<(int)aktywnosc.size();j++){
                    if(aktywnosc[j][0]==clientFd&&aktywnosc[j][2]==0){
                        aktywnosc[j][1]+=1;
                        cout<<"Do usuniecia klienta: "<<clientFd<<" | "<<aktywnosc[j][1]<<endl;
                        if(aktywnosc[j][1]>60){
                            char liti[]="Blad - przekroczono limit czasowy.\nProsze zalogowac sie ponownie.";
                            write(clientFd, liti, 73);
                            usun(clientFd);
                        }
                    break;
                    }
                }
            i++;
            }
        }

    }
}

void eventOnClientFd(int indexInDescr) {
    auto clientFd = descr[indexInDescr].fd;
    auto revents = descr[indexInDescr].revents;

    for(int i = 0;i<(int)aktywnosc.size();i++){
            if(aktywnosc[i][0]==clientFd){
                aktywnosc[i][1]=0;
            }
        }

    if(revents & POLLIN){

        char buffer[255];
        int count = read(clientFd, buffer, 255);
        if(count < 1)
            revents |= POLLERR;
        else{
            if(stanGry==0&&buffer[0]=='0'){
                    for(int i = 0;i<(int)aktywnosc.size();i++){
                        if(aktywnosc[i][0]==clientFd){
                            if(aktywnosc[i][2]==0){
                                watki++;
                                aktywnosc[i][2]=1;
                            }
                            break;
                        }
                    }
                    gotowosc();
                    thread(gra).detach();

                }
            else if(stanGry==1||stanGry==0){
                for(int i = 0;i<(int)aktywnosc.size();i++){
                        if(aktywnosc[i][0]==clientFd){
                            cout<<"Gracz "<<clientFd-4<<" nie jest gotów!"<<endl;
                            if(aktywnosc[i][2]==1){
                                watki--;
                                aktywnosc[i][2]=0;
                            }
                            aktywnosc[i][2]=0;
                            break;
                        }
                    }
                gotowosc();
                stanGry=0;

                }
            }
            if(stanGry == 2){
            struct sockaddr_in addr;
            socklen_t addr_size = sizeof(struct sockaddr_in);
            getpeername((int)clientFd,(struct sockaddr *)&addr, &addr_size);
                OtrzOdp(buffer,count,static_cast<int>(addr.sin_addr.s_addr),clientFd);
            }
            else if(buffer[0]=='.'){
                char liti[]="N";
                write(clientFd,liti,1);
            }
        }

    if(revents & ~POLLIN){

        usun(clientFd);
    }
}

int main(int argc, char ** argv){

    if(argc != 2) error(1, 0, "Need 1 arg (port)");
    auto port = readPort(argv[1]);

    servFd = socket(AF_INET, SOCK_STREAM, 0);
    if(servFd == -1) error(1, errno, "socket failed");

    signal(SIGINT, ctrl_c);

    signal(SIGPIPE, SIG_IGN);

    setReuseAddr(servFd);

    sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
    if(res) error(1, errno, "bind failed");

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
    thread(czyUsunacKlienta).detach();
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


void sendToAll(char * buffer, int count){
    int i = 1;
    while(i < descrCount){
        int clientFd = descr[i].fd;
        write(clientFd, buffer, count);
        i++;
    }
}

void sendTime(char * buffer, int count, bool poczekalnia){
    int i = 1;
    cout<<"wysylam do: ";
    while(i < descrCount){
        cout<<descr[i].fd<<", ";
        int clientFd = descr[i].fd;
        int counted = count + 6;
        string str(buffer,count);
        string stri = "Czas: "+str;
        if(poczekalnia == true){
            counted += 14;
            stri = stri + " - poczekalnia";
        }
        char buff[counted];
        strcpy(buff, stri.c_str());
        write(clientFd, buff, counted);
        i++;
    }
    cout<<endl;
}

