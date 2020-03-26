#include <iostream>
#include <fstream>
#include <algorithm>
#include <Server.h>

using namespace std;
using namespace server;

int Server::idCount = 0;

Server::Server(const int length) {
	this->playlist_.resize(length);
	autoFill(); //évite de devoir remplir la playlist à chaque fois avec le client
}

Server::~Server() {

}

void Server::add(server::Music music) {
	music.id = idCount++;
	playlist_.push_back(music);
}

void Server::addMusic(string name, string artist, string album, const ::Ice::Current&) {
	Music music;
	music.name = name;
	music.artist = artist;
	music.album = album;

	this->add(music);
	cout << "Cette musique a bien été ajoutée à la playlist." << endl;
}

bool Server::addMusicPath(string name, const ::Ice::Current&) {
	const string complete_path = music_path + name + ".mp3";
	FILE* file = fopen(complete_path.c_str(), "r+");

	if (file) {
		int readloc = getsize(file)-128; //Go to byte location of information
		int filesize = getsize(file);//stores the full file size needed for writing data later
		Music music;

		readloc = getsize(file)-125;
		music.name = readThirty(file, readloc);

		readloc+=30;
		music.artist = readThirty(file, readloc);

		readloc+=30;
		music.album = readThirty(file, readloc);

		music.path = complete_path;

		this->add(music);
		cout << "Cette musique a bien été ajoutée à la playlist." << endl;

		fclose(file);
	}
	else {
		cout << "Ce fichier n'as pas pu être lu." << endl;
		return false;
	}
	return true;
}

int Server::getsize(FILE *file) {
	//returns the size of the file as an int
	int loc = ftell(file);
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file, loc, SEEK_SET);
	return size;
}

string Server::readThirty(FILE *file, int readloc) {
	//Used to read Title, Artist, Album, Comment
	//Each is 30 bytes long
	char magic[30];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 30, file);
	string str(magic);
	return str;
}

void Server::playMusic(int id, const ::Ice::Current& c) {
	if (vlc != NULL) { //si une musique est déjà en cours
		stopMusic(c);
	}

	server::Music music = findById(id, c);
	if (music.id == -1) {
		cout << "Ce numéro de musique d'existe pas." << endl;
		return;
	}
	else
		cout << "Joue la musique n°" << id << " \"" << music.name << "\" : " << endl;

	const string complete_path = music.path;
	const char *sout = "#transcode{acodec=mp3,ab=128,channels=2,samplerate=44100}:http{dst=:8090/music.mp3}";

	vlc = libvlc_new(0, NULL);
	libvlc_vlm_add_broadcast(vlc, media_name, complete_path.c_str(), sout, 0, NULL, true, false);
	libvlc_vlm_play_media(vlc, media_name);
}

bool Server::stopMusic(const ::Ice::Current&) {
	if (vlc == NULL) {
		//cout << "Le stream n'est pas lancé." << endl;
		return false;
	}
	libvlc_vlm_stop_media(vlc, media_name);
	libvlc_vlm_release(vlc);
	vlc = NULL;
	cout << "La musique a bien été stopée." << endl;
	return true;
}

bool Server::remove(int id, const ::Ice::Current&) {
	if (id < 0 ) {
		cerr << "Impossible de supprimer un numéro de musique négatif." << endl;
		return false;
	}

	bool find = false;
	for (int i=0; i < playlist_.size(); i++) {
		if (playlist_[i].id == id) {
			playlist_.erase(playlist_.begin() + i);
			find = true;
		}
	}

	if (!find) {
		cerr << "Impossible de supprimer la musique avec le numéro : " << id << ", car il n'existe pas." << endl;
	}
	else
		cout << "Cette musique a bien été retirée de la playlist." << endl;

	return find;
}

server::Music Server::findById(int id, const ::Ice::Current&) {
	for (int i=0; i < playlist_.size(); i++) {
		if (playlist_[i].id == id) {
			return playlist_[i];
		}
	}

	server::Music music; //cas où l'id n'existe pas
	music.id = -1;
	return music;
}

server::playlist Server::findByName(string name, const ::Ice::Current&) {
	vector<Music> musics;
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	for (int i=0; i < playlist_.size(); i++) {
		string tmp_name = playlist_[i].name;
		transform(tmp_name.begin(), tmp_name.end(), tmp_name.begin(), ::tolower);

		if (tmp_name.find(name) != string::npos) {
			musics.push_back(playlist_[i]);
		}
	}

	//if (musics.size() == 0)
	//	cerr << "Rien a été trouvé pour la recherche du nom : '" << name << "'." << endl;
	return musics;
}

server::playlist Server::findByArtist(string artist, const ::Ice::Current&) {
	vector<Music> musics;
	transform(artist.begin(), artist.end(), artist.begin(), ::tolower);

	for (int i=0; i < playlist_.size(); i++) {
		string tmp_artist = playlist_[i].artist;
		transform(tmp_artist.begin(), tmp_artist.end(), tmp_artist.begin(), ::tolower);

		if (tmp_artist.find(artist) != string::npos) {
			musics.push_back(playlist_[i]);
		}
	}

	//if (musics.size() == 0)
	//	cerr << "Rien a été trouvé pour la recherche de l'artiste : '" << artist << "'." << endl;
	return musics;
}

server::playlist Server::findByAlbum(string album, const ::Ice::Current&) {
	vector<Music> musics;
	transform(album.begin(), album.end(), album.begin(), ::tolower);

	for (int i=0; i < playlist_.size(); i++) {
		string tmp_album = playlist_[i].album;
		transform(tmp_album.begin(), tmp_album.end(), tmp_album.begin(), ::tolower);

		if (tmp_album.find(album) != string::npos) {
			musics.push_back(playlist_[i]);
		}
	}

	//if (musics.size() == 0)
	//	cerr << "Rien a été trouvé pour la recherche de l'album : '" << album << "'." << endl;
	return musics;
}

void Server::display(const ::Ice::Current&) {
	if (playlist_.empty()) {
		cerr << "Liste de morceaux vide." << endl;
		return;
	}

	cout << "Affichage du contenu de la playlist : " << endl;
	for (int i=0; i < playlist_.size(); i++) {
		Music m = playlist_[i];
		cout << "\tMusique n°" << m.id << " : " << m.name << " " << m.artist << " " << m.album << endl;
	}
	cout << endl;
}

server::playlist Server::getPlaylist(const ::Ice::Current&) {
	return this->playlist_;
}

void Server::autoFill() {
	cout << "Ajout auto : " << endl;
	Ice::Current lel;
	addMusicPath("get_your_love", lel);
	addMusicPath("let_me_go", lel);
	addMusicPath("remind_me", lel);
	addMusicPath("short_song", lel);
}
