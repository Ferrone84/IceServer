#pragma once
#include <iostream>
#include <vector>
#include <Ice/Ice.h>
#include <vlc/vlc.h>
#include <IServer.h>

using namespace server;

class Server : public IServer
{
private: //variables
	const std::string music_path = "musics/";
	const char *media_name = "broadcast";
	static int idCount;
	playlist playlist_;
	libvlc_instance_t *vlc = NULL;
	
public:  //methodes
	Server(const int length = 0);
	~Server();
	void add(::server::Music music);
	
	void addMusic(::std::string name, ::std::string artist, std::string album, const ::Ice::Current&);
	bool addMusicPath(::std::string name, const ::Ice::Current&);
	void playMusic(int id, const ::Ice::Current&);
	bool stopMusic(const ::Ice::Current&);
	bool remove(int id, const ::Ice::Current&);
	::server::Music findById(int id, const ::Ice::Current&);
	::server::playlist findByName(::std::string name, const ::Ice::Current&);
    ::server::playlist findByArtist(::std::string artist, const ::Ice::Current&);
    ::server::playlist findByAlbum(std::string album, const ::Ice::Current&);

    void display(const ::Ice::Current&);
    ::server::playlist getPlaylist(const ::Ice::Current&);

private:
	int getsize(FILE *file);
	std::string readThirty(FILE *file, int readloc);
	void autoFill();
};
